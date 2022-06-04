#pragma once

#include "../ecs/world.hpp"
#include "../components/camera.hpp"
#include "../application.hpp"

#include <glm/glm.hpp>
#include <glm/gtc/constants.hpp>
#include <glm/trigonometric.hpp>
#include <glm/gtx/fast_trigonometry.hpp>

namespace our
{

    class CameraViewSystem {
        Application* app; // The application in which the state runs

    public:
        // When a state enters, it should call this function and give it the pointer to the application
        void enter(Application* app){
            this->app = app;
        }

        void update(World* world, float deltaTime) {

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
            Entity* entity=camera->getOwner();
            // We get a reference to the entity's position and rotation
            glm::vec3& position = entity->localTransform.position;
            glm::vec3& rotation = entity->localTransform.rotation;

            // look left Q key
            if (app->getKeyboard().isPressed(GLFW_KEY_Q)) 
                rotation.y = glm::radians(90.0f);
            // look right E key
            else if (app->getKeyboard().isPressed(GLFW_KEY_E)) 
                rotation.y = glm::radians(-90.0f);
            // look back TAB key
            else if (app->getKeyboard().isPressed(GLFW_KEY_TAB)) 
                rotation.y = glm::radians(180.0f);
            else
              rotation.y = 0.0f;
            
            if(app->getKeyboard().isPressed(GLFW_KEY_1))
            {
              // normal view
              position=glm::vec3(0,3,10);
              rotation=glm::vec3(0,0,0);
            }
            if(app->getKeyboard().isPressed(GLFW_KEY_2))
            {
              // far a way view
              position=glm::vec3(0,8,15);
              rotation=glm::vec3(0,0,0);
            }
            if(app->getKeyboard().isPressed(GLFW_KEY_3))
            {
              // inside car view
              position=glm::vec3(0,2.5,0);
              rotation=glm::vec3(0,0,0);
            }
            if(app->getKeyboard().isPressed(GLFW_KEY_4))
            {
              // driver seat view
              position=glm::vec3(-0.9,2.5,0);
              rotation=glm::vec3(0,0,0);
            }
            if(app->getKeyboard().isPressed(GLFW_KEY_5))
            {
              // front view
              position=glm::vec3(0,2.5,-1);
              rotation=glm::vec3(0,0,0);
            }
            if(app->getKeyboard().isPressed(GLFW_KEY_6))
            {
              // front of car view
              position=glm::vec3(0,2.5,-4.3);
              rotation=glm::vec3(0,0,0);
            }
            if(app->getKeyboard().isPressed(GLFW_KEY_7))
            {
              // bird's eye view
              position=glm::vec3(0,34,0);
              rotation=glm::vec3(glm::radians(-90.0f),0,0);
            }

        }

    };
}

