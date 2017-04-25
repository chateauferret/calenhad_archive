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
#include "../nodeedit/qneport.h"
#include "../controls/QLogSpinBox.h"
#include "../libnoiseutils/icospheremap.h"
#include "../CalenhadServices.h"

class CalenhadModel;
class MessageFactory;
class QToolBar;

class QNode : public QWidget {
Q_OBJECT


public:
    QNode (QWidget* widget);
    enum { Type = QGraphicsItem::UserType + 6 };
    virtual ~QNode();
    virtual void initialise();
    virtual void inflate (const QDomElement& element);
    virtual void serialise (QDomDocument& doc);
    virtual void setUniqueName() = 0;
    virtual void setModel (CalenhadModel* model);
    virtual QString moduleType () = 0;



    // don't want a copy constructor because subclass implementations will have to call initialise()
    virtual QNode* addCopy (CalenhadModel* model) = 0;
    QString name();
    void setNotes (const QString& notes);
    QString notes();
    void addPort (QNEPort* port);
    QList<QNEPort*> ports();
    bool isInitialised();
    void showEvent (QShowEvent *event) override;
    void closeEvent (QCloseEvent* event) override;
    CalenhadModel* model();

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
    QToolBar* _dialog;
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
    QDoubleSpinBox* angleParameterControl (const QString& text, const QString& property = QString::null);
    QLogSpinBox* logParameterControl (const QString& text, const QString& property = QString::null);
    QFormLayout* _contentLayout;
    bool _isInitialised;
    QDomElement _element;
    QDomDocument _document;
    static QString propertyName (const QString& text);
};


#endif //CALENHAD_QNODE_H
