#include "asset-loader.hpp"

#include "shader/shader.hpp"
#include "texture/texture2d.hpp"
#include "texture/texture-utils.hpp"
#include "texture/sampler.hpp"
#include "mesh/mesh.hpp"
#include "mesh/mesh-utils.hpp"
#include "material/material.hpp"
#include "deserialize-utils.hpp"

namespace our {

    // This will load all the shaders defined in "data"
    // data must be in the form:
    //    { shader_name : { "vs" : "path/to/vertex-shader", "fs" : "path/to/fragment-shader" }, ... }
    template<>
    void AssetLoader<ShaderProgram>::deserialize(const nlohmann::json& data) {
        if(data.is_object()){
            for(auto& [name, desc] : data.items()){
                std::string vsPath = desc.value("vs", "");
                std::string fsPath = desc.value("fs", "");
                auto shader = new ShaderProgram();
                shader->attach(vsPath, GL_VERTEX_SHADER);
                shader->attach(fsPath, GL_FRAGMENT_SHADER);
                shader->link();
                assets[name] = shader;
            }
        }
    };

    // This will load all the textures defined in "data"
    // data must be in the form:
    //    { texture_name : "path/to/image", ... }
    template<>
    void AssetLoader<Texture2D>::deserialize(const nlohmann::json& data) {
        if(data.is_object()){
            for(auto& [name, desc] : data.items()){
                std::string path = desc.get<std::string>();
                assets[name] = texture_utils::loadImage(path);
            }
        }
    };

    // This will load all the samplers defined in "data"
    // data must be in the form:
    //    { sampler_name : parameters, ... }
    // Where parameters is an object where:
    //      The key is the parameter name, e.g. "MAG_FILTER", "MIN_FILTER", "WRAP_S", "WRAP_T" or "MAX_ANISOTROPY"
    //      The value is the parameter value, e.g. "GL_NEAREST", "GL_REPEAT"
    //  For "MAX_ANISOTROPY", the value must be a float with a value >= 1.0f
    template<>
    void AssetLoader<Sampler>::deserialize(const nlohmann::json& data) {
        if(data.is_object()){
            for(auto& [name, desc] : data.items()){
                auto sampler = new Sampler();
                sampler->deserialize(desc);
                assets[name] = sampler;
            }
        }
    };

    // This will load all the meshes defined in "data"
    // data must be in the form:
    //    { mesh_name : "path/to/3d-model-file", ... }
    template<>
    void AssetLoader<Mesh>::deserialize(const nlohmann::json& data) {
        if(data.is_object()){
            for(auto& [name, desc] : data.items()){
                std::string path = desc.get<std::string>();
                assets[name] = mesh_utils::loadOBJ(path);
            }
        }
    };

    // This will load all the materials defined in "data"
    // Material deserialization depends on shaders, textures and samplers
    // so you must deserialize these 3 asset types before deserializing materials
    // data must be in the form:
    //    { material_name : parameters, ... }
    // Where parameters is an object where the keys can be:
    //      "type" where the value is a string defining the type of the material.
    //              the type will decide which class will be instanced in the function "createMaterialFromType" found in "material.hpp"
    //      "shader" where the value must be the name of a loaded shader
    //      "pipelineState" (optional) where the value is a json object that can be read by "PipelineState::deserialize"
    //      "transparent" (optional, default=false) where the value is a boolean indicating whether the material is transparent or not
    //      ... more keys/values can be added depending on the material type (e.g. "texture", "sampler", "tint")
    template<>
    void AssetLoader<Material>::deserialize(const nlohmann::json& data) {
        if(data.is_object()){
            for(auto& [name, desc] : data.items()){
                std::string type = desc.value("type", "");
                auto material = createMaterialFromType(type);
                material->deserialize(desc);
                assets[name] = material;
            }
        }
    };

    void deserializeAllAssets(const nlohmann::json& assetData){
        if(!assetData.is_object()) return;
        if(assetData.contains("shaders"))
            AssetLoader<ShaderProgram>::deserialize(assetData["shaders"]);
        if(assetData.contains("textures"))
            AssetLoader<Texture2D>::deserialize(assetData["textures"]);
        if(assetData.contains("samplers"))
            AssetLoader<Sampler>::deserialize(assetData["samplers"]);
        if(assetData.contains("meshes"))
            AssetLoader<Mesh>::deserialize(assetData["meshes"]);
        if(assetData.contains("materials"))
            AssetLoader<Material>::deserialize(assetData["materials"]);
    }

    void clearAllAssets(){
        AssetLoader<ShaderProgram>::clear();
        AssetLoader<Texture2D>::clear();
        AssetLoader<Sampler>::clear();
        AssetLoader<Mesh>::clear();
        AssetLoader<Material>::clear();
    }

}