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

                if(passenger->tknsteps < passenger->steps)
                {
                    position += passenger->speed * front;
                    passenger->tknsteps++;
                }

                int random = std::rand()%passenger->moveProb;

                if(random == 0 && passenger->steps == passenger->tknsteps)
                {
                    rotation.y += glm::radians((std::rand()%181)*1.0f);
                    passenger->tknsteps=0;
                }

                // passenger collision
                // get pass model matrix
                glm::mat4 passModelMatrix = entity->getLocalToWorldMatrix();
                // get minx, maxx, miny, maxy, minz, maxz from pass mesh vec3::min vec3::max
                MeshRendererComponent* MeshRender = entity->getComponent<MeshRendererComponent>();
                glm::vec3 passMin = MeshRender->mesh->min;
                glm::vec3 passMax = MeshRender->mesh->max;
                // get pass bounding box
                glm::vec3 passBoundingBox[8] = {
                    glm::vec3(passMin.x, passMin.y, passMin.z),
                    glm::vec3(passMax.x, passMin.y, passMin.z),
                    glm::vec3(passMax.x, passMax.y, passMin.z),
                    glm::vec3(passMin.x, passMax.y, passMin.z),
                    glm::vec3(passMin.x, passMin.y, passMax.z),
                    glm::vec3(passMax.x, passMin.y, passMax.z),
                    glm::vec3(passMax.x, passMax.y, passMax.z),
                    glm::vec3(passMin.x, passMax.y, passMax.z)
                };
                // multiply pass bounding box by pass model matrix
                for (int i = 0; i < 8; i++) {
                    passBoundingBox[i] = passModelMatrix * glm::vec4(passBoundingBox[i], 1.0f);
                }
                // get minx, maxx, miny, maxy, minz, maxz from bounding box
                float minx = passBoundingBox[0].x, maxx = passBoundingBox[0].x, miny = passBoundingBox[0].y, maxy = passBoundingBox[0].y, minz = passBoundingBox[0].z, maxz = passBoundingBox[0].z;
                for (int i = 1; i < 8; i++) {
                    minx = min(minx, passBoundingBox[i].x);
                    maxx = max(maxx, passBoundingBox[i].x);
                    miny = min(miny, passBoundingBox[i].y);
                    maxy = max(maxy, passBoundingBox[i].y);
                    minz = min(minz, passBoundingBox[i].z);
                    maxz = max(maxz, passBoundingBox[i].z);
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
                        
                        bool collisionHappened = false;

                        if (eq(minz2, maxz2))
                        {
                            // std::cout<<"XY Plane"<<std::endl;
                            // building is in XY plane
                            // check collision with pass
                            // check if minz and maxz are on same sides of Z-axis of building
                            if ((minz > minz2 && maxz > minz2) || (minz < minz2 && maxz < minz2))
                                continue;
                            if (maxx < minx2 || minx > maxx2)
                                continue;
                        
                            // collision detected
                            collisionHappened = true;
                            // undo the movement!
                            position-=passenger->speed * front;
                            rotation.y+=glm::radians(180.0f);
                            // break from loop of entites!
                            break;
                        }
                        
                        // same in XZ plane
                        else if (eq(minx2, maxx2))
                        {
                            // std::cout<<"XZ Plane"<<std::endl;
                            // building is in YZ plane
                            // check collision with pass
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
                            position-=passenger->speed * front;
                            rotation.y+=glm::radians(180.0f);

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

        }

    };

}
