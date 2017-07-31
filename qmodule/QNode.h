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
            QNode (const QString& nodeType, QWidget* parent);
            QNode (QWidget* parent) {
                 // just for now
            }
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

            void setHandle (calenhad::nodeedit::QNodeBlock* h);

            calenhad::nodeedit::QNodeBlock* handle ();

            // don't want a copy constructor because subclass implementations will have to call initialise()
            virtual QNode* clone () = 0;

            QString name ();

            void setNotes (const QString& notes);

            QString notes ();

            void addPort (calenhad::nodeedit::QNEPort* port);

            QList<calenhad::nodeedit::QNEPort*> ports ();

            void showEvent (QShowEvent* event) override;

            void closeEvent (QCloseEvent* event) override;

            calenhad::pipeline::CalenhadModel* model ();
            calenhad::expressions::ExpressionWidget* addParameter (const QString& label, const QString& name, const double& initial, std::function<void (const double& value)> onUpdate );
            virtual bool hasParameters ();

            Q_PROPERTY (QString name READ name WRITE setName MEMBER _name NOTIFY nameChanged);
            Q_PROPERTY (QString notes READ notes WRITE setNotes MEMBER _notes NOTIFY notesChanged);

            virtual bool isComplete ();

            void propertyChangeRequested (const QString& p, const QVariant& value);

            QStringList parameters();
            void showParameters (const bool& visible);
            void setParameter (const QString& label, const QString& value);
            QString parameter (const QString& label);
        public slots:

            virtual void invalidate ();
            virtual void valueReady (const double& value);
            void setName (const QString& name);

        signals:

            void nameChanged (const QString&);

            void notesChanged (const QString&);

            void nodeChanged ();



        protected:

            calenhad::pipeline::CalenhadModel* _model;
            QNodeGroup* _group;
            QDialog* _dialog;
            calenhad::nodeedit::QNodeBlock* _handle;
            QString _name;
            QString _notes;
            QLineEdit* _nameEdit;
            QTextEdit* _notesEdit;
            QToolBox* _expander;
            QList<calenhad::nodeedit::QNEPort*> _ports;
            QWidget* _content;

            QMap<QString, calenhad::expressions::ExpressionWidget*> _parameters;
            virtual void addInputPorts () = 0;

            int addPanel (const QString& name, QWidget* widget);

            QDoubleSpinBox* noiseValueParamControl (const QString& text, const QString& property = QString::null);

            QSpinBox* countParameterControl (const QString& text, const QString& property = QString::null);

            QwtCounter* angleParameterControl (const QString& text, const QString& property = QString::null);

            QFormLayout* _contentLayout;
            QDomElement _element;
            QDomDocument _document;
            QString _nodeType;
            static QString propertyName (const QString& text);

            QDoubleSpinBox* addParameter (const QString& text, const QString& property);

        };

    } // namespace qmodule
}// namespace calenhad
#endif //CALENHAD_QNODE_H
