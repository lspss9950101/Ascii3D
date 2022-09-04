#pragma once
#include <list>
#include <iostream>
#include "Triangle.hpp"

class Object {
    protected:
        std::list<Triangle> mesh;
    public:
        Object();
        Object(std::list<Triangle>& mesh);
        std::list<Triangle> triangles() const;
        void bounding(float& x0, float& y0, float& x1, float& y1) const;
        friend Object operator*(const Matrix4& proj, const Object& obj);
        friend std::ostream& operator<<(std::ostream& os, const Object& obj);
};

class TriangularMesh : public Object {
    public:
        TriangularMesh(Vector4 vert0, Vector4 vert1, Vector4 vert2);
};

class RectangularMesh : public Object {
    public:
        RectangularMesh(Vector4 vert0, Vector4 vert1, Vector4 vert2, Vector4 vert3);
};

class CubeMesh : public Object {
    public:
        CubeMesh();
};

class TetrahedronMesh : public Object {
    public:
        TetrahedronMesh();
};

class IcosahedronMesh : public Object {
    public:
        IcosahedronMesh();
};

class SphereMesh : public Object {
    public:
        SphereMesh(int res);
};