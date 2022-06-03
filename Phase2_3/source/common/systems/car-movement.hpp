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
            Entity* arrow = nullptr;
            for(auto entity : world->getEntities()){
                if (auto temp = entity->getComponent<CarMovementComponent>(); temp) {
                  movement = temp;
                }
                
                if (entity->name == "arrow") {
                    arrow = entity;
                }
            }

            if(!(movement)) return;

            Entity* entity = movement->getOwner();

            // We get a reference to the entity's position and rotation
            glm::vec3& entityPosition = entity->localTransform.position;
            glm::vec3& entityRotation = entity->localTransform.rotation;
            glm::vec3& arrowRotation = arrow->localTransform.rotation;

            // We get a reference to the component's params
            float& maxSpeed=movement->maxSpeed;
            float& maxAngle=movement->maxAngle;

            float& acceleration=movement->acceleration;
            float& rotation=movement->rotation;

            float& accelerationSensitivity=movement->accelerationSensitivity;
            float& rotationSensitivity=movement->rotationSensitivity;

            // [-1, 0]
            // how much the car will bounce back when it collides
            float& elasticity=movement->elasticity;

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
            arrowRotation -= up * rotation;
            entityPosition += right * rotation;
            
            entityRotation.y = glm::wrapAngle(entityRotation.y);


            // rotation=0;


            // Gheiath: Collison detection
            // get car mesh render component
            MeshRendererComponent* meshRender = entity->getComponent<MeshRendererComponent>();
            // get car model matrix
            glm::mat4 carModelMatrix = entity->localTransform.toMat4();
            // loop on vertics of car mesh
            // get minx, maxx, miny, maxy, minz, maxz
            float minx = 0, maxx = 0, miny = 0, maxy = 0, minz = 0, maxz = 0;
            for(int i = 0; i < meshRender->mesh->vertices.size(); i++)
            {
                glm::vec4 vertex = carModelMatrix * glm::vec4(meshRender->mesh->vertices[i], 1.0f);
                if(i == 0)
                {
                    minx = maxx = vertex.x;
                    miny = maxy = vertex.y;
                    minz = maxz = vertex.z;
                }
                else
                {
                    minx = min(minx, vertex.x);
                    maxx = max(maxx, vertex.x);
                    miny = min(miny, vertex.y);
                    maxy = max(maxy, vertex.y);
                    minz = min(minz, vertex.z);
                    maxz = max(maxz, vertex.z);
                }
            }


            // loop on other entities
            for(auto otherEntity : world->getEntities())
            {
              // get other entity mesh render component
              MeshRendererComponent* otherMeshRender = otherEntity->getComponent<MeshRendererComponent>();
              if (otherMeshRender == NULL)
                continue;
              // check for collision with buildings!
              if (otherEntity->name == "building") 
              {
                // buildings are planes

                // get the building model matrix
                glm::mat4 buildingModelMatrix = otherEntity->localTransform.toMat4();

                // loop on vertices of the building
                // get minx, maxx, miny, maxy, minz, maxz of builing renamed
                float minx2 = 0, maxx2 = 0, miny2 = 0, maxy2 = 0, minz2 = 0, maxz2 = 0;
                for(int i = 0; i < otherMeshRender->mesh->vertices.size(); i++)
                {
                  glm::vec4 vertex = buildingModelMatrix * glm::vec4(otherMeshRender->mesh->vertices[i], 1.0f);
                  if(i == 0)
                  {
                    minx2 = maxx2 = vertex.x;
                    miny2 = maxy2 = vertex.y;
                    minz2 = maxz2 = vertex.z;
                  }
                  else
                  {
                    minx2 = min(minx2, vertex.x);
                    maxx2 = max(maxx2, vertex.x);
                    miny2 = min(miny2, vertex.y);
                    maxy2 = max(maxy2, vertex.y);
                    minz2 = min(minz2, vertex.z);
                    maxz2 = max(maxz2, vertex.z);
                  }
                }
                
                // std::cout << "minx: " << minx << " maxx: " << maxx << std::endl;
                // std::cout << "minx2: " << minx2 << " maxx2: " << maxx2 << std::endl;

                // std::cout << "minz: " << minz << " maxz: " << maxz << std::endl;
                // std::cout << "minz2: " << minz2 << " maxz2: " << maxz2 << std::endl;
                
                auto eq = [](float a, float b) { return fabs(a - b) < 0.01f; };

                if (eq(minz2, maxz2))
                {
                  // std::cout<<"XY Plane"<<std::endl;
                  // building is in XY plane
                  // check collision with car
                  // check if minz and maxz are on same sides of Z-axis of building
                  if ((minz > minz2 && maxz > minz2) || (minz < minz2 && maxz < minz2))
                    continue;
                  if (maxx < minx2 || minx > maxx2)
                    continue;
               
                  // collision detected
                  // undo the movement!
                  entityPosition -= front * acceleration;
                  entityRotation += up * rotation;
                  entityPosition -= right * rotation;
                  entityPosition -= front * acceleration;
                  entityRotation += up * rotation;
                  entityPosition -= right * rotation;

                  acceleration *= elasticity;

                  // break from loop of entites!
                  break;
                }
                
                // same in XZ plane
                else if (eq(minx2, maxx2))
                {
                  // std::cout<<"XZ Plane"<<std::endl;
                  // building is in YZ plane
                  // check collision with car
                  if ((minx > minx2 && maxx > minx2) || (minx < minx2 && maxx < minx2))
                  {
                    // std::cout<<"1"<<std::endl;
                    continue;
                  }
                  if (maxz < minz2 || minz > maxz2)
                  {
                    // std::cout<<"2"<<std::endl;
                    continue;
                  }
                
                  // std::cout<<"COLLISION"<<std::endl;
                  // collision detected
                  // undo the movement!
                  entityPosition -= front * acceleration;
                  entityRotation += up * rotation;
                  entityPosition -= right * rotation;
                  entityPosition -= front * acceleration;
                  entityRotation += up * rotation;
                  entityPosition -= right * rotation;

                  acceleration *= elasticity;

                  // break from loop of entites!
                  break;
                }
                else 
                {
                   std::cout<<"UNKNOWN PLANE - Error in Building collision"<<std::endl;
                }
              }
            }
        }
    };

}
