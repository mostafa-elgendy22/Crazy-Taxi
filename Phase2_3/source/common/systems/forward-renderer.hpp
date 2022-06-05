#pragma once

#include "../ecs/world.hpp"
#include "../components/camera.hpp"
#include "../components/mesh-renderer.hpp"
#include "../asset-loader.hpp"

#include <glm/glm.hpp>
#include <glad/gl.h>
#include <vector>
#include <algorithm>

namespace our
{
    
    // The render command stores command that tells the renderer that it should draw
    // the given mesh at the given localToWorld matrix using the given material
    // The renderer will fill this struct using the mesh renderer components
    struct RenderCommand {
        glm::mat4 localToWorld;
        glm::vec3 center;
        Mesh* mesh;
        Material* material;
    };

    // A forward renderer is a renderer that draw the object final color directly to the framebuffer
    // In other words, the fragment shader in the material should output the color that we should see on the screen
    // This is different from more complex renderers that could draw intermediate data to a framebuffer before computing the final color
    // In this project, we only need to implement a forward renderer
    class ForwardRenderer {
        // These window size will be used on multiple occasions (setting the viewport, computing the aspect ratio, etc.)
        glm::ivec2 windowSize;
        // These are two vectors in which we will store the opaque and the transparent commands.
        // We define them here (instead of being local to the "render" function) as an optimization to prevent reallocating them every frame
        std::vector<RenderCommand> opaqueCommands;
        std::vector<RenderCommand> transparentCommands;
        // Objects used for rendering a skybox
        Mesh* skySphere;
        TexturedMaterial* skyMaterial;
        // Objects used for Postprocessing
        GLuint postprocessFrameBuffer, postProcessVertexArray;
        Texture2D *colorTarget, *depthTarget;
        TexturedMaterial* postprocessMaterial;

        // @@ Akram: 
        Texture2D *additionalTexture; // used for effects that need additional textures
        bool alwaysApply = false; // choose to make the effect applied the whole game or not
        bool apply = false; // only apply if set to true - it is always true if alwaysApply = true
        float effect_power = 0.05f;
        
        //lighting data
        glm::vec3 sky_top = {0.6, 0.6, 1.0};
        glm::vec3 sky_middle = {0.3, 0.3, 0.3};
        glm::vec3 sky_bottom = {0.1, 0.1, 0.0};

    public:
        // Initialize the renderer including the sky and the Postprocessing objects.
        // windowSize is the width & height of the window (in pixels).
        void initialize(glm::ivec2 windowSize, const nlohmann::json& config);
        // Clean up the renderer
        void destroy();
        // This function should be called every frame to draw the given world
        void render(World* world);
        // Apply postprocessing or not
        void changeApply(bool ok){
            if(!alwaysApply){ // can change it
                apply = ok;
            } else {
                apply = true;
            }
        }
    };

}