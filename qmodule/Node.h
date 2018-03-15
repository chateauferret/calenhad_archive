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
        class Port;
        class NodeBlock;
        class NodeNameValidator;
    }
    namespace expressions {
        class ExpressionWidget;
    }
    namespace qmodule {
        class Module;
        class NodeGroup;

        class Node : public QWidget, public Serializable {
        Q_OBJECT



        public:
            Node (const QString& nodeType, QWidget* parent = 0);
            Node (QWidget* parent) {
                 // just for now
            }

            void connectMenu (QMenu* menu, calenhad::nodeedit::Port* port);

            enum {
                Type = QGraphicsItem::UserType + 6
            };

            virtual ~Node ();

            virtual void initialise ();

            virtual void inflate (const QDomElement& element) override;

            virtual void serialize (QDomDocument& doc) override;

            virtual void setModel (calenhad::pipeline::CalenhadModel* model);

            virtual QString nodeType();

            void setGroup (NodeGroup* group);

            NodeGroup* group ();

            calenhad::nodeedit::NodeBlock* handle ();

            virtual Node* clone();

            QString name ();

            void setNotes (const QString& notes);

            QString notes ();

            void addPort (calenhad::nodeedit::Port* port, const unsigned& index = 0);

            QVector<nodeedit::Port*> ports ();

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
            calenhad::nodeedit::Port* output();
            virtual nodeedit::NodeBlock* makeHandle ();
            double parameterValue (const QString& name);
            virtual void addDependentNodes();

        public slots:
            virtual void showModuleDetail (const bool& visible = true);
            virtual void invalidate ();
            void setName (const QString& name);
            virtual void parameterChanged();
        signals:

            void nameChanged (const QString&);

            void notesChanged (const QString&);

            void nodeChanged ();



        protected:
            int _id;
            NodeGroup* _group;
            QDialog* _dialog;
            calenhad::nodeedit::NodeBlock* _handle;
            QString _name;
            QString _notes;
            QLineEdit* _nameEdit;
            QTextEdit* _notesEdit;
            QToolBox* _expander;
            QVector<calenhad::nodeedit::Port*> _ports;
            calenhad::nodeedit::Port* _output;
            QMap<unsigned, calenhad::nodeedit::Port*> _inputs;
            QWidget* _content;
            calenhad::pipeline::CalenhadModel* _model;

            QMap<QString, calenhad::expressions::ExpressionWidget*> _parameters;
            virtual void addInputPorts();

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
