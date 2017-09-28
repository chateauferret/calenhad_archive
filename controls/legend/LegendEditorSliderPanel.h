//
// Created by martin on 27/09/17.
//

#ifndef CALENHAD_LEGENDEDITORSLIDERPANEL_H
#define CALENHAD_LEGENDEDITORSLIDERPANEL_H

namespace calenhad {
    namespace controls {
        namespace legend {
            class LegendEditor;
            class LegendEditorSliderPanel : public QWidget {
            Q_OBJECT
            public:
                /// Constructor
                LegendEditorSliderPanel (QWidget* parent = NULL);

                LegendEditor* _editor;

            protected slots:

                /// detect a mouse is pressed
                virtual void mousePressEvent (QMouseEvent* e) override;

                /// detect a mouse is moved
                virtual void mouseMoveEvent (QMouseEvent* e) override;

                /// detect a mouse is released
                virtual void mouseReleaseEvent (QMouseEvent* e) override;

                /// detect a mouse is released
                virtual void mouseDoubleClickEvent (QMouseEvent* e) override;

            protected:
                /// the active slider
                int activeSlider_;
            };
        }
    }
}


#endif //CALENHAD_LEGENDEDITORSLIDERPANEL_H
