//
// Created by martin on 16/02/18.
//

#ifndef CALENHAD_LEGENDENTRY_H
#define CALENHAD_LEGENDENTRY_H

#include <QtCore/QString>
#include <QtGui/QColor>

namespace calenhad {
        namespace legend {
            class LegendEntry {
            public:
                LegendEntry ();

                LegendEntry (const QString& key, const QColor& color);

                LegendEntry (const double& key, const QColor& color);

                ~LegendEntry ();

                void setKey (const QString& key);

                void setKey (const double& key);

                void setColor (const QColor& color);

                QString key () const;

                double keyValue ();

                QColor color () const;

                QString label ();

                bool operator== (const LegendEntry& other);

            protected:
                QString _key;
                QColor _color;


            };

        }
}


#endif //CALENHAD_LEGENDENTRY_H
