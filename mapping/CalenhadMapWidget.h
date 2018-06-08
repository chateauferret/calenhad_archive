#ifndef CALENHADMAPWIDGET_H
#define CALENHADMAPWIDGET_H


#include <QOpenGLWidget>
#include <QWidget>
#include <QOpenGLFunctions_4_3_Core>
#include <QOpenGLBuffer>
#include <QOpenGLShader>
#include <QOpenGLShaderProgram>
#include <QOpenGLTexture>
#include <QOpenGLVertexArrayObject>
#include <QDir>
#include <geoutils.h>
#include <icosphere/Bounds.h>
#include <controls/globe/HypsographyWidget.h>
#include <GeographicLib/Geodesic.hpp>
#include <controls/globe/CalenhadNavigator.h>
#include "../matrices.h"
#include "controls/globe/CalenhadGlobeConstants.h"

namespace calenhad {
    namespace graph {
        class Graph;
    }
    namespace mapping {
        class Viewport;

        namespace projection {
            class Projection;
        }
    }

    namespace mapping {

        enum OverviewPreviewType {
            WholeWorld, ExplorerBounds
        };

        class Graticule;
        class CalenhadMapWidget : public QOpenGLWidget, protected QOpenGLFunctions_4_3_Core {
            Q_OBJECT
        public:



            CalenhadMapWidget (QWidget* parent = 0);

            ~CalenhadMapWidget ();

            void setGraph (calenhad::graph::Graph* g);

            void showEvent (QShowEvent* e) override;

            void setScale (const double& scale);

            double scale ();

            void rotate (const geoutils::Geolocation& rotation);

            const geoutils::Geolocation& rotation ();

            projection::Projection* projection ();

            icosphere::Bounds bounds ();
            calenhad::mapping::Statistics statistics ();
            bool valueAt (const QPointF& sc, double& value);

            void setInset (bool);
            QRectF insetRect();
            bool inset();
            bool geoCoordinates (QPointF pos, geoutils::Geolocation& geolocation);
            QPoint texCoordinates (const QPointF& sc);
            bool screenCoordinates (geoutils::Geolocation geolocation, QPointF& sc);
            void setCoordinatesFormat (geoutils::CoordinatesFormat format);
            geoutils::CoordinatesFormat coordinatesFormat();
            void setGraticuleVisible (const bool& visible);

            Graticule* graticule ();
            bool isGraticuleVisible ();
            bool isInViewport (geoutils::Geolocation g);

            QSize heightMapSize () const;
            GLfloat* heightMapBuffer() const;
            QImage* heightmap ();

            calenhad::qmodule::Module* source ();

            void setSource (calenhad::qmodule::Module* qm);

            void zoomInTo (const icosphere::Bounds& target);

            void zoomOutFrom (const icosphere::Bounds& target);

            calenhad::controls::globe::CalenhadGlobeDoubleClickMode mouseDoubleClickMode();
            calenhad::controls::globe::CalenhadGlobeDragMode mouseDragMode();

            void setDatumFormat (calenhad::controls::globe::DatumFormat format);
            calenhad::controls::globe::DatumFormat datumFormat();
            void setSensitivity (double sensitivity);
            double sensitivity();
            void goTo (const geoutils::Geolocation& geolocation);


        public slots:
            void compute ();
            void setProjection (const QString& projection);
            void navigate (const calenhad::controls::globe::NavigationEvent& e);
            void render ();
            void setMouseDoubleClickMode (const calenhad::controls::globe::CalenhadGlobeDoubleClickMode& mode);
            void setMouseDragMode (const calenhad::controls::globe::CalenhadGlobeDragMode& mode);

        signals:
            void rendered (const bool& success);
            void zoomRequested (const double& zoom);

        protected:
            void initializeGL() override;
            void paintGL() override;
            void paintEvent (QPaintEvent* e) override;
            void resizeGL (int width, int height) override;
            geoutils::Geolocation _rotation;
            calenhad::graph::Graph* _graph;
            QString _shader;
            float* _colorMapBuffer;
            double _scale;
            calenhad::mapping::projection::Projection* _projection;
            QPointF _translation;
            Graticule* _graticule;
            bool _graticuleVisible;
            bool _inset;
            geoutils::CoordinatesFormat _coordinatesFormat;

            icosphere::Bounds _bounds;
            OverviewPreviewType _previewType;
            calenhad::qmodule::Module* _source;

            icosphere::Bounds _zoomBox;
            bool _zoomDrag;

            double _sensitivity = 0.2;
            calenhad::controls::globe::DatumFormat _datumFormat;

            void mousePressEvent (QMouseEvent* e);

            void mouseDoubleClickEvent (QMouseEvent* e);

            void mouseMoveEvent (QMouseEvent* e);

            void mouseReleaseEvent (QMouseEvent* e);

            QPoint _moveFrom;
            calenhad::controls::globe::CalenhadGlobeDragMode _mouseDragMode;
            calenhad::controls::globe::CalenhadGlobeDoubleClickMode _mouseDoubleClickMode;


            void wheelEvent (QWheelEvent* event);

            GeographicLib::Geodesic* _geodesic;

            // render pass identifiers
            static const int PASS_INSET = 1;
            static const int PASS_MAINMAP = 2;
            static const int PASS_STATISTICS = 3;

            // inset geometry
            double _insetHeight;

            // boilerplate code for compute, vertex and fragment shaders
            QString _shaderTemplate;
            QString _vertexShaderCode;
            QString _fragmentShaderCode;


            QOpenGLVertexArrayObject m_vao;
            QOpenGLBuffer* _vertexBuffer;
            QOpenGLBuffer* _indexBuffer;
            QOpenGLShaderProgram* _computeProgram;
            QOpenGLShaderProgram* _renderProgram;
            QOpenGLShader* _computeShader;
            QOpenGLShader* _fragmentShader;
            QOpenGLShader* _vertexShader;
            QOpenGLTexture* _globeTexture, * _rasterTexture;


            GLfloat* _heightMapBuffer;
            const char* name = "heightMapBuffer";
            QString _code;

            float* _rasterBuffer;
        };
    }
}
#endif // CALENHADMAPWIDGET_H
