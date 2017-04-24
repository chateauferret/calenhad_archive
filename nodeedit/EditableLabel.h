//
// Created by martin on 21/04/17.
//

#ifndef CALENHAD_EDITABLELABEL_H
#define CALENHAD_EDITABLELABEL_H


#include <QtWidgets/QGraphicsTextItem>
#include <QtWidgets/QGraphicsProxyWidget>
#include <QtWidgets/QLineEdit>

class EditableLabel : public QGraphicsTextItem {
    Q_OBJECT

public:
    EditableLabel (QGraphicsItem* parent = 0);
    virtual ~EditableLabel();
    void setText (const QString& text);
    void mousePressEvent (QGraphicsSceneMouseEvent* event) override;

    public slots:
    void finishedEditing ();

    signals:
    void editingStateChanged (const bool& state);
    void textChanged (const QString& text);
    void textEdited (const QString& text);

protected:
    QGraphicsProxyWidget* _proxy;
    QLineEdit* _textEdit;

};


#endif //CALENHAD_EDITABLELABEL_H
