#pragma once

#include <unordered_set>
#include "entity.hpp"

namespace our {

    // This class holds a set of entities
    class World {
        std::unordered_set<Entity*> entities; // These are the entities held by this world
        std::unordered_set<Entity*> markedForRemoval; // These are the entities that are awaiting to be deleted
                                                      // when deleteMarkedEntities is called
    public:

        World() = default;

        // This will deserialize a json array of entities and add the new entities to the current world
        // If parent pointer is not null, the new entities will be have their parent set to that given pointer
        // If any of the entities has children, this function will be called recursively for these children
        void deserialize(const nlohmann::json& data, Entity* parent = nullptr);

        // This adds an entity to the entities set and returns a pointer to that entity
        // WARNING The entity is owned by this world so don't use "delete" to delete it, instead, call "markForRemoval"
        // to put it in the "markedForRemoval" set. The elements in the "markedForRemoval" set will be removed and
        // deleted when "deleteMarkedEntities" is called.
        Entity* add() {
            Entity* entity = new Entity();
            entity->world = this;
            entities.insert(entity);
            return entity;
        }

        // This returns and immutable reference to the set of all entites in the world.
        const std::unordered_set<Entity*>& getEntities() {
            return entities;
        }

        // This marks an entity for removal by adding it to the "markedForRemoval" set.
        // The elements in the "markedForRemoval" set will be removed and deleted when "deleteMarkedEntities" is called.
        void markForRemoval(Entity* entity){
            if(entities.find(entity) != entities.end())
                markedForRemoval.insert(entity);
        }

        // This removes the elements in "markedForRemoval" from the "entities" set.
        // Then each of these elements are deleted.
        void deleteMarkedEntities(){
            for(auto entity: markedForRemoval){
                entities.erase(entity);
                delete entity;
            }
            markedForRemoval.clear();
        }

        //This deletes all entities in the world
        void clear(){
            for(auto entity: entities){
                delete entity;
            }
            entities.clear();
            markedForRemoval.clear();
        }

        //Since the world owns all of its entities, they should be deleted alongside it.
        ~World(){
            clear();
        }

        // The world should not be copyable
        World(const World&) = delete;
        World &operator=(World const &) = delete;
    };

}