//
// Created by martin on 16/01/17.
//

#ifndef CALENHAD_CALENHADMARBLEWIDGET_H
#define CALENHAD_CALENHADMARBLEWIDGET_H


#include <marble/MarbleWidget.h>
#include <QtWidgets/QPushButton>
#include "QNoiseMapViewer.h"
#include "CalenhadNavigator.h"
#include <QTimer>
#include <QSlider>
#include <qwt/qwt_slider.h>

class CalenhadOverviewMap;
class CalenhadLayer;
class CalenhadNavigator;
class CalenhadGlobeContextMenu;
class CalenhadGlobeConfigDialog;


namespace Marble {
    class MarbleMap;
}

namespace GeographicLib {
    class Geodesic;
}


enum CalenhadGlobeMouseMode { Disabled, Pan, Zoom };

class CalenhadGlobe : public QWidget {//}; Marble::MarbleWidget {
Q_OBJECT


public:
    CalenhadGlobe (QModule* source, QWidget* parent);
    virtual ~CalenhadGlobe();
    void resizeEvent (QResizeEvent* e) override;

    void mousePressEvent (QMouseEvent* e) override;
    void mouseMoveEvent (QMouseEvent* e) override;
    void mouseReleaseEvent (QMouseEvent* e) override;
    void paintEvent (QPaintEvent* e) override;

    const CalenhadGlobeMouseMode mouseMode() ;
    bool isCompassVisible ();
    bool isZoomBarVisible ();
    bool isOverviewVisible ();
    bool isFloatItemVisible (const QString& nameId);
    void setSensitivity (const double& sensitivity);
    double sensitivity();


public slots:
    void setMouseMode (const CalenhadGlobeMouseMode& mouseMode);
    void showContextMenu (const QPoint& pos);
    void showConfigDialog();
    void invalidate ();
    void setZoom (const double& zoom);
    void changeView ();
    void showOverviewMap (const bool& show = true);
    void showZoomSlider (const bool& show = true);
    void setFloatItemVisible (const bool& visible = true);
    void showNavigator (const bool& show = true);
    void navigate (const NavigationEvent& e);
    void updateConfig();

signals:
    void resized (const QSize& size);
    void viewChanged (const GeoDataLatLonAltBox&);

protected:
    CalenhadOverviewMap* _overview;
    CalenhadNavigator* _navigator;
    CalenhadGlobeContextMenu* _contextMenu;
    MarbleMap* _map;
    QModule* _source;
    CalenhadLayer* _layer = nullptr;
    QTimer timer;
    QwtSlider* _zoomSlider;
    QPoint _moveFrom;
    GeographicLib::Geodesic* _geodesic;
    double _sensitivity = 0.2;
    CalenhadGlobeMouseMode _mouseMode;
    double _zoom;
    CalenhadGlobeConfigDialog* _configDialog;
    double zoom ();

    void setFloatItemVisible (const bool& visible, const QString& nameId);

};


#endif //CALENHAD_CALENHADMARBLEWIDGET_H
