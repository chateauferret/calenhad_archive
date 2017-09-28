//
// Created by martin on 27/09/17.
//

#ifndef CALENHAD_LEGENDPREVIEW_H
#define CALENHAD_LEGENDPREVIEW_H

#include <qwt/qwt_scale_engine.h>
#include <qwt/qwt_scale_widget.h>
#include <qwt/qwt_scale_map.h>
#include <legend/Legend.h>

namespace calenhad {
    namespace controls {
        namespace legend {
            class LegendEditor;
            class LegendPreview : public QwtScaleWidget {
            public:
                LegendPreview (QWidget* parent = NULL);
                virtual ~LegendPreview();
                LegendEditor* _editor;

            protected:
                void paintEvent (QPaintEvent* e);

                QwtScaleDraw* _draw;
                QwtLinearScaleEngine* _scaleEngine;
                QwtScaleDiv* _scaleDivisions;

                void updatePreview (calenhad::legend::Legend* legend);
            };
        }
    }
}

#endif //CALENHAD_LEGENDPREVIEW_H
