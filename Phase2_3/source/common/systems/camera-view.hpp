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

            // change the camera view if V is pressed
            if(!(app->getKeyboard().isPressed(GLFW_KEY_V)))return;
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
            
            if(app->getKeyboard().isPressed(GLFW_KEY_1))position=glm::vec3(0,3,10);
            if(app->getKeyboard().isPressed(GLFW_KEY_2))position=glm::vec3(0,8,15);
            if(app->getKeyboard().isPressed(GLFW_KEY_3))position=glm::vec3(0,2.5,0);
            if(app->getKeyboard().isPressed(GLFW_KEY_4))position=glm::vec3(-.5,2.5,0);
            if(app->getKeyboard().isPressed(GLFW_KEY_5))position=glm::vec3(0,2.5,-1);

        }

    };
}

