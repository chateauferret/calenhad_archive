#ifndef CALENHAD_GLOBE_LIBRARY_H
#define CALENHAD_GLOBE_LIBRARY_H

#include <libnoise/module/modulebase.h>
#include <QtCore/QString>
#include <QDomNode>
#include "../exprtk/exprtk.hpp"

namespace calenhad {
    namespace pipeline {
        class CalenhadModel;
    }
    namespace qmodule {
        class QNode;
    }
    namespace graph {

        class Graph {
        public:
            Graph (const QString& xml, const QString& nodeName);
            Graph (const QDomDocument& doc, const QString& nodeName);
            ~Graph();
            QString glsl();
            float* altitudeMapBuffer ();
            int altitudeMapBufferSize();
            float* colorMapBuffer();
            int colorMapBufferSize ();
            QString readParameter (const QDomElement& element, const QString param);

        protected:
            QDomNode _node;
            QString glsl (const QDomNode& node);
            void parseLegend ();

            pipeline::CalenhadModel* _model;
            QDomNodeList _connections;
            QDomNodeList list;

            QString addAltitudeMapBuffer (QDomElement map);
            QString _code;
            QString _xml;
            QDomDocument _doc;
            QString _nodeName;
            int _bufferCount;
            float* _altitudeMapBuffer;
            float* _colorMapBuffer;


            exprtk::parser<double>* _parser;
        };


    }
}
#endif