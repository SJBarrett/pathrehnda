#include <iostream>
#include <spdlog/spdlog.h>

#include "Color.hpp"
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

using namespace PathRehnda;

HittableList random_scene() {
    HittableList world;

    auto ground_material = std::make_shared<LambertianMaterial>(ColorRgb(0.5, 0.5, 0.5));
    world.add(std::make_shared<Sphere>(Point3(0, -1000, 0), 1000, ground_material));

    for (int a = -11; a < 11; a++) {
        for (int b = -11; b < 11; b++) {
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

ColorRgb sample_ray(const Ray& ray, const Hittable& world, int depth) {
    // If exceeded max number of light bounces, stop sampling
    if (depth <= 0)
        return ColorRgb::zero();

    // setting t_min to 0.001 avoids shadow acne, where floating point approximation allow reflecting things at t = +/- 0.000001
    if (auto hit_result = world.hit(ray, 0.001, infinity)) {
        if (auto scatter = hit_result.material->scatter(ray, hit_result)) {
            return scatter->attenuation * sample_ray(scatter->scattered_ray, world, depth - 1);
        }
        return ColorRgb::zero();
    }

    Vec3 unit_direction = unit_vector(ray.direction);
    auto t = 0.5 * (unit_direction.y() + 1.0);
    return (1.0 - t) * ColorRgb(1.0, 1.0, 1.0) + t * ColorRgb(0.5, 0.7, 1.0);
}

// up to https://raytracing.github.io/books/RayTracingInOneWeekend.html#dielectrics

int main() {
    // image properties
    const double aspect_ratio = 3.0 / 2.0;
    const int image_width = 1200;
    const int image_height = static_cast<int>(image_width / aspect_ratio);
    const int samples_per_pixel = 50;
    const int max_depth = 30;

    // ppm image format header

    // world setup
    Point3 look_from(13, 2, 3);
    Point3 look_at(0, 0, 0);
    Vec3 up(0, 1, 0);
    auto dist_to_focus = 10.0;
    auto aperture = 0.1;
    Camera camera(look_from, look_at, up, 20.0, aspect_ratio, aperture, dist_to_focus);
    HittableList world = random_scene();

    std::cout << "P3\n" << image_width << ' ' << image_height << "\n255\n";
    for (int j = image_height - 1; j >= 0; --j) {
        std::cerr << "\rScanlines remaining: " << j << ' ' << std::flush;
        for (int i = 0; i < image_width; ++i) {
            ColorRgb pixel_color(0, 0, 0);
            for (int s = 0; s < samples_per_pixel; s++) {
                auto u = (i + random_double()) / (image_width - 1);
                auto v = (j + random_double()) / (image_height - 1);
                Ray ray = camera.get_ray(u, v);
                pixel_color += sample_ray(ray, world, max_depth);
            }
            write_color(std::cout, pixel_color, samples_per_pixel);
        }
    }
    std::cerr << "\nDone.\n";
    return 0;
}
