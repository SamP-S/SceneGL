#pragma once

#include <filesystem>
#include <string>
#include <fstream>
#include <sstream>
#include <iostream>
#include <unordered_map>
#include <vector>

#include "tai/tai.hpp"
#include "core/filepath.hpp"
#include "renderer/shader.hpp"

//  GLSL standard shader file extensions
// .vert - a vertex shader
// .tesc - a tessellation control shader
// .tese - a tessellation evaluation shader
// .geom - a geometry shader
// .frag - a fragment shader
// .comp - a compute shader

class ShaderLoader : public Tai::IAssetLoader {
private:
    std::vector<std::string> _extensions = {".vert", ".tesc", ".tese", ".geom", ".frag", ".comp"};
    
    std::unordered_map<std::string, int> _extToType = {
        {".vert", SHADER_VERTEX},
        {".tesc", SHADER_TESSELLATION_CONTROL},
        {".tese", SHADER_TESSELLATION_EVALUATION},
        {".geom", SHADER_GEOMETRY},
        {".frag", SHADER_FRAGMENT},
        {".comp", SHADER_COMPUTE}
    };

public:

	ShaderLoader()
		: IAssetLoader() {}

	bool Load(const std::string& filepath) {
		// inspect file extension
        std::string ext = GetFileExtension(filepath);
        if (!CanLoad(ext)) {
            std::cout << "WARNING (ShaderLoader): Trying to open file of unsupported ext @ " << filepath << std::endl;
            return false;
        }

        // open file
        std::ifstream f(filepath);
        if (!f.good()) {
            std::cout << "WARNING (ShaderLoader): Failed to open file successfully @ " << filepath << std::endl;
            return false;
        }
        // load file contents as string
        try {
            std::stringstream strBuffer;
            strBuffer << f.rdbuf();
            // close file handlers
            f.close();
            // dump file as string
            std::string source = strBuffer.str();
            // get name from filepath
            std::string name = GetFileName(filepath) + GetFileExtension(filepath);
            // replace ".ext" to "_ext"
            std::replace(name.begin(), name.end(), '.', '_');
            // look up stage type
            auto it = _extToType.find(ext);
            if (it != _extToType.end()) {
                int stage = it->second;
                assetManager.CreateAsset<ShaderStage>(name, source, stage);
            } else {
                std::cout << "WARNING (ShaderLoader): Unsupported shader extension @ " << filepath << std::endl;
                return false;
            }
        }
        catch (std::ifstream::failure& e) {
            std::cout << "ERROR (ShaderLoader): " << e.what() << std::endl;
            return false;
        }
		return true;
	}

	std::vector<std::string> GetSupportedExt() {
		return _extensions;
	}
    bool CanLoad(const std::string& filepath) {
        std::string test_ext = GetFileExtension(filepath);
        for (const auto& ext : _extensions) {
            if (test_ext.compare(ext) == 0)
                return true;
        }
		return false;
	};

};