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
            class LegendEditor;

            class LegendEditorScale : public QWidget {
            Q_OBJECT


            public:
                LegendEditorScale (QWidget* parent = NULL);
                void setEditor (LegendEditor* editor);
            protected:
                LegendEditor* _editor;
                void paintEvent (QPaintEvent* e) override;
            };
        }
    }
}

#endif //CALENHAD_LEGENDEDITORSCALE_H
