#pragma once

#include <map>
#include <string>
#include <stdexcept>

// Future add handles as additional layer of seperation
// Create asset manager that loads meta as XML/JSON for all resources
// Only currently used/cached assets should be in the resource manager at any given time

template <typename T>
class ResourceManager {
    
    private:
        std::map<int, T*> _resourceMap = {};

    public:
        static int idIterator;

        ResourceManager() {}

        ~ResourceManager() {
            for (int i = 0; i < _resourceMap.size(); i++) {
                delete _resourceMap.at(i);
            }
        }

        int Add(T* item) {
            int id = idIterator++;
            _resourceMap.insert({id, item});
            return id;
        }

        bool Remove(int id) {
            try {
                T* itemPtr = _resourceMap.at(id);
                delete itemPtr;
                return _resourceMap.erase(id); 
            } catch (std::out_of_range err) {
                std::cout << "Error: ResourceManager::Remove " << id << " out of range" << std::endl;
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

        auto Begin() {
            return _resourceMap.begin();
        }

        auto End() {
            return _resourceMap.end();
        }

        T* At(int idx) {
            if (idx < 0 || idx >= Size())
                return NULL;
            auto it = Begin();
            std::advance(it, idx);
            return it->second;
        }

        int Size() {
            return _resourceMap.size();
        }

        int GetId(std::string name) {
            for (auto const& [id, item] : _resourceMap) {
                // std::cout << "mesh " << id << ": " << item->GetName() << std::endl;
                if (((Resource*)item)->GetName() == name) {
                    return id;
                }
            }
            std::cout << "Error: ResourceManager::GetId " << name << " does not exist" << std::endl;
            return -1;
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
        
};

// Set iterator value
template <typename T>
int ResourceManager<T>::idIterator = 0;
