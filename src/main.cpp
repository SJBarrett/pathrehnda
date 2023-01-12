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

using namespace PathRehnda;

Point3 lambertian_diffuse_target(const HitResult& hit_result) {
    return hit_result.hit_location + hit_result.normal + random_unit_vector();
}

// random hemisphere does not depend on the normal
Point3 random_hemisphere_target(const HitResult& hit_result) {
    Vec3 in_unit_sphere = Vec3::random_in_unit_sphere();
    if (dot(in_unit_sphere, hit_result.normal) > 0.0) // in the same hemisphere as the normal
        return in_unit_sphere;
    else
        return -in_unit_sphere;
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
    const double aspect_ratio = 16.0 / 9.0;
    const int image_width = 400;
    const int image_height = static_cast<int>(image_width / aspect_ratio);
    const int samples_per_pixel = 100;
    const int max_depth = 30;

    // ppm image format header

    // world setup
    Camera camera(aspect_ratio);
    HittableList world;

    auto material_ground = std::make_shared<LambertianMaterial>(ColorRgb(0.8, 0.8, 0.0));
    auto material_center = std::make_shared<LambertianMaterial>(ColorRgb(0.7, 0.3, 0.3));
    auto material_left = std::make_shared<MetalMaterial>(ColorRgb(0.8, 0.8, 0.8), 0.3);
    auto material_right = std::make_shared<MetalMaterial>(ColorRgb(0.8, 0.6, 0.2), 1.0);

    world.add(std::make_shared<Sphere>(Point3( 0.0, -100.5, -1.0), 100.0, material_ground));
    world.add(std::make_shared<Sphere>(Point3( 0.0,    0.0, -1.0),   0.5, material_center));
    world.add(std::make_shared<Sphere>(Point3(-1.0,    0.0, -1.0),   0.5, material_left));
    world.add(std::make_shared<Sphere>(Point3( 1.0,    0.0, -1.0),   0.5, material_right));

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
