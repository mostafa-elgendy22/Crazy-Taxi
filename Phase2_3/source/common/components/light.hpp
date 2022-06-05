#pragma once

#include "../ecs/component.hpp"

#include <glm/mat4x4.hpp>
#include <glm/glm.hpp>

#include "../shader/shader.hpp"

namespace our {

    // An enum that defines the type of light (DIRECTIONAL , POINT or SPOT) 
    enum class LightType {
        DIRECTIONAL,
        POINT,
        SPOT
    };

    // This component denotes that any renderer should draw the scene relative to this camera.
    // We do not define the eye, center or up here since they can be extracted from the entity local to world matrix
    class LightComponent : public Component {
    public:
        LightType lightType = LightType::DIRECTIONAL; // The type of light
        glm::vec3 color = glm::vec3(1, 0.9, 0.7); // always: specular = diffuse - Take ambient as skylight
        glm::vec2 cone_angles = glm::vec2(glm::radians(10.f), glm::radians(11.f)); // used in case of SPOT x: inner, y: outer
        glm::vec3 attenuation = glm::vec3(1, 0, 0); // a*d^2 + b*d + c -> vec3(a, b, c) - used in SPOT, POINT lights
        bool visible = true;

        // Direction & Position should be calculated from entity

        // The ID of this component type is "Light"
        static std::string getID() { return "Light"; }

        // Reads Light parameters from the given json object
        void deserialize(const nlohmann::json& data) override;

        // send light data to the given shader - Send to first light by default
        // Don't forget to set light_count before calling this
        void sendData(ShaderProgram* shader, int lightIndex, glm::vec3 direction = glm::vec3(-1, 0, 0), glm::vec3 position = glm::vec3(0, 0, 0));
    };

}