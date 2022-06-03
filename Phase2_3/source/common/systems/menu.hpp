#pragma once

#include "../ecs/world.hpp"
#include "../components/car-movement.hpp"

#include "../application.hpp"

#include <glm/glm.hpp>
#include <glm/gtc/constants.hpp>
#include <glm/trigonometric.hpp>
#include <glm/gtx/fast_trigonometry.hpp>

namespace our
{

    class MenuSystem {
        Application* app; // The application in which the state runs

    public:
        // When a state enters, it should call this function and give it the pointer to the application
        void enter(Application* app){
            this->app = app;
        }

        void update(World* world, float deltaTime) 
        {
            if(app->getKeyboard().isPressed(GLFW_KEY_ENTER))app->changeState("main");       
        }
    };

}
