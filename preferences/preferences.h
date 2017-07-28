//
// Created by martin on 16/10/16.
//

#ifndef CALENHAD_PREFERENCES_H
#define CALENHAD_PREFERENCES_H
#include <QSettings>
#include <QFont>
#include <QString>
#include <QColor>
#include "PreferencesService.h"
namespace calenhad {
    namespace preferences {
        class Preferences : public PreferencesService {
        public:
            Preferences ();

            ~Preferences ();

            void saveSettings () override;

            void loadSettings () override;

            QSettings* settings () override { return _settings; }


        private:
            QSettings* _settings;

        };
    }
}


#endif //CALENHAD_PREFERENCES_H