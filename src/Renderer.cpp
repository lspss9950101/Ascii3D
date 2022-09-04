#include "Renderer.hpp"
#include <memory.h>
#include <iostream>
#include <cmath>
#define MAX(x,y) ((x)>(y)?(x):(y))
#define MIN(x,y) ((x)<(y)?(x):(y))
#define ABS(x) ((x)>0?(x):(-(x)))

Renderer::Renderer(int width, int height, float zfar, float znear) :
    _width(width), _height(height), zfar(zfar), znear(znear), dirty_x0(width/2), dirty_y0(height/2), dirty_x1(-width/2), dirty_y1(-height/2) {
    this->frame_buffer = new char[width*height];
    this->depth_buffer = new float[width*height];
}

Renderer::~Renderer() {
    delete [] this->frame_buffer;
    delete [] this->depth_buffer;
}

void Renderer::clear() {
    memset(this->frame_buffer, ' ', sizeof(char)*_width*_height);
    for (int i = 0; i < _width*_height; i++)
        this->depth_buffer[i] = 0;
}

void Renderer::draw(const Matrix4& P, const Object& obj, float intensity) {
    float scan_x0f, scan_y0f, scan_x1f, scan_y1f;

    for (const Triangle& tri : obj.triangles()) {
        Triangle p_tri = P * tri;
        p_tri.bounding(scan_x0f, scan_y0f, scan_x1f, scan_y1f);
        int scan_x0 = MAX(-_width/2, std::floor(scan_x0f) * _width);
        int scan_y0 = MAX(-_height/2, std::floor(scan_y0f) * _height);
        int scan_x1 = MIN(_width/2, std::ceil(scan_x1f) * _width);
        int scan_y1 = MIN(_height/2, std::ceil(scan_y1f) * _height);

        dirty_x0 = MIN(dirty_x0, scan_x0);
        dirty_y0 = MIN(dirty_y0, scan_y0);
        dirty_x1 = MAX(dirty_x1, scan_x1);
        dirty_y1 = MAX(dirty_y1, scan_y1);

        float z;
        Vector4 normal = tri.normal();
        for (int x = scan_x0; x < scan_x1; x++) {
            for (int y = scan_y0; y < scan_y1; y++) {
                int pos = x+_width/2 + (y+_height/2) * _width;
                if (p_tri.contains((x+0.5) / _width, (y+0.5) / _height, z) && z <= zfar && z >= znear && z > depth_buffer[pos]) {
                    depth_buffer[pos] = z;
                    z = (z - P[std::pair<int,int>(2,3)]) / P[std::pair<int,int>(2,2)];
                    Vector4 frag(x+0.5, y+0.5, z, 0);
                    frame_buffer[pos] = Renderer::intensity2char(Renderer::fragment2intensity(frag, normal, intensity));
                }
            }
        }
    }
}

void Renderer::render() {
    std::cout << "\033[2J";
    for (int y = dirty_y0; y < dirty_y1; y++) {
        std::cout << "\033[" << (y + _height/2) << ';' << (dirty_x0 + _width/2) << 'H';
        for (int x = dirty_x0; x < dirty_x1; x++)
            std::cout << frame_buffer[x+_width/2 + (y+_height/2) * _width];
    }
    std::cout << "\033[" << _height+1 << ";0H";
    std::cout.flush();
    dirty_x0 = _width / 2;
    dirty_y0 = _height / 2;
    dirty_x1 = -_width / 2;
    dirty_y1 = -_height / 2;
}

float Renderer::fragment2intensity(const Vector4& pos, const Vector4& normal, float intensity) {
    Vector4 s(0, 0, 0, 1);
    Vector4 l(0, -500, 350, 1);
    Vector4 view = (s - pos).normalize();
    Vector4 light = (l - pos).normalize();
    Vector4 reflected = (2 * light.project(normal) - light).normalize();

    float diffusion = MAX(0, light.dot(normal));
    // float diffusion = MAX(0, l.normalize().dot(normal));
    float spacular = std::pow(MAX(0, reflected.dot(view)), 2);
    float ambient = 0.15;

    intensity *= (0 * spacular + 1.2 * diffusion + ambient);

    return intensity;
}

char Renderer::intensity2char(float intensity) {
    std::string ramp;
    if (detail_charset)
        ramp = "$@B%8&WM#*oahkbdpqwmZO0QLCJUYXzcvunxrjft/\\|()1{}[]?-_+~<>i!lI;:,\"^`'. ";
    else
        ramp = "@%#*+=-:. ";

    int steps = ramp.length()-1;
    int idx = steps;
    for (int i = 1; i <= steps; i++) {
        if (intensity <= i / (float)steps) return ramp[idx];
        idx--;
    }
    return '@';
}

void Renderer::set_size(int width, int height) {
    delete [] frame_buffer;
    delete [] depth_buffer;
    _width = width;
    _height = height;
    frame_buffer = new char[width*height];
    depth_buffer = new float[width*height];
}

float Renderer::width() const {
    return _width;
}

float Renderer::height() const {
    return _height;
}

#undef MAX
#undef MIN
#undef ABS