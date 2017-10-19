#ifndef MYGLWIDGET_H
#define MYGLWIDGET_H


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

            geoutils::Geolocation rotation();

            projection::Projection* projection ();

            icosphere::Bounds bounds ();

            void setInset (bool);
            QRectF insetRect();
            bool inset();
            bool geoCoordinates (QPointF pos, geoutils::Geolocation& geolocation);
            bool screenCoordinates (geoutils::Geolocation geolocation, QPointF& screenCoordinates);

            void setGraticuleVisible (const bool& visible);

            bool isGraticuleVisible ();
            bool isInViewport (geoutils::Geolocation g);
        public slots:

            void setProjection (const QString& projection);

        protected:
            void initializeGL ();

            void paintGL ();

            void resizeGL (int width, int height);
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

            // render pass identifiers
            static const int PASS_INSET = 1;
            static const int PASS_MAINMAP = 2;
            static const int PASS_STATISTICS = 3;

            // inset geometry
            int _insetHeight;
            QPoint _insetPos;


            // boilerplate code for compute, vertex and fragment shaders
            QString _shaderTemplate;
            QString _vertexShader;
            QString _fragmentShader;


            QOpenGLVertexArrayObject m_vao;
            QOpenGLBuffer* m_vertexBuffer;
            QOpenGLBuffer* m_indexBuffer;
            QOpenGLShaderProgram* m_computeProgram;
            QOpenGLShaderProgram* m_renderProgram;
            QOpenGLShader* m_computeShader;
            QOpenGLShader* m_fragmentShader;
            QOpenGLShader* m_vertexShader;
            QOpenGLTexture* m_texture;


        };
    }
}
#endif // MYGLWIDGET_H
