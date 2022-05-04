#pragma once

#include "texture2d.hpp"
#include <string>

#include <glad/gl.h>
#include <glm/vec2.hpp>

namespace our::texture_utils {
    // This function create an empty texture with a specific format (useful for framebuffers)
    Texture2D* empty(GLenum format, glm::ivec2 size);
    // This function loads an image and sends its data to the given Texture2D 
    Texture2D* loadImage(const std::string& filename, bool generate_mipmap = true);
}