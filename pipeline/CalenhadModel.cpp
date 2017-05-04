//
// Created by martin on 06/01/17.
//

#include "CalenhadModel.h"
#include "../nodeedit/CalenhadController.h"
#include "../nodeedit/qneconnection.h"
#include "../nodeedit/qneblock.h"
#include "../nodeedit/Calenhad.h"
#include "../icosphere/icosphere.h"
#include "../CalenhadServices.h"
#include "../actions/DuplicateModuleCommand.h"
#include "../actions/AddModuleCommand.h"

using namespace icosphere;

CalenhadModel::CalenhadModel() : QGraphicsScene(),
    conn (nullptr),
    _port (nullptr),
    _author (""),
    _title (""),
    _description (""),
    _date (QDateTime::currentDateTime()),
    _controller (nullptr) {
    installEventFilter (this);
}

// determine whether connection from given input to given output is allowed
bool CalenhadModel::canConnect (QNEPort* output, QNEPort* input, const bool& verbose) {
    //To do: externalise message strings
    if (output && input) {

        // can't connect a block to itself
        if (output -> block() == input -> block()) {
            if (verbose) {
                emit showMessage ("Cannot connect owner to itself");
            }
            return false;
        }

        // can only connect an output port to an input port
        if (input -> portType() ==  QNEPort::OutputPort) {
            if (verbose) {
                emit showMessage ("Cannot make connection to another owner output");
            }
            return false;
        }

        // can't create circular paths so reject if a path already exists from proposed output back to proposed input
        // (in which case this connection would complete a circle)
        if (existsPath (input -> block(), output -> block())) {
            if (verbose) {
                emit showMessage ("Connection would form a circuit within the network");
            }
            return false;
        }

        // can't connect to a port that's already connected to another output
        if (! (input -> connections().empty ())) {
            if (verbose) {
                emit showMessage ("Port is already connected");
                return false;
            }
        }

        // if survived all that, connection is OK
        return true;
    } else {
        emit showMessage ("No target node selected");
        return false;
    }
};

// Return true if there is a direct or indirect path from one block to another.
// If there is we can't create a new connection between them as that would complete a circuit.
bool CalenhadModel::existsPath (QNEBlock* from, QNEBlock* to) {
    // base case: block with no inputs can't have any paths to it
    if (to -> inputs().isEmpty()) {
        return false;

        // see if the two blocks are connected
    } else {
        for (QNEPort* input: to -> inputs ()) {
            for (QNEPort* output : from -> outputs ()) {
                if (output -> isConnected (input)) {
                    return true;
                }
            }
        }
    }

    // recursively examine blocks which feed the previous from block
    for (QNEPort* output : from -> outputs()) {
        for (QNEConnection* c : output -> connections ()) {
            QNEPort* input = output == c -> port1() ? c -> port2() : c -> port1();
            if (existsPath (input -> block(), to)) {
                return true;
            }
        }
    }
    return false;
}

bool CalenhadModel::connectPorts (QNEPort* output, QNEPort* input) {
    if (canConnect (output, input, true)) {
        QNEConnection* c = new QNEConnection (0);
        addItem (c);
        c -> setPort1 (output);
        c -> setPort2 (input);
        c -> updatePosFromPorts();
        c -> updatePath();
        update();

        // if we are connecting QModules, we need to link the underlying noise modules too
        if ((input -> portType() == QNEPort::InputPort || input -> portType() == QNEPort::ControlPort) && output -> portType() == QNEPort::OutputPort) {
            noise::module::Module* source, * target;
            source = ((QModule*) output -> owner()) -> module ();
            target = ((QModule*) input -> owner()) -> module ();
            if (target) {
                target -> SetSourceModule (input -> index (), *source);
            } else {
                CalenhadServices::messages() -> message ("", "No target owner");
            }
        }

        // tell the target owner to declare change requiring rerender
        output -> owner () -> invalidate();

        // this propogates changes on the source owner to the target so that the target can update any visible views when its inputs change
        connect (output -> owner (), SIGNAL (nodeChanged()), input -> owner (), SLOT (invalidate()));

        // colour the input to show its connected status
        input -> setHighlight (PortHighlight::CONNECTED);

        // tell the target owner to declare change requiring rerender
        output -> owner () -> invalidate();
        //input -> invalidateRenders();
        return true;
    } else {
        return false;
    }
}

void CalenhadModel::disconnectPorts (QNEConnection* connection) {
    if (connection -> port1()) { connection -> port1() -> initialise (); }
    if (connection -> port2()) { connection -> port2() -> initialise (); }

    if (connection -> port2() -> portType() == QNEPort::InputPort || connection -> port2() -> portType() == QNEPort::OutputPort) {
        noise::module::Module* m = ((QModule*) connection -> port2() -> owner()) -> module ();

        // disconnect the modules in the libnoise engine
        m->SetSourceModule (connection->port2 ()->index (), *nullModule);
    }

    // colour the input port to show its availability
    if (connection -> port1() -> type() != QNEPort::OutputPort) { connection -> port1() -> setHighlight (PortHighlight::NONE); }
    if (connection -> port2() -> type() != QNEPort::OutputPort) { connection -> port2() -> setHighlight (PortHighlight::NONE); }

    // reproduce the renders to reflect the change
//    connection -> port2() -> invalidateRenders();

    // update the model
    removeItem (connection);
    delete connection;
    update();

}


QGraphicsItem* CalenhadModel::itemAt (const QPointF& pos) {
    QList<QGraphicsItem*> list = items (QRectF (pos - QPointF (1, 1), QSize (3, 3)));
    return list.isEmpty () ? nullptr : list.at (0);
}

// This handler is required to stop a right-click which brings up the context menu from clearing the selection.
// Left clicking in space however should discard any current selection.
void CalenhadModel::mousePressEvent(QGraphicsSceneMouseEvent *event) {
    if (event -> button() != Qt::LeftButton) {
        event -> accept();
        return;
    }
    QGraphicsScene::mousePressEvent (event);
}

bool CalenhadModel::eventFilter (QObject* o, QEvent* e) {
    QGraphicsSceneMouseEvent* me = (QGraphicsSceneMouseEvent*) e;
    switch ((int) e -> type ()) {
        case QEvent::GraphicsSceneMousePress: {
            switch ((int) me -> button()) {
                case Qt::LeftButton: {
                    QGraphicsItem* item = itemAt (me -> scenePos ()) ;

                    // click on nothing - deselect anything selected
                    if (! item && ! conn) {
                        for (QGraphicsItem* modelItem : items()) {
                            if (modelItem -> isSelected()) {
                                modelItem -> setSelected (false);
                                modelItem -> update();
                            }
                        }
                    }

                    // click on an output port - create a connection which we can connect to another owner's input or control port
                    if (item && item -> type () == QNEPort::Type) {
                        // only allow connections from output ports to input ports
                        QNEPort* port = ((QNEPort*) item);
                        if (port -> portType() == QNEPort::OutputPort) {
                            for (QGraphicsView* view : views()) {
                                view -> setDragMode (QGraphicsView::NoDrag);
                            }
                            if (conn) { delete conn; }
                            conn = new QNEConnection (0);
                            addItem (conn);
                            conn -> setPort1 ((QNEPort*) item);
                            conn -> setPos1 (item -> scenePos ());
                            conn -> setPos2 (me -> scenePos ());
                            conn -> updatePath ();
                            conn -> canDrop = false;
                            return true;
                        }
                    }
                    break;
                }

                case Qt::RightButton: {

                    if (! conn) {
                        QGraphicsItem* item = itemAt (me -> scenePos ());
                        /*if (item) {

                            // right click on existing connection: context menu (to do)
                            if (item -> moduleType() == QNEConnection::Type) {


                            }
                        } */
                        QMenu* menu = _controller -> getContextMenu (item);
                        if (menu) {
                            menu -> exec (me -> screenPos());
                        }

                    }
                    break;
                }
            }
        }
        case QEvent::GraphicsSceneMouseMove: {

            // if we moved off a port, set it back to its ordinary style
            if (_port) {
                _port -> initialise ();
            }
            if (conn) {
               // _viewToolsGroup -> toolToggled (false);
                conn -> setPos2 (me -> scenePos());
                conn -> updatePath ();
                conn -> canDrop = false;

                QGraphicsItem* item = itemAt (me -> scenePos ());
                if (item && item -> type () == QNEPort::Type) {
                    QNEPort* port = (QNEPort*) item;
                    if (port != conn -> port1() && ! (port -> hasConnection())) {
                        if (canConnect (conn -> port1(), port)) {
                            // Change colour of a port if we mouse over it and can make a connection to it
                            port -> setHighlight (PortHighlight::CAN_CONNECT);
                            _port = port;
                        }
                    }
                } else {
                    if (_port) {
                        // If we moved off a port without making a connection to it, set it back to its unoccupied colour
                        _port -> setHighlight (PortHighlight::NONE);
                        _port = nullptr;
                    }
                }
                return true;
            } else {
                if (! _activeTool) {
                    for (QGraphicsView* view : views()) {
                        view -> viewport() -> setCursor (Qt::ArrowCursor);
                    }
                }
            }
            break;
        }

        case QEvent::GraphicsSceneMouseRelease: {
            for (QGraphicsView* view : views ()) {
                view -> setDragMode (QGraphicsView::RubberBandDrag);
            }
            lastClick = me -> scenePos();
            if (conn) {
                if (_port) {
                    _port -> initialise();
                    _port -> update();
                }
                if (conn && me -> button() == Qt::LeftButton) {
                    QGraphicsItem* item = itemAt (me -> scenePos());
                    if (item && item -> type () == QNEPort::Type) {
                        QNEPort* port1 = conn -> port1();
                        QNEPort* port2 = (QNEPort*) item;
                        removeItem (conn);
                        if (!connectPorts (port1, port2)) {
                            // if connection successful, clear the connection to avoid starting a new one immediately
                            delete conn;
                            conn = nullptr;
                            return true;
                        }
                    }
                }
            }
            if (_activeTool) {
                QString type = _activeTool -> data().toString();
                if (! type.isNull ()) {

                    addModule (me -> scenePos(), type);
                }
                _controller -> clearTools();
                setActiveTool (nullptr);
            }
        }
        // deactivate any in-progress connection whenever a mouse release occurs
        if (conn) {
            delete conn;
            conn = nullptr;
        }
    }
    return QObject::eventFilter (o, e);
}

QModule* CalenhadModel::addModule (const QPointF& initPos, const QString& type, const QString& name) {
    if (type != QString::null) {
        QModule* module = _moduleFactory.createModule (type, this);
        module -> setName (name);
        AddModuleCommand* command = new AddModuleCommand (module, initPos, this);
        _controller -> doCommand (command);
       //return addModule (module, initPos);
    } else {
        CalenhadServices::messages() -> message ("", "Couldn't create owner of type " + type + "\n");
        return nullptr;
    }
}

QModule* CalenhadModel::addModule (QModule* module, const QPointF& initPos) {
    QNEBlock* b = new QNEBlock (module);
    module -> setHandle (b);
    addItem (b);
    b -> setPos (initPos.x (), initPos.y ());
    b -> initialise();
    connect (module, &QNode::nameChanged, b, &QNEBlock::moduleChanged);
    for (QNEPort* port : module->ports ()) {
        b -> addPort (port);
    }
    module -> setModel (this);
    if (module -> name().isNull () || module -> name().isEmpty()) {
        module -> setUniqueName();
    }
    module -> invalidate ();
    return module;
}

void CalenhadModel::deleteModule (QModule* module) {
    // first delete any connections to or from the doomed owner
    for (QGraphicsItem* item : items()) {
        if (item -> type() == QNEConnection::Type) {
            for (QNEPort* p : module -> ports ()) {
                if (((QNEConnection*) item) -> port1() == p || ((QNEConnection*) item) -> port2() == p) {
                    removeItem (item);
                    delete item;
                }
            }
        }
    }


    noise::module::Module* m = module -> module ();


    // remove the visible appartions from the display

    for (QGraphicsItem* item : items()) {
        if (item -> type () == QNEBlock::Type && ((QNEBlock*) item) -> module() == module) {
            removeItem (item -> parentItem());
            delete item;
        }
    }
    update();

    // now delete the underlying libnoise owner
    if (m) { delete m; }

    _port = nullptr; // otherwise it keeps trying to do stuff to the deleted port
}

CalenhadModel::~CalenhadModel() {

}

QList<QModule*> CalenhadModel::modules() {
    QList<QModule*> modules;
            foreach (QGraphicsItem* item, items()) {
            int type = item -> type();
            if (type == QGraphicsItem::UserType + 3) {  // is a QNEBlock
                QNEBlock* handle = (QNEBlock*) item;
                QModule* qm = handle -> module();
                modules.append (qm);
            }
        }
    return modules;
}

QList<QNEConnection*> CalenhadModel::connections() {
    QList<QNEConnection*> connections;
            foreach (QGraphicsItem* item, items()) {
            int type = item -> type();
            if (type == QGraphicsItem::UserType + 2) {  // is a QNEConnection
                QNEConnection* c = (QNEConnection*) item;
                connections.append (c);
            }
        }
    return connections;
}

QModule* CalenhadModel::findModule (const QString& name) {
    for (QModule* qm : modules()) {
        if ((! qm -> name().isNull ()) && (qm -> name() == name)) {
            return qm;
        }
    }
    return nullptr;
}

void CalenhadModel::setActiveTool (QAction* tool) {
    _activeTool = tool;
}

void CalenhadModel::setController (CalenhadController* controller) {
    if (! _controller)  {
        _controller = controller;
    } else {
        std::cout << "Can't replace controller once assigned\n";
    }
}

CalenhadController* CalenhadModel::controller () {
    return _controller;
}

QDomDocument CalenhadModel::serialise() {
    QDomDocument doc;
    QDomElement root = doc.createElement ("model");
    doc.appendChild (root);
    writeMetadata (doc);
    for (QModule* qm : modules()) {
        qm -> serialise (doc);
    }
    for (QNEConnection* c : connections()) {
        c -> serialise (doc);
    }
    return doc;
}

void CalenhadModel::writeMetadata (QDomDocument& doc) {
    QDomElement metadata = doc.createElement ("metadata");
    doc.documentElement().appendChild (metadata);
    if (! _title.isEmpty()) {
        QDomElement titleElement = doc.createElement ("title");
        metadata.appendChild (titleElement);
        QDomText titleContent = doc.createTextNode (_title);
        titleElement.appendChild (titleContent);
    }

    QDomElement dateElement = doc.createElement ("date");
    metadata.appendChild (dateElement);
    QDomText dateContent = doc.createTextNode (_date.toString ("dd MMMM yyyy hh:mm"));
    dateElement.appendChild (dateContent);

    if (! _author.isEmpty()) {
        QDomElement authorElement = doc.createElement ("author");
        metadata.appendChild (authorElement);
        QDomText authorContent = doc.createTextNode (_author);
        authorElement.appendChild (authorContent);
    }
    if (! _description.isEmpty()) {
        QDomElement descriptionElement = doc.createElement ("description");
        metadata.appendChild (descriptionElement);
        QDomText descriptionContent = doc.createTextNode (_description);
        descriptionElement.appendChild (descriptionContent);
    }
}

void CalenhadModel::readMetadata (const QDomDocument& doc) {
    QDomElement metadataElement = doc.documentElement().firstChildElement ("metadata");
    QDomElement authorElement = metadataElement.firstChildElement ("author");
    _author = authorElement.isNull() ? "" : authorElement.nodeValue();
    QDomElement titleElement = metadataElement.firstChildElement ("title");
    _title = titleElement.isNull() ? "" : titleElement.nodeValue();
    QDomElement descriptionElement = metadataElement.firstChildElement ("description");
    _description = descriptionElement.isNull() ? "" : descriptionElement.nodeValue();
    QDomElement dateElement = metadataElement.firstChildElement ("date");
    _date = dateElement.isNull() ? QDateTime::currentDateTime() : QDateTime::fromString (dateElement.nodeValue(), "dd MMMM yyyy hh:mm");
}

void CalenhadModel::inflate (const QDomDocument& doc) {
    readMetadata (doc);
    QDomNodeList moduleNodes = doc.documentElement().elementsByTagName ("owner");
    for (int i = 0; i < moduleNodes.count(); i++) {
        QDomElement positionElement = moduleNodes.at (i).firstChildElement ("position");
        int x = positionElement.attributes().namedItem ("x").nodeValue().toInt();
        int y = positionElement.attributes().namedItem ("y").nodeValue().toInt();
        QPointF pos (x, y);
        QString type = moduleNodes.at(i).attributes().namedItem ("type").nodeValue();
        QDomElement nameNode = moduleNodes.at (i).firstChildElement ("name");
        QString name = nameNode.text();
        QModule* qm = addModule (pos, type, name);
        qm -> inflate (moduleNodes.at (i).toElement());
    }

    // In the connections, we save and retrieve the types of output ports in case we ever have further types of output ports.
    // Does not support a port serving as both input and output (because index presently not unique across both).
    // For the time being however all output ports will be of type 2 (QNEPort::Output).

    QDomNodeList connectionNodes = doc.documentElement().elementsByTagName ("connection");
    for (int i = 0; i < connectionNodes.count(); i++) {
        QDomElement fromElement = connectionNodes.at (i).firstChildElement ("source");
        QDomElement toElement = connectionNodes.at (i).firstChildElement ("target");
        QModule* fromModule = findModule (fromElement.attributes().namedItem ("owner").nodeValue());
        QModule* toModule = findModule (toElement.attributes().namedItem ("owner").nodeValue());
        if (fromModule && toModule) {
            QNEPort* fromPort = nullptr, * toPort = nullptr;
            for (QNEPort* port : fromModule -> ports()) {
                bool okIndex;
                int index = fromElement.attribute ("output").toInt (&okIndex);
                if (okIndex) {
                    if (port -> index() == index && port -> portType() == QNEPort::OutputPort) {
                        fromPort = port;
                        fromPort -> setName (fromElement.attribute ("name"));
                    }
                }
            }
            for (QNEPort* port : toModule -> ports()) {
                bool okIndex;
                int index = toElement.attribute ("input").toInt (&okIndex);
                if (okIndex) {
                    if (port -> index() == index && port -> portType() != QNEPort::OutputPort) {
                        toPort = port;
                        toPort -> setName (fromElement.attribute ("name"));
                    }
                }
            }
            if (fromPort && toPort) {
                connectPorts (fromPort, toPort);
            } else {
                // to do - message couldn't connect ports - but first need to implement connection names
                if (! fromPort) { CalenhadServices::messages() -> message ("", "Couldn't connect source"); }
                if (! toPort) { CalenhadServices::messages() -> message ("", "Couldn't connect target"); }
            }
        }
    }
}

QString CalenhadModel::readParameter (const QDomElement& element, const QString param) {
    QDomElement e = element.firstChildElement ("parameter");
    for (; ! e.isNull(); e = e.nextSiblingElement ("parameter")) {
        if (e.attributeNode ("name").value () == param) {
            return e.attributeNode ("value").value ();
        }
    }
    return QString::null;
}

void CalenhadModel::writeParameter (QDomElement& element, const QString& param, const QString& value) {
    QDomElement child = element.ownerDocument().createElement ("parameter");
    element.appendChild (child);
    child.setAttribute ("value", value);
    child.setAttribute ("name", param);
}

void CalenhadModel::redraw() {
    update();
    for (QGraphicsView* view : views ()) {
        view -> update();
    }
}
