#include <iostream>
#include <spdlog/spdlog.h>
#include <optional>

#include "Color.hpp"
#include "Ray.hpp"
#include "Vec3.hpp"
#include "hittable/Sphere.hpp"

using namespace PathRehnda;

ColorRgb sample_ray(const Ray& ray, const Sphere& sphere) {
    if (auto hit_result = sphere.hit(ray, 0, 1000)) {
        Vec3 normal = hit_result.normal;
        return 0.5 * ColorRgb{normal.x() + 1, normal.y() + 1, normal.z() + 1}; // Color map normal values which are (-1, 1) to (0, 1)
    }
    Vec3 unit_direction = unit_vector(ray.direction);
    auto t = 0.5 * (unit_direction.y() + 1.0);
    return (1.0 - t) * ColorRgb(1.0, 1.0, 1.0) + t * ColorRgb(0.5, 0.7, 1.0);
}

// up to https://raytracing.github.io/books/RayTracingInOneWeekend.html#addingasphere

int main() {
    // image properties
    const auto aspect_ratio = 16.0 / 9.0;
    const int image_width = 400;
    const int image_height = static_cast<int>(image_width / aspect_ratio);

    // camera properties
    auto viewport_height = 2.0;
    auto viewport_width = aspect_ratio * viewport_height;
    auto focal_length = 1.0;

    auto origin = Point3(0, 0, 0);
    auto horizontal = Vec3(viewport_width, 0, 0);
    auto vertical = Vec3(0, viewport_height, 0);
    auto lower_left_corner = origin - horizontal / 2 - vertical / 2 - Vec3(0, 0, focal_length);

    // rendering

    std::cout << "P3\n" << image_width << ' ' << image_height << "\n255\n";

    Sphere sphere({0, 0, -1}, 0.5);
    for (int j = image_height - 1; j >= 0; --j) {
        std::cerr << "\rScanlines remaining: " << j << ' ' << std::flush;
        for (int i = 0; i < image_width; ++i) {
            auto u = double(i) / (image_width - 1);
            auto v = double(j) / (image_height - 1);
            Ray ray(origin, lower_left_corner + u * horizontal + v * vertical - origin);
            ColorRgb pixel_color = sample_ray(ray, sphere);
            write_color(std::cout, pixel_color);
        }
    }
    std::cerr << "\nDone.\n";
    return 0;
}
