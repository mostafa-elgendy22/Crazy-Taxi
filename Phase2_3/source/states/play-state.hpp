#pragma once

#include <application.hpp>

#include <ecs/world.hpp>
#include <systems/forward-renderer.hpp>
#include <systems/free-camera-controller.hpp>
#include <systems/movement.hpp>
#include <asset-loader.hpp>
#include <systems/car-movement.hpp>
#include <systems/deliver.hpp>
#include <systems/camera-view.hpp>
#include <systems/passenger-movement.hpp>

// This state shows how to use the ECS framework and deserialization.
class Playstate: public our::State {

    our::World world;
    our::ForwardRenderer renderer;
    our::FreeCameraControllerSystem cameraController;
    our::MovementSystem movementSystem;
    our::CarMovementSystem carMovementSystem;
    our::DeliverSystem deliverSystem;
    our::CameraViewSystem cameraViewSystem;
    our::PassengerMovementSystem passengerMovementSystem;

    void onInitialize() override {
        // First of all, we get the scene configuration from the app config
        auto& config = getApp()->getConfig()["scene"];
        // If we have assets in the scene config, we deserialize them
        if(config.contains("assets")){
            our::deserializeAllAssets(config["assets"]);
        }
        // If we have a world in the scene config, we use it to populate our world
        if(config.contains("world")){
            world.deserialize(config["world"]);
        }
        // We initialize the camera controller system since it needs a pointer to the app
        cameraController.enter(getApp());
        // We initialize the car movement system since it needs a pointer to the app
        carMovementSystem.enter(getApp());
        // We initialize the deliver system since it needs a pointer to the app
        deliverSystem.enter(getApp());
        // We initialize the camera view system since it needs a pointer to the app
        cameraViewSystem.enter(getApp());
        // We initialize the passenger movement system since it needs a pointer to the app
        passengerMovementSystem.enter(getApp());
        // Then we initialize the renderer
        auto size = getApp()->getFrameBufferSize();
        renderer.initialize(size, config["renderer"]);
    }

    void onDraw(double deltaTime) override {
        // Here, we just run a bunch of systems to control the world logic
        movementSystem.update(&world, (float)deltaTime);
        cameraController.update(&world, (float)deltaTime);
        carMovementSystem.update(&world,(float)deltaTime, &renderer);
        deliverSystem.update(&world,(float)deltaTime);
        cameraViewSystem.update(&world,(float)deltaTime);
        passengerMovementSystem.update(&world,(float)deltaTime);
        // And finally we use the renderer system to draw the scene
        renderer.render(&world);
    }

    void onDestroy() override {
        // Don't forget to destroy the renderer
        renderer.destroy();
        // On exit, we call exit for the camera controller system to make sure that the mouse is unlocked
        cameraController.exit();
        // and we delete all the loaded assets to free memory on the RAM and the VRAM
        our::clearAllAssets();
    }
};