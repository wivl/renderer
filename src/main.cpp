#include <Eigen/Dense>

#include <Eigen/src/Core/Matrix.h>
#include <iostream>
#include <vector>

#include "camera.hpp"
#include "light.hpp"
#include "object.hpp"
#include "ppmpp/ppmpp.hpp"
#include "shader.hpp"

using namespace Eigen;

#define WIDTH 1920
#define HEIGHT 1080

ppm::Image flip_image(ppm::Image &image);

int main(void) {
    // object and object list
    std::vector<Object> list(1);
    Object african_head("./models/african_head.obj", FT_OBJ);
    list.push_back(african_head);

    // shader
    Shader shader;
    
    // camera
    Vector3f eye, target, up;
    eye << 0.0f, 2.0f, 1.0f;
    target << 0.0f, 0.0f, 0.0f;
    up << 0.0f, 1.0f, 0.0f;
    Camera camera(eye, target, up, WIDTH, HEIGHT, 90.0f, float(WIDTH)/HEIGHT, 1.0f, 20.0f);

    // light
    Light light(1, 1, 1);

    // png
    ppm::Image image(WIDTH, HEIGHT);

    std::cout << "this is image speaking" << std::endl;

    // zbuffer
    std::vector<float> zbuffer(WIDTH*HEIGHT, -std::numeric_limits<float>::max());

    camera.render(list, image, zbuffer, light);

    ppm::Image flipped = flip_image(image);

    flipped.save("output.ppm");
    

}

// FIX: segment fault
ppm::Image flip_image(ppm::Image &image) {
    ppm::Image flipped_image(WIDTH, HEIGHT);

    for (int i = 0; i < image.get_height(); i++) {
		for (int j = 0; j < image.get_width(); j++) {
			int y = image.get_height() - 1 - i;
			int x = j;
			flipped_image.set(j, i, image.get(y, x));
		}
	}
    return flipped_image;
}
