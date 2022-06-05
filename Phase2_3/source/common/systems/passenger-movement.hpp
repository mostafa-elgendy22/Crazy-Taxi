#pragma once

#include "../ecs/world.hpp"
#include "../components/passenger.hpp"

#include "../application.hpp"

#include <glm/glm.hpp>
#include <glm/gtc/constants.hpp>
#include <glm/trigonometric.hpp>
#include <glm/gtx/fast_trigonometry.hpp>

namespace our
{

    class PassengerMovementSystem {
        Application* app; // The application in which the state runs

    public:
        // When a state enters, it should call this function and give it the pointer to the application
        void enter(Application* app){
            this->app = app;
        }

        void update(World* world, float deltaTime) {
            
            for(auto entity:world->getEntities())
            {
                PassengerComponent* passenger = entity->getComponent<PassengerComponent>();
                if(!passenger || passenger->inside == true) continue;
                // We get a reference to the entity's position and rotation
                glm::vec3& position = entity->localTransform.position;
                glm::vec3& rotation = entity->localTransform.rotation;

                // We get the camera model matrix (relative to its parent) to compute the front, up and right directions
                glm::mat4 matrix = entity->localTransform.toMat4();

                glm::vec3 front = glm::vec3(matrix * glm::vec4(0, 0, 1, 0));

                if(passenger->steps)
                {
                    position += passenger->speed * front;
                    passenger->steps--;
                }

                int random = std::rand()%500;

                if(random == 1 && passenger->steps == 0)
                {
                    passenger->speed = .02;            //(std::rand()%10+1)/200.0f;
                    rotation.y += std::rand()%181;
                    passenger->steps = 500;
                }

            }

        }

    };

}
