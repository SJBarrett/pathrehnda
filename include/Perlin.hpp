//
// Created by sjbar on 21/01/2023.
//

#pragma once

#include "Vec3.hpp"

namespace PathRehnda {

    class Perlin {
    public:
        Perlin();
        ~Perlin();

        [[nodiscard]]
        double noise(const Point3& point) const;

        [[nodiscard]]
        double turbulence(const Point3& point, int depth = 7) const;

    private:
        static const int point_count = 256;

        Vec3* random_vecs;
        int* perm_x;
        int* perm_y;
        int* perm_z;

        static int* perlin_generate_perm();

        static void permute(int* p, int n);

        static double trilinear_interp(double c[2][2][2], double u, double v, double w);

        static double perlin_interp(Vec3 c[2][2][2], double u, double v, double w);
    };

} // PathRehnda