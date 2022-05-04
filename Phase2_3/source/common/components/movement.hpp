#pragma once

#include "../ecs/component.hpp"

#include <glm/glm.hpp>

namespace our {

    // This component denotes that the MovementSystem will move the owning entity by a certain linear and angular velocity.
    // This component is added as a simple example for how use the ECS framework to implement logic.
    // For more information, see "common/systems/movement.hpp"
    // For a more complex example of how to use the ECS framework, see "free-camera-controller.hpp"
    class MovementComponent : public Component {
    public:
        glm::vec3 linearVelocity = {0, 0, 0}; // Each frame, the entity should move as follows: position += linearVelocity * deltaTime 
        glm::vec3 angularVelocity = {0, 0, 0}; // Each frame, the entity should rotate as follows: rotation += angularVelocity * deltaTime

        // The ID of this component type is "Movement"
        static std::string getID() { return "Movement"; }

        // Reads linearVelocity & angularVelocity from the given json object
        void deserialize(const nlohmann::json& data) override;
    };

}