#include <iostream>
#include <spdlog/spdlog.h>
#include <fstream>

#include "Ray.hpp"
#include "Vec3.hpp"
#include "RehndaMath.hpp"
#include "materials/Material.hpp"
#include "hittable/Sphere.hpp"
#include "hittable/HittableList.hpp"
#include "Camera.hpp"
#include "materials/LambertianMaterial.hpp"
#include "materials/MetalMaterial.hpp"
#include "materials/DielectricMaterial.hpp"
#include "ImageBuffer.hpp"
#include "ImageWriter.hpp"
#include "Sampler.hpp"

using namespace PathRehnda;

HittableList random_scene() {
    HittableList world;

    auto ground_material = std::make_shared<LambertianMaterial>(ColorRgb(0.5, 0.5, 0.5));
    world.add(std::make_shared<Sphere>(Point3(0, -1000, 0), 1000, ground_material));

    for (int a = -3; a < 3; a++) {
        for (int b = -3; b < 3; b++) {
            auto choose_mat = random_double();
            Point3 centre(a + 0.9 * random_double(), 0.2, b + 0.9 * random_double());

            if ((centre - Point3(4, 0.2, 0)).length() > 0.9) {
                std::shared_ptr<Material> sphere_material;

                if (choose_mat < 0.8) {
                    auto albedo = ColorRgb::random() * ColorRgb::random();
                    sphere_material = std::make_shared<LambertianMaterial>(albedo);
                    world.add(std::make_shared<Sphere>(centre, 0.2, sphere_material));
                } else if (choose_mat < 0.95) {
                    auto albedo = ColorRgb::random(0.5, 1);
                    auto fuzz = random_double(0, 0.5);
                    sphere_material = std::make_shared<MetalMaterial>(albedo, fuzz);
                    world.add(std::make_shared<Sphere>(centre, 0.2, sphere_material));
                } else {
                    sphere_material = std::make_shared<DielectricMaterial>(1.5);
                    world.add(std::make_shared<Sphere>(centre, 0.2, sphere_material));
                }
            }
        }
    }

    auto material1 = std::make_shared<DielectricMaterial>(1.5);
    world.add(make_shared<Sphere>(Point3(0, 1, 0), 1.0, material1));

    auto material2 = std::make_shared<LambertianMaterial>(ColorRgb(0.4, 0.2, 0.1));
    world.add(make_shared<Sphere>(Point3(-4, 1, 0), 1.0, material2));

    auto material3 = std::make_shared<MetalMaterial>(ColorRgb(0.7, 0.6, 0.5), 0.0);
    world.add(make_shared<Sphere>(Point3(4, 1, 0), 1.0, material3));

    return world;
}

// up to https://raytracing.github.io/books/RayTracingInOneWeekend.html#dielectrics

int main() {
    // image properties
    const double aspect_ratio = 3.0 / 2.0;
    const int image_width = 480;
    const int image_height = static_cast<int>(image_width / aspect_ratio);
    const int samples_per_pixel = 30;
    const int max_depth = 10;

    // ppm image format header

    // world setup
    Point3 look_from(13, 2, 3);
    Point3 look_at(0, 0, 0);
    Vec3 up(0, 1, 0);
    auto dist_to_focus = 10.0;
    auto aperture = 0.1;
    Camera camera(look_from, look_at, up, 20.0, aspect_ratio, aperture, dist_to_focus);
    HittableList world = random_scene();

    ImageBuffer image_buffer(image_width, image_height);
    Sampler sampler(max_depth);
    sampler.sample_pixels(camera, world, image_buffer, samples_per_pixel);

    ImageWriter image_writer;
    std::ofstream output("image.ppm");
    image_writer.write_image_buffer_to_stream(output, image_buffer, samples_per_pixel);
    std::cerr << "\nDone.\n";
    return 0;
}
