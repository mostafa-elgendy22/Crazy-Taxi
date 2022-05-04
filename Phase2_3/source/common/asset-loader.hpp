#pragma once

#include <unordered_map>
#include <string>
#include <json/json.hpp>

namespace our {

    // This static template class will hold the loaded assets
    // and can be called from anywhere to get an asset by its name.
    // Since we have different types of assets, this declared as a template class
    // and for each asset type, we define a specialization in "asset-loader.cpp"
    template<typename T>
    class AssetLoader {
        // This map stores a pointer to each asset identified by its name
        // All assets in this map are owned by the asset loader so it should not be deleted outside of this class
        static inline std::unordered_map<std::string, T*> assets;
    public:
        // This function loads the assets defined by the given json object
        // The json object should be defined in the form: {asset_name: asset_description}
        // For example: {"white": "textures/white.png", "polka": "textures/polka.png"} defines 2 textures
        // where the key will be asset name and the description holds the path to the texture file
        static void deserialize(const nlohmann::json&);
        // This function find an asset by its name and returns a pointer to it
        // If no asset with the given name was found, the function returns a nullptr
        // WARNING: never delete the asset returned by the function.
        // The asset could be shared with another object and
        // all the assets will be automatically cleared when the function "clear" is called
        static T* get(const std::string& name) {
            if(auto it = assets.find(name); it != assets.end()){
                return it->second;
            }
            return nullptr;
        };
        // This function deletes all the assets held by this class and clear the assets map 
        static void clear(){
            for(auto& [name, asset] : assets){
                delete asset;
            }
            assets.clear();
        }
    };

    // Given a json holding the data for all the assets
    // This function will call "AssetLoader<T>::deserialize" for all the different asset types T
    // For example, a json in the form {"shaders": ... , "textures": ... } will call "deserialize" for:
    // AssetLoader<ShaderProgram> and AssetLoader<Texture2D>
    void deserializeAllAssets(const nlohmann::json& assetData);
    // This will call "AssetLoader<T>::clear" for all the different asset types T
    void clearAllAssets();
}