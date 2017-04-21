//
// Created by martin on 16/10/16.
//

#ifndef CALENHAD_QNEBLOCKHANDLE_H
#define CALENHAD_QNEBLOCKHANDLE_H
#include <QString>
#include <QtCore/QRect>
#include <QtWidgets/QGraphicsItem>
#include <QPainter>
#include <QPen>
#include <QtWidgets/QLineEdit>

class QModule;

class QNELineEdit;

class QNEBlockHandle : public QObject, public QGraphicsItem {
    Q_OBJECT
public:
    enum { Type = QGraphicsItem::UserType + 5 };
    QNEBlockHandle (QModule* w, QGraphicsItem* parent = 0);
    ~QNEBlockHandle();
    void paint (QPainter* painter, const QStyleOptionGraphicsItem* option, QWidget* widget) override;
    virtual QRectF boundingRect() const;
    int type() const { return Type; }
    public slots:
    void refresh();
    QModule* module();
    void mousePressEvent(QGraphicsSceneMouseEvent *event) override;
    void mouseReleaseEvent(QGraphicsSceneMouseEvent *event) override;

public slots:
    void setName();

protected:

private:
    QModule* _module;
    QRectF rect;
    QLineEdit* _nameEdit;
    QGraphicsProxyWidget* _nameProxy;
    void createNameEditor ();

    void mouseDoubleClickEvent (QGraphicsSceneMouseEvent* event);

    void editModuleName ();
};



#endif //CALENHAD_QNEBLOCKHANDLE_H
