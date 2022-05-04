#pragma once

#include "../ecs/component.hpp"
#include "../mesh/mesh.hpp"
#include "../material/material.hpp"
#include "../asset-loader.hpp"

namespace our {

    // This component denotes that any renderer should draw the given mesh using the given material at the transformation of the owning entity.
    class MeshRendererComponent : public Component {
    public:
        Mesh* mesh; // The mesh that should be drawn
        Material* material; // The material used to draw the mesh

        // The ID of this component type is "Mesh Renderer"
        static std::string getID() { return "Mesh Renderer"; }

        // Receives the mesh & material from the AssetLoader by the names given in the json object
        void deserialize(const nlohmann::json& data) override;
    };

}