#pragma once

#include "../ecs/world.hpp"
#include "../components/car-movement.hpp"

#include "../application.hpp"

#include <glm/glm.hpp>
#include <glm/gtc/constants.hpp>
#include <glm/trigonometric.hpp>
#include <glm/gtx/fast_trigonometry.hpp>

#include<iostream>
#include<algorithm>
using namespace std;

namespace our
{

    class CarMovementSystem {
        Application* app; // The application in which the state runs

    public:
        // When a state enters, it should call this function and give it the pointer to the application
        void enter(Application* app){
            this->app = app;
        }

        void update(World* world, float deltaTime) {

            CarMovementComponent *movement = nullptr;
            for(auto entity : world->getEntities()){
                movement = entity->getComponent<CarMovementComponent>();
                if(movement) break;
            }

            if(!(movement)) return;

            Entity* entity = movement->getOwner();

            // We get a reference to the entity's position and rotation
            glm::vec3& entityPosition = entity->localTransform.position;
            glm::vec3& entityRotation = entity->localTransform.rotation;

            // We get a reference to the component's params
            float& maxSpeed=movement->maxSpeed;
            float& maxAngle=movement->maxAngle;

            float& acceleration=movement->acceleration;
            float& rotation=movement->rotation;

            float& accelerationSensitivity=movement->accelerationSensitivity;
            float& rotationSensitivity=movement->rotationSensitivity;

            // We get the camera model matrix (relative to its parent) to compute the front, up and right directions
            glm::mat4 matrix = entity->localTransform.toMat4();

            glm::vec3 front = glm::vec3(matrix * glm::vec4(0, 0, -1, 0)),
                      up = glm::vec3(matrix * glm::vec4(0, 1, 0, 0)), 
                      right = glm::vec3(matrix * glm::vec4(1, 0, 0, 0));

            // We change the camera position based on the keys WASD/QE
            // S & W moves the player back and forth
            if(app->getKeyboard().isPressed(GLFW_KEY_W)) acceleration += deltaTime * accelerationSensitivity;
            if(app->getKeyboard().isPressed(GLFW_KEY_S)) acceleration -= deltaTime * accelerationSensitivity;
            if(!app->getKeyboard().isPressed(GLFW_KEY_W) && !app->getKeyboard().isPressed(GLFW_KEY_S))
            {   
                if(acceleration>0)acceleration=max(0.0f,acceleration-deltaTime*accelerationSensitivity);
                else acceleration=min(0.0f,acceleration+deltaTime*accelerationSensitivity);
            }

            if(acceleration>0)acceleration=min(maxSpeed,acceleration);
            else acceleration=max(-maxSpeed,acceleration);

            entityPosition += front * acceleration;

            if(acceleration == 0)
            {
                rotation=0;
                return;
            }

            // A & D moves the player left or right 
            if(app->getKeyboard().isPressed(GLFW_KEY_D)) rotation += deltaTime * rotationSensitivity;
            if(app->getKeyboard().isPressed(GLFW_KEY_A)) rotation -= deltaTime * rotationSensitivity;
            if(!app->getKeyboard().isPressed(GLFW_KEY_D) && !app->getKeyboard().isPressed(GLFW_KEY_A))rotation=0;
            // {   
            //     if(rotation>0)rotation=max(0.0f,rotation-deltaTime*rotationSensitivity);
            //     else rotation=min(0.0f,rotation+deltaTime*rotationSensitivity);
            // }
            
            if(rotation>0)rotation=min(maxAngle,rotation);
            else rotation=max(-maxAngle,rotation);

            entityRotation -= up * rotation;
            entityPosition += right * rotation;
            
            entityRotation.y = glm::wrapAngle(entityRotation.y);


            // rotation=0;
        }
    };

}
