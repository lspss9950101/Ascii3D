#include "Object.hpp"
#include <limits>
#include <cmath>
#define MAX(x,y) ((x)>(y)?(x):(y))
#define MIN(x,y) ((x)<(y)?(x):(y))

Object::Object() {}

Object::Object(std::list<Triangle>& mesh) :
    mesh(mesh) {}

std::list<Triangle> Object::triangles() const {
    return this->mesh;
}

void Object::bounding(float& x0, float& y0, float& x1, float& y1) const {
    x0 = std::numeric_limits<float>::infinity();
    y0 = std::numeric_limits<float>::infinity();
    x1 = -std::numeric_limits<float>::infinity();
    y1 = -std::numeric_limits<float>::infinity();

    float xb0, yb0, xb1, yb1;
    for (const Triangle& tri : mesh) {
        tri.bounding(xb0, yb0, xb1, yb1);
        x0 = MIN(x0, xb0);
        y0 = MIN(y0, yb0);
        x1 = MAX(x1, xb1);
        y1 = MAX(y1, yb1);
    }
}

Object operator*(const Matrix4& proj, const Object& obj) {
    std::list<Triangle> mesh(obj.mesh);
    for (Triangle& tri : mesh)
        tri = proj * tri;
    return Object(mesh);
}

std::ostream& operator<<(std::ostream& os, const Object& obj) {
    for (const Triangle& tri : obj.mesh) {
        os << tri << '\n';
    }
    return os;
}

TriangularMesh::TriangularMesh(Vector4 vert0, Vector4 vert1, Vector4 vert2) {
    mesh.push_back(Triangle(vert0, vert1, vert2));
}

RectangularMesh::RectangularMesh(Vector4 vert0, Vector4 vert1, Vector4 vert2, Vector4 vert3) {
    mesh.push_back(Triangle(vert0, vert1, vert2));
    mesh.push_back(Triangle(vert0, vert2, vert3));
}

CubeMesh::CubeMesh() {
    std::list<Triangle> tris;
    RectangularMesh rect = RectangularMesh(
        Vector4(-0.5, -0.5, -0.5,  1.0),
        Vector4(-0.5,  0.5, -0.5,  1.0),
        Vector4( 0.5,  0.5, -0.5,  1.0),
        Vector4( 0.5, -0.5, -0.5,  1.0)
    );
    tris = rect.triangles();
    mesh.insert(mesh.end(), tris.begin(), tris.end());
    rect = RectangularMesh(
        Vector4(-0.5, -0.5,  0.5,  1.0),
        Vector4( 0.5, -0.5,  0.5,  1.0),
        Vector4( 0.5,  0.5,  0.5,  1.0),
        Vector4(-0.5,  0.5,  0.5,  1.0)
    );
    tris = rect.triangles();
    mesh.insert(mesh.end(), tris.begin(), tris.end());
    rect = RectangularMesh(
        Vector4(-0.5, -0.5, -0.5,  1.0),
        Vector4(-0.5, -0.5,  0.5,  1.0),
        Vector4(-0.5,  0.5,  0.5,  1.0),
        Vector4(-0.5,  0.5, -0.5,  1.0)
    );
    tris = rect.triangles();
    mesh.insert(mesh.end(), tris.begin(), tris.end());
    rect = RectangularMesh(
        Vector4( 0.5, -0.5, -0.5,  1.0),
        Vector4( 0.5,  0.5, -0.5,  1.0),
        Vector4( 0.5,  0.5,  0.5,  1.0),
        Vector4( 0.5, -0.5,  0.5,  1.0)
    );
    tris = rect.triangles();
    mesh.insert(mesh.end(), tris.begin(), tris.end());
    rect = RectangularMesh(
        Vector4(-0.5, -0.5, -0.5,  1.0),
        Vector4( 0.5, -0.5, -0.5,  1.0),
        Vector4( 0.5, -0.5,  0.5,  1.0),
        Vector4(-0.5, -0.5,  0.5,  1.0)
    );
    tris = rect.triangles();
    mesh.insert(mesh.end(), tris.begin(), tris.end());
    rect = RectangularMesh(
        Vector4(-0.5,  0.5, -0.5,  1.0),
        Vector4(-0.5,  0.5,  0.5,  1.0),
        Vector4( 0.5,  0.5,  0.5,  1.0),
        Vector4( 0.5,  0.5, -0.5,  1.0)
    );
    tris = rect.triangles();
    mesh.insert(mesh.end(), tris.begin(), tris.end());
}

TetrahedronMesh::TetrahedronMesh() {
    mesh.push_back(Triangle(
        Vector4(std::sqrt(8), 0, -1, 1),
        Vector4(-std::sqrt(2), std::sqrt(6), -1, 1),
        Vector4(-std::sqrt(2), -std::sqrt(6), -1, 1)
    ));
    mesh.push_back(Triangle(
        Vector4(std::sqrt(8), 0, -1, 1),
        Vector4(0, 0, 3, 1),
        Vector4(-std::sqrt(2), -std::sqrt(6), -1, 1)
    ));
    mesh.push_back(Triangle(
        Vector4(std::sqrt(8), 0, -1, 1),
        Vector4(-std::sqrt(2), std::sqrt(6), -1, 1),
        Vector4(0, 0, 3, 1)
    ));
    mesh.push_back(Triangle(
        Vector4(0, 0, 3, 1),
        Vector4(-std::sqrt(2), std::sqrt(6), -1, 1),
        Vector4(-std::sqrt(2), -std::sqrt(6), -1, 1)
    ));
}

IcosahedronMesh::IcosahedronMesh() {
    float t = (1.0 + std::sqrt(5)) / 2;
    Vector4 vertices[12] = {
        Vector4(-1,  t,  0, 1),
        Vector4( 1,  t,  0, 1),
        Vector4(-1, -t,  0, 1),
        Vector4( 1, -t,  0, 1),

        Vector4( 0, -1,  t, 1),
        Vector4( 0,  1,  t, 1),
        Vector4( 0, -1, -t, 1),
        Vector4( 0,  1, -t, 1),

        Vector4( t,  0, -1, 1),
        Vector4( t,  0,  1, 1),
        Vector4(-t,  0, -1, 1),
        Vector4(-t,  0,  1, 1)
    };

    int indices[20][3] = {
        { 0, 11,  5},
        { 0,  5,  1},
        { 0,  1,  7},
        { 0,  7, 10},
        { 0, 10, 11},

        { 1,  5,  9},
        { 5, 11,  4},
        {11, 10,  2},
        {10,  7,  6},
        { 7,  1,  8},

        { 3,  9,  4},
        { 3,  4,  2},
        { 3,  2,  6},
        { 3,  6,  8},
        { 3,  8,  9},

        { 4,  9,  5},
        { 2,  4, 11},
        { 6,  2, 10},
        { 8,  6,  7},
        { 9,  8,  1}
    };

    for (int i = 0; i < 20; i++) {
        mesh.push_back(Triangle(vertices[indices[i][0]], vertices[indices[i][1]], vertices[indices[i][2]]));
    }
}

SphereMesh::SphereMesh(int res) {
    std::list<Triangle> tris = IcosahedronMesh().triangles();;
    while (res--) {
        mesh.clear();
        for (const Triangle& tri : tris) {
            Vector4 mid0 = (tri[0] + tri[1]) / 2;
            Vector4 mid1 = (tri[1] + tri[2]) / 2;
            Vector4 mid2 = (tri[2] + tri[0]) / 2;
            float scale = tri[0].magnitude() / mid0.magnitude();
            Vector4 scale_vec(scale, scale, scale, 1);
            mid0 = mid0 * scale_vec;
            mid1 = mid1 * scale_vec;
            mid2 = mid2 * scale_vec;
            // mesh.push_back(Triangle(tri[0], mid0, mid2));
            // mesh.push_back(Triangle(tri[1], mid1, mid0));
            // mesh.push_back(Triangle(tri[2], mid2, mid1));
            mesh.push_back(Triangle(mid0, mid1, mid2));
        }
        tris = mesh;
    }
}

#undef MAX
#undef MIN