#include "world.hpp"

namespace our {

    // This will deserialize a json array of entities and add the new entities to the current world
    // If parent pointer is not null, the new entities will be have their parent set to that given pointer
    // If any of the entities has children, this function will be called recursively for these children
    void World::deserialize(const nlohmann::json& data, Entity* parent){
        if(!data.is_array()) return;
        for(const auto& entityData : data){
            Entity* entity = add();
            entity->parent = parent;
            entity->deserialize(entityData);
            if(entityData.contains("children"))
                this->deserialize(entityData["children"], entity);
        }
    }

}