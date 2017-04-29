//
// Created by martin on 26/04/17.
//

#ifndef CALENHAD_ANGLEDIAL_H
#define CALENHAD_ANGLEDIAL_H

#include <QtWidgets/QPushButton>
#include <QtWidgets/QLineEdit>

class QwtDial;

class AngleDial : public QWidget {
Q_OBJECT
public:
    AngleDial (QWidget* parent);
    virtual ~AngleDial();
    void initialise();
    //double value();

public slots:
    void setValue (const double&);
    void requestDial();
    //void dismissDial();

    signals:
    void valueChanged (const double& value);

protected:
    QPushButton* _dialButton;
    QDoubleSpinBox* _angleEdit;
    QwtDial* _dial;
    double _value;
};


#endif //CALENHAD_ANGLEDIAL_H
