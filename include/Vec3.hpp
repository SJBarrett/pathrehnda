//
// Created by sjbar on 9/01/2023.
//

#pragma once

#include <cmath>
#include <iostream>
#include "RehndaMath.hpp"

namespace PathRehnda {
    class Vec3 {
    public:
        Vec3() : e{0, 0, 0} {}

        Vec3(double e0, double e1, double e2) : e{e0, e1, e2} {}

        double x() const {
            return e[0];
        }

        double y() const {
            return e[1];
        }

        double z() const {
            return e[2];
        }

        Vec3 operator-() const {
            return {-e[0], -e[1], -e[2]};
        }

        double operator[](int i) const { return e[i]; }

        double &operator[](int i) { return e[i]; }

        Vec3 &operator+=(const Vec3 &other_vector) {
            e[0] += other_vector.e[0];
            e[1] += other_vector.e[1];
            e[2] += other_vector.e[2];
            return *this;
        }

        Vec3 &operator*=(const double t) {
            e[0] *= t;
            e[1] *= t;
            e[2] *= t;
            return *this;
        }

        Vec3 &operator/=(const double t) {
            return *this *= 1 / t;
        }

        double length() const {
            return sqrt(length_squared());
        }

        double length_squared() const {
            return e[0] * e[0] + e[1] * e[1] + e[2] * e[2];
        }

        // Return true if the vector is close to zero in all dimensions
        bool near_zero() const {
            const auto s = 1e-8;
            return (fabs(e[0]) < s) && (fabs(e[1]) < s) && (fabs(e[2]) < s);
        }

    public:
        // ------------- STATICS -----------------
        static Vec3 zero() {
            return {0, 0, 0};
        }

        static Vec3 random() {
            return Vec3(random_double(), random_double(), random_double());
        }

        static Vec3 random(double min, double max) {
            return Vec3(random_double(min, max), random_double(min, max), random_double(min, max));
        }

        static Vec3 random_in_unit_sphere() {
            while (true) {
                auto candidate = Vec3::random(-1, 1);
                if (candidate.length_squared() < 1) {
                    return candidate;
                }
            }
        }


    public:
        double e[3];


    };

    using Point3 = Vec3;
    using ColorRgb = Vec3;

    inline std::ostream &operator<<(std::ostream &out, const Vec3 &v) {
        return out << v.e[0] << ' ' << v.e[1] << ' ' << v.e[2];
    }

    inline Vec3 operator+(const Vec3 &u, const Vec3 &v) {
        return {u.e[0] + v.e[0], u.e[1] + v.e[1], u.e[2] + v.e[2]};
    }

    inline Vec3 operator-(const Vec3 &u, const Vec3 &v) {
        return {u.e[0] - v.e[0], u.e[1] - v.e[1], u.e[2] - v.e[2]};
    }

    inline Vec3 operator*(const Vec3 &u, const Vec3 &v) {
        return {u.e[0] * v.e[0], u.e[1] * v.e[1], u.e[2] * v.e[2]};
    }

    inline Vec3 operator*(double t, const Vec3 &v) {
        return {t * v.e[0], t * v.e[1], t * v.e[2]};
    }

    inline Vec3 operator*(const Vec3 &v, double t) {
        return t * v;
    }

    inline Vec3 operator/(Vec3 v, double t) {
        return (1 / t) * v;
    }

    inline double dot(const Vec3 &u, const Vec3 &v) {
        return u.e[0] * v.e[0]
               + u.e[1] * v.e[1]
               + u.e[2] * v.e[2];
    }

    inline Vec3 cross(const Vec3 &u, const Vec3 &v) {
        return {u.e[1] * v.e[2] - u.e[2] * v.e[1],
                u.e[2] * v.e[0] - u.e[0] * v.e[2],
                u.e[0] * v.e[1] - u.e[1] * v.e[0]};
    }

    inline Vec3 unit_vector(Vec3 v) {
        return v / v.length();
    }

    inline Vec3 random_unit_vector() {
        return unit_vector(Vec3::random_in_unit_sphere());
    }

    inline Vec3 reflect(const Vec3& v, const Vec3& n) {
        return v - 2 * dot(v, n) * n;
    }

    inline Vec3 refract(const Vec3& incident_ray, const Vec3& normal, double etai_over_etat) {
        auto cos_theta = fmin(dot(-incident_ray, normal), 1.0);
        Vec3 r_out_perp = etai_over_etat * (incident_ray + cos_theta * normal);
        Vec3 r_our_parallel = -sqrt(fabs(1.0 - r_out_perp.length_squared())) * normal;
        return r_out_perp + r_our_parallel;
    }
}