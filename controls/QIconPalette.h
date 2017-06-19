//
// Created by martin on 08/06/17.
//

#ifndef CALENHAD_NODEPALETTE_H
#define CALENHAD_NODEPALETTE_H

#include <QColormap>

class QScroller;

class QIconPalette : public QWidget {
Q_OBJECT
public:
    QIconPalette (QWidget* parent);

    void mousePressEvent (QMouseEvent* event);
    void resizeEvent (QResizeEvent* e) override;

    public slots:
    void moved (Qt::DockWidgetArea area);

protected:

    QPushButton* _leftScrollButton, * _rightScrollButton, * _topScrollButton, * _bottomScrollButton;
    void layoutIcons ();
    Qt::DockWidgetArea _area;
    QWidget* _verticalPanel, * _horizontalPanel, * _horizontalIconPanel, * _verticalIconPanel, * _iconsPanel;
    QScrollArea* _horizontalScrollPanel, * _verticalScrollPanel;
    QStringList _types;
    void updateScrollButtons ();

    int _iconSize;
};


#endif //CALENHAD_NODEPALETTE_H
