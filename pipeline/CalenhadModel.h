//
// Created by martin on 06/01/17.
//

#ifndef CALENHAD_CALENHADMODEL_H
#define CALENHAD_CALENHADMODEL_H


#include <QtWidgets/QGraphicsScene>
#include <QtCore/QDateTime>
#include <QtXml/QDomDocument>
#include <QAction>
#include <libnoise/module/modulebase.h>
#include <Serializable.h>
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
    namespace qmodule {
        class Node;
        class Module;
        class NodeGroup;
    }
    namespace nodeedit {
        class CalenhadController;
        class NodeGroupBlock;
        class Connection;
        class Port;
        class NodeBlock;
    }
    namespace pipeline {

        class CalenhadModel : public QGraphicsScene {
        Q_OBJECT



        public:
            CalenhadModel ();

            virtual ~CalenhadModel ();

            qmodule::Node* findModule (const QString& name);

            calenhad::qmodule::NodeGroup* findGroup (const QString& name);

            calenhad::qmodule::Node* addNode (const QPointF& initPos, const QString& type);

            calenhad::qmodule::Module* addModule (const QPointF& initPos, const QString& type, const QString& name = QString::null);

            calenhad::qmodule::NodeGroup* addNodeGroup (const QPointF& initPos, const QString& name);

            void deleteNode (calenhad::qmodule::Node* node);

            bool canConnect (calenhad::nodeedit::Port* output, calenhad::nodeedit::Port* input, const bool& verbose = false);

            nodeedit::Connection* connectPorts (calenhad::nodeedit::Port* output, calenhad::nodeedit::Port* input);

            void disconnectPorts (calenhad::nodeedit::Connection* connection);
            void rerouteConnection (nodeedit::Port* from, nodeedit::Port* oldPort, nodeedit::Port* newPort);
            bool eventFilter (QObject* o, QEvent* e);

            void setActiveTool (QAction* tool);

            void setController (calenhad::nodeedit::CalenhadController* controller);

            calenhad::nodeedit::CalenhadController* controller ();

            void mousePressEvent (QGraphicsSceneMouseEvent* event) override;

            QDomDocument serialize (const calenhad::nodeedit::CalenhadFileType& fileType = calenhad::nodeedit::CalenhadFileType::CalenhadModelFile);
            void serialize (const QString& filename, const nodeedit::CalenhadFileType& fileType = calenhad::nodeedit::CalenhadFileType::CalenhadModelFile);
            void inflate (const QString& filename, const nodeedit::CalenhadFileType& fileType = calenhad::nodeedit::CalenhadFileType::CalenhadModelFile);
            void inflate (const QDomDocument& doc, const calenhad::nodeedit::CalenhadFileType& fileType = calenhad::nodeedit::CalenhadFileType::CalenhadModelFile);
            bool isChanged();


            QList<calenhad::qmodule::Node*> nodes ();

            QList<calenhad::qmodule::NodeGroup*> nodeGroups ();

            QList<calenhad::nodeedit::Connection*> connections ();

            QString readParameter (const QDomElement& element, const QString param);

            void writeParameter (QDomElement& element, const QString& param, const QString& value);

            void highlightGroupAt (QPointF pos);

            calenhad::qmodule::Node* addNode (calenhad::qmodule::Node* node, const QPointF& initPos);
            bool nameExists (const QString& name);
            QString uniqueName (QString original);
            public slots:
            void commitLegends();
            void rollbackLegends();
            bool existsPath (calenhad::nodeedit::NodeBlock* output, calenhad::nodeedit::NodeBlock* input);


            const QString& description();

            const QString& filename();

            const QDateTime& lastSave();

            const QString& author();

            const QString& title();
            void setDescription (const QString& description);

            void setAuthor (const QString& author);

            void setTitle (const QString& title);


        signals:

            void showMessage (QString);
            void titleChanged (QString);

        protected:



            const noise::module::Module* _nullModule;
            QPointF lastClick;

            QAction* _activeTool = nullptr;
            calenhad::nodeedit::Connection* conn;
            calenhad::nodeedit::Port* _port = nullptr; // last port we coloured in as dropping on
            calenhad::nodeedit::CalenhadController* _controller = nullptr;

            QString _title, _author, _description;
            QDateTime _date;

            void writeMetadata (QDomDocument& doc);

            void readMetadata (const QDomDocument& doc);

            calenhad::nodeedit::NodeGroupBlock* _highlighted;



            QMenu* _menu;
            QMenu* _connectMenu, * _connectSubMenu;

            QMenu* makeMenu (QGraphicsItem* item);

            QAction* makeMenuItem (const QIcon& icon, const QString& name, const QString& statusTip, const QVariant& id, QGraphicsItem* item = 0);

            bool _changed;
            QString _filename;
            const QDateTime _lastSaved;
            bool _existingConnection;
            calenhad::nodeedit::Port* _wasConnectedTo;

        };
    }
}
#endif //CALENHAD_CALENHADMODEL_H
