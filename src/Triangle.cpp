#include "Triangle.hpp"
#define MAX(x,y) ((x)>(y)?(x):(y))
#define MIN(x,y) ((x)<(y)?(x):(y))

Triangle::Triangle(const Vector4 vert0, const Vector4 vert1, const Vector4 vert2) {
    this->vertex[0] = vert0;
    this->vertex[1] = vert1;
    this->vertex[2] = vert2;
}

Vector4& Triangle::operator[](const int coord) {
    if (coord < 0 || coord >= 3)
        throw;
    return this->vertex[coord];
}

Vector4 Triangle::operator[](const int coord) const {
    if (coord < 0 || coord >= 3)
        throw;
    return this->vertex[coord];
}

Triangle Triangle::operator+(const Vector4& vec) const {
    return Triangle(this->vertex[0] + vec, this->vertex[1] + vec, this->vertex[2] + vec);
}

Triangle Triangle::operator-(const Vector4& vec) const {
    return *this + (-vec);
}

Triangle operator*(const Matrix4& proj, const Triangle& tri) {
    Triangle t = Triangle(proj * tri[0], proj * tri[1], proj * tri[2]);
    t[0] = t[0] / t[0][3];
    t[1] = t[1] / t[1][3];
    t[2] = t[2] / t[2][3];
    return t;
}

float sign(float x0, float y0, float x1, float y1, float x2, float y2) {
    return (x0 - x2) * (y1 - y2) - (x1 - x2) * (y0 - y2);
}

bool Triangle::contains(float x, float y, float& z) const {
    float d0 = sign(x, y, this->vertex[0][0], this->vertex[0][1], this->vertex[1][0], this->vertex[1][1]);
    float d1 = sign(x, y, this->vertex[1][0], this->vertex[1][1], this->vertex[2][0], this->vertex[2][1]);
    float d2 = sign(x, y, this->vertex[2][0], this->vertex[2][1], this->vertex[0][0], this->vertex[0][1]);

    bool has_neg = (d0 < 0) || (d1 < 0) || (d2 < 0);
    bool has_pos = (d0 > 0) || (d1 > 0) || (d2 > 0);

    bool inside = !(has_neg && has_pos);

    if (inside) {
        Vector4 normal = this->normal();
        z = (normal.dot(this->vertex[0]) - normal[0]*x - normal[1]*y) / normal[2];
    }

    return inside;
}

void Triangle::bounding(float& x0, float& y0, float& x1, float& y1) const {
    for (int i = 0; i < 3; i++) {
        x0 = MIN(this->vertex[i][0], x0);
        y0 = MIN(this->vertex[i][1], y0);
        x1 = MAX(this->vertex[i][0], x1);
        y1 = MAX(this->vertex[i][1], y1);
    }
}

Vector4 Triangle::normal() const {
    return (vertex[1] - vertex[0]).cross(vertex[2] - vertex[0]).normalize();
}

std::ostream& operator<<(std::ostream& os, const Triangle tri) {
    os << tri[0] << tri[1] << tri[2];
    return os;
}

#undef MAX
#undef MIN