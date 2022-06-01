#pragma once

#include "../ecs/component.hpp"

#include <glm/glm.hpp>

namespace our {

    class CarMovementComponent : public Component {
    public:
        float speed=0.1f;

        // The ID of this component type is "CarMovement"
        static std::string getID() { return "Car Movement"; }

        // Reads linearVelocity & angularVelocity from the given json object
        void deserialize(const nlohmann::json& data) override;
    };

}