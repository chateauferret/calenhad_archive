#ifndef LEGEND_H
#define LEGEND_H
#include <stdexcept>
#include <iostream>
#include <map>
#include <limits.h>
#include <qcolor.h>
namespace noise {
    namespace utils {
        class Legend {

        public:
            Legend ();
            virtual ~Legend ();
            Legend (const Legend& other);
            virtual QColor lookup (const int& value) = 0;
            void setNoValueColour (const QColor& color);
            virtual void addEntry (const int& value, const QColor& colour) = 0;
            virtual unsigned removeEntries (const int& from, const int& unto) = 0;

        protected:
            QColor noValueColour = QColor (0, 0, 0, 0);
            int lowerBound = INT_MIN, upperBound = INT_MAX;
        };

        class IllegalLegendAccessException : std::runtime_error {
        public:
            IllegalLegendAccessException (const std::string& msg) : std::runtime_error (msg) {}
        };
    } // utils
} // noise
#endif // LEGEND_H
