//
// Created by martin on 27/01/17.
//

#ifndef CALENHAD_GRADIENTLEGEND_H
#define CALENHAD_GRADIENTLEGEND_H

#include <QVector>
#include <QPair>
#include <QWidget>
#include <QtXml/QDomNode>
#include <experimental/optional>

namespace calenhad {
    namespace legend {

        class LegendWidget;
        typedef QPair<qreal, QColor> LegendEntry;

        class Legend : public QObject {
        Q_OBJECT

        public:
            Legend (const QString& name = "Legend");

            Legend (const Legend& other);

            virtual ~Legend ();

            QColor lookup (const double& value);

            QColor lookup (const std::experimental::fundamentals_v1::optional<double>& value);

            QColor interpolateColors (std::map<double, QColor>::iterator lower, std::map<double, QColor>::iterator higher, const double& value);

            double interpolateValues (const double& p1, const double& v1, const double& p2, const double& v2, const double& value);

            const bool isValid () const;

            const bool& isInterpolated () const;

            void addEntry (const double& value, const QColor& colour);

            unsigned int removeEntries (const double& from, const double& unto);

            const QList<LegendEntry> entries () const;

            int size ();

            LegendEntry at (int i);

            void clear ();

            void setDefaultColor (const QColor& colour);

            QString name ();

            const QString& notes () const;

            void setNotes (const QString& notes);

            void inflate (const QDomNode& n);

            void serialise (QDomDocument doc);

            QIcon icon ();


            LegendWidget* widget ();

        public slots:

            void setEntries (const QList<LegendEntry>& entries);

            void setInterpolated (const bool& interpolate);

            void setName (const QString& name);

        signals:

            void legendChanged ();

            void renamed (const QString&);

        protected:
            bool _interpolate;
            std::map<qreal, QColor> _entries;
            LegendWidget* _widget;
            QColor _defaultColor;
            QString _notes;
            QString _name;
        };
    }
}
#endif //CALENHAD_GRADIENTLEGEND_H
