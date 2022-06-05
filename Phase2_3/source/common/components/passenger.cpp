#include "passenger.hpp"
#include "../ecs/entity.hpp"
#include "../deserialize-utils.hpp"

namespace our {

    void PassengerComponent::deserialize(const nlohmann::json& data){
        if(!data.is_object()) return;
        destination = data.value("destination", destination);
        speed = data.value("speed", speed);
        steps = data.value("steps", steps);
        // appearingTime = data.value("appearingTime", appearingTime);
    }
}