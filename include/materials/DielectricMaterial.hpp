//
// Created by sjbar on 13/01/2023.
//

#pragma once

#include "Material.hpp"

namespace PathRehnda {
    class DielectricMaterial : public Material {
    public:
        explicit DielectricMaterial(double refractive_index) : refractive_index(refractive_index) {}

        [[nodiscard]]
        std::optional<Scatter> scatter(const Ray &ray_in, const HitResult &hit_result) const override;

    public:
        double refractive_index;
    };
}
