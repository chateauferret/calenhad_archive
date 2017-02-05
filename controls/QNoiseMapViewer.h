//
// Created by martin on 16/12/16.
//

#ifndef CALENHAD_QSPHERICALVIEWER_H
#define CALENHAD_QSPHERICALVIEWER_H


#include <QLabel>
#include <QtWidgets/QGridLayout>
#include <QtWidgets/QProgressBar>
#include <QGraphicsPixmapItem>
#include <QGraphicsView>
#include "../pipeline/RenderJob.h"
#include <memory>
#include "../geoutils.h"
#include <marble/GeoDataLatLonBox.h>


class QModule;
class QNoiseMapExplorer;


class QNoiseMapViewer : public QWidget {
Q_OBJECT
public:
    QNoiseMapViewer (QModule* parent = 0);
    virtual ~QNoiseMapViewer();
    void setBounds (const Marble::GeoDataLatLonBox& bounds);
    Marble::GeoDataLatLonBox bounds();
    void setSize (const int& height, const int& width);
    int height();
    int width();
    bool isRendered();
    std::shared_ptr<QImage> image();
    QModule* source();
    void setSource (QModule* qm);
    void initialise();
    void showEvent (QShowEvent*) override;
    bool eventFilter (QObject*, QEvent*);

public slots:
    void jobUpdate (RenderJobStatus);
    void jobComplete ();
    void render();
    void setProgress (int p);

    signals:
    void abandon();

protected:
    Marble::GeoDataLatLonBox _bounds;
    std::shared_ptr<QImage> _image;
    QGraphicsView* _view;
    QGraphicsScene* _scene;
    QGraphicsPixmapItem* _item = nullptr;
    bool _isRendered;
    QLabel* _label;
    QModule* _source;
    RenderJob* _job = nullptr;
    QVBoxLayout* _layout;
    QWidget* _content;
    QNoiseMapExplorer* _explorer;
    QProgressBar* _progressBar;
};


#endif //CALENHAD_QSPHERICALVIEWER_H

