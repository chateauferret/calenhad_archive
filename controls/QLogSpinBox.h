//
// Created by martin on 20/12/16.
//

#ifndef CALENHAD_QLOGSPINBOX_H
#define CALENHAD_QLOGSPINBOX_H


#include <QtWidgets/QDoubleSpinBox>

class QLogSpinBox : public QDoubleSpinBox {
public:
    QLogSpinBox (QWidget* parent);
    virtual ~QLogSpinBox();
    void stepBy (int steps) override;
};


#endif //CALENHAD_QLOGSPINBOX_H
