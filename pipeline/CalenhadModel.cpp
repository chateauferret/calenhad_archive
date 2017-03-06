//
// Created by martin on 06/01/17.
//

#include "CalenhadModel.h"
#include "../nodeedit/CalenhadController.h"
#include "../nodeedit/qneconnection.h"
#include "../nodeedit/qneblock.h"
#include "../nodeedit/qnemainwindow.h"
#include "../preferences.h"
#include "../nodeedit/qneblockhandle.h"
#include "../icosphere/icosphere.h"
#include <QMouseEvent>

using namespace icosphere;

CalenhadModel::CalenhadModel() : QGraphicsScene(), conn (nullptr), _port (nullptr) {
    installEventFilter (this);

}

// determine whether connection from given input to given output is allowed
bool CalenhadModel::canConnect (QNEPort* output, QNEPort* input) {
    //To do: externalise message strings
    if (output && input) {

        // can't connect a block to itself
        if (output -> block() == input -> block()) {
            emit showMessage ("Cannot connect module to itself");
            return false;
        }

        // can only connect an output port to an input port
        if (input -> portType() ==  QNEPort::OutputPort) {
            emit showMessage ("Cannot make connection to another module output");
            return false;
        }

        // can't create circular paths so reject if a path already exists from proposed output back to proposed input
        // (in which case this connection would complete a circle)
        if (existsPath (input -> block(), output -> block())) {
            emit showMessage ("Connection would form a circuit within the network");
            return false;
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

void CalenhadModel::connectPorts (QNEPort* output, QNEPort* input) {
    if (canConnect (output, input)) {
        QNEConnection* c = new QNEConnection (0);
        addItem (c);
        c -> setPort1 (output);
        c -> setPort2 (input);
        c -> updatePosFromPorts();
        c -> updatePath();
        update();
        input -> module() -> SetSourceModule (input -> index(), *(output -> module()));
        input -> invalidateRenders();
    }
}

void CalenhadModel::disconnectPorts (QNEConnection* connection) {
    if (connection -> port1()) { connection -> port1() -> initialise (); }
    if (connection -> port2()) { connection -> port2() -> initialise (); }
    noise::module::Module* m = connection -> port2() -> module ();

    m -> SetSourceModule (connection -> port2 ()->index (), *nullModule);
    removeItem (connection);
    connection -> port2() -> invalidateRenders();
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

                    // click on an output port - create a connection which we can connect to another module's input or control port
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
                            if (item -> type() == QNEConnection::Type) {


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
                            QBrush brush = QBrush (QNEMainWindow::preferences -> calenhad_port_in_fill_color_drop);
                            QPen pen = QPen (QNEMainWindow::preferences -> calenhad_port_in_border_color_drop, QNEMainWindow::preferences -> calenhad_port_border_weight);
                            port -> setBrush (brush);
                            port -> setPen (pen);
                            conn -> canDrop = true;
                            _port = port;
                        }
                    }
                } else {
                    if (_port && ! item) {
                        // If we moved off a port without making a connection to it, set it back to its normal colour
                        QBrush brush = QBrush (QNEMainWindow::preferences -> calenhad_port_in_fill_color);
                        QPen pen = QPen (QNEMainWindow::preferences -> calenhad_port_in_border_color, QNEMainWindow::preferences -> calenhad_port_border_weight);
                        _port -> setBrush (brush);
                        _port -> setPen (pen);
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

                        if (conn -> canDrop) {
                            removeItem (conn);
                            connectPorts (port1, port2);
                            return true;
                        }
                    }
                }
            }
            if (_activeTool) {
                ModuleType type = static_cast<ModuleType> (_activeTool -> data().toInt());
                if (type) {
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


void CalenhadModel::addModule (const QPointF& initPos, const ModuleType& type, const QString& name) {
    QModule* w = _moduleFactory.createModule (type, this);
    if (! name.isNull() || name.isEmpty()) {
        w -> setUniqueName();
    } else {
        w -> setName (name); // to do - make sure this is unique
    }
    ComponentProxyWidget* proxy = new ComponentProxyWidget();
    QNEBlockHandle* handle = new QNEBlockHandle (w);
    proxy -> setWidget (w);
    proxy -> setParentItem (handle);
    QNEBlock* b = new QNEBlock (proxy);
    handle -> setPos (initPos.x(), initPos.y());
    proxy -> setPos (0, 20);
    addItem (handle);
    setFocusItem (proxy);
    w -> nextInFocusChain () -> setFocus();
    connect (w, &QNode::nodeChanged, this, [=] () { handle -> refresh(); });

    for (QNEPort* port : w -> ports()) {
        b -> addPort (port);
    }
    w -> invalidate();
}

void CalenhadModel::deleteModule (QModule* module) {
    // first delete any connections to or from the doomed module
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
        if (item -> type () == ComponentProxyWidget::Type && ((ComponentProxyWidget*) item) -> widget () == module) {
            removeItem (item -> parentItem());
            delete item;
        }
    }
    update();

    // now delete the underlying libnoise module
    if (m) { delete m; }

    _port = nullptr; // otherwise it keeps trying to do stuff to the deleted port
}

void CalenhadModel::save (QDataStream& ds) {
        foreach(QGraphicsItem* item, items ()){
            if (item -> type() == QNEBlock::Type) {
                ds << item -> type ();
                ((QNEBlock*) item) -> save (ds);
            }
        }

        foreach(QGraphicsItem* item, items ()){
            if (item -> type () == QNEConnection::Type) {
                ds << item -> type ();
                ((QNEConnection*) item) -> save (ds);
            }
        }
}

void CalenhadModel::load (QDataStream& ds) {
   clear();

    QMap<quint64, QNEPort*> portMap;

    while (! ds.atEnd ()) {
        int type;
        ds >> type;
        if (type == QNEBlock::Type) {
            QNEBlock* block = new QNEBlock (0);
            addItem (block);
            block -> load (ds, portMap);
        } else if (type == QNEConnection::Type) {
            QNEConnection* conn = new QNEConnection (0);
            addItem (conn);
            conn -> load (ds, portMap);
        }
    }
}

CalenhadModel::~CalenhadModel() {

}

QModule* CalenhadModel::findModule (const QString& name) {
    foreach (QGraphicsItem* item, items()) {
            int type = item -> type();
        if (type == QGraphicsItem::UserType + 5) {  // is a QNEBlockHandle
            QNEBlockHandle* handle = (QNEBlockHandle*) item;
            QModule* qm = handle -> module();
            if (qm -> name() == name) {
                return qm;
            }
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


