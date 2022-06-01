#include "car-movement.hpp"
#include "../ecs/entity.hpp"
#include "../deserialize-utils.hpp"

namespace our {

    void CarMovementComponent::deserialize(const nlohmann::json& data){
        if(!data.is_object()) return;
        speed = data.value("speed", speed);
    }
}