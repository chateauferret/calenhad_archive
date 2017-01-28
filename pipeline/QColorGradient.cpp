//
// Created by martin on 27/01/17.
//

#include <libnoise/exception.h>
#include "QColorGradient.h"
#include "../libnoiseutils/NoiseContstants.h"
#include "../libnoiseutils/Interpolation.h"
#include <libnoise/misc.h>
#include <assert.h>

using namespace noise;
using namespace noise::utils;

QColorGradient::QColorGradient()  {
}

QColorGradient::~QColorGradient() {
}

void QColorGradient::addGradientPoint (double gradientPos, const Color& gradientColor) {
    // Find the insertion point for the new gradient point and insert the new
    // gradient point at that insertion point.  The gradient point array will
    // remain sorted by gradient position.
    int insertionPos = findInsertionPos (gradientPos);
    insertAtPos (insertionPos, gradientPos, gradientColor);
}

void QColorGradient::clear () {
    points = NULL;
    pointCount = 0;
}

int QColorGradient::findInsertionPos (double gradientPos) {
    int insertionPos;
    for (insertionPos = 0; insertionPos < pointCount;
         insertionPos++) {
        if (gradientPos < points[insertionPos].pos) {
            // We found the array index in which to insert the new gradient point.
            // Exit now.
            break;
        } else if (gradientPos == points[insertionPos].pos) {
            // Each gradient point is required to contain a unique gradient
            // position, so throw an exception.
            throw noise::ExceptionInvalidParam ();
        }
    }
    return insertionPos;
}

const Color& QColorGradient::getColor (double gradientPos) const {
    assert (pointCount >= 2);

    // Find the first element in the gradient point array that has a gradient
    // position larger than the gradient position passed to this method.
    int indexPos;
    for (indexPos = 0; indexPos < pointCount; indexPos++) {
        if (gradientPos < points. (indexPos).pos) {
            break;
        }
    }

    // Find the two nearest gradient points so that we can perform linear
    // interpolation on the color.
    int index0 = ClampValue (indexPos - 1, 0, pointCount - 1);
    int index1 = ClampValue (indexPos, 0, pointCount - 1);

    // If some gradient points are missing (which occurs if the gradient
    // position passed to this method is greater than the largest gradient
    // position or less than the smallest gradient position in the array), get
    // the corresponding gradient color of the nearest gradient point and exit
    // now.
    if (index0 == index1) {
        workingColor = points[index1].color;
        return workingColor;
    }

    // Compute the alpha value used for linear interpolation.
    double input0 = points[index0].pos;
    double input1 = points[index1].pos;
    double alpha = (gradientPos - input0) / (input1 - input0);

    // Now perform the linear interpolation given the alpha value.
    const Color& color0 = points[index0].color;
    const Color& color1 = points[index1].color;
    Interpolation::LinearInterpColor (color0, color1, (float) alpha, workingColor);
    return workingColor;
}

void QColorGradient::insertAtPos (int insertionPos, double gradientPos, const Color& gradientColor) {
    // Make room for the new gradient point at the specified insertion position
    // within the gradient point array.  The insertion position is determined by
    // the gradient point's position; the gradient points must be sorted by
    // gradient position within that array.
    GradientPoint* newGradientPoints;
    newGradientPoints = new GradientPoint[pointCount + 1];
    for (int i = 0; i < pointCount; i++) {
        if (i < insertionPos) {
            newGradientPoints[i] = points[i];
        } else {
            newGradientPoints[i + 1] = points[i];
        }
    }
    delete[] points;
    points = newGradientPoints;
    ++pointCount;

    // Now that we've made room for the new gradient point within the array, add
    // the new gradient point.
    points[insertionPos].pos = gradientPos;
    points[insertionPos].color = gradientColor;
}

QColorGradient QColorGradient::terrainGradient () {
    QColorGradient gradient = QColorGradient();
    gradient.addGradientPoint (-1.0000, Color (0, 0, 128, 255)); // deeps
    gradient.addGradientPoint (-0.2500, Color (0, 0, 255, 255)); // shallow
    gradient.addGradientPoint (0.0000, Color (0, 128, 255, 255)); // shore
    gradient.addGradientPoint (0.0625, Color (240, 240, 64, 255)); // sand
    gradient.addGradientPoint (0.1250, Color (32, 160, 0, 255)); // grass
    gradient.addGradientPoint (0.3750, Color (224, 224, 0, 255)); // dirt
    gradient.addGradientPoint (0.7500, Color (128, 128, 128, 255)); // rock
    gradient.addGradientPoint (1.0000, Color (255, 255, 255, 255)); // snow
    return gradient;
}

QColorGradient QColorGradient::greyscaleGradient () {
    QColorGradient gradient = QColorGradient();
    gradient.addGradientPoint (-1.0000, Color (0, 0, 0, 255)); // black
    gradient.addGradientPoint (1.0000, Color (255, 255, 255, 255)); // white
    return gradient;
}

