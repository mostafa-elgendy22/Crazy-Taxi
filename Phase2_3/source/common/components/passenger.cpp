#include "passenger.hpp"
#include "../ecs/entity.hpp"
#include "../deserialize-utils.hpp"

namespace our {

    void PassengerComponent::deserialize(const nlohmann::json& data){
        if(!data.is_object()) return;
        destination = data.value("destination", destination);
        // appearingTime = data.value("appearingTime", appearingTime);
    }
}