#pragma once

#include <application.hpp>

#include <ecs/world.hpp>
#include <systems/forward-renderer.hpp>
#include <systems/free-camera-controller.hpp>
#include <systems/movement.hpp>
#include <asset-loader.hpp>
#include <systems/car-movement.hpp>
#include <systems/deliver.hpp>
#include <systems/menu.hpp>

// This state shows how to use the ECS framework and deserialization.
class Menustate: public our::State {

    our::World world;
    our::ForwardRenderer renderer;
    our::MovementSystem movementSystem;
    our::MenuSystem menuSystem;

    void onInitialize() override {
        // First of all, we get the scene configuration from the app config
        auto& config = getApp()->getConfig()["scene"];
        // If we have assets in the scene config, we deserialize them
        if(config.contains("assets")){
            our::deserializeAllAssets(config["assets"]);
        }
        // If we have a world in the scene config, we use it to populate our world
        if(config.contains("world")){
            world.deserialize(config["world"], "menu");
        }

        menuSystem.enter(getApp());

        auto size = getApp()->getFrameBufferSize();
        renderer.initialize(size, config["renderer"]);
    }

    void onDraw(double deltaTime) override {
        // Here, we just run a bunch of systems to control the world logic
        movementSystem.update(&world, (float)deltaTime);

        menuSystem.update(&world, (float)deltaTime);

        // And finally we use the renderer system to draw the scene
        renderer.render(&world);
    }

    void onDestroy() override {
        // Don't forget to destroy the renderer
        renderer.destroy();
        // and we delete all the loaded assets to free memory on the RAM and the VRAM
        our::clearAllAssets();
    }
};