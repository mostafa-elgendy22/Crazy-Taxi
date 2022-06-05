#pragma once

#include "../ecs/world.hpp"
#include "../components/car-movement.hpp"
#include "../components/passenger.hpp"
#include "../components/camera.hpp"
#include "../application.hpp"

#include <glm/glm.hpp>
#include <glm/gtx/euler_angles.hpp>
#include <glm/ext/matrix_transform.hpp>
#include <glm/ext/matrix_clip_space.hpp>

namespace our
{

    class DeliverSystem {
        Application* app; // The application in which the state runs
        // float overallTime = 0.0;
        // TODO 13 Read this from JSON
        const float CLOSE_DISTANCE = 5.0;
        Entity* tempPassengerParent = nullptr;
    public:
        // When a state enters, it should call this function and give it the pointer to the application
        void enter(Application* app){
            this->app = app;
        }

        void update(World* world, float deltaTime) {

            // overallTime += deltaTime;
            Entity* greenCarpet = nullptr;
            Entity* arrow = nullptr;
            Entity *camera = nullptr;
            std::vector<PassengerComponent*> passengers;
            CarMovementComponent* car = nullptr;
            Entity* carEntity = nullptr;
            PassengerComponent* passengerInside = nullptr;

            for(auto entity : world->getEntities()){
                if(auto passenger = entity->getComponent<PassengerComponent>(); passenger){
                    passengers.push_back(passenger);

                    if(passenger->inside){
                        passengerInside = passenger;
                    }
                }
                if(entity->name=="greenCarpet"){ // used as an indicator for waiting passenger
                    greenCarpet = entity;
                }
                if (entity->name == "arrow") {
                    arrow = entity;
                }
                if(auto tempCar = entity->getComponent<CarMovementComponent>(); tempCar){ // if you will add other cars, check for its name
                    car = tempCar;
                    carEntity = car->getOwner();
                }
                if (entity->name == "camera") {
                    camera = entity;
                }
            }

            if(passengers.size()==0 || !(car))
            {
              // don't draw arrow
              if (arrow)
                arrow->localTransform.scale = glm::vec3(0.0f);
              return;
            }

            glm::vec3 carPos = carEntity->localTransform.position;
            // make arrow point at greenCarpet
            if (arrow && greenCarpet->localTransform.position.y > 0.0f)
            {
              auto camera_gaze_point = camera->getLocalToWorldMatrix() * glm::vec4(0.0f, 0.0f, -1.0f, 1.0f);
              auto camera_pos = camera->getLocalToWorldMatrix() * glm::vec4(0.0f, 0.0f, 0.0f, 1.0f);

              auto carpet_pos = greenCarpet->getLocalToWorldMatrix() * glm::vec4(0.0f, 0.0f, 0.0f, 1.0f);

              // std::cout<<"camera_gaze_point: "<<camera_gaze_point.x<<" "<<camera_gaze_point.y<<" "<<camera_gaze_point.z<<std::endl;
              // std::cout<<"camera_pos: "<<camera_pos.x<<" "<<camera_pos.y<<" "<<camera_pos.z<<std::endl;
              // std::cout<<"carpet_pos: "<<carpet_pos.x<<" "<<carpet_pos.y<<" "<<carpet_pos.z<<std::endl;

              // angle
              auto v1 = camera_gaze_point - camera_pos;
              auto v2 = carpet_pos - camera_pos;
              auto angle = glm::acos(glm::dot(v1, v2) / (glm::length(v1) * glm::length(v2)));
              angle = glm::min(glm::abs(angle), glm::radians(90.0f));

              // direction of arrow
              auto isFront = glm::dot(glm::normalize(v1), glm::normalize(v2)) > 0;
              auto isRight = v1.x * -v2.z + v1.z * v2.x > 0;

              // std::cout << isFront << ' ' << isRight << std::endl;
              if (isRight)
                 angle = -angle;

              arrow->localTransform.rotation = glm::vec3(0.0f, 0.0f, angle);
            }
            
            // ++ Deliver passenger inside if any ++
            if(passengerInside){
                
                Entity* entity = passengerInside->getOwner();

                // We get a reference to the entity's position
                glm::vec3& entityPosition = entity->localTransform.position;
                glm::vec3& entityRotation = entity->localTransform.rotation;
                float distance = glm::distance(passengerInside->destination, carPos);
                // std::cout<<"distance to destination = "<<distance<<endl;
                if(distance < CLOSE_DISTANCE){// && car->acceleration==0){
                    if(greenCarpet){
                        greenCarpet->localTransform.position.y = -100; // hide carpet 
                    }
                    
                    if(tempPassengerParent){ // reached their destination, return their parent back
                        entity->parent = tempPassengerParent;
                    }
                    else{ // just to be safe if it was nullptr
                        entity->parent = carEntity->parent;
                    }
                    entityPosition = passengerInside->destination;
                    entityPosition.y += 1;
                    entityRotation = glm::vec3(0.0, 0.0, 0.0);
                    passengerInside->reached = true;
                    passengerInside->inside = false; // inside ----> reached
                    // std::cout<<"Passenger reached their destination!!"<<std::endl;
                }
                else{
                    entity->parent = carEntity; // make the car your parent
                    entity->localTransform.position = glm::vec3(1, 2, 0); // local transform as you wish
                    entity->localTransform.rotation = glm::vec3(0.0, glm::radians(180.0f), 0.0);
                }
                return;
            }
            else
            {
              // don't draw arrow
              if (arrow)
                arrow->localTransform.scale = glm::vec3(0.0f);
            }


            // ++ search for nearest passenger ++
            for(auto passenger: passengers){
                // TODO 13 make passengers appear by time? ---- Add bool hidden in entity class???
                if(passenger->reached){ // passenger has reached their destination
                    continue;
                }
                Entity* entity = passenger->getOwner();

                // We get a reference to the entity's position
                glm::vec3& entityPosition = entity->localTransform.position;
                float distance = glm::distance(entityPosition, carPos);
                // std::cout<<"distance to passenger = "<< distance <<std::endl;
                if(distance<CLOSE_DISTANCE && passenger->waiting){ // passenger is close, take him
                    if(greenCarpet){
                        greenCarpet->localTransform.position = passenger->destination;
                        greenCarpet->localTransform.position.y = 0.1f;
                    }
                    if (arrow)
                      arrow->localTransform.scale = car->arrowScale;
                    
                    passenger->inside = true;
                    passenger->waiting = false; // waiting ----> inside
                    // entityPosition = glm::vec3(0, -100, 0); // hide it -// TODO 13 search for a better way to hide it
                    tempPassengerParent = entity->parent; // save current parent
                    // std::cout<<"Passenger inside!!"<<std::endl;
                }
            }
        }
    };

}
