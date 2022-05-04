#pragma once

#include "component.hpp"
#include "transform.hpp"
#include <unordered_map>
#include <string>
#include <type_traits>
#include <glm/glm.hpp>

namespace our {

    class World; // A forward declaration of the World Class

    class Entity{
        World *world; // This defines what world own this entity
        std::unordered_map<std::string, Component*> components; // A map of components that are owned by this entity
                                                                // The key is the ID of the component so an entity can only have one component of each type

        friend World; // The world is a friend since it is the only class that is allowed to instantiate an entity
        Entity() = default; // The entity constructor is private since only the world is allowed to instantiate an entity
    public:
        std::string name; // The name of the entity. It could be useful to refer to an entity by its name
        Entity* parent;   // The parent of the entity. The transform of the entity is relative to its parent.
                          // If parent is null, the entity is a root entity (has no parent).
        Transform localTransform; // The transform of this entity relative to its parent.

        World* getWorld() const { return world; } // Returns the world to which this entity belongs

        glm::mat4 getLocalToWorldMatrix() const; // Computes and returns the transformation from the entities local space to the world space
        void deserialize(const nlohmann::json&); // Deserializes the entity data and components from a json object
        
        // This template method create a component of type T,
        // adds it to the components map and returns a pointer to it 
        template<typename T>
        T* addComponent(){
            static_assert(std::is_base_of<Component, T>::value, "T must inherit from Component");
            T* component = new T();
            component->owner = this;
            components[T::getID()] = component;
            return component;
        }

        // This template method searhes for a component of type T and returns a pointer to it
        // If no component of type T was found, it returns a nullptr 
        template<typename T>
        T* getComponent(){
            static_assert(std::is_base_of<Component, T>::value, "T must inherit from Component");
            if(auto it = components.find(static_cast<std::string>(T::getID())); it != components.end()){
                return dynamic_cast<T*>(it->second);
            }
            return nullptr;
        }

        // This template method searhes for a component of type T and deletes it
        template<typename T>
        void deleteComponent(){
            static_assert(std::is_base_of<Component, T>::value, "T must inherit from Component");
            if(auto it = components.find(static_cast<std::string>(T::getID())); it != components.end()){
                delete it->second;
                components.erase(it);
            }
        }

        // Since the entity owns its components, they should be deleted alongside the entity
        ~Entity(){
            for(auto& it : components){
                delete it.second;
            }
        }

        // Entities should not be copyable
        Entity(const Entity&) = delete;
        Entity &operator=(Entity const &) = delete;
    };

}