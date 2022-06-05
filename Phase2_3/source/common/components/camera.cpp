#include "camera.hpp"
#include "../ecs/entity.hpp"
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp> 

#include "../deserialize-utils.hpp"
#include <glm/gtx/euler_angles.hpp>
#include <glm/ext/matrix_transform.hpp>
#include <glm/ext/matrix_clip_space.hpp>

namespace our {
    // Reads camera parameters from the given json object
    void CameraComponent::deserialize(const nlohmann::json& data){
        if(!data.is_object()) return;
        std::string cameraTypeStr = data.value("cameraType", "perspective");
        if(cameraTypeStr == "orthographic"){
            cameraType = CameraType::ORTHOGRAPHIC;
        } else {
            cameraType = CameraType::PERSPECTIVE;
        }
        near = data.value("near", 0.01f);
        far = data.value("far", 100.0f);
        fovY = data.value("fovY", 90.0f) * (glm::pi<float>() / 180);
        orthoHeight = data.value("orthoHeight", 1.0f);

        key1position = data.value("key1position",key1position);
        key1rotation = glm::radians(data.value("key1rotation",glm::degrees(key1rotation)));
        key2position = data.value("key2position",key2position);
        key2rotation = glm::radians(data.value("key2rotation",glm::degrees(key2rotation)));
        key3position = data.value("key3position",key3position);
        key3rotation = glm::radians(data.value("key3rotation",glm::degrees(key3rotation)));
        key4position = data.value("key4position",key4position);
        key4rotation = glm::radians(data.value("key4rotation",glm::degrees(key4rotation)));
        key5position = data.value("key5position",key5position);
        key5rotation = glm::radians(data.value("key5rotation",glm::degrees(key5rotation)));
        key6position = data.value("key6position",key6position);
        key6rotation = glm::radians(data.value("key6rotation",glm::degrees(key6rotation)));
        key7position = data.value("key7position",key7position);
        key7rotation = glm::radians(data.value("key7rotation",glm::degrees(key7rotation)));

    }

    // Creates and returns the camera view matrix
    glm::mat4 CameraComponent::getViewMatrix() const {
        auto owner = getOwner();
        auto M = owner->getLocalToWorldMatrix();
        //TODO: (Req 7) Complete this function
        //HINT:
        // In the camera space:
        // - eye is the origin (0,0,0)
        // - center is any point on the line of sight. So center can be any point (0,0,z) where z < 0. For simplicity, we let center be (0,0,-1)
        // - up is the direction (0,1,0)
        // but to use glm::lookAt, we need eye, center and up in the world state.
        // Since M (see above) transforms from the camera to thw world space, you can use M to compute:
        // - the eye position which is the point (0,0,0) but after being transformed by M
        // - the center position which is the point (0,0,-1) but after being transformed by M
        // - the up direction which is the vector (0,1,0) but after being transformed by M
        // then you can use glm::lookAt

        glm::vec4 eye = M * glm::vec4(0,0,0,1);
        glm::vec4 center = M * glm::vec4(0,0,-1,1);
        glm::vec4 up = M * glm::vec4(0,1,0,0);

        glm::mat4 view = glm::lookAt(
            glm::vec3(eye.x,eye.y,eye.z),
            glm::vec3(center.x,center.y,center.z),
            glm::vec3(up.x,up.y,up.z)
        );

        return view;
    }

    // Creates and returns the camera projection matrix
    // "viewportSize" is used to compute the aspect ratio
    glm::mat4 CameraComponent::getProjectionMatrix(glm::ivec2 viewportSize) const {
        //TODO: (Req 7) Wrtie this function
        // NOTE: The function glm::ortho can be used to create the orthographic projection matrix
        // It takes left, right, bottom, top. Bottom is -orthoHeight/2 and Top is orthoHeight/2.
        // Left and Right are the same but after being multiplied by the aspect ratio
        // For the perspective camera, you can use glm::perspective

        glm::mat4 ret;
        float aspect_ratio=(viewportSize.x*1.0f)/viewportSize.y;

        if(cameraType == CameraType::ORTHOGRAPHIC)
            ret = glm::ortho(aspect_ratio*-orthoHeight/2.0f, aspect_ratio*orthoHeight/2.0f, -orthoHeight/2.0f, orthoHeight/2.0f, near, far);
        else
            ret = glm::perspective(fovY, aspect_ratio, near, far);

        return ret;
    }
}