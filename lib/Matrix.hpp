#pragma once
#include <utility>
#include <ostream>

class Vector4 {
    private:
        float vec[4];

    public:
        Vector4();
        Vector4(const Vector4& vec);
        Vector4(const float vec[4]);
        Vector4(const float m0, const float m1, const float m2, const float m3);
        float& operator[](const int coord);
        float operator[](const int coord) const;
        Vector4& operator=(const Vector4& vec);
        Vector4 operator-() const;
        Vector4 operator+(const Vector4& vec) const;
        Vector4 operator-(const Vector4& vec) const;
        Vector4 operator*(const Vector4& vec) const;
        Vector4 operator*(const float& scalar) const;
        Vector4 operator/(const float& scalar) const;
        friend Vector4 operator*(const float& scalar, const Vector4& vec);

        float magnitude() const;
        float squared_magnitude() const;
        Vector4 normalize() const;
        float dot(const Vector4& vec) const;
        Vector4 cross(const Vector4& vec) const;
        Vector4 project(const Vector4& vec) const;

        friend std::ostream& operator<<(std::ostream& os, const Vector4 vec);
};

class Matrix4 {
    private:
        float mat[16];

    public:
        static const Matrix4 Identity;
        static const Matrix4 Rotation(int axis, float angle);
        static const Matrix4 Scale(float x, float y, float z);
        static const Matrix4 Translation(float x, float y, float z);
        static const Matrix4 Perspective(float aspect, float fov, float zfar, float znear);

        Matrix4();
        Matrix4(const Matrix4& mat);
        Matrix4(const float mat[16]);
        Matrix4(const float m00, const float m01, const float m02, const float m03,
                const float m10, const float m11, const float m12, const float m13,
                const float m20, const float m21, const float m22, const float m23,
                const float m30, const float m31, const float m32, const float m33);
        float& operator[](const std::pair<int, int> coord);
        float operator[](const std::pair<int, int> coord) const;
        Matrix4& operator=(const Matrix4& mat);
        Matrix4 operator-() const;
        Matrix4 operator*(const Matrix4& mat) const;
        Matrix4 operator+(const Matrix4& mat) const;
        Matrix4 operator-(const Matrix4& mat) const;
        Matrix4 operator*(const float& scalar) const;
        Matrix4 operator/(const float& scalar) const;
        friend Matrix4 operator*(const float& scalar, const Matrix4& mat);

        Vector4 operator*(const Vector4& vec) const;
        friend std::ostream& operator<<(std::ostream& os, const Matrix4 mat);
};