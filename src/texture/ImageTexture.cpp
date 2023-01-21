//
// Created by sjbar on 21/01/2023.
//
#define STB_IMAGE_IMPLEMENTATION
#include <stb_image.h>
#include <spdlog/spdlog.h>
#include "texture/ImageTexture.hpp"

namespace PathRehnda {
    ImageTexture::ImageTexture() : data(nullptr), width(0), height(0), bytes_per_scanline(0) {
    }

    ImageTexture::ImageTexture(const char *filename) {
        auto components_per_pixel = bytes_per_pixel;
        data = stbi_load(filename, &width, &height, &components_per_pixel, components_per_pixel);

        if (!data) {
            spdlog::error("ERROR: Could not load texture image file '{}'", filename);
            width = height = 0;
        }

        bytes_per_scanline = bytes_per_pixel * width;
    }

    ImageTexture::~ImageTexture() {
        delete data;
    }

    ColorRgb ImageTexture::sample(Uv uv, const Point3 &point) const {
        // return solid color as a debugging aid if no image is loaded
        if (data == nullptr) {
            return {1.0, 0, 1.0};
        }
        auto u = clamp(uv.u, 0.0, 1.0);
        auto v = 1 - clamp(uv.v, 0.0, 1.0);

        auto i = static_cast<int>(u * width);
        auto j = static_cast<int>(v * height);

        // Clamp integer mapping, since actual coordinates should be less than 1.0
        if (i >= width) {
            i = width - 1;
        }
        if (j >= height) {
            j = height - 1;
        }

        const auto color_scale = 1.0 / 255.0;
        auto pixel = data + j * bytes_per_scanline + i * bytes_per_pixel;

        return {
            color_scale * pixel[0],
            color_scale * pixel[1],
            color_scale * pixel[2],
        };
    }


} // PathRehnda