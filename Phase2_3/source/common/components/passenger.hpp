#pragma once

#include "../ecs/component.hpp"

#include <glm/glm.hpp>

namespace our {

    class PassengerComponent : public Component {
    public:
        glm::vec3 destination = {0, 0, 0};
        bool waiting = true; // waiting to be delivered
        bool inside = false; // being devlivered (inside the car)
        bool reached = false; // reached destination
        int steps = 500; // steps to take before stopping
        int tknsteps = 0; // taken steps so far
        float speed = 0.01; // speed walking with
        int moveProb = 500;
        // float appearingTime = 0.0; // time that the passenger should be drawn
        // float reachedTime = 0.0;


        // The ID of this component type is "Passenger"
        static std::string getID() { return "Passenger"; }

        // Reads destination, etc. from the given json object
        void deserialize(const nlohmann::json& data) override;
    };

}