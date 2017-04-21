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
    // don't want a copy constructor because subclass implementations will have to call initialise()
    virtual QNode* addCopy (CalenhadModel* model) = 0;
    QString name();
    void setNotes (const QString& notes);
    QString notes();
    void addPort (QNEPort* port);
    QList<QNEPort*> ports();
    bool isInitialised();
    void showEvent (QShowEvent *event) override;
    QString preservedXml();

    Q_PROPERTY (QString name READ name WRITE setName MEMBER _name);
    Q_PROPERTY (QString notes READ notes WRITE setNotes MEMBER _notes);

    virtual bool isRenderable();
    virtual bool isComplete();


public slots:
    virtual void invalidate();
    void setName (const QString& name);


signals:
    void nodeChanged (const QString&, const QVariant&);
    void nameChanged (const QString&);
    void notesChanged (const QString&);
    void initialised();

protected:

    CalenhadModel* _model;
    QDialog* _dialog;
    QString _name;
    QString _notes;
    QLineEdit* _nameEdit;
    QTextEdit* _notesEdit;
    QToolBox* _expander;
    QList<QNEPort*> _ports;
    QWidget* _content;
    QString _preservedXml;
    void preserve();
    virtual void addInputPorts() = 0;
    int addPanel (const QString& name, QWidget* widget);
    QDoubleSpinBox* noiseValueParamControl (const QString& text);
    QSpinBox* countParameterControl (const QString& text);
    QDoubleSpinBox* angleParameterControl (const QString& text);
    QLogSpinBox* logParameterControl (const QString& text);
    QFormLayout* _contentLayout;
    bool _isInitialised;
    QDomElement _element;
    QDomDocument _document;
    bool _warnings;

};


#endif //CALENHAD_QNODE_H
