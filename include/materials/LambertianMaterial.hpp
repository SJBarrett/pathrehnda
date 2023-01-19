//
// Created by sjbar on 12/01/2023.
//

#pragma once

#include "Material.hpp"

namespace PathRehnda {

    class LambertianMaterial : public Material {
    public:
        explicit LambertianMaterial(const ColorRgb& color);
        explicit LambertianMaterial(std::shared_ptr<Texture> albedo_texture);

        [[nodiscard]]
        std::optional<Scatter> scatter(const Ray &ray_in, const HitResult &hit_result) const override;

    public:
        std::shared_ptr<Texture> albedo_texture;
    };

} // PathRehnda