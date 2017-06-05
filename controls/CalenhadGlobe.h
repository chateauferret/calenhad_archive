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
#include <qwt/qwt_thermo.h>
#include "libnoise/model/sphere.h"

class CalenhadOverviewMap;
class CalenhadLayer;
class CalenhadNavigator;
class CalenhadGlobeContextMenu;
class CalenhadGlobeConfigDialog;


namespace noise {
    namespace utils {
        class Legend;
    }
}

namespace Marble {
    class MarbleMap;
}

namespace GeographicLib {
    class Geodesic;
}


enum CalenhadGlobeDragMode { NoDrag, Pan, Zoom };
enum CalenhadGlobeDoubleClickMode { NoDoubleClick, Goto, Place };

class CalenhadGlobe : public QWidget {//}; Marble::MarbleWidget {
Q_OBJECT

    //bool isInView (const GeoDataCoordinates& coordinates);


public:
    CalenhadGlobe (QModule* source, QWidget* parent);
    virtual ~CalenhadGlobe();
    void resizeEvent (QResizeEvent* e) override;

    void mousePressEvent (QMouseEvent* e) override;
    void mouseDoubleClickEvent (QMouseEvent* event);
    void mouseMoveEvent (QMouseEvent* e) override;
    void mouseReleaseEvent (QMouseEvent* e) override;
    void paintEvent (QPaintEvent* e) override;

    const CalenhadGlobeDragMode dragMode();
    const CalenhadGlobeDoubleClickMode doubleClickMode();
    bool isCompassVisible();
    bool isZoomBarVisible();
    bool isOverviewVisible();
    bool isGraticuleVisible();
    bool isFloatItemVisible (const QString& nameId);
    void setSensitivity (const double& sensitivity);
    double sensitivity();
    GeoDataCoordinates centre();
    void zoomInTo (const GeoDataLatLonBox& target);
    void zoomOutFrom (const GeoDataLatLonBox& target);
    const Projection& projection();
    bool isInView (const GeoDataCoordinates& coordinates);
    void setLegend (Legend* legend);
    Legend* legend();

public slots:
    void setMouseDragMode (const CalenhadGlobeDragMode& mouseMode);
    void setMouseDoubleClickMode (const CalenhadGlobeDoubleClickMode& mouseMode);
    void showContextMenu (const QPoint& pos);
    void showConfigDialog();
    void invalidate ();
    void setZoom (const double& zoom);
    void changeView ();
    void showOverviewMap (const bool& show = true);
    void showZoomSlider (const bool& show = true);
    void setFloatItemVisible (const bool& visible = true);
    void setGraticuleVisible (const bool& visible = true);
    void showNavigator (const bool& show = true);
    void navigate (const NavigationEvent& e);
    void updateConfig();
    void rollbackConfig();
    void goTo (const GeoDataCoordinates& target);
    void setProjection (const Projection& projection);
    void setProgress (const int& progress);


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
    QTimer _renderTimer;
    QTimer _progressTimer;
    QwtSlider* _zoomSlider;
    QPoint _moveFrom;
    GeographicLib::Geodesic* _geodesic;
    double _sensitivity = 0.2;
    CalenhadGlobeDragMode _mouseDragMode;
    CalenhadGlobeDoubleClickMode _mouseDoubleClickMode;
    double _zoom;
    CalenhadGlobeConfigDialog* _configDialog;
    noise::model::Sphere sphereModel;

    double zoom ();
    void setFloatItemVisible (const bool& visible, const QString& nameId);
    bool _zoomDrag;
    GeoDataLatLonBox _zoomBox;
    QLabel* _positionLabel;
    QString geoLocationString (GeoDataCoordinates pos);
    int zoomFactorToRadius (const double& zoom);
    double radiusToZoomFactor (const int& radius);

    static QMap<QString, Marble::Projection> projections;

    Projection _projection;
    QwtThermo* _progressBar;
    int _progress;
    QMutex mutex;
    bool _rendering;
    int pass = 0;
    const int& getProgress ();

    int _mouseX, _mouseY;
    QString _pointingAt;
};


#endif //CALENHAD_CALENHADMARBLEWIDGET_H
