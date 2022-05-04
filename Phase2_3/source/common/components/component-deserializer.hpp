#pragma once

#include "../ecs/entity.hpp"
#include "camera.hpp"
#include "mesh-renderer.hpp"
#include "free-camera-controller.hpp"
#include "movement.hpp"

namespace our {

    // Given a json object, this function picks and creates a component in the given entity
    // based on the "type" specified in the json object which is later deserialized from the rest of the json object
    inline void deserializeComponent(const nlohmann::json& data, Entity* entity){
        std::string type = data.value("type", "");
        Component* component = nullptr;
        if(type == CameraComponent::getID()){
            component = entity->addComponent<CameraComponent>();
        } else if (type == MeshRendererComponent::getID()) {
            component = entity->addComponent<MeshRendererComponent>();
        } else if (type == FreeCameraControllerComponent::getID()) {
            component = entity->addComponent<FreeCameraControllerComponent>();
        } else if (type == MovementComponent::getID()) {
            component = entity->addComponent<MovementComponent>();
        }
        if(component) component->deserialize(data);
    }

}