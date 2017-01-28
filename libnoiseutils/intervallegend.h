#ifndef INTERVALLEGEND_H
#define INTERVALLEGEND_H
#include <legend.h>
#include <map>
#include <algorithm>
#include <QtGui/QColor>
#include "legend.h"

namespace noise {
    namespace utils {
        class IntervalLegend : public Legend {

        public:
            IntervalLegend ();
            IntervalLegend (const IntervalLegend& other);
            virtual ~IntervalLegend ();
            virtual QColor lookup (const int& value);
            void addEntry (const int& value, const QColor& colour);
            unsigned removeEntries (const int& from, const int& unto);

        protected:
            std::map<int, QColor> intervals;
        };
    } // utils
} noise
#endif // INTERVALLEGEND_H
