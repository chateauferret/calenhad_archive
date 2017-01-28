//
// Created by martin on 27/01/17.
//

#ifndef CALENHAD_QGRADIENT_H
#define CALENHAD_QGRADIENT_H

#include <QtCore/QObject>
#include <QtCore/QSet>
#include "../libnoiseutils/Color.h"

struct GradientPoint {

    /// The position of this gradient point.
    double pos;

    /// The color of this gradient point.
    noise::utils::Color color;

};


class QColorGradient : public QObject {
Q_OBJECT

public:
    QColorGradient();
    QColorGradient (QColorGradient& other const);
    virtual ~QColorGradient();


    /// Deletes all the gradient points from this gradient object.
    ///
    /// @post All gradient points from this gradient object are deleted.
    void clear ();

    /// Returns the color at the specified position in the color gradient.
    ///
    /// @param gradientPos The specified position.
    ///
    /// @returns The color at that position.
    const noise::utils::Color& getColor (double gradientPos) const;

    /// Returns the number of gradient points stored in this object.
    ///
    /// @returns The number of gradient points stored in this object.
    int getGradientPointCount () const {
        return pointCount;
    }

    void addGradientPoint (double gradientPos, const noise::utils::Color& gradientColor);

    static QColorGradient terrainGradient();
    static QColorGradient greyscaleGradient();


private:

    /// Determines the array index in which to insert the gradient point
    /// into the internal gradient-point array.
    ///
    /// @param gradientPos The position of this gradient point.
    ///
    /// @returns The array index in which to insert the gradient point.
    ///
    /// @pre No two gradient points have the same input value.
    ///
    /// @throw noise::ExceptionInvalidParam See the precondition.
    ///
    /// By inserting the gradient point at the returned array index, this
    /// object ensures that the gradient-point array is sorted by input
    /// value.  The code that maps a value to a color requires a sorted
    /// gradient-point array.
    int findInsertionPos (double gradientPos);

    /// Inserts the gradient point at the specified position in the
    /// internal gradient-point array.
    ///
    /// @param insertionPos The zero-based array position in which to
    /// insert the gradient point.
    /// @param gradientPos The position of this gradient point.
    /// @param gradientColor The color of this gradient point.
    ///
    /// To make room for this new gradient point, this method reallocates
    /// the gradient-point array and shifts all gradient points occurring
    /// after the insertion position up by one.
    ///
    /// Because this object requires that all gradient points in the array
    /// must be sorted by the position, the new gradient point should be
    /// inserted at the position in which the order is still preserved.
    void insertAtPos (int insertionPos, double gradientPos, const noise::utils::Color& gradientColor);

    /// Number of gradient points.
    int pointCount;

    /// Array that stores the gradient points.
    QSet<GradientPoint> points;

    /// A color object that is used by a gradient object to store a
    /// temporary value.
    mutable noise::utils::Color workingColor;
private:

};




#endif //CALENHAD_QGRADIENT_H
