//
// Created by martin on 02/05/17.
//

#ifndef CALENHAD_CALENHADOVERVIEW_H
#define CALENHAD_CALENHADOVERVIEW_H


#include <QLabel>
#include <QtWidgets/QGridLayout>
#include <QGraphicsPixmapItem>
#include <QGraphicsView>
#include "marble/GeoDataLatLonBox.h"

class QModule;
class ImageRenderJob;

enum OverviewPreviewType { WholeWorld, ExplorerBounds };

using namespace Marble;

class CalenhadPreview : public QWidget {
Q_OBJECT
public:
    CalenhadPreview (QModule* module, QWidget* parent = 0);
    virtual ~CalenhadPreview();
    bool isRendered ();
    void showEvent (QShowEvent*) override;

    QModule* source();
    void setSource (QModule* qm);
    std::shared_ptr<QImage> image();
    virtual void initialise();
    virtual QSize renderSize();
    GeoDataLatLonBox bounds ();

    public slots:
    void render();
    virtual void setBounds (const GeoDataLatLonAltBox& bounds);
    signals:
    void renderComplete (std::shared_ptr<QImage> image);

protected:
    virtual ImageRenderJob* prepareRender();
    std::shared_ptr<QImage> _image;
    QPixmap _pixmap;
    GeoDataLatLonBox _bounds;
    OverviewPreviewType _previewType;
    bool _isRendered;
    double _ratio;
    QModule* _source;
    protected slots:
    virtual void jobComplete (std::shared_ptr<QImage> image);
};


#endif //CALENHAD_CALENHADOVERVIEW_H
