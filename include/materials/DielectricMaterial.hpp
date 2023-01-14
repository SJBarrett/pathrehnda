//
// Created by sjbar on 13/01/2023.
//

#pragma once

#include "Material.hpp"

namespace PathRehnda {
    /**
     * Dielectric material is an implementation of transparent materials such as water and glass.
     *
     * Each ray that hits the material can either be reflected like a mirror, or refracted into.
     *
     * The light will be reflected if the observation angle is steep enough (determined with Schlick's
     * approximation) or if Snell's law has no solution when (eta/eta_prime) * sin(theta) > 1.
     *
     * If the light is refracted, it does so according to Snell's law.
     */
    class DielectricMaterial : public Material {
    public:
        explicit DielectricMaterial(double refractive_index) : refractive_index(refractive_index) {}

        [[nodiscard]]
        std::optional<Scatter> scatter(const Ray &ray_in, const HitResult &hit_result) const override;

    public:
        double refractive_index;

    private:
        // implementation of Schlick's approximation
        static double reflectance(double cosine, double ref_idx);
    };
}
