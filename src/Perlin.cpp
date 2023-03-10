//
// Created by sjbar on 21/01/2023.
//

#include "Perlin.hpp"

namespace PathRehnda {
    Perlin::Perlin() {
        random_vecs = new Vec3[point_count];
        for (int i = 0; i < point_count; i++) {
            random_vecs[i] = unit_vector(Vec3::random(-1, 1));
        }
        perm_x = perlin_generate_perm();
        perm_y = perlin_generate_perm();
        perm_z = perlin_generate_perm();
    }

    Perlin::~Perlin() {
        delete[] random_vecs;
        delete[] perm_x;
        delete[] perm_y;
        delete[] perm_z;
    }

    double Perlin::noise(const Point3 &point) const {
        auto u = point.x() - floor(point.x());
        auto v = point.y() - floor(point.y());
        auto w = point.z() - floor(point.z());
        auto i = static_cast<int>(floor(point.x()));
        auto j = static_cast<int>(floor(point.y()));
        auto k = static_cast<int>(floor(point.z()));

        Vec3 c[2][2][2];
        for (int di = 0; di < 2; di++) {
            for (int dj = 0; dj < 2; dj++) {
                for (int dk = 0; dk < 2; dk++) {
                    c[di][dj][dk] = random_vecs[
                            perm_x[(i + di) & 255] ^
                            perm_y[(j + dj) & 255] ^
                            perm_z[(k + dk) & 255]
                    ];
                }
            }
        }

        return perlin_interp(c, u, v, w);
    }

    double Perlin::turbulence(const Point3 &point, int depth) const {
        auto accum = 0.0;
        auto temp_point = point;
        auto weight = 1.0;

        for (int i = 0; i < depth; i++) {
            accum += weight * noise(temp_point);
            weight *= 0.5;
            temp_point *= 2;
        }
        return fabs(accum);
    }

    int *Perlin::perlin_generate_perm() {
        auto p = new int[point_count];
        for (int i = 0; i < Perlin::point_count; i++) {
            p[i] = i;
        }

        permute(p, point_count);
        return p;
    }

    void Perlin::permute(int *p, int n) {
        for (int i = n - 1; i > 0; i--) {
            int target = random_int(0, i);
            int tmp = p[i];
            p[i] = p[target];
            p[target] = tmp;
        }
    }

    double Perlin::trilinear_interp(double c[2][2][2], double u, double v, double w) {
        auto accum = 0.0;
        for (int i = 0; i < 2; i++) {
            for (int j = 0; j < 2; j++) {
                for (int k = 0; k < 2; k++) {
                    accum += (i * u + (1 - i) * (1 - u)) *
                             (j * v + (1 - j) * (1 - v)) *
                             (k * w + (1 - k) * (1 - w)) * c[i][j][k];
                }
            }
        }
        return accum;
    }

    double Perlin::perlin_interp(Vec3 c[2][2][2], double u, double v, double w) {
        auto uu = u * u * (3 - 2 * u);
        auto vv = v * v * (3 - 2 * v);
        auto ww = w * w * (3 - 2 * w);
        auto accum = 0.0;
        for (int i = 0; i < 2; i++) {
            for (int j = 0; j < 2; j++) {
                for (int k = 0; k < 2; k++) {
                    Vec3 weight_v(u - i, v - j, w - k);
                    accum += (i * uu + (1 - i) * (1 - uu)) *
                             (j * vv + (1 - j) * (1 - vv)) *
                             (k * ww + (1 - k) * (1 - ww)) *
                             dot(c[i][j][k], weight_v);
                }
            }
        }
        return accum;
    }


} // PathRehnda