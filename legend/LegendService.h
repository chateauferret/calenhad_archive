//
// Created by martin on 15/04/17.
//

#ifndef CALENHAD_LEGENDSERVICE_H
#define CALENHAD_LEGENDSERVICE_H


#include <QtCore/QString>
#include <QtXml/QDomDocument>

namespace calenhad {
    namespace legend {
        class Legend;

        class LegendService : public QObject {
            Q_OBJECT
        public:
            virtual Legend* find (const QString& name) = 0;

            virtual bool exists (const QString& name) = 0;

            virtual void provide (Legend* legend) = 0;

            virtual void rename (const QString& from, const QString& to) = 0;

            virtual int legendCount () = 0;

            virtual bool remove (const QString& name) = 0;

            virtual Legend* defaultLegend () = 0;

            virtual bool isDirty () = 0;

            virtual void setDirty (const bool& dirty = true) = 0;

            virtual void commit () = 0;

            virtual void rollback () = 0;

            virtual QList<Legend*> all() = 0;

            virtual void clear() = 0;

        signals:
            void commitRequested();
            void rollbackRequested();


        };
    }
}


#endif //CALENHAD_LEGENDSERVICE_H
