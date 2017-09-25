//
// Created by martin on 24/08/17.
//

#include <iostream>
#include "Curve.h"
#include "CubicSpline.h"

using namespace calenhad::mapping;

Curve::Curve (): m_controlPointCount (0), m_pControlPoints (nullptr) {
    m_controlPointCount = 0;
}

Curve::~Curve () {
    if (m_pControlPoints && m_controlPointCount > 0) {
        delete[] m_pControlPoints;
    }
}

void Curve::GetValues (float* outputs, int size, double& from, double& to) const {
    from = m_pControlPoints [0].inputValue;
    to = m_pControlPoints [m_controlPointCount - 1].inputValue;

    double dx = (to - from) / size;
    for (int i = 0; i < size; i++) {
        float value = GetValue (i * dx + from);
        outputs [i] = value;
        std::cout << i << " " << value << "\n";
    }
}


void Curve::AddControlPoint (double inputValue, double outputValue)
{
    // Find the insertion point for the new control point and insert the new
    // point at that position.  The control point array will remain sorted by
    // input value.
    int insertionPos = FindInsertionPos (inputValue);
    InsertAtPos (insertionPos, inputValue, outputValue);
}


int Curve::FindInsertionPos (double inputValue) {
    if (m_controlPointCount == 0) { return 0; }
    int insertionPos;
    for (insertionPos = 0; insertionPos < m_controlPointCount; insertionPos++) {
        if (inputValue < m_pControlPoints[insertionPos].inputValue) {
            // We found the array index in which to insert the new control point.
            // Exit now.
            break;
        } /*else if (inputValue == m_pControlPoints[insertionPos].inputValue) {
            // Each control point is required to contain a unique input value, so
            // throw an exception.
            throw noise::ExceptionInvalidParam ();
        } --- do nothing in this case */
    }
    return insertionPos;
}


void Curve::InsertAtPos (int insertionPos, double inputValue, double outputValue) {
    // Make room for the new control point at the specified position within the
    // control point array.  The position is determined by the input value of
    // the control point; the control points must be sorted by input value
    // within that array.
    ControlPoint* newControlPoints = new ControlPoint[m_controlPointCount + 1];
    for (int i = 0; i < m_controlPointCount; i++) {
        if (i < insertionPos) {
            newControlPoints[i] = m_pControlPoints[i];
        } else {
            newControlPoints[i + 1] = m_pControlPoints[i];
        }
    }
    if (m_pControlPoints) {
        delete[] m_pControlPoints;
    }
    m_pControlPoints = newControlPoints;
    ++m_controlPointCount;

    // Now that we've made room for the new control point within the array, add
    // the new control point.
    m_pControlPoints[insertionPos].inputValue  = inputValue ;
    m_pControlPoints[insertionPos].outputValue = outputValue;
}


void Curve::ClearAllControlPoints ()
{
    delete[] m_pControlPoints;
    m_pControlPoints = NULL;
    m_controlPointCount = 0;
}

void Curve::AddControlPoint (double value) {
    AddControlPoint (value, value);
}
