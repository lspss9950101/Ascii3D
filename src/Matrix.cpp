#include "Matrix.hpp"
#include <memory.h>
#include <cmath>

Vector4::Vector4() {
    memset(this->vec, 0, sizeof(float)*4);
}

Vector4::Vector4(const Vector4& vec) {
    memcpy(this->vec, vec.vec, sizeof(float)*4);
}

Vector4::Vector4(const float vec[4]) {
    memcpy(this->vec, vec, sizeof(float)*4);
}

Vector4::Vector4(const float m0, const float m1, const float m2, const float m3) {
    float vec[4] = { m0, m1, m2, m3 };
    memcpy(this->vec, vec, sizeof(float)*4);
}

float& Vector4::operator[](const int coord) {
    if (coord < 0 || coord >= 4)
        throw;
    return this->vec[coord];
}

float Vector4::operator[](const int coord) const {
    if (coord < 0 || coord >= 4)
        throw;
    return this->vec[coord];
}

Vector4& Vector4::operator=(const Vector4& vec) {
    memcpy(this->vec, vec.vec, sizeof(this->vec));
    return *this;
}

Vector4 Vector4::operator-() const {
    return *this * -1;
}

Vector4 Vector4::operator+(const Vector4& vec) const {
    float ret[4];
    for (int i = 0; i < 4; i++)
        ret[i] = this->vec[i] + vec.vec[i];
    return Vector4(ret);
}

Vector4 Vector4::operator-(const Vector4& vec) const {
    return *this + (-vec);
}

Vector4 Vector4::operator*(const float& scalar) const {
    float ret[4];
    for (int i = 0; i < 4; i++)
        ret[i] = this->vec[i] * scalar;
    return Vector4(ret);
}

Vector4 Vector4::operator/(const float& scalar) const {
    return *this * (1/scalar);
}

Vector4 Vector4::operator*(const Vector4& vec) const {
    return Vector4(
        this->vec[0] * vec.vec[0],
        this->vec[1] * vec.vec[1],
        this->vec[2] * vec.vec[2],
        this->vec[3] * vec.vec[3]
    );
}

Vector4 operator*(const float& scalar, const Vector4& vec) {
    return vec * scalar;
}

float Vector4::magnitude() const {
    return std::sqrt(this->squared_magnitude());
}

float Vector4::squared_magnitude() const {
    float ret = 0;
    for (int i = 0; i < 4; i++)
        ret += this->vec[i] * this->vec[i];
    return ret;
}

Vector4 Vector4::normalize() const {
    return *this / this->magnitude();
}

float Vector4::dot(const Vector4& vec) const {
    return this->vec[0]*vec.vec[0] + this->vec[1]*vec.vec[1] + this->vec[2]*vec.vec[2] + this->vec[3]*vec.vec[3];
}

Vector4 Vector4::cross(const Vector4& vec) const {
    return Vector4(
        vec.vec[1]*this->vec[2]-vec.vec[2]*this->vec[1],
        vec.vec[2]*this->vec[0]-vec.vec[0]*this->vec[2],
        vec.vec[0]*this->vec[1]-vec.vec[1]*this->vec[0],
        0
    );
}

Vector4 Vector4::project(const Vector4& vec) const {
    return dot(vec) * vec;
}

std::ostream& operator<<(std::ostream& os, const Vector4 vec) {
    os << '[' << vec[0] << ", " << vec[1] << ", " << vec[2] << ", " << vec[3] << ']';
    return os;
}





Matrix4::Matrix4() {
    memset(this->mat, 0, sizeof(float)*16);
}

Matrix4::Matrix4(const Matrix4& mat) {
    memcpy(this->mat, mat.mat, sizeof(float)*16);
}

Matrix4::Matrix4(const float mat[16]) {
    memcpy(this->mat, mat, sizeof(float)*16);
}

Matrix4::Matrix4(const float m00, const float m01, const float m02, const float m03,
                 const float m10, const float m11, const float m12, const float m13,
                 const float m20, const float m21, const float m22, const float m23,
                 const float m30, const float m31, const float m32, const float m33) {
    float mat[16] = { m00, m01, m02, m03,
                      m10, m11, m12, m13,
                      m20, m21, m22, m23,
                      m30, m31, m32, m33 };
    memcpy(this->mat, mat, sizeof(float)*16);
}

float& Matrix4::operator[](const std::pair<int, int> coord) {
    if (coord.first < 0 || coord.first >= 4 || coord.second < 0 || coord.second >= 4)
        throw;
    return this->mat[coord.first + (coord.second << 2)];
}

float Matrix4::operator[](const std::pair<int, int> coord) const {
    if (coord.first < 0 || coord.first >= 4 || coord.second < 0 || coord.second >= 4)
        throw;
    return this->mat[coord.second + (coord.first << 2)];
}

Matrix4& Matrix4::operator=(const Matrix4& mat) {
    memcpy(this->mat, mat.mat, sizeof(this->mat));
    return *this;
}

Matrix4 Matrix4::operator-() const {
    return *this * -1;
}

Matrix4 Matrix4::operator*(const Matrix4& mat) const {
    float ret[16];
    for (int i = 0; i < 4; i++) {
        for (int j = 0; j < 4; j++) {
            ret[j+(i<<2)] = 0;
            for (int k = 0; k < 4; k++) {
                ret[j+(i<<2)] += this->mat[k+(i<<2)] * mat.mat[j+(k<<2)];
            }
        }
    }
    return Matrix4(ret);
}

Matrix4 Matrix4::operator+(const Matrix4& mat) const {
    float ret[16];
    for (int i = 0; i < 16; i++)
        ret[i] = this->mat[i] + mat.mat[i];
    return Matrix4(ret);
}

Matrix4 Matrix4::operator-(const Matrix4& mat) const {
    return *this + (-mat);
}

Matrix4 Matrix4::operator*(const float& scalar) const {
    float ret[16];
    for (int i = 0; i < 16; i++)
        ret[i] = this->mat[i] * scalar;
    return Matrix4(ret);
}

Matrix4 Matrix4::operator/(const float& scalar) const {
    return *this * (1/scalar);
}

Matrix4 operator*(const float& scalar, const Matrix4& mat) {
    return mat * scalar;
}

const Matrix4 Matrix4::Identity = Matrix4(1.f, 0.f, 0.f, 0.f,
                                          0.f, 1.f, 0.f, 0.f,
                                          0.f, 0.f, 1.f, 0.f,
                                          0.f, 0.f, 0.f, 1.f);

const Matrix4 Matrix4::Rotation(int axis, float angle) {
    if (axis < 0 || axis >= 3)
        throw;
    float cos = std::cos(angle * M_PI / 180);
    float sin = std::sin(angle * M_PI / 180);
    switch(axis) {
        case 0:
            return Matrix4(
                1.f,    0.f,    0.f,    0.f,
                0.f,    cos,    -sin,   0.f,
                0.f,    sin,    cos,    0.f,
                0.f,    0.f,    0.f,    1.f
            );
            break;
        case 1:
            return Matrix4(
                cos,    0.f,    sin,    0.f,
                0.f,    1.f,    0.f,    0.f,
                -sin,   0.f,    cos,    0.f,
                0.f,    0.f,    0.f,    1.f
            );
            break;
        case 2:
            return Matrix4(
                cos,    -sin,   0.f,    0.f,
                sin,    cos,    0.f,    0.f,
                0.f,    0.f,    1.f,    0.f,
                0.f,    0.f,    0.f,    1.f
            );
            break;
    }
    return Matrix4::Identity;
}

const Matrix4 Matrix4::Scale(float x, float y, float z) {
    return Matrix4(
        x,      0.f,    0.f,    0.f,
        0.f,    y,      0.f,    0.f,
        0.f,    0.f,    z,      0.f,
        0.f,    0.f,    0.f,    1.f
    );
}

const Matrix4 Matrix4::Translation(float x, float y, float z) {
    return Matrix4(
        1.f,    0.f,    0.f,    x,
        0.f,    1.f,    0.f,    y,
        0.f,    0.f,    1.f,    z,
        0.f,    0.f,    0.f,    1.f
    );
}

const Matrix4 Matrix4::Perspective(float aspect, float fov, float zfar, float znear) {
    float m00 = 1.0 / (aspect * std::tan(fov * M_PI / 360));
    float m11 = 1.0 / std::tan(fov * M_PI / 360);
    float m22 = -(zfar + znear) / (zfar - znear);
    float m23 = -(2*zfar*znear) / (zfar - znear);
    return Matrix4(
        m00,    0.f,    0.f,    0.f,
        0.f,    m11,    0.f,    0.f,
        0.f,    0.f,    m22,    m23,
        0.f,    0.f,    -1.f,   0.f
    );
}

Vector4 Matrix4::operator*(const Vector4& vec) const {
    float ret[4];
    for (int i = 0; i < 4; i++) {
        ret[i] = 0;
        for (int j = 0; j < 4; j++) {
            ret[i] += this->mat[j+(i<<2)] * vec[j];
        }
    }
    return Vector4(ret);
}

std::ostream& operator<<(std::ostream& os, const Matrix4 mat) {
    os << '[' << mat[std::pair<int, int>(0, 0)] << ", " << mat[std::pair<int, int>(0, 1)] << ", " << mat[std::pair<int, int>(0, 2)] << ", " << mat[std::pair<int, int>(0, 3)] << ", \n"
       << ' ' << mat[std::pair<int, int>(1, 0)] << ", " << mat[std::pair<int, int>(1, 1)] << ", " << mat[std::pair<int, int>(1, 2)] << ", " << mat[std::pair<int, int>(1, 3)] << ", \n"
       << ' ' << mat[std::pair<int, int>(2, 0)] << ", " << mat[std::pair<int, int>(2, 1)] << ", " << mat[std::pair<int, int>(2, 2)] << ", " << mat[std::pair<int, int>(2, 3)] << ", \n"
       << ' ' << mat[std::pair<int, int>(3, 0)] << ", " << mat[std::pair<int, int>(3, 1)] << ", " << mat[std::pair<int, int>(3, 2)] << ", " << mat[std::pair<int, int>(3, 3)] << ']';
    return os;
}
