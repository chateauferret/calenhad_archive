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

class QModule;

class QNEBlockHandle : public QGraphicsItem {
public:
    enum { Type = QGraphicsItem::UserType + 5 };
    QNEBlockHandle (QModule* w, QGraphicsItem* parent = 0);
    ~QNEBlockHandle();
    void paint (QPainter* painter, const QStyleOptionGraphicsItem* option, QWidget* widget);
    virtual QRectF boundingRect() const;
    int type() const { return Type; }
    public slots:
    void refresh();
    QModule* module();
protected:
    void mousePressEvent(QGraphicsSceneMouseEvent *event);
    void mouseReleaseEvent(QGraphicsSceneMouseEvent *event);
private:
    QModule* _module;
    QRectF rect;
};



#endif //CALENHAD_QNEBLOCKHANDLE_H
