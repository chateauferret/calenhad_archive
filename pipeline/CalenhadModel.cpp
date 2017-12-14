//
// Created by martin on 06/01/17.
//

#include <controls/QColoredIcon.h>
#include "CalenhadModel.h"
#include "../CalenhadServices.h"
#include "../nodeedit/CalenhadController.h"
#include "../nodeedit/CalenhadView.h"
#include "../nodeedit/qneconnection.h"
#include "../nodeedit/QNodeBlock.h"
#include "../qmodule/QNodeGroup.h"
#include "../icosphere/icosphere.h"
#include "../pipeline/ModuleFactory.h"
#include "../actions/DuplicateNodeCommand.h"
#include "../actions/AddNodeCommand.h"
#include "../nodeedit/QNodeGroupBlock.h"
#include "../nodeedit/qneport.h"
#include "../qmodule/QModule.h"
#include "../exprtk/CalculatorService.h"
#include "../libnoiseutils/nullmodule.h"
#include <QGraphicsSceneMouseEvent>
#include <actions/ContextAction.h>
#include "../legend/LegendService.h"
#include "../preferences/PreferencesService.h"
#include <QList>
#include <QGraphicsItem>

using namespace icosphere;
using namespace calenhad;
using namespace calenhad::pipeline;
using namespace calenhad::nodeedit;
using namespace calenhad::qmodule;
using namespace calenhad::actions;
using namespace calenhad::expressions;
using namespace calenhad::preferences;
using namespace calenhad::legend;

CalenhadModel::CalenhadModel() : QGraphicsScene(),
    conn (nullptr),
    _port (nullptr),
    _author (""),
    _title ("New model"),
    _description (""),
    _date (QDateTime::currentDateTime()),
    _controller (nullptr),
    _highlighted (nullptr),
    _menu (nullptr),
    _changed (false) {
    installEventFilter (this);
    connect (CalenhadServices::legends(), &LegendService::commitRequested, this, &CalenhadModel::commitLegends);
    connect (CalenhadServices::legends(), &LegendService::rollbackRequested, this, &CalenhadModel::rollbackLegends);
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
        if (existsPath (output -> owner() -> handle(), input -> owner() -> handle())) {
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

// Return true if there is a direct or indirect path from a given output port to a given input port.
// If there would be a path from output to input on the same node we can't create a new connection between them as that would complete a circuit.
bool CalenhadModel::existsPath (QNodeBlock* output, QNodeBlock* input) {
    if (output -> outputs().isEmpty ()) {
        return false;
    }
    QNode* outputNode = output -> node();
    QNEPort* outputPort = output -> outputs() [0];
    QModule* outputModule = dynamic_cast<QModule*> (outputNode);
    QModule* inputModule = dynamic_cast<QModule*> (input -> node());
    if (outputPort -> connections().isEmpty()) {
        return false;
    } else {
        QNEConnection* connection = outputPort->connections ()[0];
        // we're only interested in QModules here
        if ((!outputModule) || (!inputModule)) {
            return false;
        }

        // base case: block with no inputs can't have any paths to it
        if (inputModule->handle ()->inputs ().isEmpty ()) {
            return false;

            // see if the two blocks are connected
        } else {
            for (QNEPort* inputPort: inputModule->handle ()->inputs ()) {
                if (connection -> otherEnd (outputPort) == inputPort) {
                    return true;
                } else {
                    std::cout << "Looking for path " << outputPort -> owner() -> name().toStdString () << " - " << inputPort -> owner() -> name().toStdString () << "\n";
                    if (! (inputPort -> connections().isEmpty())) {
                        QNEConnection* c = inputPort -> connections()[ 0];
                        return existsPath (outputPort->block (), c -> otherEnd (inputPort) -> block());
                    }
                }
            }
        }
    }
    return false;
}

bool CalenhadModel::connectPorts (QNEPort* output, QNEPort* input) {
    if (canConnect (output, input, true)) {
        QNEConnection* c = new QNEConnection ();
        c -> setParentItem (0);
        c -> setZValue (-900);
        addItem (c);
        c -> setPort1 (output);
        c -> setPort2 (input);
        c -> updatePosFromPorts();
        c -> updatePath();


        // tell the target owner to declare change requiring rerender
        output -> owner() -> invalidate();

        // this propogates changes on the source owner to the target so that the target can update any visible views when its inputs change
        connect (output -> owner(), SIGNAL (nodeChanged()), input -> owner(), SLOT (invalidate()));

        // colour the input to show its connected status
        input -> setHighlight (QNEPort::PortHighlight::CONNECTED);

        // tell the target owner to declare change requiring rerender
        output -> owner () -> invalidate();

        // model has changed so save if close
        _changed = true;

        return true;
    } else {
        return false;
    }
}

void CalenhadModel::disconnectPorts (QNEConnection* connection) {
    if (connection -> port1()) { connection -> port1() -> initialise(); }
    if (connection -> port2()) { connection -> port2() -> initialise(); }

    // colour the input port to show its availability
    if (connection -> port1() -> type() != QNEPort::OutputPort) { connection -> port1() -> setHighlight (QNEPort::PortHighlight::NONE); }
    if (connection -> port2() -> type() != QNEPort::OutputPort) { connection -> port2() -> setHighlight (QNEPort::PortHighlight::NONE); }

    // reproduce the renders to reflect the change
//    connection -> port2() -> invalidateRenders();

    // update the model
    removeItem (connection);
    _changed = true;
    delete connection;
    update();
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
                    QPointF pos = me -> scenePos();
                    QList<QGraphicsItem*> items = QGraphicsScene::items (pos) ;
                    foreach (QGraphicsItem* item, items) {
                        // click on nothing - deselect anything selected
                        if (!item && !conn) {
                            for (QGraphicsItem* modelItem : QGraphicsScene::items ()) {
                                if (modelItem->isSelected ()) {
                                    modelItem->setSelected (false);
                                    modelItem->update ();
                                }
                            }
                        }

                        // click on an output port - create a connection which we can connect to another owner's input or control port
                        if (item && item->type () == QNEPort::Type) {
                            // only allow connections from output ports to input ports
                            QNEPort* port = ((QNEPort*) item);
                            if (port->portType () == QNEPort::OutputPort) {
                                for (QGraphicsView* view : views ()) {
                                    view->setDragMode (QGraphicsView::NoDrag);
                                }
                                if (conn) { delete conn; }
                                conn = new QNEConnection (0);
                                addItem (conn);
                                conn->setPort1 ((QNEPort*) item);
                                conn->setPos1 (item -> scenePos());
                                conn->setPos2 (me->scenePos ());
                                conn->updatePath ();
                                conn->canDrop = false;
                                return true;
                            }
                        }
                    }
                    break;
                }

                case Qt::RightButton: {

                    if (! conn) {
                        QList<QGraphicsItem*> items = QGraphicsScene::items (me -> scenePos ());
                        if (items.empty ()) {
                            QMenu* menu = makeMenu (nullptr);
                            if (menu) {
                                menu -> exec (me->screenPos ());
                            }
                        } else {
                            foreach (QGraphicsItem* item, items) {
                                QMenu* menu = makeMenu (item);
                                if (menu) {
                                    menu -> exec (me->screenPos ());
                                    break;
                                }
                            }
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
                conn -> setParentItem (0);
                conn -> setZValue (1000);

                conn -> setPos2 (me -> scenePos());
                conn -> updatePath ();
                conn -> canDrop = false;

                QList<QGraphicsItem*> items = QGraphicsScene::items (me -> scenePos ());
                foreach (QGraphicsItem* item, items) {
                        if (item && item->type () == QNEPort::Type) {
                            QNEPort* port = (QNEPort*) item;
                            if (port != conn -> port1 () && !(port->hasConnection ())) {
                                if (canConnect (conn -> port1 (), port)) {
                                    // Change colour of a port if we mouse over it and can make a connection to it
                                    port -> setHighlight (QNEPort::PortHighlight::CAN_CONNECT);
                                    _port = port;
                                }
                            }
                        } else {
                            if (_port) {
                                // If we moved off a port without making a connection to it, set it back to its unoccupied colour
                                _port -> setHighlight (QNEPort::PortHighlight::NONE);
                                _port = nullptr;
                            }
                        }
                    }
                update();
                return true;
            } else {
                if (! _activeTool) {
                    for (QGraphicsView* view : views()) {
                        view -> viewport() -> setCursor (Qt::ArrowCursor);
                        for (QNodeGroup* group : nodeGroups ()) {
                            group -> handle() -> setCursor (Qt::ClosedHandCursor);
                        }
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
                    QList<QGraphicsItem*> items = QGraphicsScene::items (me -> scenePos());
                    foreach (QGraphicsItem* item, items) {
                        if (item && item->type () == QNEPort::Type) {
                            QNEPort* port1 = conn -> port1 ();
                            QNEPort* port2 = (QNEPort*) item;
                            removeItem (conn);
                            if (! connectPorts (port1, port2)) {
                                // if connection successful, clear the connection to avoid starting a new one immediately
                                delete conn;
                                conn = nullptr;
                                return true;
                            }
                        }
                    }
                }
            }
            if (_activeTool) {
                QString type = _activeTool -> data().toString();
                if (! type.isNull ()) {
                    addNode (me->scenePos (), type);
                }
                ((Calenhad*) _controller -> parent()) -> clearTools();
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

QNode* CalenhadModel::addNode (const QPointF& initPos, const QString& type) {
    QString name = "New_" + type;
    int i = 0;
    QNode* n;
    if (type == "NodeGroup") {
        n = addNodeGroup (initPos, name);
    } else {
        n = addModule (initPos, type, name);
    }
    _changed = true;
}

QModule* CalenhadModel::addModule (const QPointF& initPos, const QString& type, const QString& name) {
    if (type != QString::null) {
        QModule* module = (QModule*) CalenhadServices::modules() -> createModule (type);
        module -> setModel (this);
        AddNodeCommand* command = new AddNodeCommand (module, initPos, this);
        _controller -> doCommand (command);
        module = (QModule*) command -> node();
        module -> setName (uniqueName (name));
        module -> setLegend (CalenhadServices::legends() -> lastUsed());
       return module;
    } else {
        CalenhadServices::messages() -> message ("error", "Couldn't create module of type " + type + "\n");
        return nullptr;
    }
}

QNodeGroup* CalenhadModel::addNodeGroup (const QPointF& initPos, const QString& name) {
    QNodeGroup* group = new QNodeGroup();
    group -> setModel (this);
    group -> initialise();
    AddNodeCommand* command = new AddNodeCommand (group, initPos, this);
    _controller -> doCommand (command);
    group = (QNodeGroup*) command -> node();
    group -> setName (uniqueName (name));

    return group;
}

QNode* CalenhadModel::addNode (QNode* node, const QPointF& initPos) {
    QNodeBlock* b = node -> makeHandle();
    addItem (b);
    b -> setPos (initPos.x(), initPos.y());

    connect (node, &QNode::nameChanged, b, &QNodeBlock::nodeChanged);
    for (QNEPort* port : node -> ports ()) {
        b -> addPort (port);
    }

    b -> assignGroup();
    b -> assignIcon();
    update();
    return node;
}

bool CalenhadModel::nameExists (const QString& name) {
    for (QNode* n : nodes()) {
        if (name == n -> name ()) {
            return true;
        }
    }
    return false;
}

void CalenhadModel::deleteNode (QNode* node) {
    // first delete any connections to or from the module
    for (QGraphicsItem* item : items()) {
        if (item -> type() == QNEConnection::Type) {
            for (QNEPort* p : node -> ports ()) {
                if (((QNEConnection*) item) -> port1() == p || ((QNEConnection*) item) -> port2() == p) {
                    removeItem (item);
                    delete item;
                    _changed = true;
                }
            }
        }
    }

    noise::module::Module* m = nullptr;
    QModule* module = dynamic_cast<QModule*> (node);

    // remove the visible appartions from the display

    for (QGraphicsItem* item : items()) {
        if (item -> type () == QNodeBlock::Type && ((QNodeBlock*) item) -> node() == node) {
            removeItem (item -> parentItem());
            delete item;
        }
    }
    delete node;
    update();
    _port = nullptr; // otherwise it keeps trying to do stuff to the deleted port
}

CalenhadModel::~CalenhadModel() {
    if (_menu) { delete _menu; }
}

QList<QNodeGroup*> CalenhadModel::nodeGroups() {
    QList<QNodeGroup*> groups;
            foreach (QGraphicsItem* item, items()) {
            int type = item -> type();
            if (type == QGraphicsItem::UserType + 3) {  // is a QNodeBlock
                QNodeBlock* handle = (QNodeBlock*) item;
                QNode* node = handle -> node();
                if (dynamic_cast<QNodeGroup*> (node)) {
                    groups.append (dynamic_cast<QNodeGroup*> (node));
                }
            }
        }
    return groups;
}

QList<QNode*> CalenhadModel::nodes() {
    QList<QNode*> nodes;
            foreach (QGraphicsItem* item, items()) {
            int type = item -> type();
            if (type == QGraphicsItem::UserType + 3) {  // is a QNodeBlock
                QNodeBlock* handle = (QNodeBlock*) item;
                QNode* node = handle -> node();
                nodes.append (node);
            }
        }
    return nodes;
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


QNodeGroup* CalenhadModel::findGroup (const QString& name) {
    for (QNodeGroup* qm : nodeGroups()) {
        if ((! qm -> name().isNull ()) && (qm -> name() == name)) {
            return qm;
        }
    }
    return nullptr;
}


QNode* CalenhadModel::findModule (const QString& name) {
    for (QNode* qm : nodes()) {
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

    }
}

CalenhadController* CalenhadModel::controller () {
    return _controller;
}


void CalenhadModel::serialize (const QString& filename, const CalenhadFileType& fileType) {
    QFile file (filename);
    QTextStream ds (&file);
    QDomDocument doc = serialize (fileType);

    std::cout << doc.toString().toStdString();
    std::cout.flush();
    if (! file.open (QIODevice::WriteOnly | QIODevice::Text )) {
        CalenhadServices::messages() -> message ("error", "Failed to open file for writing");
    } else {
        ds << doc.toString ();
        file.close ();
        CalenhadServices::messages() -> message ("info", "Wrote file " + filename);
    }
}

QDomDocument CalenhadModel::serialize (const CalenhadFileType& fileType) {
    QDomDocument doc;
    QDomElement root = doc.createElement ("model");
    doc.appendChild (root);
    writeMetadata (doc);

    // Always include all known legends in the file
    QDomElement legendsElement = doc.createElement ("legends");
    root.appendChild (legendsElement);
    for (Legend* legend : CalenhadServices::legends () -> all()) {
        legend -> serialise (doc);
    }

    if (fileType == CalenhadFileType::CalenhadModelFile) {
        CalenhadServices::calculator() -> serialize (doc);
        for (QNode* qm : nodes ()) {
            qm->serialize (doc);
        }
        for (QNEConnection* c : connections ()) {
            c->serialise (doc);
        }
        _changed = false;
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


void CalenhadModel::inflate (const QString& filename, const CalenhadFileType& fileType) {
    QDomDocument doc;
    if (CalenhadServices::readXml (filename, doc)) {
        inflate (doc, fileType);
    }
}

void CalenhadModel::inflate (const QDomDocument& doc, const CalenhadFileType& fileType) {
    readMetadata (doc);

    // Always retrieve all legends from the file
    QDomNodeList legendNodes = doc.documentElement ().elementsByTagName ("legend");
    for (int i = 0; i < legendNodes.size(); i++) {
        Legend* legend = new Legend();
        legend -> inflate (legendNodes.at (i));
        CalenhadServices::legends() -> provide (legend);
    }

    // if we are pasting, clear the selection, so that we can select the pasted items instead
    if (fileType == CalenhadFileType::CalenhadModelFragment) {
        foreach (QGraphicsItem* item, items()) {
                item -> setSelected (false);
            }
    }

    if (fileType == CalenhadFileType::CalenhadModelFile || fileType == CalenhadFileType::CalenhadModelFragment) {
        QDomElement variablesElement = doc.documentElement ().firstChildElement ("variables");
        CalenhadServices::calculator() -> inflate (variablesElement);
        QDomNodeList moduleNodes = doc.documentElement ().elementsByTagName ("module");
        QDomNodeList connectionNodes = doc.documentElement ().elementsByTagName ("connection");
        for (int i = 0; i < moduleNodes.count (); i++) {

            // put the node at the requested position on the canvas
            QDomElement positionElement = moduleNodes.at (i).firstChildElement ("position");
            int x = positionElement.attributes ().namedItem ("x").nodeValue ().toInt ();
            int y = positionElement.attributes ().namedItem ("y").nodeValue ().toInt ();
            QPointF pos (x, y);


            // if we are pasting, offset the positions so that we can see copied and pasted items separately
            if (fileType == CalenhadFileType::CalenhadModelFragment) {
                pos.setX (pos.x () + CalenhadServices::preferences ()->calenhad_module_duplicate_offset_x);
                pos.setY (pos.y () + CalenhadServices::preferences ()->calenhad_module_duplicate_offset_y);
            }

            QString type = moduleNodes.at (i).attributes ().namedItem ("type").nodeValue ();
            QDomElement nameNode = moduleNodes.at (i).firstChildElement ("name");
            QString name = nameNode.text ();
            QString newName = uniqueName (name);
            QModule* qm = addModule (pos, type, newName);
            qm -> handle() -> setSelected (fileType == CalenhadFileType::CalenhadModelFragment);
            qm->inflate (moduleNodes.at (i).toElement ());

            // update connection names so that the module is still found if it was renamed

            for (int i = 0; i < connectionNodes.count (); i++) {
                QDomElement fromElement = connectionNodes.at (i).firstChildElement ("source");
                QDomElement toElement = connectionNodes.at (i).firstChildElement ("target");
                if (fromElement.attribute ("module") == name) {
                    fromElement.setAttribute ("module", newName);
                }
                if (toElement.attribute ("module") == name) {
                    toElement.setAttribute ("module", newName);
                }
            }
            _changed = false;
        }

        // In the connections, we save and retrieve the types of output ports in case we ever have further types of output ports.
        // Does not support a port serving as both input and output (because index presently not unique across both).
        // For the time being however all output ports will be of type 2 (QNEPort::Output).


        for (int i = 0; i < connectionNodes.count (); i++) {
            QDomElement fromElement = connectionNodes.at (i).firstChildElement ("source");
            QDomElement toElement = connectionNodes.at (i).firstChildElement ("target");
            QNode* fromNode = findModule (fromElement.attributes ().namedItem ("module").nodeValue ());
            QNode* toNode = findModule (toElement.attributes ().namedItem ("module").nodeValue ());
            if (fromNode && toNode) {
                QNEPort* fromPort = nullptr, * toPort = nullptr;
                for (QNEPort* port : fromNode->ports ()) {
                    bool okIndex;
                    int index = fromElement.attribute ("output").toInt (&okIndex);
                    if (okIndex) {
                        if (port->index () == index && port->portType () == QNEPort::OutputPort) {
                            fromPort = port;
                            fromPort->setName (fromElement.attribute ("name"));
                        }
                    }
                }
                for (QNEPort* port : toNode->ports ()) {
                    bool okIndex;
                    int index = toElement.attribute ("input").toInt (&okIndex);
                    if (okIndex) {
                        if (port->index () == index && port->portType () != QNEPort::OutputPort) {
                            toPort = port;
                            toPort->setName (fromElement.attribute ("name"));
                        }
                    }
                }
                if (fromPort && toPort) {
                    connectPorts (fromPort, toPort);
                } else {
                    // to do - message couldn't connect ports - but first need to implement connection names
                    if (!fromPort) { CalenhadServices::messages ()->message ("error", "Couldn't connect source"); }
                    if (!toPort) { CalenhadServices::messages ()->message ("error", "Couldn't connect target"); }
                }
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

void CalenhadModel::highlightGroupAt (QPointF pos) {
    QList<QGraphicsItem*> list = items (pos, Qt::ContainsItemShape);
    QList<QGraphicsItem*>::iterator i = list.begin ();
    while ( i != list.end() && ! (dynamic_cast<QNodeGroupBlock*> (*i))) {
        i++;
    }
    QNodeGroupBlock* target = i == list.end() ? nullptr : (QNodeGroupBlock*) *i;
    for (QGraphicsItem* item : items()) {
        if (dynamic_cast<QNodeGroupBlock*> (item)) {
            ((QNodeGroupBlock*) item) -> setHighlight (item == target);
        }
    }
}

void CalenhadModel::commitLegends () {
    QString file = CalenhadServices::preferences() -> calenhad_legends_filename;
    serialize (file, CalenhadFileType::CalenhadLegendFile);
    _changed = true;
}

void CalenhadModel::rollbackLegends() {
    CalenhadServices::legends() -> clear();
    QString file = CalenhadServices::preferences() -> calenhad_legends_filename;
    inflate (file, CalenhadFileType::CalenhadLegendFile);
}

QMenu* CalenhadModel::makeMenu (QGraphicsItem* item) {
    if (_menu) { delete _menu; }
    if (! item) {
        _menu = new QMenu ("Model");
    }
    // construct menu for whatever item type here because QGraphicsItem does not extend QObject, so we can't call connect within QGraphicsItem
    if (dynamic_cast<QNEPort*> (item)) {

    }
    if (dynamic_cast<QNEConnection*> (item)) {
        // connection actions
        _menu = new QMenu ("Connection");
        QNEConnection* c = static_cast<QNEConnection*> (item);
        _menu -> addAction (makeMenuItem (QIcon (":/appicons/controls/disconnect.png"), "Disconnect", "Delete this connection from the model", CalenhadAction::DeleteConnectionAction, c));
    }
    if (dynamic_cast<QNodeBlock*> (item)) {
        QNodeBlock* block = static_cast<QNodeBlock*> (item);
        QNode* n = block -> node();
        _menu = new QMenu (n -> name() + " (" + n -> nodeType() + ")");
        _menu -> addAction (makeMenuItem (QIcon (":/appicons/controls/duplicate.png"), tr ("Duplicate module"), "Duplicate module", CalenhadAction::DuplicateModuleAction, block));
        _menu -> addAction (makeMenuItem (QIcon (":/appicons/controls/delete.png"), tr ("Delete module"), "Delete module", CalenhadAction::DeleteModuleAction, block));

        _menu -> addSeparator();

        QAction* editAction = new QAction (QIcon (":/appicons/controls/edit.png"), tr ("Edit"));
        editAction -> setToolTip ("Edit module's details and parameters");
        connect (editAction, &QAction::triggered, this, [=] () { n -> showParameters (true); });
        _menu -> addAction (editAction);

        if (dynamic_cast<QModule*> (n)) {
            QAction* globeAction = new QAction (QIcon (":/appicons/controls/globe.png"), "Show globe");
            connect (globeAction, &QAction::triggered, (QModule*) n, &QModule::showGlobe);
            _menu->addAction (globeAction);
        }
    }

    // actions that operate on selections
    _menu -> addSeparator();
    QAction* copy = makeMenuItem (QIcon (":/appicons/controls/copy.png"), tr ("Copy selection"), "Copy selection", CalenhadAction::CopyAction, nullptr);
    _menu -> addAction (copy);
    QAction* cut = makeMenuItem (QIcon (":/appicons/controls/cut.png"), tr ("Cut selection"), "Cut selection", CalenhadAction::CutAction, nullptr);
    _menu -> addAction (cut);
    QAction* deleteSelection = makeMenuItem (QIcon (":/appicons/controls/delete_selection.png"), tr ("Delete selection"), "Delete selection", CalenhadAction::DeleteSelectionAction, nullptr);
    _menu -> addAction (deleteSelection);
    copy -> setEnabled (selectedItems().size() > 0);
    cut -> setEnabled (selectedItems().size() > 0);
    deleteSelection -> setEnabled (selectedItems().size() > 0);
    return _menu;
}

QAction* CalenhadModel::makeMenuItem (const QIcon& icon, const QString& name, const QString& statusTip, const QVariant& id, QGraphicsItem* item) {
    ContextAction<QGraphicsItem>* action = new ContextAction<QGraphicsItem> (item, icon, name, this);
    action -> setStatusTip (statusTip);
    action -> setCheckable (false);
    action -> setData (id);
    connect (action, &QAction::triggered, _controller, &CalenhadController::actionTriggered);
    return action;
}

QString CalenhadModel::uniqueName (QString original) {
    int i = 0;
    QString suffix;
    while (nameExists (original + suffix)) {
        suffix = "_" + QString::number (i++);
        std::cout << (original + suffix).toStdString () << "\n";
    } ;
    return (original + suffix);
}

bool CalenhadModel::isChanged() {
    return _changed;
}