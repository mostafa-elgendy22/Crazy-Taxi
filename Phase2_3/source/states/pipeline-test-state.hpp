#pragma once

#include <shader/shader.hpp>
#include <mesh/mesh.hpp>
#include <mesh/mesh-utils.hpp>
#include <ecs/transform.hpp>
#include <material/pipeline-state.hpp>
#include <application.hpp>
#include <deserialize-utils.hpp>

#include <vector>
#include <glm/gtc/matrix_transform.hpp> 


// This state tests and shows how to use the PipelineState struct.
class PipelineTestState: public our::State {

    our::ShaderProgram* shader;
    our::Mesh* mesh;
    std::vector<our::Transform> transforms;
    glm::mat4 VP;
    our::PipelineState pipeline;
    
    void onInitialize() override {
        // First of all, we get the scene configuration from the app config
        auto& config = getApp()->getConfig()["scene"];
        // Then we load the shader that will be used for this scene
        shader = new our::ShaderProgram();
        shader->attach("assets/shaders/transform-test.vert", GL_VERTEX_SHADER);
        shader->attach("assets/shaders/transform-test.frag", GL_FRAGMENT_SHADER);
        shader->link();
        // Then we load the mesh
        mesh = our::mesh_utils::loadOBJ("assets/models/monkey.obj");
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
        // Then we read the pipeline state from the json config
        if(config.contains("pipeline")){
            pipeline.deserialize(config["pipeline"]);
        }
        // We also read the clear color and depth since we may want to change it
        glm::vec4 clearColor = config.value("clearColor", glm::vec4(0, 0, 0, 0));
        glClearColor(clearColor.r, clearColor.g, clearColor.b, clearColor.a);
        glClearDepth(config.value("clearDepth", 1.0f));
    }

    void onDraw(double deltaTime) override {
        // We make sure the color and depth masks are true (just in case the pipeline set any of them to false)
        // to make sure that glClear works correctly
        glColorMask(true, true, true, true);
        glDepthMask(true);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        shader->use();
        // Before drawing, we setup the pipeline state
        pipeline.setup();
        // Then we draw the objects
        for(auto& transform : transforms){
            shader->set("transform", VP * transform.toMat4());
            mesh->draw();
        }
    }

    void onDestroy() override {
        delete shader;
        delete mesh;
    }
};