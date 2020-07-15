#ifndef CALENHAD_GLOBE_LIBRARY_H
#define CALENHAD_GLOBE_LIBRARY_H

#include <QtCore/QString>
#include "../exprtk/exprtk.hpp"

namespace calenhad {
    namespace pipeline {
        class CalenhadModel;
    }
    namespace module {
        class Module;
        class Convolution;
    }

    namespace legend {
        class Legend;
    }
    namespace graph {

        class Graph {
        public:
            //Graph (const QString& xml, const QString& nodeName);
            //Graph (const QDomDocument& doc, const QString& nodeName);
            explicit Graph (calenhad::module::Module* module);
            ~Graph();
            QString glsl();

            QImage* raster (const int& index);

            int rasterCount() const;

        protected:
            QString glsl (calenhad::module::Module* node);

            calenhad::module::Module* _module;
            pipeline::CalenhadModel* _model;
            QString _code;
            QString _nodeName;
            float* _colorMapBuffer;
            QMap<int, calenhad::module::Convolution*> _rasters;

            exprtk::parser<double>* _parser;
            int _rasterId;

        };


    }
}
#endif