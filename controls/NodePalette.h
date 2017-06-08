//
// Created by martin on 08/06/17.
//

#ifndef CALENHAD_NODEPALETTE_H
#define CALENHAD_NODEPALETTE_H


class NodePalette : public QWidget {
public:
    NodePalette (QWidget* parent);

    void mousePressEvent (QMouseEvent* event);
};


#endif //CALENHAD_NODEPALETTE_H
