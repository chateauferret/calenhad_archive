//
// Created by martin on 23/08/17.
//

#ifndef CALENHAD_CURVE_H
#define CALENHAD_CURVE_H

namespace calenhad {
    namespace mapping {

        /// This structure defines a control point.
        ///
        /// Control points are used for defining splines.
        struct ControlPoint {

            /// The input value.
            double inputValue;

            /// The output value that is mapped from the input value.
            double outputValue;

        };

        class Curve {
        public:
            Curve ();
            virtual ~Curve ();
            virtual float GetValue (const float& sourceModuleValue) const = 0;
            virtual void GetValues (float* outputs, int size) const;


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

            void AddControlPoint (double inputValue, double outputValue);
            void AddControlPoint (double value);
            /// Deletes all the control points on the curve.
            ///
            /// @post All points on the curve are deleted.
            void ClearAllControlPoints ();

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
            const ControlPoint* GetControlPointArray () const {
                return m_pControlPoints;
            }

            const int GetControlPointCount() const {
                return m_controlPointCount;
            }

        protected:
           int m_controlPointCount;


            /// Determines the array index in which to insert the control point
            /// into the internal control point array.
            ///
            /// @param inputValue The input value of the control point.
            ///
            /// @returns The array index in which to insert the control point.
            ///
            /// @pre No two control points have the same input value.
            ///
            /// @throw noise::ExceptionInvalidParam An invalid parameter was
            /// specified; see the preconditions for more information.
            ///
            /// By inserting the control point at the returned array index, this
            /// class ensures that the control point array is sorted by input
            /// value.  The code that maps a value onto the curve requires a
            /// sorted control point array.
            int FindInsertionPos (double inputValue);

            /// Inserts the control point at the specified position in the
            /// internal control point array.
            ///
            /// @param insertionPos The zero-based array position in which to
            /// insert the control point.
            /// @param inputValue The input value stored in the control point.
            /// @param outputValue The output value stored in the control point.
            ///
            /// To make room for this new control point, this method reallocates
            /// the control point array and shifts all control points occurring
            /// after the insertion position up by one.
            ///
            /// Because the curve mapping algorithm used by this noise module
            /// requires that all control points in the array must be sorted by
            /// input value, the new control point should be inserted at the
            /// position in which the order is still preserved.
            void InsertAtPos (int insertionPos, double inputValue,
                              double outputValue);

            /// Array that stores the control points.
            ControlPoint* m_pControlPoints;
        };
    }
}
#endif //CALENHAD_CURVE_H
