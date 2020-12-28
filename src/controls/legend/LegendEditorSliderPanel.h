//
// Created by martin on 27/09/17.
//

#ifndef CALENHAD_LEGENDEDITORSLIDERPANEL_H
#define CALENHAD_LEGENDEDITORSLIDERPANEL_H

#include <legend/Legend.h>
#include "LegendEditorSlider.h"

namespace calenhad {
    namespace controls {
        namespace legend {
            class LegendEditor;
            class LegendEditorSliderPanel : public QWidget {
            Q_OBJECT

            friend class LegendEditorScale;

            public:
                /// Constructor
                explicit LegendEditorSliderPanel (LegendEditor* parent);
                ~LegendEditorSliderPanel() override;
                void createSliders (calenhad::legend::Legend* pLegend);
                void updateSliders();
                QVector<::calenhad::legend::LegendEntry> entries ();
                void addSlider (const int& i, const int& pos);
                int orientation();

                double valueAt (const double& pos);

            protected slots:

                /// detect a mouse is pressed
                void mousePressEvent (QMouseEvent* e) override;

                /// detect a mouse is moved
                void mouseMoveEvent (QMouseEvent* e) override;

                /// detect a mouse is released
                void mouseReleaseEvent (QMouseEvent* e) override;

                /// detect a mouse is released
                //void mouseDoubleClickEvent (QMouseEvent* e) override;


            protected:

                LegendEditor* _editor;


                /// all poses with its sliders
                QList<LegendEditorSlider*> _sliders;

                /// sort the slider list
                static bool SliderSort (const LegendEditorSlider* a1, const LegendEditorSlider* a2);

                /// the active slider
                int _activeSlider;

                int updatePos (LegendEditorSlider* sl);

                /// bound space
                int _bspace;
                void setSlider (const int& index, const QString& key, const QColor& col);

                void deleteSlider (const int& index);

                double posForValue (const double& value);

                qreal updateKey (LegendEditorSlider* sl);
            };
        }
    }
}


#endif //CALENHAD_LEGENDEDITORSLIDERPANEL_H
