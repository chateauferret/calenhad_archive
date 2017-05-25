#ifndef LEGEND_H
#define LEGEND_H
#include <stdexcept>
#include <iostream>
#include <map>
#include <limits.h>
#include <qcolor.h>
#include <experimental/optional>
#include <QDomNode>
#include <QPair>


typedef QPair<qreal, QColor> LegendEntry;


namespace icosphere {

        class Legend {

        public:
            Legend (const QString& name);
            virtual ~Legend ();
            Legend (const Legend& other);
            virtual QColor lookup (const std::experimental::optional<double>& value);
            virtual QColor lookup (const double& value) = 0;
            void setDefaultColor (const QColor& color);
            virtual void addEntry (const double& value, const QColor& colour) = 0;
            virtual void clear() = 0;
            virtual unsigned removeEntries (const double& from, const double& unto) = 0;
            virtual QVector<QPair<qreal, QColor>> entries() = 0;
            virtual bool isValid() = 0;
            QString name();
            static Legend* fromNode (const QDomNode& n);
            void setName (const QString& name);
            void setNotes (const QString& notes);
            QString notes();
            virtual int size() = 0;
            virtual QPair<qreal, QColor> at (int i) = 0;
        protected:
            QColor _defaultColor = QColor (0, 0, 0, 0);
            QString _name;
            QString _notes;


            void setName (QString qString);
        };

        class IllegalLegendAccessException : std::runtime_error {
        public:
            IllegalLegendAccessException (const std::string& msg) : std::runtime_error (msg) {}
        };

} // icosphere
#endif // LEGEND_H
