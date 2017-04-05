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

enum NoiseMapPreviewType { WholeWorld, ExplorerBounds };

class QNoiseMapViewer : public QWidget {
Q_OBJECT
public:
    QNoiseMapViewer (QModule* parent = 0);
    virtual ~QNoiseMapViewer ();
    void setBounds (const Marble::GeoDataLatLonBox& bounds);
    Marble::GeoDataLatLonBox bounds ();
    void setSize (const int& height);
    int height ();
    int width ();
    bool isRendered ();
    QModule* source ();
    void setSource (QModule* qm);
    void initialise ();
    void showEvent (QShowEvent*) override;
    bool eventFilter (QObject*, QEvent*);

    signals:
    void renderComplete();

public slots:

    void jobComplete (TileId, std::shared_ptr<QImage> image);
    void render ();
    void setProgress (int p);


protected:
    Marble::GeoDataLatLonBox _bounds;
    QGraphicsView* _view;
    QGraphicsScene* _scene;
    QGraphicsPixmapItem* _item = nullptr;
    bool _isRendered;
    QLabel* _label;
    QModule* _source;
    QVBoxLayout* _layout;
    QWidget* _content;
    QNoiseMapExplorer* _explorer;
    QProgressBar* _progressBar;
    std::shared_ptr<QImage> _image;
    NoiseMapPreviewType _previewType;
    QPixmap _pixmap;
};


#endif //CALENHAD_QSPHERICALVIEWER_H

