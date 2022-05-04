#pragma once

#include <json/json.hpp>
#include <string>

namespace our {

    class Entity; // A forward declaration of the Entity Class

    // A component is a data container that can be added to an entity.
    // The role of the entity in the world is defined by the components it holds.
    // For example, an entity with a camera component specifies that this entity should be used as a camera
    // Thus any renderer system should look for an entity holding a camera component in order to compute the camera related uniforms (e.g. VP matrix)
    class Component {
        Entity* owner; // A pointer to the entity that owns this component
        friend Entity; // The entity is a friend since it is the only one allowed to set itself as an owner of a certain component.
    public:
        // This static method returns a unique string that identifies each type of components
        // This ID will be used as the key to store a component into the entity's component map 
        // When you create a new type of components, override this function to return a new unique ID
        static std::string getID() { return "Component"; }
        // Reads the data of the component from a json object
        // It is abstract since it must be overriden by derived components
        virtual void deserialize(const nlohmann::json& data) = 0;
        // Returns the owner of this component
        Entity* getOwner() const { return owner; }
        // Define a virtual destructor
        virtual ~Component(){}
    };

}