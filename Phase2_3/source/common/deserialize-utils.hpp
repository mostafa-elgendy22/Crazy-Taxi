#pragma once

#include <glm/glm.hpp>
#include <glad/gl.h>
#include <string>
#include <unordered_map>
#include <json/json.hpp>

// This file contains some helper code for deserialization which includes:
// - Deserializing glm vectors from json via nlohmann::json
// - Converting strings to GLenums

// This template function allows us to read glm vectors from json
// We define it in the glm namespace since 
// it is the easiest way to get nlohmann::json working with glm data types
namespace glm {
    template<length_t L, typename T, qualifier Q>
    void from_json(const nlohmann::json& j, vec<L, T, Q>& v){
        for(length_t index = 0; index < L; ++index)
            v[index] = j[index].get<T>();
    }
}

namespace our {
    // This namespace will hold a set of inline maps that map between strings and GLenums
    // These will be used to deserialize Sampler and PipelineState Configurations
    namespace gl_enum_deserialize {
        typedef std::unordered_map<std::string, GLenum> const EnumMap;

        inline EnumMap primitives = {
                {"GL_POINTS", GL_POINTS},
                {"GL_LINES", GL_LINES},
                {"GL_LINE_STRIP", GL_LINE_STRIP},
                {"GL_LINE_LOOP", GL_LINE_LOOP},
                {"GL_TRIANGLES", GL_TRIANGLES},
                {"GL_TRIANGLE_STRIP", GL_TRIANGLE_STRIP},
                {"GL_TRIANGLE_FAN", GL_TRIANGLE_FAN},
        };

        inline EnumMap polygon_modes = {
                {"GL_POINT", GL_POINT},
                {"GL_LINE", GL_LINE},
                {"GL_FILL", GL_FILL}
        };

        inline EnumMap comparison_functions = {
                {"GL_ALWAYS", GL_ALWAYS},
                {"GL_NEVER", GL_NEVER},
                {"GL_EQUAL", GL_EQUAL},
                {"GL_NOTEQUAL", GL_NOTEQUAL},
                {"GL_LESS", GL_LESS},
                {"GL_LEQUAL", GL_LEQUAL},
                {"GL_GREATER", GL_GREATER},
                {"GL_GEQUAL", GL_GEQUAL}
        };

        inline EnumMap face_windings = {
                {"GL_CCW", GL_CCW},
                {"GL_CW", GL_CW}
        };

        inline EnumMap facets = {
                {"GL_FRONT", GL_FRONT},
                {"GL_BACK", GL_BACK},
                {"GL_FRONT_AND_BACK", GL_FRONT_AND_BACK}
        };

        inline EnumMap texture_magnification_filters = {
                {"GL_NEAREST", GL_NEAREST},
                {"GL_LINEAR", GL_LINEAR}
        };

        inline EnumMap texture_minification_filters = {
                {"GL_NEAREST", GL_NEAREST},
                {"GL_LINEAR", GL_LINEAR},
                {"GL_NEAREST_MIPMAP_NEAREST", GL_NEAREST_MIPMAP_NEAREST},
                {"GL_LINEAR_MIPMAP_NEAREST", GL_LINEAR_MIPMAP_NEAREST},
                {"GL_NEAREST_MIPMAP_LINEAR", GL_NEAREST_MIPMAP_LINEAR},
                {"GL_LINEAR_MIPMAP_LINEAR", GL_LINEAR_MIPMAP_LINEAR}
        };

        inline EnumMap texture_wrapping_modes = {
                {"GL_CLAMP_TO_EDGE", GL_CLAMP_TO_EDGE},
                {"GL_CLAMP_TO_BORDER", GL_CLAMP_TO_BORDER},
                {"GL_REPEAT", GL_REPEAT},
                {"GL_MIRRORED_REPEAT", GL_MIRRORED_REPEAT},
                {"GL_MIRROR_CLAMP_TO_EDGE", GL_MIRROR_CLAMP_TO_EDGE}
        };

        inline EnumMap blend_functions = {
                {"GL_ZERO", GL_ZERO},
                {"GL_ONE", GL_ONE},
                {"GL_SRC_COLOR", GL_SRC_COLOR},
                {"GL_ONE_MINUS_SRC_COLOR", GL_ONE_MINUS_SRC_COLOR},
                {"GL_DST_COLOR", GL_DST_COLOR},
                {"GL_ONE_MINUS_DST_COLOR", GL_ONE_MINUS_DST_COLOR},
                {"GL_SRC_ALPHA", GL_SRC_ALPHA},
                {"GL_ONE_MINUS_SRC_ALPHA", GL_ONE_MINUS_SRC_ALPHA},
                {"GL_DST_ALPHA", GL_DST_ALPHA},
                {"GL_ONE_MINUS_DST_ALPHA", GL_ONE_MINUS_DST_ALPHA},
                {"GL_CONSTANT_COLOR", GL_CONSTANT_COLOR},
                {"GL_ONE_MINUS_CONSTANT_COLOR", GL_ONE_MINUS_CONSTANT_COLOR},
                {"GL_CONSTANT_ALPHA", GL_CONSTANT_ALPHA},
                {"GL_ONE_MINUS_CONSTANT_ALPHA", GL_ONE_MINUS_CONSTANT_ALPHA}
        };

        inline EnumMap blend_equations = {
                {"GL_FUNC_ADD", GL_FUNC_ADD},
                {"GL_FUNC_SUBTRACT", GL_FUNC_SUBTRACT},
                {"GL_FUNC_REVERSE_SUBTRACT", GL_FUNC_REVERSE_SUBTRACT},
                {"GL_MIN", GL_MIN},
                {"GL_MAX", GL_MAX}
        };
    }

}