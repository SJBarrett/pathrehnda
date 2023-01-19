//
// Created by sjbar on 19/01/2023.
//

#include "texture/CheckerTexture.hpp"
#include "texture/SolidColor.hpp"

namespace PathRehnda {
    CheckerTexture::CheckerTexture(std::shared_ptr<Texture> even, std::shared_ptr<Texture> odd) : even_texture(std::move(even)),
                                                                                                  odd_texture(std::move(odd)) {
    }

    CheckerTexture::CheckerTexture(ColorRgb even_color, ColorRgb odd_color) : even_texture(std::make_shared<SolidColor>(even_color)),
                                                                              odd_texture(std::make_shared<SolidColor>(odd_color)) {
    }

    ColorRgb CheckerTexture::sample(Uv uv, const Point3 &point) const {
        auto sines = sin(10 * point.x()) * sin(10 * point.y()) * sin(10 * point.z());
        if (sines < 0) {
            return odd_texture->sample(uv, point);
        } else {
            return even_texture->sample(uv, point);
        }
    }
} // PathRehnda