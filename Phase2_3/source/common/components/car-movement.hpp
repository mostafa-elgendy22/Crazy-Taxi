#pragma once

#include "../ecs/component.hpp"

#include <glm/glm.hpp>

namespace our {

    class CarMovementComponent : public Component {
    public:
        float maxSpeed=5.0f;
        float maxAngle=3.0f;

        float acceleration=0;
        float rotation=0;

        float accelerationSensitivity=1.0f;
        float rotationSensitivity=1.0f;

        float elasticity=-0.5f;

        // The ID of this component type is "CarMovement"
        static std::string getID() { return "Car Movement"; }

        // Reads linearVelocity & angularVelocity from the given json object
        void deserialize(const nlohmann::json& data) override;
    };

}
