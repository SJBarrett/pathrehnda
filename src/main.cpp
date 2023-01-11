#include <iostream>
#include <spdlog/spdlog.h>

#include "Color.hpp"
#include "Ray.hpp"
#include "Vec3.hpp"
#include "hittable/Sphere.hpp"
#include "RehndaMath.hpp"
#include "hittable/HittableList.hpp"
#include "Camera.hpp"

using namespace PathRehnda;

ColorRgb sample_ray(const Ray& ray, const Hittable& world) {
    if (auto hit_result = world.hit(ray, 0, infinity)) {
        return 0.5 * ColorRgb{hit_result.normal + ColorRgb(1, 1, 1)}; // Color map normal values which are (-1, 1) to (0, 1)
    }
    Vec3 unit_direction = unit_vector(ray.direction);
    auto t = 0.5 * (unit_direction.y() + 1.0);
    return (1.0 - t) * ColorRgb(1.0, 1.0, 1.0) + t * ColorRgb(0.5, 0.7, 1.0);
}


// up to https://raytracing.github.io/books/RayTracingInOneWeekend.html#antialiasing

int main() {
    Camera camera(16.0 / 9.0);
    // image properties
    const int image_width = 400;
    const int image_height = static_cast<int>(image_width / camera.get_aspect_ratio());
    const int samples_per_pixel = 100;

    // ppm image format header
    std::cout << "P3\n" << image_width << ' ' << image_height << "\n255\n";

    HittableList world;
    world.add(std::make_shared<Sphere>(Point3{0, 0, -1}, 0.5));
    world.add(std::make_shared<Sphere>(Point3{0, -100.5, -1}, 100));
    for (int j = image_height - 1; j >= 0; --j) {
        std::cerr << "\rScanlines remaining: " << j << ' ' << std::flush;
        for (int i = 0; i < image_width; ++i) {
            ColorRgb pixel_color(0, 0, 0);
            for (int s = 0; s < samples_per_pixel; s++) {
                auto u = (i + random_double()) / (image_width - 1);
                auto v = (j + random_double()) / (image_height - 1);
                Ray ray = camera.get_ray(u, v);
                pixel_color += sample_ray(ray, world);
            }
            write_color(std::cout, pixel_color, samples_per_pixel);
        }
    }
    std::cerr << "\nDone.\n";
    return 0;
}
