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

//#include "../compute/compute.h"


namespace calenhad {
    namespace module {
        class Module;
    }
    namespace graph {
        class ComputeService {
        public:
            ComputeService();
            ~ComputeService();
            float* compute (calenhad::module::Module* module);

        private:
            QString _sourceCode;
            QOffscreenSurface _surface;
            QOpenGLContext _context;
            //QMap <QString, QString> _templates;
            //QString sourceCode (const QString& codeTemplate, const QString& function);
            QOpenGLShader* _computeShader;
            QOpenGLShaderProgram* _computeProgram;
            QOpenGLFunctions_4_3_Core* f;
            GLuint heightMap;
            void execute (calenhad::module::Module* module);
        };
    }
}



#endif //MESSAGES_COMPUTESERVICE_H