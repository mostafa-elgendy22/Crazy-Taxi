#include "car-movement.hpp"
#include "../ecs/entity.hpp"
#include "../deserialize-utils.hpp"

namespace our {

    void CarMovementComponent::deserialize(const nlohmann::json& data){
        if(!data.is_object()) return;
        maxSpeed = data.value("maxSpeed", maxSpeed);
        maxAngle = data.value("maxSpeed", maxAngle);

        accelerationSensitivity = data.value("accelerationSensitivity", accelerationSensitivity);
        brakesSensitivity = data.value("brakesSensitivity", brakesSensitivity);
        rotationSensitivity = data.value("rotationSensitivity", rotationSensitivity);

        collisionTime = data.value("collisionTime", collisionTime);
        closeDistance = data.value("closeDistance", closeDistance);


        elasticity = data.value("elasticity", elasticity);
        arrowScale = data.value("arrowScale", arrowScale);
    }
}
