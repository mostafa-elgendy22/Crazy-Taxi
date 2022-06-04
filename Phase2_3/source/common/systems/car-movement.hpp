#pragma once

#include "../ecs/world.hpp"
#include "../components/car-movement.hpp"
#include "../components/camera.hpp"
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
        float timeInCollision = 0.0;
        const float collisionReturningTime = 0.5f;
    public:
        // When a state enters, it should call this function and give it the pointer to the application
        void enter(Application* app){
            this->app = app;
        }

        void update(World* world, float deltaTime, ForwardRenderer* renderer) {

            CarMovementComponent *movement = nullptr;
            Entity* arrow = nullptr;
            bool collisionHappened = false;
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
            if (arrow) {
              glm::vec3& arrowRotation = arrow->localTransform.rotation;
              arrowRotation -= up * rotation;
            }
            entityPosition += right * rotation;
            
            entityRotation.y = glm::wrapAngle(entityRotation.y);


            // rotation=0;


            // Gheiath: Collison detection
            // get car model matrix
            glm::mat4 carModelMatrix = entity->getLocalToWorldMatrix();
            // get minx, maxx, miny, maxy, minz, maxz from car mesh vec3::min vec3::max
            MeshRendererComponent* MeshRender = entity->getComponent<MeshRendererComponent>();
            glm::vec3 carMin = MeshRender->mesh->min;
            glm::vec3 carMax = MeshRender->mesh->max;
            // get car bounding box
            glm::vec3 carBoundingBox[8] = {
                glm::vec3(carMin.x, carMin.y, carMin.z),
                glm::vec3(carMax.x, carMin.y, carMin.z),
                glm::vec3(carMax.x, carMax.y, carMin.z),
                glm::vec3(carMin.x, carMax.y, carMin.z),
                glm::vec3(carMin.x, carMin.y, carMax.z),
                glm::vec3(carMax.x, carMin.y, carMax.z),
                glm::vec3(carMax.x, carMax.y, carMax.z),
                glm::vec3(carMin.x, carMax.y, carMax.z)
            };
            // multiply car bounding box by car model matrix
            for (int i = 0; i < 8; i++) {
                carBoundingBox[i] = carModelMatrix * glm::vec4(carBoundingBox[i], 1.0f);
            }
            // get minx, maxx, miny, maxy, minz, maxz from bounding box
            float minx = carBoundingBox[0].x, maxx = carBoundingBox[0].x, miny = carBoundingBox[0].y, maxy = carBoundingBox[0].y, minz = carBoundingBox[0].z, maxz = carBoundingBox[0].z;
            for (int i = 1; i < 8; i++) {
                minx = min(minx, carBoundingBox[i].x);
                maxx = max(maxx, carBoundingBox[i].x);
                miny = min(miny, carBoundingBox[i].y);
                maxy = max(maxy, carBoundingBox[i].y);
                minz = min(minz, carBoundingBox[i].z);
                maxz = max(maxz, carBoundingBox[i].z);
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
                glm::mat4 buildingModelMatrix = otherEntity->getLocalToWorldMatrix();
                // get minx, maxx, miny, maxy, minz, maxz from building mesh vec3::min vec3::max
                glm::vec3 buildingMin = otherMeshRender->mesh->min;
                glm::vec3 buildingMax = otherMeshRender->mesh->max;
                if (otherMeshRender == NULL)
                  continue;
                // get building bounding box
                glm::vec3 buildingBoundingBox[8] = {
                    glm::vec3(buildingMin.x, buildingMin.y, buildingMin.z),
                    glm::vec3(buildingMax.x, buildingMin.y, buildingMin.z),
                    glm::vec3(buildingMax.x, buildingMax.y, buildingMin.z),
                    glm::vec3(buildingMin.x, buildingMax.y, buildingMin.z),
                    glm::vec3(buildingMin.x, buildingMin.y, buildingMax.z),
                    glm::vec3(buildingMax.x, buildingMin.y, buildingMax.z),
                    glm::vec3(buildingMax.x, buildingMax.y, buildingMax.z),
                    glm::vec3(buildingMin.x, buildingMax.y, buildingMax.z)
                };
                // multiply building bounding box by building model matrix
                for (int i = 0; i < 8; i++) {
                    buildingBoundingBox[i] = buildingModelMatrix * glm::vec4(buildingBoundingBox[i], 1.0f);
                }
                // get minx2, maxx2, miny2, maxy2, minz2, maxz2 from bounding box
                float minx2 = buildingBoundingBox[0].x, maxx2 = buildingBoundingBox[0].x, miny2 = buildingBoundingBox[0].y, maxy2 = buildingBoundingBox[0].y, minz2 = buildingBoundingBox[0].z, maxz2 = buildingBoundingBox[0].z;
                for (int i = 1; i < 8; i++) {
                    minx2 = min(minx2, buildingBoundingBox[i].x);
                    maxx2 = max(maxx2, buildingBoundingBox[i].x);
                    miny2 = min(miny2, buildingBoundingBox[i].y);
                    maxy2 = max(maxy2, buildingBoundingBox[i].y);
                    minz2 = min(minz2, buildingBoundingBox[i].z);
                    maxz2 = max(maxz2, buildingBoundingBox[i].z);
                }

                // std::cout << "minx: " << minx << " maxx: " << maxx << std::endl;
                // std::cout << "minx2: " << minx2 << " maxx2: " << maxx2 << std::endl;

                // std::cout << "minz: " << minz << " maxz: " << maxz << std::endl;
                // std::cout << "minz2: " << minz2 << " maxz2: " << maxz2 << std::endl;
                
                auto eq = [](float a, float b) { return fabs(a - b) < 0.1f; };

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
                  collisionHappened = true;
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
                  collisionHappened = true;
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


          // change the camera view if V is pressed
          if(app->getKeyboard().isPressed(GLFW_KEY_V))
          {
            CameraComponent* camera = nullptr;
            // get the camera
            for(auto entity : world->getEntities())
            {
              camera = entity->getComponent<CameraComponent>();
              if(camera)break;
            }
            //return if there is no camera
            if(!camera)return;
            //get the entity of the camera
            Entity *camEntity=camera->getOwner();
            

          }
          if(collisionHappened){
            renderer->changeApply(true);
            timeInCollision = deltaTime;
            // std::cout<<"COllISION!!!\n";
          }
          if(timeInCollision>collisionReturningTime){
            timeInCollision = 0.0;
            renderer->changeApply(false);
          } else{
            timeInCollision+=deltaTime;
          }
        }
    };

}
