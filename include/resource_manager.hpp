#pragma once

#include <map>
#include <string>
#include <stdexcept>

// Future add handles as additional layer of separation
// Create asset manager that loads meta as XML/JSON for all resources
// Only currently used/cached assets should be in the resource manager at any given time

template <typename T, typename = std::enable_if_t<std::is_base_of_v<Resource, T>>>
class ResourceManager {
    
    private:
        std::map<int, T*> _resourceMap = {};

    public:

        ResourceManager() {}

        ~ResourceManager() {
            Clear();
        }

        int Add(T* item) {
            int id = ((Resource*)item)->GetId();
            _resourceMap.insert({id, item});
            return id;
        }

        bool Remove(ObjId id) {
            try {
                T* itemPtr = _resourceMap.at(id);
                delete itemPtr;
                return _resourceMap.erase(id); 
            } catch (std::out_of_range err) {
                std::cout << "Error: ResourceManager::Remove " << id << " not in map" << std::endl;
                return false;
            }
        }

        bool Remove(std::string name) {
            return Remove(GetId(name));
        }

        void Clear() {
            for (auto const& [id, item] : _resourceMap) {
                delete item;
            }
            _resourceMap.clear();
        }

        auto begin() {
            return _resourceMap.begin();
        }

        auto end() {
            return _resourceMap.end();
        }

        T* At(int idx) {
            if (idx < 0 || idx >= _resourceMap.size())
                return NULL;
            auto it = begin();
            std::advance(it, idx);
            return it->second;
        }

        int Size() {
            return _resourceMap.size();
        }

        int GetId(std::string name) {
            for (auto const& [id, item] : _resourceMap) {
                if (((Resource*)item)->GetName().compare(name) == 0) {
                    return id;
                }
            }
            std::cout << "Error: ResourceManager::GetId " << name << " does not exist" << std::endl;
            return 0;
        }

        // currently allows for a pointer to be given out and object deleted while pointer is still outside object
        // move to smart pointers?
        T* Get(int id) {
            try {
                return _resourceMap.at(id);
            } catch (std::out_of_range err) {
                std::cout << "Error: ResourceManager::Get " << id << " out of range" << std::endl;
                return NULL;
            }
        }

        T* Get(std::string name) {
            return Get(GetId(name));
        }
        
        template<typename... Args>
        int Create(Args&&... args) {
            T* item = new T(std::forward<Args>(args)...);
            int id = ((Resource*)item)->GetId();
            _resourceMap.insert({id, item});
            return id;
        }
};
