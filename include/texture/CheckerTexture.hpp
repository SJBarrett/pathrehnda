//
// Created by sjbar on 19/01/2023.
//

#pragma once

#include <memory>
#include "Texture.hpp"

namespace PathRehnda {

    class CheckerTexture : public Texture {
    public:
        CheckerTexture() = default;
        CheckerTexture(std::shared_ptr<Texture> even, std::shared_ptr<Texture> odd);
        CheckerTexture(ColorRgb even_color, ColorRgb odd_color);

        ColorRgb sample(Uv uv, const Point3 &point) const override;

    public:
        std::shared_ptr<Texture> even_texture;
        std::shared_ptr<Texture> odd_texture;
    };

} // PathRehnda