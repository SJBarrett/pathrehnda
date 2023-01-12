//
// Created by sjbar on 12/01/2023.
//

#pragma once

#include "Material.hpp"

namespace PathRehnda {

    class LambertianMaterial : public Material {
    public:
        explicit LambertianMaterial(const ColorRgb& color);

        [[nodiscard]]
        std::optional<Scatter> scatter(const Ray &ray_in, const HitResult &hit_result) const override;

    public:
        ColorRgb albedo;
    };

} // PathRehnda