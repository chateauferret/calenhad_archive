//
// Created by martin on 23/08/17.
//

#ifndef CALENHAD_CURVE_H
#define CALENHAD_CURVE_H

#include <QtCore/QString>
#include <QtCore/QVector>
#include "../controls/altitudemap/AltitudeMapping.h"

namespace calenhad {
    namespace mapping {

        class Curve {
        public:
            Curve ();
            virtual ~Curve ();
            virtual float GetValue (const float& sourceModuleValue) const = 0;
            virtual void GetValues (float* outputs, int size, double& from, double& to) const;


            /// Adds a control point to the curve.
            ///
            /// @param inputValue The input value stored in the control point.
            /// @param outputValue The output value stored in the control point.
            ///
            /// @pre No two control points have the same input value.
            ///
            /// @throw noise::ExceptionInvalidParam An invalid parameter was
            /// specified; see the preconditions for more information.
            ///
            /// It does not matter which order these points are added.

            void addMapping (QString inputValue, QString outputValue);
            void addMapping (controls::altitudemap::AltitudeMapping mapping);
            void AddControlPoint (QString value);
            /// Deletes all the control points on the curve.
            ///
            /// @post All points on the curve are deleted.
            void clear ();

            /// Returns a pointer to the array of control points on the curve.
            ///
            /// @returns A pointer to the array of control points.
            ///
            /// Before calling this method, call GetControlPointCount() to
            /// determine the number of control points in this array.
            ///
            /// It is recommended that an application does not store this pointer
            /// for later use since the pointer to the array may change if the
            /// application calls another method of this object.
            QVector<controls::altitudemap::AltitudeMapping> mappings () const {
                return _mappings;
            }

            const int count () const {
                return _mappings.size();
            }

        protected:

            /// Array that stores the control points.
            QVector<controls::altitudemap::AltitudeMapping> _mappings;
        };
    }
}
#endif //CALENHAD_CURVE_H
