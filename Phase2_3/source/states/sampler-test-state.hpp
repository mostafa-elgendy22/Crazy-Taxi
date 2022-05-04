#pragma once

#include <shader/shader.hpp>
#include <mesh/mesh.hpp>
#include <texture/texture2d.hpp>
#include <texture/texture-utils.hpp>
#include <texture/sampler.hpp>
#include <application.hpp>


// This state tests and shows how to use the Sampler class.
class SamplerTestState: public our::State {

    our::ShaderProgram* shader;
    our::Mesh* mesh;
    our::Texture2D* texture;
    our::Sampler* sampler;
    
    void onInitialize() override {
        // First of all, we get the scene configuration from the app config
        auto& config = getApp()->getConfig()["scene"];
        // Then we load the shader that will be used for this scene
        shader = new our::ShaderProgram();
        shader->attach("assets/shaders/texture-test.vert", GL_VERTEX_SHADER);
        shader->attach("assets/shaders/texture-test.frag", GL_FRAGMENT_SHADER);
        shader->link();
        
        // We create a simple 2D plane to use for viewing the plane
        std::vector<our::Vertex> vertices = {
            { {-1, -1,  0}, {255, 255, 255, 255}, {-1.00, -1.00}, {0, 0, 1} },
            { { 1, -1,  0}, {255, 255, 255, 255}, { 2.00, -1.00}, {0, 0, 1} },
            { { 1,  1,  0}, {255, 255, 255, 255}, { 2.00,  2.00}, {0, 0, 1} },
            { {-1,  1,  0}, {255, 255, 255, 255}, {-1.00,  2.00}, {0, 0, 1} },
        };
        std::vector<unsigned int> elements = {
            0, 1, 2,
            2, 3, 0,
        };
        mesh = new our::Mesh(vertices, elements);

        // Then we create a texture and load an image into it
        texture = our::texture_utils::loadImage(config.value("texture", ""));

        // Then we create a sampler and load its paramters from the json config
        sampler = new our::Sampler();
        if(config.contains("sampler")){
            sampler->deserialize(config["sampler"]);
        }
    }

    void onDraw(double deltaTime) override {
        glClear(GL_COLOR_BUFFER_BIT);
        shader->use();
        // Here we set the active texture unit to 0 then bind the texture to it
        glActiveTexture(GL_TEXTURE0);
        texture->bind();
        // Then we bind the sampler to unit 0
        sampler->bind(0);
        // Then we send 0 (the index of the texture unit we used above) to the "tex" uniform
        shader->set("tex", 0);
        mesh->draw();
    }

    void onDestroy() override {
        delete shader;
        delete mesh;
        delete texture;
        delete sampler;
    }
};