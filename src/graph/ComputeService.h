//
// Created by martin on 21/11/2019.
//

#ifndef MESSAGES_COMPUTESERVICE_H
#define MESSAGES_COMPUTESERVICE_H

#include <QtCore/QString>

#include <QOpenGLWidget>
#include <QtCore/QFile>
#include <QtCore/QTextStream>
#include <QtCore/QDirIterator>
#include <QOpenGLFunctions_4_3_Core>
#include <QOpenGLBuffer>
#include <QOpenGLShader>
#include <QOpenGLShaderProgram>
#include <QOpenGLTexture>
#include <QtGui/QOffscreenSurface>
#include <src/controls/globe/CalenhadStatistics.h>


//#include "../compute/compute.h"


namespace calenhad {


    namespace module {
        class Module;
        class Cache;
        class Procedure;
    }
    namespace grid {
        class CubicSphere;
    }

    namespace graph {
        class Graph;
        class ComputeService {
        public:
            ComputeService();
            ~ComputeService();
            void compute (calenhad::module::Module* module, calenhad::grid::CubicSphere *buffer);

            controls::globe::CalenhadStatistics statistics ();
            void computeStatistics (grid::CubicSphere* buffer);

        private:
            QString _computeTemplate, _processTemplate;
            QOffscreenSurface _surface;
            QOpenGLContext _context;
            //QMap <QString, QString> _templates;
            //QString sourceCode (const QString& _computeTemplate, const QString& function);
            QOpenGLShader* _computeShader;
            QOpenGLShaderProgram* _computeProgram;
            QOpenGLFunctions_4_3_Core* f;
            GLuint _heightMap, _reduction, _buckets;
            void execute(GLfloat *buffer, const calenhad::graph::Graph& graph);
            calenhad::controls::globe::CalenhadStatistics _statistics;
            QString code;
            //QOpenGLTexture* _rasterTexture;
            GLuint _rasterBuffer;
            bool _forceRender;
            uint _tileSize;

            void extractRasters (const Graph& graph, const int& xIndex, const int& yIndex);

            const uint _tiles;
            calenhad::grid::CubicSphere* _tile;

            uint setupGrid ();

            void downloadBuffer (GLfloat* buffer, const int& size);


            QString _histogramCode;
            std::vector<calenhad::grid::CubicSphere*> _cubes;

            void prepareRasters (module::Module* module);
        };
    }
}



#endif //MESSAGES_COMPUTESERVICE_H
