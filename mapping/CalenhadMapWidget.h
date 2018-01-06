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
#include "../matrices.h"

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
        public slots:

            void compute ();
            void setProjection (const QString& projection);

        signals:
            void rendered (const bool& success);

        protected:
            void initializeGL() override;
            void paintGL() override;
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

            // render pass identifiers
            static const int PASS_INSET = 1;
            static const int PASS_MAINMAP = 2;
            static const int PASS_STATISTICS = 3;

            // inset geometry
            int _insetHeight;
            QPoint _insetPos;


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


            float* _rasterBuffer;
        };
    }
}
#endif // CALENHADMAPWIDGET_H
