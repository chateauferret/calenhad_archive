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
#include <QtXml/QDomElement>
#include <QtXml/QDomDocument>
#include <QtWidgets/QGraphicsItem>
#include "../controls/QLogSpinBox.h"

class CalenhadModel;
class QNotificationFactory;
class QToolBar;
class QwtCounter;
class QModule;
class QNEPort;
class QNodeBlock;
class QNodeGroup;


class QNode : public QWidget {
Q_OBJECT


public:
    QNode (QWidget* parent);
    enum { Type = QGraphicsItem::UserType + 6 };
    virtual ~QNode();
    virtual void initialise();
    virtual void inflate (const QDomElement& element);
    virtual void serialise (QDomDocument& doc);
    virtual void setModel (CalenhadModel* model);
    virtual QString nodeType() = 0;

    void setGroup (QNodeGroup* group);
    QNodeGroup* group();

    void setHandle (QNodeBlock* h);
    QNodeBlock* handle();

    // don't want a copy constructor because subclass implementations will have to call initialise()
    virtual QNode* clone () = 0;
    QString name();
    void setNotes (const QString& notes);
    QString notes();
    void addPort (QNEPort* port);
    QList<QNEPort*> ports();
    bool isInitialised();
    void showEvent (QShowEvent *event) override;
    void closeEvent (QCloseEvent* event) override;
    CalenhadModel* model();
    virtual bool hasParameters();

    Q_PROPERTY (QString name READ name WRITE setName MEMBER _name NOTIFY nameChanged);
    Q_PROPERTY (QString notes READ notes WRITE setNotes MEMBER _notes NOTIFY notesChanged);

    virtual bool isRenderable();
    virtual bool isComplete();
    void propertyChangeRequested (const QString& p, const QVariant& value);

    void showParameters (const bool& visible);


public slots:
    virtual void invalidate();
    void setName (const QString& name);

signals:
    void nameChanged (const QString&);
    void notesChanged (const QString&);
    void nodeChanged();
    void initialised();

protected:

    CalenhadModel* _model;
    QNodeGroup* _group;
    QDialog* _dialog;
    QNodeBlock* _handle;
    QString _name;
    QString _notes;
    QLineEdit* _nameEdit;
    QTextEdit* _notesEdit;
    QToolBox* _expander;
    QList<QNEPort*> _ports;
    QWidget* _content;
    virtual void addInputPorts() = 0;
    int addPanel (const QString& name, QWidget* widget);
    QDoubleSpinBox* noiseValueParamControl (const QString& text, const QString& property = QString::null);
    QSpinBox* countParameterControl (const QString& text, const QString& property = QString::null);
    QwtCounter* angleParameterControl (const QString& text, const QString& property = QString::null);
    QLogSpinBox* logParameterControl (const QString& text, const QString& property = QString::null);
    QFormLayout* _contentLayout;
    bool _isInitialised;
    QDomElement _element;
    QDomDocument _document;
    static QString propertyName (const QString& text);
};


#endif //CALENHAD_QNODE_H
