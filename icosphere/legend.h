#ifndef LEGEND_H
#define LEGEND_H
#include <stdexcept>
#include <iostream>
#include <map>
#include <limits.h>
#include <qcolor.h>
#include <experimental/optional>

namespace icosphere {

        class Legend {

        public:
            Legend ();
            virtual ~Legend ();
            Legend (const Legend& other);
            virtual QColor lookup (const std::experimental::optional<double>& value);
            virtual QColor lookup (const double& value) = 0;
            void setDefaultColor (const QColor& color);
            virtual void addEntry (const double& value, const QColor& colour) = 0;
            virtual unsigned removeEntries (const double& from, const double& unto) = 0;
            virtual bool isValid() = 0;
        protected:
            QColor _defaultColor = QColor (0, 0, 0, 0);
        };

        class IllegalLegendAccessException : std::runtime_error {
        public:
            IllegalLegendAccessException (const std::string& msg) : std::runtime_error (msg) {}
        };

} // icosphere
#endif // LEGEND_H
