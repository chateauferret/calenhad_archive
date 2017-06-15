//
// Created by martin on 08/06/17.
//

#ifndef CALENHAD_NODEPALETTE_H
#define CALENHAD_NODEPALETTE_H

class QScroller;

class NodePalette : public QWidget {
Q_OBJECT
public:
    NodePalette (QWidget* parent);

    void mousePressEvent (QMouseEvent* event);
    void resizeEvent (QResizeEvent* e) override;

    public slots:
    void moved (Qt::DockWidgetArea area);

protected:

    QPushButton* _leftScrollButton, * _rightScrollButton, * _topScrollButton, * _bottomScrollButton;
    void layoutIcons ();

    Qt::DockWidgetArea _area;
    QWidget* _horizontalPanel, * _verticalPanel, * _iconsPanel;
    QScrollArea* _horizontalScrollPanel, * _verticalScrollPanel;
    QStringList _types;

    void updateScrollButtons ();
};


#endif //CALENHAD_NODEPALETTE_H
