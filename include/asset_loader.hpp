#include <iostream>
#include <vector>
#include <string>

#include "filepath.hpp"
#include "asset.hpp"

class AssetLoader {
private:
    std::vector<std::string> _extensions;
    
public:
    virtual bool Load(const std::string& path) = 0;
    
    std::vector<std::string> GetExtensions() {
        return _extensions;
    }

    bool CanLoad(const std::string& path) {
        std::string toCheck = GetFileExtension(path);
        for (auto const& ext : _extensions) {
            if (toCheck.compare(ext) == 0)
                return true;
        }
        return false;
    }
};