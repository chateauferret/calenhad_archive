#ifndef CALENHAD_GLOBE_LIBRARY_H
#define CALENHAD_GLOBE_LIBRARY_H

#include <libnoise/module/modulebase.h>
#include <QtCore/QString>
#include "../exprtk/exprtk.hpp"

namespace calenhad {
    namespace pipeline {
        class CalenhadModel;
    }
    namespace qmodule {
        class QModule;
    }
    namespace graph {

        class Graph {
        public:
            //Graph (const QString& xml, const QString& nodeName);
            //Graph (const QDomDocument& doc, const QString& nodeName);
            Graph (calenhad::qmodule::QModule* module);
            ~Graph();
            QString glsl();
            float* colorMapBuffer();
            int colorMapBufferSize ();
            int rasterCount ();
            QImage* raster (const int& index);
        protected:
            QString glsl (calenhad::qmodule::QModule* node);
            void parseLegend ();
            calenhad::qmodule::QModule* _module;
            pipeline::CalenhadModel* _model;
            QString _code;
            QString _nodeName;
            float* _colorMapBuffer;
            QMap<int, QImage*> _rasters;

            exprtk::parser<double>* _parser;
            int _rasterId;

        };


    }
}
#endif