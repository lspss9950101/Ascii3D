#pragma once
#include "Object.hpp"

class Renderer {
    private:
        int _width, _height;
        float zfar, znear;
        int dirty_x0, dirty_y0, dirty_x1, dirty_y1;
        char *frame_buffer;
        float *depth_buffer;

        float fragment2intensity(const Vector4& pos, const Vector4& normal, float intensity);
        char intensity2char(float intensity);

    public:
        bool detail_charset = false;

        Renderer(int widht, int height, float zfar, float znear);
        ~Renderer();
        void clear();
        void draw(const Matrix4& P, const Object& obj, float intensity);
        void render();
        void set_size(int width, int height);
        float width() const;
        float height() const;
};