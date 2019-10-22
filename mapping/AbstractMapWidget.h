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
#include <legend/Legend.h>
#include "../matrices.h"
#include "controls/globe/CalenhadGlobeConstants.h"

namespace calenhad {
    namespace graph {
        class Graph;
    }
    namespace controls {
        namespace globe {
            class InsetWidget;
        }
    }

    namespace mapping {
        class Viewport;

        namespace projection {
            class Projection;
        }

        enum OverviewPreviewType {
            WholeWorld, ExplorerBounds
        };

        class Graticule;
        class AbstractMapWidget : public QOpenGLWidget, protected QOpenGLFunctions_4_3_Core {
            Q_OBJECT



        public:

            AbstractMapWidget (const controls::globe::RenderMode& mode = controls::globe::RenderMode::RenderModeGlobe, QWidget* parent = 0);
            ~AbstractMapWidget();
            virtual calenhad::legend::Legend* legend() = 0;

            void showEvent (QShowEvent* e) override;
            void setScale (const double& scale);
            double scale ();
            void rotate (const geoutils::Geolocation& rotation);
            const geoutils::Geolocation& rotation ();
            projection::Projection* projection ();
            calenhad::mapping::Statistics statistics ();
            bool valueAt (const QPointF& sc, double& value);
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
            GLfloat* heightMapBuffer();
            QImage* heightmap ();

            void setCreateHeightMap (const bool& createHeightMap);
            calenhad::controls::globe::CalenhadGlobeDoubleClickMode mouseDoubleClickMode();
            calenhad::controls::globe::CalenhadGlobeDragMode mouseDragMode();
            void setDatumFormat (calenhad::controls::globe::DatumFormat format);
            calenhad::controls::globe::DatumFormat datumFormat();
            void setSensitivity (double sensitivity);
            double sensitivity();
            void goTo (const geoutils::Geolocation& geolocation);

            controls::globe::RenderMode renderMode ();
            void setMainMap (AbstractMapWidget* mainMap);

        public slots:
            virtual void compute() = 0;
            void setProjection (const QString& projection);
            void navigate (const calenhad::controls::globe::NavigationEvent& e);
            virtual void render() = 0;
            void setMouseDoubleClickMode (const calenhad::controls::globe::CalenhadGlobeDoubleClickMode& mode);
            void setMouseDragMode (const calenhad::controls::globe::CalenhadGlobeDragMode& mode);

        signals:
            void rendered();
            void zoomRequested (const double& zoom);

        protected:
            void paintGL() override;
            void resizeGL (int width, int height) override;
            void paintEvent (QPaintEvent* e) override;
            geoutils::Geolocation _rotation;
            calenhad::graph::Graph* _graph;
            QString _shader;
            GLfloat* _colorMapBuffer, * _icosphereBuffer;
            GLfloat _scale;
            calenhad::mapping::projection::Projection* _projection;
            QPointF _translation;
            Graticule* _graticule;
            bool _graticuleVisible;

            geoutils::CoordinatesFormat _coordinatesFormat;

            OverviewPreviewType _previewType;
            calenhad::module::Module* _source;
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

            // boilerplate code for compute, vertex and fragment shaders
            QString _shaderTemplate;
            QString _vertexShaderCode;
            QString _fragmentShaderCode;

            QOpenGLVertexArrayObject _vao;
            QOpenGLBuffer* _indexBuffer, * _vertexBuffer;
            QOpenGLShaderProgram* _computeProgram;
            QOpenGLShaderProgram* _renderProgram;
            QOpenGLShader* _computeShader;
            QOpenGLShader* _fragmentShader;
            QOpenGLShader* _vertexShader;
            QOpenGLTexture* _globeTexture, * _rasterTexture, * _insetTexture;


            GLfloat* _heightMapBuffer;
            const char* name = "heightMapBuffer";
            QString _code;

            calenhad::controls::globe::RenderMode _mode;
            int _renderTime;

            void createTexture ();

            void updateParams ();

            GLint _tileSize;
            GLuint heightMap = 1;
            void redraw ();

            int _yTiles;
            bool _refreshHeightMap;
            clock_t start;
            int _createHeightMap;

            void prepareRasters ();
            bool _computeVertices;

            AbstractMapWidget* _mainMap;
            int textureHeight ();
        };
    }
}
#endif // CALENHADMAPWIDGET_H
