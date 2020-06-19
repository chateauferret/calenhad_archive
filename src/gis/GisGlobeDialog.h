//
// Created by ferret on 18/06/2020.
//

#ifndef MESSAGES_GISGLOBEDIALOG_H
#define MESSAGES_GISGLOBEDIALOG_H
#include "../controls/globe/CalenhadGlobeDialog.h"
#include "GisGlobeWidget.h"

namespace calenhad {
    namespace gis {

       class GisGlobeDialog : calenhad::controls::globe::CalenhadGlobeDialog {
        Q_OBJECT

        public:
           GisGlobeDialog (QWidget* parent);
           ~GisGlobeDialog();
           GisGlobeWidget* widget();
           void initialise();

        };

    }
}

#endif //MESSAGES_GISGLOBEDIALOG_H
