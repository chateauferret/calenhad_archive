#ifndef MYGLWIDGET_H
#define MYGLWIDGET_H


#include <QOpenGLWidget>
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
        class CalenhadMapWidget : public QOpenGLWidget, protected QOpenGLFunctions_4_3_Core {
            Q_OBJECT
            QOpenGLVertexArrayObject m_vao;
            QOpenGLBuffer* m_vertexBuffer;
            QOpenGLBuffer* m_indexBuffer;
            QOpenGLShaderProgram* m_computeProgram;
            QOpenGLShaderProgram* m_renderProgram;
            QOpenGLShader* m_computeShader;
            QOpenGLShader* m_fragmentShader;
            QOpenGLShader* m_vertexShader;
            QOpenGLTexture* m_texture;
            float roll;

        public:
            CalenhadMapWidget (QWidget* parent = 0);

            ~CalenhadMapWidget ();

            void setGraph (calenhad::graph::Graph* g);

            void showEvent (QShowEvent* e) override;

            void setScale (const double& scale);

            double scale ();

            void rotate (const geoutils::Geolocation& rotation);

            geoutils::Geolocation rotation();

            void setTranslation (const QPointF& translation);

            QPointF translation();

            matrices::Mat4 modelMatrix();

            projection::Projection* projection ();

            icosphere::Bounds bounds ();

            void setParentMap (CalenhadMapWidget* parentMap);

        public slots:

            void setProjection (const QString& projection);

        protected:
            void initializeGL ();

            void paintGL ();

            void resizeGL (int width, int height);

            Viewport* _viewport;
            calenhad::graph::Graph* _graph;
            QString _shader;
            float* _altitudeMapBuffer;
            float* _colorMapBuffer;

            double _scale;
            calenhad::mapping::projection::Projection* _projection;

            geoutils::Geolocation _rotation;
            QPointF _translation;
            bool _graticule;
            CalenhadMapWidget* _parentMap;
            void drawGraticule (QPainter& p);

        };
    }
}
#endif // MYGLWIDGET_H
