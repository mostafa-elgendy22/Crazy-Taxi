#pragma once

#include <asset-loader.hpp>
#include <ecs/transform.hpp>
#include <application.hpp>
#include <deserialize-utils.hpp>

#include <vector>
#include <glm/gtc/matrix_transform.hpp> 

// This state shows how to use the Material class.
// It also shows how to use the AssetLoader to load assets
class MaterialTestState: public our::State {

    our::Material* material;
    our::Mesh* mesh;
    std::vector<our::Transform> transforms;
    glm::mat4 VP;
    
    void onInitialize() override {
        // First of all, we get the scene configuration from the app config
        auto& config = getApp()->getConfig()["scene"];
        // If we have assets in the scene config, we deserialize them
        if(config.contains("assets")){
            our::deserializeAllAssets(config["assets"]);
        }
        // We get the mesh and the material from AssetLoader 
        mesh = our::AssetLoader<our::Mesh>::get("mesh");
        material = our::AssetLoader<our::Material>::get("material");

        // Then we read a list of transform objects from the shader
        // In draw, we will render a mesh for each of the transforms
        transforms.clear();
        if(config.contains("objects")){
            if(auto& objects = config["objects"]; objects.is_array()){
                for(auto& object : objects){
                    our::Transform transform;
                    transform.deserialize(object);
                    transforms.push_back(transform);
                }
            }
        }
        // Then we read the camera information to compute the VP matrix
        if(config.contains("camera")){
            if(auto& camera = config["camera"]; camera.is_object()){
                glm::vec3 eye = camera.value("eye", glm::vec3(0, 0, 0));
                glm::vec3 center = camera.value("center", glm::vec3(0, 0, -1));
                glm::vec3 up = camera.value("up", glm::vec3(0, 1, 0));
                glm::mat4 V = glm::lookAt(eye, center, up);

                float fov = glm::radians(camera.value("fov", 90.0f));
                float near = camera.value("near", 0.01f);
                float far = camera.value("far", 1000.0f);

                glm::ivec2 size = getApp()->getFrameBufferSize();
                float aspect = float(size.x)/size.y;
                glm::mat4 P = glm::perspective(fov, aspect, near, far);

                VP = P * V;
            }
        }
        
    }

    void onDraw(double deltaTime) override {
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        // The material setup will use the shader, setup the pipeline state
        // and send the uniforms that are common between objects using the same material
        material->setup();
        for(auto& transform : transforms){
            // For each transform, we compute the MVP matrix and send it to the "transform" uniform
            material->shader->set("transform", VP * transform.toMat4());
            // Then we draw a mesh instance
            mesh->draw();
        }
    }

    void onDestroy() override {
        our::clearAllAssets();
    }
};