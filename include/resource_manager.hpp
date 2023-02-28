#include <map>
#include <string>
#include <stdexcept>

// Future add handles as additional layer of seperation
// Create asset manager that loads meta as XML/JSON for all resources
// Only currently used/cached assets should be in the resource manager at any given time

template <typename T>
class ResourceManager {
    
    private:
        std::map<std::string, T*> _resourceMap = {};

    public:
        ResourceManager() {

        }

        bool Add(std::string key, T* item) {
            _resourceMap.insert({key, item});
        }

        bool Remove(std::string key) {
            try {
                T* itemPtr = _resourceMap.at(key);
                delete itemPtr;
                return _resourceMap.erase(key); 
            } catch (std::out_of_range err) {
                std::cout << "Error: ResourceManager::Remove " << key << " out of range" << std::endl;
                return false;
            }
        }

        bool Clear() {
            for (auto const& [key, item] : _resourceMap) {
                delete item;
            }
            _resourceMap.clear();
        }

        // currently allows for a pointer to be given out and object deleted while pointer is still outside object
        // move to smart pointers?
        T* Get(std::string key) {
            try {
                return _resourceMap.at(key);
            } catch (std::out_of_range err) {
                std::cout << "Error: ResourceManager::Get " << key << " out of range" << std::endl;
            }
           
        }

};