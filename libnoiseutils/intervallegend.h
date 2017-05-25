#ifndef INTERVALLEGEND_H
#define INTERVALLEGEND_H

#include <map>
#include <algorithm>
#include <QtGui/QColor>
#include "../icosphere/legend.h"

namespace noise {
    namespace utils {
        class IntervalLegend : public icosphere::Legend {

        public:
            IntervalLegend (const QString& name = "IntervalLegend");
            IntervalLegend (const IntervalLegend& other);
            virtual ~IntervalLegend ();
            virtual QColor lookup (const double& value) override;
            virtual void clear() override;
            void addEntry (const double& value, const QColor& colour) override;
            virtual QVector<QPair<qreal, QColor>> entries() override;
            unsigned removeEntries (const double& from, const double& unto);
            bool isValid() override;
            int size() override;
            QPair<qreal, QColor> at (int i) override;
        protected:
            std::map<double, QColor> _entries;
        };
    } // utils
} // noise
#endif // INTERVALLEGEND_H
