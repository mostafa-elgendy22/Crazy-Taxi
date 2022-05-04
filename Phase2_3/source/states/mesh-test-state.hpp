#pragma once

#include <shader/shader.hpp>
#include <mesh/mesh.hpp>
#include <mesh/mesh-utils.hpp>
#include <application.hpp>

// This state tests and shows how to use the Mesh Class.
class MeshTestState: public our::State {

    our::ShaderProgram* shader;
    our::Mesh* mesh;
    
    void onInitialize() override {
        // First of all, we get the scene configuration from the app config
        auto& config = getApp()->getConfig()["scene"];
        // Then we load the shader that will be used for this scene
        shader = new our::ShaderProgram();
        shader->attach("assets/shaders/mesh-test.vert", GL_VERTEX_SHADER);
        shader->attach("assets/shaders/mesh-test.frag", GL_FRAGMENT_SHADER);
        shader->link();
        // Then we will set the output_type: 0=Position, 1=Color, 2=TexCoord, 3=Normal
        shader->use();
        shader->set("output_type", config.value("output_type", 0));
        // Then we get the path to the mesh data
        std::string meshPath = config.value("mesh", "");
        if(meshPath.size() != 0){
            // If it is not empty, we load the OBJ file
            mesh = our::mesh_utils::loadOBJ(meshPath);
        } else {
            // Otherwise, we create a simple diamond object
            std::vector<our::Vertex> vertices = {
                { { 1,  0,  0}, {  0, 128,  64, 255}, {0.00, 1.00}, {-1,  0,  0} },
                { { 0,  1,  0}, { 64,   0, 128, 255}, {1.00, 1.00}, { 0, -1,  0} },
                { { 0,  0,  1}, {128,  64,   0, 255}, {0.33, 0.67}, { 0,  0, -1} },
                { {-1,  0,  0}, {255, 128,  64, 255}, {0.67, 0.33}, { 1,  0,  0} },
                { { 0, -1,  0}, { 64, 255, 128, 255}, {0.00, 0.00}, { 0,  1,  0} },
                { { 0,  0, -1}, {128,  64, 255, 255}, {1.00, 0.00}, { 0,  0,  1} },
            };
            std::vector<unsigned int> elements = {
                0, 1, 2,
                2, 1, 3,
                3, 1, 5,
                5, 1, 0,
                0, 4, 5,
                5, 4, 3,
                3, 4, 2,
                2, 4, 0
            };
            mesh = new our::Mesh(vertices, elements);
        }
    }

    void onDraw(double deltaTime) override {
        // Clear the screen color
        glClear(GL_COLOR_BUFFER_BIT);
        // Use the shader then draw the mesh
        shader->use();
        mesh->draw();
    }

    void onDestroy() override {
        delete shader;
        delete mesh;
    }
};