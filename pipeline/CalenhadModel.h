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
        class QNode;
        class QModule;
        class QNodeGroup;
    }
    namespace nodeedit {
        class CalenhadController;
        class QNodeGroupBlock;
        class QNEConnection;
        class QNEPort;
        class QNodeBlock;
    }
    namespace pipeline {

        class CalenhadModel : public QGraphicsScene {
        Q_OBJECT
        public:
            CalenhadModel ();

            virtual ~CalenhadModel ();

            qmodule::QNode* findModule (const QString& name);

            calenhad::qmodule::QNodeGroup* findGroup (const QString& name);

            calenhad::qmodule::QNode* addNode (const QPointF& initPos, const QString& type);

            calenhad::qmodule::QModule* addModule (const QPointF& initPos, const QString& type, const QString& name = QString::null);

            calenhad::qmodule::QNodeGroup* addNodeGroup (const QPointF& initPos, const QString& name);

            void deleteNode (calenhad::qmodule::QNode* node);

            bool canConnect (calenhad::nodeedit::QNEPort* output, calenhad::nodeedit::QNEPort* input, const bool& verbose = false);

            bool connectPorts (calenhad::nodeedit::QNEPort* output, calenhad::nodeedit::QNEPort* input);

            void disconnectPorts (calenhad::nodeedit::QNEConnection* connection);

            bool eventFilter (QObject* o, QEvent* e);

            void setActiveTool (QAction* tool);

            QList<QGraphicsItem>* itemAt (const QPointF& pos);

            void setController (calenhad::nodeedit::CalenhadController* controller);

            calenhad::nodeedit::CalenhadController* controller ();

            void mousePressEvent (QGraphicsSceneMouseEvent* event) override;

            QDomDocument serialize (const calenhad::nodeedit::CalenhadFileType& fileType = calenhad::nodeedit::CalenhadFileType::CalenhadModelFile);
            void serialize (const QString& filename, const nodeedit::CalenhadFileType& fileType = calenhad::nodeedit::CalenhadFileType::CalenhadModelFile);
            void inflate (const QString& filename, const nodeedit::CalenhadFileType& fileType = calenhad::nodeedit::CalenhadFileType::CalenhadModelFile);
            void inflate (const QDomDocument& doc, const calenhad::nodeedit::CalenhadFileType& fileType = calenhad::nodeedit::CalenhadFileType::CalenhadModelFile);
            bool isChanged();


            QList<calenhad::qmodule::QNode*> nodes ();

            QList<calenhad::qmodule::QNodeGroup*> nodeGroups ();

            QList<calenhad::nodeedit::QNEConnection*> connections ();

            QString readParameter (const QDomElement& element, const QString param);

            void writeParameter (QDomElement& element, const QString& param, const QString& value);

            void highlightGroupAt (QPointF pos);

            calenhad::qmodule::QNode* addNode (calenhad::qmodule::QNode* node, const QPointF& initPos);
            bool nameExists (const QString& name);
            QString uniqueName (QString original);
            public slots:
            void commitLegends();
            void rollbackLegends();
            bool existsPath (calenhad::nodeedit::QNodeBlock* output, calenhad::nodeedit::QNodeBlock* input);

        signals:

            void showMessage (QString);

        protected:



            const noise::module::Module* _nullModule;
            QPointF lastClick;

            QAction* _activeTool = nullptr;
            calenhad::nodeedit::QNEConnection* conn;
            calenhad::nodeedit::QNEPort* _port = nullptr; // last port we coloured in as dropping on
            calenhad::nodeedit::CalenhadController* _controller = nullptr;

            QString _title, _author, _description;
            QDateTime _date;

            void writeMetadata (QDomDocument& doc);

            void readMetadata (const QDomDocument& doc);

            calenhad::nodeedit::QNodeGroupBlock* _highlighted;



            QMenu* _menu;

            QMenu* makeMenu (QGraphicsItem* item);

            QAction* makeMenuItem (const QIcon& icon, const QString& name, const QString& statusTip, const QVariant& id, QGraphicsItem* item = 0);

            bool _changed;
        };
    }
}
#endif //CALENHAD_CALENHADMODEL_H
