//
// Created by martin on 31/03/17.
//

#ifndef CALENHAD_ALTITUDEMAPEDITOR_H
#define CALENHAD_ALTITUDEMAPEDITOR_H


#include <QtWidgets/QDialog>
#include <QtCore/QMap>
#include <qwt/qwt_series_data.h>

class AltitudeMapPlot;

class AltitudeMapEditor : public QDialog {
    Q_OBJECT

public:
    AltitudeMapEditor (QWidget* parent = 0);
    ~AltitudeMapEditor();
    QVector<QPointF> getEntries();

    public slots:
    void setEntries (const QVector<QPointF>& entries);
    void accept();
    void reject();

protected:
    AltitudeMapPlot* _plot;
};



#endif //CALENHAD_ALTITUDEMAPEDITOR_H
