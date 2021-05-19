//
// Created by martin on 06/01/17.
//

#ifndef CALENHAD_CALENHADMODEL_H
#define CALENHAD_CALENHADMODEL_H


#include <QtWidgets/QGraphicsScene>
#include <QtCore/QDateTime>
#include <QtXml/QDomDocument>
#include <QAction>
#include "../module/Module.h"
#include <src/Serializable.h>
#include <QtOpenGL/QtOpenGL>
#include "../nodeedit/Calenhad.h"

namespace icosphere {
    class Icosphere;
}

namespace noise {
    namespace module {
        class NullModule;
    }
}

namespace calenhad {
    namespace module {
        class Node;
        class Module;
        class NodeGroup;
    }
    namespace nodeedit {
        class CalenhadController;
        class Connection;
        class Port;
        class NodeBlock;
    }
    namespace pipeline {

        class CalenhadModel : public QGraphicsScene {
        Q_OBJECT



        public:
            CalenhadModel ();

            ~CalenhadModel() override;

            module::Module* findModule (const QString& name);


            calenhad::module::Module* addModule (const QPointF& initPos, const QString& type, const QString& name = QString(), calenhad::module::NodeGroup* group = 0);



            bool canConnect (calenhad::nodeedit::Port* output, calenhad::nodeedit::Port* input, const bool& verbose = false);


            // undoable commands
            void doDisconnectPorts (calenhad::nodeedit::Connection* connection);
            void doDuplicateNode  (calenhad::module::Node* node);
            nodeedit::Connection* connectPorts (calenhad::nodeedit::Port* output, calenhad::nodeedit::Port* input);
            calenhad::module::Node* doCreateNode (const QPointF& initPos, const QString& type);

            void doDeleteNode (calenhad::module::Node* node);

            bool eventFilter (QObject* o, QEvent* e) override;

            void setActiveTool (QAction* tool);

            void setController (calenhad::nodeedit::CalenhadController* controller);

            calenhad::nodeedit::CalenhadController* controller ();

            void mousePressEvent (QGraphicsSceneMouseEvent* event) override;

            QDomDocument serialize (const calenhad::nodeedit::CalenhadFileType& fileType = calenhad::nodeedit::CalenhadFileType::CalenhadModelFile);
            void serialize (const QString& filename, const nodeedit::CalenhadFileType& fileType = calenhad::nodeedit::CalenhadFileType::CalenhadModelFile);
            void inflate (const QString& filename, const nodeedit::CalenhadFileType& fileType = calenhad::nodeedit::CalenhadFileType::CalenhadModelFile);
            void inflate (const QDomElement& element, const calenhad::nodeedit::CalenhadFileType& fileType = calenhad::nodeedit::CalenhadFileType::CalenhadModelFile, calenhad::module::NodeGroup* group = nullptr);
            void inflate (const QDomDocument& doc, const calenhad::nodeedit::CalenhadFileType& fileType = calenhad::nodeedit::CalenhadFileType::CalenhadModelFile);
            bool isChanged();
            QString snapshot ();
            QString lastSnapshot ();
            void setChanged (const bool& changed = true);
            void setRestorePoint (const QString& text = "Command");
            void preserve ();
            void setUndoEnabled (const bool& enabled);

            void snapToGrid (QPointF& pos);
            QList<calenhad::module::Node*> nodes ();
            QList<calenhad::module::Module*> modules();
            QList<calenhad::module::Module*> modules (calenhad::module::NodeGroup* group);
            QSet<calenhad::module::NodeGroup*> nodeGroups ();

            QList<calenhad::nodeedit::Connection*> connections ();

            calenhad::module::Node* addNode (calenhad::module::Node* node, const QPointF& initPos, calenhad::module::NodeGroup* group = 0);
            bool nameExists (const QString& name);
            QString uniqueName (QString original);
            public slots:
            void commitLegends();
            void rollbackLegends();
            bool existsPath (calenhad::nodeedit::NodeBlock* output, calenhad::nodeedit::NodeBlock* input);
            void createConnection (calenhad::nodeedit::Port* from, calenhad::nodeedit::Port* to);

            QString selectionToXml ();
            const QString& description();

            const QString& filename();

            const QDateTime& lastSave();
            const QString& author();
            const QString& title();
            void setDescription (const QString& description);

            void setAuthor (const QString& author);
            void setTitle (const QString& title);
            void restore (const QString& xml);
            void removeAll();
            void setMouseMode (QGraphicsView::DragMode mode);

        public slots:
            void goTo (module::Module* module);
            void showXml();

        signals:
            void modelChanged();
            void showMessage (QString);
            void titleChanged (QString);

        protected:

            QPointF lastClick;


            QAction* _activeTool = nullptr;
            calenhad::nodeedit::CalenhadController* _controller = nullptr;

            QString _title, _author, _description;
            QDateTime _date;

            void writeMetadata (QDomDocument& doc);
            void readMetadata (const QDomDocument& doc);
            calenhad::nodeedit::Port* _port = nullptr;
            calenhad::nodeedit::Connection* _conn;

            QSet<calenhad::module::NodeGroup*> _groups;
            QMenu* _menu;
            QMenu* _connectMenu, * _connectSubMenu;

            bool _changed;
            QString _filename;
            const QDateTime _lastSaved;
            bool _existingConnection;
            calenhad::nodeedit::Port* _wasConnectedTo;
            QString _oldXml;
            bool _undoEnabled;
            void inflateConnections (const QDomDocument& doc, const nodeedit::CalenhadFileType& fileType);
            void inflateConnections (QDomNodeList& connectionNodes);

            QGraphicsView::DragMode _dragMode = QGraphicsView::RubberBandDrag;
        };
    }
}
#endif //CALENHAD_CALENHADMODEL_H
