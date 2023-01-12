//
// Created by sjbar on 12/01/2023.
//

#pragma once

#include "Material.hpp"

namespace PathRehnda {

    class MetalMaterial : public Material {
    public:
        MetalMaterial(const ColorRgb& albedo, double fuzz);

        [[nodiscard]]
        std::optional<Scatter> scatter(const Ray &ray_in, const HitResult &hit_result) const override;

    public:
        ColorRgb albedo;
        double fuzz;
    };

} // PathRehnda