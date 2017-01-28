//
// Created by martin on 05/01/17.
//

#ifndef CALENHAD_QNOISEMAPEXPLORER_H
#define CALENHAD_QNOISEMAPEXPLORER_H

#include <libnoise/module/modulebase.h>
#include "../qmodule/QNode.h"
#include "../pipeline/TileProducer.h"
#include "CalenhadMarbleWidget.h"
#include "../mapping/CalenhadLayer.h"
#include <marble/MarbleWidget.h>
#include <QDialog>

class QColorGradient;

class QNoiseMapExplorer : public QDialog {
    Q_OBJECT

public:
    QNoiseMapExplorer (const QString& title, const QWidget* parent = 0);
    virtual ~QNoiseMapExplorer();
    void setSource (noise::module::Module* source);
protected:
    CalenhadMarbleWidget* _mapWidget;
    TileProducer* _producer;
    noise::module::Module* _source = nullptr;
    CalenhadLayer* _layer = nullptr;
    QColorGradient* _gradient;
};

#endif //CALENHAD_QNOISEMAPEXPLORER_H
