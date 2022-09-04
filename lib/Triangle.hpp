#pragma once
#include "Matrix.hpp"
#include <ostream>

class Triangle {
    private:
        Vector4 vertex[3];

    public:
        Triangle(const Vector4 vert0, const Vector4 vert1, const Vector4 vert2);
        Vector4& operator[](const int coord);
        Vector4 operator[](const int coord) const;
        Triangle operator+(const Vector4& vec) const;
        Triangle operator-(const Vector4& vec) const;
        friend Triangle operator*(const Matrix4& proj, const Triangle& tri);

        bool contains(float x, float y, float& z) const;
        void bounding(float& x0, float& y0, float& x1, float& y1) const;
        Vector4 normal() const;
        friend std::ostream& operator<<(std::ostream& os, const Triangle tri);
};