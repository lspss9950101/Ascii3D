#include "Renderer.hpp"
#include <iostream>
#include <unistd.h>
#include <cmath>
#include <atomic>
#include <thread>
#include <termios.h>

std::atomic_bool stop(false);

Renderer renderer(64, 48, 1000, 0.3);
Matrix4 P, M;
Object mesh;

int fps = 60;
float trans_mag = 1.5;
float trans_freq = 0.08;
float rot_freq_x = 0.2;
float rot_freq_y = 0.8;
float rot_freq_z = 1.0;
float scale = 4.5;

int getch(void)
{
    struct termios oldattr, newattr;
    int ch;
    tcgetattr( STDIN_FILENO, &oldattr );
    newattr = oldattr;
    newattr.c_lflag &= ~( ICANON | ECHO );
    tcsetattr( STDIN_FILENO, TCSANOW, &newattr );
    ch = getchar();
    tcsetattr( STDIN_FILENO, TCSANOW, &oldattr );
    return ch;
}

void render() {
    float angle = 0;
    float ytrans = 0;
    while(!stop) {
        renderer.clear();
        M = Matrix4::Translation(0, trans_mag*std::cos(trans_freq*ytrans), 35) * Matrix4::Rotation(0, rot_freq_x*angle) * Matrix4::Rotation(1, rot_freq_y*angle) * Matrix4::Rotation(2, rot_freq_z*angle) * Matrix4::Scale(scale, scale, scale);
        renderer.draw(P, M * mesh, 0.8);
        renderer.render();
        angle += 1;
        ytrans += 1;
        usleep(1000.0*1000/fps);
    }
}

void update_mesh(int idx) {
    switch (idx) {
        case 0:
            mesh = TriangularMesh(
                Vector4(-1, -1, 0, 1),
                Vector4( 1, -1, 0, 1),
                Vector4( 0, std::sqrt(3)-1, 0, 1)
            );
            break;
        case 1:
            mesh = RectangularMesh(
                Vector4(-1, -1, 0, 1),
                Vector4(-1,  1, 0, 1),
                Vector4( 1,  1, 0, 1),
                Vector4( 1, -1, 0, 1)
            );
            break;
        case 2:
            mesh = Matrix4::Scale(0.5, 0.5, 0.5) * TetrahedronMesh();
            break;
        case 3:
            mesh = Matrix4::Scale(2, 2, 2) * CubeMesh();
            break;
        case 4:
            mesh = IcosahedronMesh();
            break;
        case 5:
            mesh = SphereMesh(1);
            break;
    }
}

int main(int argc, char** argv) {
    if (argc == 3)
        renderer.set_size(atoi(argv[1]), atoi(argv[2]));
    P = Matrix4::Perspective((renderer.width() / 2.0) / renderer.height(), 60, 1000, 0.3);

    int mesh_type = 0;
    int mesh_num = 6;
    update_mesh(mesh_type);

    std::thread t(render);
    char c;
    while ((c = getch())) {
        if (c == 'q') break;
        switch (c) {
            case 'v':
                renderer.detail_charset ^= 1;
                break;           
            case 'p':
                mesh_type = (mesh_type + 1) % mesh_num;
                update_mesh(mesh_type);
                break;
            case 'o':
                mesh_type = (mesh_type - 1 + mesh_num) % mesh_num;
                update_mesh(mesh_type);
                break;
            case 'u':
                fps--;
                if (fps < 1) fps = 1;
                break;
            case 'i':
                fps++;
                break;
            case 'a':
                rot_freq_x -= 0.01;
                rot_freq_x = rot_freq_x < 0 ? 0 : rot_freq_x;
                break;
            case 'z':
                rot_freq_x += 0.01;
                break;
            case 's':
                rot_freq_y -= 0.01;
                rot_freq_y = rot_freq_y < 0 ? 0 : rot_freq_y;
                break;
            case 'x':
                rot_freq_y += 0.01;
                break;
            case 'd':
                rot_freq_z -= 0.01;
                rot_freq_z = rot_freq_z < 0 ? 0 : rot_freq_z;
                break;
            case 'c':
                rot_freq_z += 0.01;
                break;
            case 't':
                trans_freq -= 0.01;
                trans_freq = trans_freq < 0 ? 0 : trans_freq;
                break;
            case 'y':
                trans_freq += 0.01;
                break;
            case 'e':
                trans_mag -= 0.01;
                trans_mag = trans_mag < 0 ? 0 : trans_mag;
                break;
            case 'r':
                trans_mag += 0.01;
                break;
            case '+':
                scale += 0.01;
                break;
            case '-':
                scale -= 0.01;
                scale = scale < 0 ? 0 : scale;
                break;
        }
    }
    stop = true;
    t.join();
    std::cout << "\033[2J";
    return 0;
}