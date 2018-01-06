//
// Created by martin on 16/12/16.
//

#ifndef CALENHAD_QNODE_H
#define CALENHAD_QNODE_H


#include <QWidget>
#include <QtWidgets/QFormLayout>
#include <QtWidgets/QToolBox>
#include <QtWidgets/QDoubleSpinBox>
#include <QtWidgets/QTextEdit>
#include <QtCore/QSet>

#include <QtWidgets/QGraphicsItem>
#include <qwt/qwt_counter.h>
#include <QtXml/QDomElement>
#include <exprtk/ExpressionWidget.h>
#include "Serializable.h"
#include "ParamValidator.h"
#include <limits>

namespace calenhad {
    namespace pipeline {
        class CalenhadModel;
    }
    namespace notification {
        class QNotificationFactory;
    }
    namespace nodeedit {
        class QNEPort;
        class QNodeBlock;
        class NodeNameValidator;
    }
    namespace expressions {
        class ExpressionWidget;
    }
    namespace qmodule {
        class QModule;
        class QNodeGroup;

        class QNode : public QWidget, public Serializable {
        Q_OBJECT



        public:
            QNode (const QString& nodeType, int inputs = 0, QWidget* parent = 0);
            QNode (QWidget* parent) {
                 // just for now
            }

            void connectMenu (QMenu* menu, calenhad::nodeedit::QNEPort* port);

            enum {
                Type = QGraphicsItem::UserType + 6
            };

            virtual ~QNode ();

            virtual void initialise ();

            virtual void inflate (const QDomElement& element) override;

            virtual void serialize (QDomDocument& doc) override;

            virtual void setModel (calenhad::pipeline::CalenhadModel* model);

            virtual QString nodeType();

            void setGroup (QNodeGroup* group);

            QNodeGroup* group ();

            calenhad::nodeedit::QNodeBlock* handle ();

            virtual QNode* clone();

            QString name ();

            void setNotes (const QString& notes);

            QString notes ();

            void addPort (calenhad::nodeedit::QNEPort* port);

            QList<calenhad::nodeedit::QNEPort*> ports ();

            void showEvent (QShowEvent* event) override;

            void closeEvent (QCloseEvent* event) override;

            calenhad::pipeline::CalenhadModel* model ();
            calenhad::expressions::ExpressionWidget* addParameter (const QString& label, const QString& name, const double& initial, ParamValidator* validator = new AcceptAnyRubbish(), QWidget* panel = 0);
            virtual bool hasParameters ();

            Q_PROPERTY (QString name READ name WRITE setName MEMBER _name NOTIFY nameChanged);
            Q_PROPERTY (QString notes READ notes WRITE setNotes MEMBER _notes NOTIFY notesChanged);

            virtual bool isComplete ();

            void propertyChangeRequested (const QString& p, const QVariant& value);

            QStringList parameters();

            void setParameter (const QString& name, const double& value);
            void setParameter (const QString& label, const QString& value);
            QString parameter (const QString& label);
            int id();
            calenhad::nodeedit::QNEPort* output();
            virtual nodeedit::QNodeBlock* makeHandle ();
            double parameterValue (const QString& name);

        public slots:
            void showParameters (const bool& visible = true);
            virtual void invalidate ();
            void setName (const QString& name);
            virtual void parameterChanged();
        signals:

            void nameChanged (const QString&);

            void notesChanged (const QString&);

            void nodeChanged ();



        protected:
            int _id;
            QNodeGroup* _group;
            QDialog* _dialog;
            calenhad::nodeedit::QNodeBlock* _handle;
            QString _name;
            QString _notes;
            QLineEdit* _nameEdit;
            QTextEdit* _notesEdit;
            QToolBox* _expander;
            QList<calenhad::nodeedit::QNEPort*> _ports;
            calenhad::nodeedit::QNEPort* _output;
            QWidget* _content;
            int _inputCount;
            calenhad::pipeline::CalenhadModel* _model;

            QMap<QString, calenhad::expressions::ExpressionWidget*> _parameters;
            virtual void addInputPorts () = 0;

            int addPanel (const QString& name, QWidget* widget);

            QFormLayout* _contentLayout;
            QDomElement _element;
            QDomDocument _document;
            QString _nodeType;
            static QString propertyName (const QString& text);

            void addContentPanel ();

            calenhad::nodeedit::NodeNameValidator* _validator;
            QPalette* _palette;


            QMenu* _connectMenu;

        };

    } // namespace qmodule
}// namespace calenhad
#endif //CALENHAD_QNODE_H
