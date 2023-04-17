#include "camera.hpp"
#include "object.hpp"
#include "ppmpp/ppmpp.hpp"
#include <Eigen/src/Core/Matrix.h>

#include <cmath>
#include <vector>
#include <float.h>

#define cot(x) (1.0/std::tan(x))

Matrix4f _cal_view(Vector3f position, Vector3f target, Vector3f up) {
    // VIEW MATRIX
    Vector3f zaxis = position - target; zaxis.normalize();
    Vector3f xaxis = up.cross(zaxis); xaxis.normalize();
    Vector3f yaxis = zaxis.cross(xaxis); yaxis.normalize();

    // M_t
    Matrix4f Mtranslate = Matrix4f::Identity();
    for (int i = 0; i < 3; i++) {
        Mtranslate(i, 3) = -position(i);
    }

    // M_r
    Matrix4f Mrotate = Matrix4f::Identity();
    for (int row = 0; row < 3; row++) {
        Mrotate(row, 0) = xaxis(row);
        Mrotate(row, 1) = yaxis(row);
        Mrotate(row, 2) = zaxis(row);
    }
    return Mrotate * Mtranslate;
    
}

Matrix4f _cal_proj(float fovy, float aspect, float near, float far) {
    Matrix4f temp;
    temp << cot(fovy)/2/aspect, 0,              0,                      0,
            0,                  cot(fovy)/2,    0,                      0,
            0,                  0,              (near+far)/(near-far),  -(2*near*far)/(near-far),
            0,                  0,              1,                      0;
    return temp;
}

Matrix4f _cal_viewport(int width, int height) {
    Matrix4f temp;
    temp << (float)width/2,     0,                  0, (float)width/2,
            0,                  (float)height/2,    0, (float)height/2,
            0,                  0,                  1, 0,
            0,                  0,                  0, 1;    
    return temp;
}

Camera::Camera() {
    position << 0.0f, 0.0f, 4.0f;
    target << 0.0f, 0.0f, 0.0f;
    up << 0.0f, 1.0f, 0.0f;
    fovy = 60.0f;
    far = 100.0f;
    near = 1.0f;
}



Camera::Camera(Vector3f position, Vector3f target, Vector3f up, int width, int height, float fovy, float aspect, float near, float far) {
    this->position = position;
    this->target = target;
    this->up = up;

    this->fovy = fovy;
    this->aspect = aspect;
    this->near = near;
    this->far = far;

    this->width = width;
    this->height = height;
    // calculate view and projection

    // VIEW MATRIX
    this->view = _cal_view(position, target, up);

    // PROJECTION
    // fov,aspect,far,near
    this->projection = _cal_proj(fovy, aspect, near, far);

    // VIEWPORT
    this->viewport = _cal_viewport(width, height);

    // dev log
    std::cout << "[LOG]Camera::Camera: view: " << std::endl;
    for (int i = 0; i < 4; i++) {
        for (int j = 0; j < 4; j++) {
            std::cout << view(i, j) << " ";
        }
        std::cout << std::endl;
    }
    std::cout << "[LOG]Camera::Camera: projection: " << std::endl;
    for (int i = 0; i < 4; i++) {
        for (int j = 0; j < 4; j++) {
            std::cout << projection(i, j) << " ";
        }
        std::cout << std::endl;
    }
    std::cout << "[LOG]Camera::Camera: viewport: " << std::endl;
    for (int i = 0; i < 4; i++) {
        for (int j = 0; j < 4; j++) {
            std::cout << viewport(i, j) << " ";
        }
        std::cout << std::endl;
    }
}

void Camera::set_position(Vector3f position) {
    this->position = position;
    this->view = _cal_view(position, target, up);
}

void Camera::set_target(Vector3f target) {
    this->target = target;
    this->view = _cal_view(position, target, up);
}

void Camera::set_up(Vector3f up) {
    this->up = up;
    this->view = _cal_view(position, target, up);
}


Vector3f Camera::get_position() {
    return position;
}

Vector3f Camera::get_target() {
    return target;
}

Vector3f Camera::get_up() {
    return up;
}

void Camera::set_fov(float fovy) {
    this->fovy = fovy;
    this->projection = _cal_proj(fovy, aspect, near, far);
}
void Camera::set_far(float far) {
    this->far = far;
    this->projection = _cal_proj(fovy, aspect, near, far);
}
void Camera::set_near(float near) {
    this->near = near;
    this->projection = _cal_proj(fovy, aspect, near, far);
}

float Camera::get_fov() {
    return fovy;
}
float Camera::get_far() {
    return far;
}
float Camera::get_near() {
    return near;
}


void Camera::set_shader(Shader &shader) {
    this->shader = shader;
}

Vector3f barycentric(Vector2f A, Vector2f B, Vector2f C, Vector2f P) {
    // (AB.x AC.x PA.x)
    Vector3f vec_x;
    vec_x << C(0)-A(0), B(0)-A(0), A(0)-P(0);
    
    // (AB.y AC.y PA.y)
    Vector3f vec_y;
    vec_y << C(1)-A(1), B(1)-A(1), A(1)-P(1);
    
    // (u v 1)
    Vector3f u = vec_x.cross(vec_y); 
    Vector3f result; 

    if (fabs(u(2)) > 1e-2) {
        result << 1.f-(u(0)+u(1))/u(2), u(1)/u(2), u(0)/u(2);
        return result;
    } 
    result << -1, 1, 1;
    return result;
}

void draw_triangle(Shader &shader, ppm::Image &image, Object &obj, std::vector<Vector4f> pts, std::vector<float> &zbuffer) {
    // set bounding box
    Vector2i bboxmin, bboxmax;
    bboxmin << image.get_width()-1, image.get_height()-1;
    bboxmax << 0, 0;
    for (int i = 0; i < 3; i++) {
        for (int j = 0; j < 2; j++) {
            bboxmin(j) = fminf(bboxmin(j), pts[i](j)/pts[i](3));
            bboxmax(j) = fmaxf(bboxmax(j), pts[i](j)/pts[i](3));
        }
    }
    Vector2i P;
    ppm::Color color;

    for (P(0) = std::max(bboxmin(0), 0); P(0) <= std::min(bboxmax(0), image.get_width()-1); P(0)++) {
        for (P(1) = std::max(bboxmin(1), 0); P(1) <= std::min(bboxmax(1), image.get_height()-1); P(1)++) {
            Vector3f c = barycentric(
                    Vector2f({pts[0](0)/pts[0](3), pts[0](1)/pts[0](3)}),
                    Vector2f({pts[1](0)/pts[1](3), pts[1](1)/pts[1](3)}),
                    Vector2f({pts[2](0)/pts[2](3), pts[2](1)/pts[2](3)}),
                    Vector2f({P(0), P(1)})
                    );
            float z = pts[0](2)*c(0) + pts[1](2)*c(1) + pts[2](2)*c(2);
            float w = pts[0](3)*c(0) + pts[1](3)*c(1) + pts[2](3)*c(2);

            float frag_depth = z/w;

            // smaller zbuffer means closer
            if (c(0)<0 || c(1)<0 || c(2)<0 || zbuffer[P(1)*image.get_width()+P(0)] < frag_depth) {
                continue;
            }
            bool discard = shader.fragment(obj, c, color);
            if (!discard) {
                // std::cout << "[LOG]Camera::render: set " << P(0) << " " << P(1) << " "  << color.get_raw() << std::endl;
                zbuffer[P(1)*image.get_width()+P(0)] = frag_depth;
                image.set(P(0), P(1), color);
            }

        }
    }
}

// camera represent's the world. objects are stored in the list
void Camera::render(std::vector<Object> obj_list, ppm::Image &image, std::vector<float> &zbuffer, Light light) {
    // for every object
    //      for every face
    //          for every vertex
    //              vertex shader
    //          fragment shader

    // for every obj
    for (auto obj = obj_list.begin(); obj != obj_list.end(); ++obj) {
        // get uniform matrix
        
        shader.set_uniform(viewport * projection * view * obj->get_model());

        std::cout << "[LOG]Camera::render: shader->uniform: " << std::endl;
        for (int k = 0; k < 4; k++) {
            for (int l = 0; l < 4; l++) {
                std::cout << shader.get_uniform()(k, l) << " ";
            }
            std::cout << std::endl;
        }

        // for every face
        for (int i = 0; i < obj->nface(); i++) {
            std::vector<Vector3f> face(obj->get_face(i));
            std::vector<Vector3f> normal;
            std::vector<Vector2f> uv;
            if (obj->nnormal() != 0) {
                normal = obj->get_normal(i);
            }
            if (obj->nuv() != 0) {
                uv = obj->get_uv(i);
            }

            std::vector<Vector4f> screen_coords(3);
            for (int j = 0; j < 3; j++) {
                Vertex vertex(face[j]);
                if (normal.size() != 0) {
                    vertex.set_normal(normal[j]);
                }
                if (uv.size() != 0) {
                    vertex.set_uv(uv[j]);
                }
                screen_coords[j] = this->shader.vert(vertex, j, light);
            }

            // dev log
            // std::cout << "[LOG]Camera::render: screen_coords: ";
            // for (int k = 0; k < 3; k++) {
                // for (int l = 0; l < 3; l++) {
                    // std::cout << screen_coords[k](l) << " ";
                // }
            // }
            // std::cout << std::endl;

            draw_triangle(shader, image, *obj, screen_coords, zbuffer);
        }
    }
}

Matrix4f Camera::get_modelview(Matrix4f model) {
    return view*model;
}
