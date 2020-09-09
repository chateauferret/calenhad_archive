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
#include <src/geoutils.h>
#include <GeographicLib/Geodesic.hpp>
#include <controls/globe/CalenhadNavigator.h>
#include <legend/Legend.h>
#include <src/controls/globe/CalenhadStatsPanel.h>
#include "src/matrices.h"
#include "controls/globe/CalenhadGlobeConstants.h"

namespace calenhad {
    namespace module {
        class Module;
    }
    namespace graph {
        class Graph;
    }
    namespace controls {
        namespace globe {
            class InsetWidget;
        }
    }

    namespace grid {
        class CubicSphere;
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
        class CalenhadMapWidget : public QOpenGLWidget, protected QOpenGLFunctions_4_3_Core {
            Q_OBJECT

        public:
            explicit CalenhadMapWidget (const controls::globe::RenderMode& mode = controls::globe::RenderModeGlobe, QWidget* parent = 0);

            ~CalenhadMapWidget() override;
            legend::Legend* legend();

            void setSource (module::Module* qm);

            void subscribe (calenhad::controls::globe::CalenhadStatsPanel* panel);

            void compute();

            void render();

            module::Module* source ();

            void showEvent (QShowEvent* event) override;
            void setScale (const double& scale);
            double scale ();
            void rotate (const geoutils::Geolocation& rotation);
            const geoutils::Geolocation& rotation ();
            projection::Projection* projection ();
            calenhad::grid::CubicSphere* buffer();
            bool geoCoordinates (QPointF pos, geoutils::Geolocation& geolocation);
            QPoint texCoordinates (const QPointF& sc);
            bool screenCoordinates (const geoutils::Geolocation& geolocation, QPointF& sc);
            void setCoordinatesFormat (geoutils::CoordinatesFormat format);
            geoutils::CoordinatesFormat coordinatesFormat();
            void setGraticuleVisible (const bool& visible);
            Graticule* graticule ();
            bool isGraticuleVisible () const;
            bool isInViewport (geoutils::Geolocation g);
            QSize heightMapSize () const;
            calenhad::grid::CubicSphere* heightMapBuffer();
            void setCreateHeightMap (const bool& createHeightMap);
            calenhad::controls::globe::CalenhadGlobeDoubleClickMode mouseDoubleClickMode();
            calenhad::controls::globe::CalenhadGlobeDragMode mouseDragMode();
            void setDatumFormat (calenhad::controls::globe::DatumFormat format);
            calenhad::controls::globe::DatumFormat datumFormat();
            void setSensitivity (double sensitivity);
            double sensitivity() const;
            void goTo (const geoutils::Geolocation& geolocation);

            controls::globe::RenderMode renderMode ();
            void setMainMap (CalenhadMapWidget* mainMap);
            void setLegend (legend::Legend *legend);
        public slots:

            void setProjection (const QString& projection);
            void navigate (const calenhad::controls::globe::NavigationEvent& e);

            void exportImages();
            void setMouseDoubleClickMode (const calenhad::controls::globe::CalenhadGlobeDoubleClickMode& mode);
            void setMouseDragMode (const calenhad::controls::globe::CalenhadGlobeDragMode& mode);

            int textureHeight ();
        signals:
            void rendered();
            void computed();
            void zoomRequested (const double& zoom);
            void zoomInRequested();
            void zoomOutRequested();

        protected:
            void paintGL() override;
            void resizeGL (int width, int height) override;
            void paintEvent (QPaintEvent* e) override;
            geoutils::Geolocation _rotation;
            calenhad::graph::Graph* _graph;
            QString _shader;
            GLfloat* _colorMapBuffer, * _icosphereBuffer{};
            GLfloat _scale;
            calenhad::mapping::projection::Projection* _projection;
            QPointF _translation;
            Graticule* _graticule;
            bool _graticuleVisible;

            geoutils::CoordinatesFormat _coordinatesFormat;
            void notifySubscribers();
            OverviewPreviewType _previewType;
            module::Module *_source;
            bool _zoomDrag;

            double _sensitivity = 0.2;
            calenhad::controls::globe::DatumFormat _datumFormat;

            void mousePressEvent (QMouseEvent* e) override;

            void mouseDoubleClickEvent (QMouseEvent* e) override;

            void mouseMoveEvent (QMouseEvent* e) override;

            void mouseReleaseEvent (QMouseEvent* e) override;

            QPoint _moveFrom;
            calenhad::controls::globe::CalenhadGlobeDragMode _mouseDragMode;
            calenhad::controls::globe::CalenhadGlobeDoubleClickMode _mouseDoubleClickMode;


            void wheelEvent (QWheelEvent* event) override;

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
            QOpenGLTexture* _globeTexture, * _rasterTexture, * _insetTexture{};


            calenhad::grid::CubicSphere* _buffer;
            const char* name = "heightMapBuffer";
            QString _code;

            calenhad::controls::globe::RenderMode _mode;
            int _renderTime;

            void createTexture ();
            GLuint colorMap = 1;
            GLint _tileSize;
            GLuint heightMap = 1;
            void redraw ();

            int _yTiles{};
            bool _refreshHeightMap;
            clock_t start{};
            int _createHeightMap;
            bool _computeVertices;

            CalenhadMapWidget* _mainMap;

            void initializeGL() override;

            bool _computeCubeMap{};
            GLfloat* _cubeMapBuffer{};
            GLfloat* _gridBuffer{};

            calenhad::legend::Legend*  _legend;
            int _size;

            QVector<calenhad::controls::globe::CalenhadStatsPanel*>  _subscribers;
        };
    }
}
#endif // CALENHADMAPWIDGET_H
