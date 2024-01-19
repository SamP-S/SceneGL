#pragma once

#include <string>
#include <fstream>
#include <iostream>
#include <vector>

#include "ngine/object.hpp"


class Asset : public Object {
    private:
        std::string _path;
        bool _isGood = false;
        
    public:
        Asset(std::string path) :
        Object(),
        _path(path) {
            std::cout << "Loading Asset: " << path << std::endl;
        }

        std::string GetPath() {
            return _path;
        }

        bool IsGood() {
            return _isGood;
        }


};
