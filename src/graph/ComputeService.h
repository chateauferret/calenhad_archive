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
#include <src/grid/CubicSphere.h>


//#include "../compute/compute.h"


namespace calenhad {


    namespace module {
        class Module;
    }

    namespace grid {
        class CubicSphere;
    }

    namespace graph {
        class ComputeService {
        public:
            ComputeService();
            ~ComputeService();
            void compute (calenhad::module::Module* module, calenhad::grid::CubicSphere *buffer);

        private:
            QString _codeTemplate;
            QOffscreenSurface _surface;
            QOpenGLContext _context;
            //QMap <QString, QString> _templates;
            //QString sourceCode (const QString& _codeTemplate, const QString& function);
            QOpenGLShader* _computeShader;
            QOpenGLShaderProgram* _computeProgram;
            QOpenGLFunctions_4_3_Core* f;
            GLuint _heightMap;
            void execute(GLfloat *buffer);

            QString code;
            //QOpenGLTexture* _rasterTexture;
            GLuint _rasterBuffer;
            bool _forceRender;

            void computeDetail(module::Module *module, grid::CubicSphere *buffer);
        };
    }
}



#endif //MESSAGES_COMPUTESERVICE_H
