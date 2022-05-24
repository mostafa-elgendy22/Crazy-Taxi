#include "light.hpp"
#include "../ecs/entity.hpp"
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp> 
#include "../deserialize-utils.hpp"


namespace our {
    // Reads light parameters from the given json object
    void LightComponent::deserialize(const nlohmann::json& data){
        if(!data.is_object()) return;
        std::string lightTypeStr = data.value("lightType", "directional");
        if(lightTypeStr == "spot"){
            lightType = LightType::SPOT;
        } else if(lightTypeStr=="point"){
            lightType = LightType::POINT;
        } else {
            lightType = LightType::DIRECTIONAL;
        }
        color = data.value("color", color);
        cone_angles = data.value("cone_angles", cone_angles);
        attenuation = data.value("attenuation", attenuation);
    }

    // send light data to the given shader
    void LightComponent::sendData(ShaderProgram* shader, int lightIndex, glm::vec3 direction, glm::vec3 position){
        std::string indexStr = std::to_string(lightIndex);
        std::string uni = "lights[" + indexStr + "].";
        
        // Setting shader data
        shader->set(uni + "type", int(lightType)); // === lights[i].type = int(lightType);
        shader->set(uni + "position", position); // not used for directional light
        shader->set(uni + "direction", direction);
        shader->set(uni + "diffuse", color); // diffuse = specular = color
        shader->set(uni + "specular", color); // diffuse = specular = color
        shader->set(uni + "attenuation", attenuation); // not used for directional light
        shader->set(uni + "cone_angles", cone_angles); // not used for directional, point light

        // if(lightType == LightType::DIRECTIONAL){

        // } else if (lightType == LightType::SPOT){

        // }else{

        // }
        
    }

}