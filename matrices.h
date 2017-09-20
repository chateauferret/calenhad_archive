//
// Created by martin on 09/09/17.
//

#ifndef CALENHAD_MATRICES_H
#define CALENHAD_MATRICES_H


namespace matrices {

    // Four-dimensional vector class

    class Vec4 {
    public:
        Vec4();
        Vec4 (double x, double y, double z, double w);
        Vec4 (double values [4]);
        virtual ~Vec4 ();

        // vector components
        double x ();
        double y ();
        double z ();
        double w ();
        double setX (double x);
        double setY (double y);
        double setZ (double z);
        double setW (double w);

        void setValues (double* values);
        void setValue (int i, double value);
        double value (int i);

        // dot product is used for computing matrix multiplication
        double dotProduct (Vec4 other);

        double& operator[] (int i);
        static Vec4 nullVector();
        void debug();

        // magnitude is the length of the vector calculated by Pythagoras
        double magnitude();

        // divide each component by the length to obtain a unit vector - we need this to make versors
        void normalise();

    protected:
        double _values[4];

    };

    class Mat4 {
    public:
        Mat4();

        Mat4 (Vec4 a, Vec4 b, Vec4 c, Vec4 d);
        Mat4 (Vec4 vectors [4]);
        ~Mat4();
        void setValue (int i, int j, double value);
        double value (int i, int j);

        // matrix multiplication - note non-commutative
        Mat4 operator* (Mat4 other);
        Vec4 operator* (Vec4 v);
        Mat4 operator* (double k);

        Mat4 operator+ (Mat4 other);
        Vec4& operator[] (int i);
        bool operator== (Mat4 other);

        // turns row-major into column-major and vice versa
        Mat4 transpose();

        Vec4 column (int i);
        Vec4 row (int i);

        void debug ();

        // obtain translation and scaling matrices from orthogonal components
        static Mat4 translationMatrix (const double& x, const double& y, const double& z);
        static Mat4 scalingMatrix (const double& x, const double& y, const double& z);
        static Mat4 nullMatrix();


    protected:
        Vec4 _vectors [4];


    };

    class Versor : public Vec4 {
    public:
        Versor (Vec4 values);

        // multiply two versors. Note not commutative
        Versor operator* (Versor other);

        // Construct a versor from an angle of rotation theta and x y and z components defining the axis of rotation
        static Versor fromParameters (const double& theta, const double& x, const double& y, const double& z);

        // Obtain a matrix from the versor - we can then multiply this matrix by any vector to produce the rotation result.
        Mat4 toRotationMatrix ();

    };

} // namespace matrices
#endif //CALENHAD_MATRICES_H
