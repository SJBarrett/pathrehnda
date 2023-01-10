#include <iostream>
#include <spdlog/spdlog.h>

#include "Color.hpp"
#include "Ray.hpp"
#include "Vec3.hpp"

/**
 * We can solve for if a ray hits a sphere by finding a solution to the following equation:
 *      (P(t) - C) . (P(t) - C) = r^2
 * Which expands to:
 *      t^2*b . b + 2tb . (A - C) + (A - C) . (A - C) - r^2 = 0
 * If we find 0 solutions to the quadratic then there is no intersection with the surface of a sphere. 1 solution means it just skims
 * the surface. 2 solutions means the ray goes through the sphere, entering and exiting it.
 *
 * We can use the quadratic formula to find solutions when ax^2 + bx + c = 0:
 *     x = (-b +/- sqrt(b^2 - 4ac)) / (2 * a)
 */
bool hitSphere(const Point3& centre, double radius, const Ray& ray) {
    Vec3 oc = ray.origin - centre; // (A - C)
    auto a = dot(ray.direction, ray.direction); // b . b from the above
    auto b = 2.0 * dot(oc, ray.direction); // 2 * b . (A - C)
    auto c = dot(oc, oc) - radius * radius; // (A - C) . (A - C) - r^2
    auto discriminant = b*b - 4*a*c;
    return discriminant > 0; // there is a solution if the discriminant is greater than 0 and therefore the sqrt is a real number
}

Color sampleRay(const Ray& ray) {
    if (hitSphere(Point3(0, 0, -1), 0.5, ray)) {
        return {1, 0, 0};
    }
    Vec3 unitDirection = unitVector(ray.direction);
    auto t = 0.5 * (unitDirection.y() + 1.0);
    return (1.0 - t) * Color(1.0, 1.0, 1.0) + t * Color(0.5, 0.7, 1.0);
}

// up to https://raytracing.github.io/books/RayTracingInOneWeekend.html#addingasphere

int main() {
    // image properties
    const auto aspectRatio = 16.0 / 9.0;
    const int imageWidth = 400;
    const int imageHeight = static_cast<int>(imageWidth / aspectRatio);

    // camera properties
    auto viewportHeight = 2.0;
    auto viewportWidth = aspectRatio * viewportHeight;
    auto focalLength = 1.0;

    auto origin = Point3(0, 0, 0);
    auto horizontal = Vec3(viewportWidth, 0, 0);
    auto vertical = Vec3(0, viewportHeight, 0);
    auto lowerLeftCorner = origin - horizontal / 2 - vertical / 2 - Vec3(0, 0, focalLength);

    // rendering

    std::cout << "P3\n" << imageWidth << ' ' << imageHeight << "\n255\n";

    for (int j = imageHeight - 1; j >= 0; --j) {
        std::cerr << "\rScanlines remaining: " << j << ' ' << std::flush;
        for (int i = 0; i < imageWidth; ++i) {
            auto u = double(i) / (imageWidth - 1);
            auto v = double(j) / (imageHeight - 1);
            Ray ray(origin, lowerLeftCorner + u * horizontal + v * vertical - origin);
            Color pixelColor = sampleRay(ray);
            writeColor(std::cout, pixelColor);
        }
    }
    std::cerr << "\nDone.\n";
    return 0;
}
