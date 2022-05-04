#include "pipeline-state.hpp"
#include "../deserialize-utils.hpp"

namespace our {

    // Given a json object, this function deserializes a PipelineState structure
    void PipelineState::deserialize(const nlohmann::json& data){
        // If the given json data does not represent a json object, return
        if(!data.is_object()) return;
        
        // If data contain a key "faceCulling" whose value is an object, read its content into the faceCulling struct
        if(data.contains("faceCulling")){
            if(const auto& config = data["faceCulling"]; config.is_object()){
            
                faceCulling.enabled = config.value("enabled", faceCulling.enabled);
                
                if(auto it = gl_enum_deserialize::facets.find(config.value("culledFace","")); 
                it != gl_enum_deserialize::facets.end())
                    faceCulling.culledFace = it->second;
                
                if(auto it = gl_enum_deserialize::face_windings.find(config.value("frontFace","")); 
                it != gl_enum_deserialize::face_windings.end())
                    faceCulling.frontFace = it->second;
            }
        }

        // If data contain a key "depthTesting" whose value is an object, read its content into the depthTesting struct
        if(data.contains("depthTesting")){
            if(const auto& config = data["depthTesting"]; config.is_object()){
        
                depthTesting.enabled = config.value("enabled", depthTesting.enabled);
                
                if(auto it = gl_enum_deserialize::comparison_functions.find(config.value("function",""));
                it != gl_enum_deserialize::comparison_functions.end())
                    depthTesting.function = it->second;
            }
        }

        // If data contain a key "blending" whose value is an object, read its content into the blending struct
        if(data.contains("blending")){
            if(const auto& config = data["blending"]; config.is_object()){
        
                blending.enabled = config.value("enabled", blending.enabled);
                
                if(auto it = gl_enum_deserialize::blend_equations.find(config.value("equation",""));
                it != gl_enum_deserialize::blend_equations.end())
                    blending.equation = it->second;
                
                if(auto it = gl_enum_deserialize::blend_functions.find(config.value("sourceFactor","")); 
                it != gl_enum_deserialize::blend_functions.end())
                    blending.sourceFactor = it->second;
                
                if(auto it = gl_enum_deserialize::blend_functions.find(config.value("destinationFactor","")); 
                it != gl_enum_deserialize::blend_functions.end())
                    blending.destinationFactor = it->second;
                
                blending.constantColor = config.value("constantColor", blending.constantColor);
            }
        }

        // Read the color mask and depth mask from the json data if they exist
        colorMask = data.value("colorMask", colorMask);
        depthMask = data.value("depthMask", depthMask);
    }

}