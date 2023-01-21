//
// Created by sjbar on 21/01/2023.
//

#pragma once

#include "Texture.hpp"

namespace PathRehnda {

    class ImageTexture : public Texture {
    public:
        const static int bytes_per_pixel = 3;
        ImageTexture();
        explicit ImageTexture(const char* filename);
        ~ImageTexture();

        [[nodiscared]]
        ColorRgb sample(Uv uv, const Point3 &point) const override;

    private:
        unsigned char* data;
        int width, height;
        int bytes_per_scanline;

    };

} // PathRehnda