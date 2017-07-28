//
// Created by martin on 15/04/17.
//

#ifndef CALENHAD_LEGENDROSTER_H
#define CALENHAD_LEGENDROSTER_H


#include <QMap>
#include "LegendService.h"

namespace calenhad {
    namespace legend {
        class Legend;
        class LegendRoster : public QObject, public LegendService {
        Q_OBJECT
        public:
            LegendRoster ();

            ~LegendRoster ();

            Legend* find (const QString& name) override;

            bool exists (const QString& name) override;

            void provide (Legend* legend) override;

            void inflate (const QString& filename = _filename) override;

            QMap<QString, Legend*> all ();

            void rename (const QString& from, const QString& to) override;

            int legendCount () override;

            bool remove (const QString& name) override;

            void commit () override;

            void rollback () override;

            Legend* defaultLegend () override;

            bool isDirty () override;

            void setDirty (const bool& dirty = true) override;

        private:
            QMap<QString, Legend*> _legends;
            bool _dirty;
            static QString _filename;

            void serialise (QString filename);

        };
    }
}

#endif //CALENHAD_LEGENDROSTER_H
