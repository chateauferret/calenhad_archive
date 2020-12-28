//
// Created by martin on 27/09/17.
//

#ifndef CALENHAD_LEGENDEDITORSCALE_H
#define CALENHAD_LEGENDEDITORSCALE_H
#include <QPaintEvent>
#include <QWidget>

namespace calenhad {
    namespace controls {
        namespace legend {
            class LegendEditorSliderPanel;

            class LegendEditorScale : public QWidget {
            Q_OBJECT


            public:
                explicit LegendEditorScale (LegendEditorSliderPanel* parent);

            protected:
                void paintEvent (QPaintEvent* e) override;
                LegendEditorSliderPanel* _panel;
            };
        }
    }
}

#endif //CALENHAD_LEGENDEDITORSCALE_H
