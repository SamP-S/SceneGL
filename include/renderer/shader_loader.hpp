#pragma once

#include <filesystem>
#include <string>
#include <fstream>
#include <sstream>
#include <iostream>
#include <unordered_map>
#include <vector>

#include "ecs/filepath.hpp"
#include "renderer/shader.hpp"

//  GLSL standard shader file extensions
// .vert - a vertex shader
// .tesc - a tessellation control shader
// .tese - a tessellation evaluation shader
// .geom - a geometry shader
// .frag - a fragment shader
// .comp - a compute shader

class ShaderLoader : public AssetLoader {
private:
    std::vector<std::string> _extensions = {".vert", ".tesc", ".tese", ".geom", ".frag", ".comp"};
    std::unordered_map<std::string, map> _extToType = {
        {".vert", SHADER_VERTEX},
        {".tesc", SHADER_TESSELLATION_CONTROL},
        {".tese", SHADER_TESSELLATION_EVALUATION},
        {".geom", SHADER_GEOMETRY},
        {".frag", SHADER_FRAGMENT},
        {".comp", SHADER_COMPUTE}
    };

public:

	ShaderLoader()
		: AssetLoader() {}

	bool Load(const std::string& path) {
		// inspect file extension
        std::string ext = GetFileExtension(path);
        if (!CanLoad(ext)) {
            std::cout << "WARNING (ShaderLoader): Trying to open file of unsupported ext @ " << path << std::endl;
            return false;
        }

        // open file
        std::ifstream f(path);
        if (!f.good()) {
            std::cout << "WARNING (ShaderLoader): Failed to open file successfully @ " << path << std::endl;
            return false;
        }
        // load file contents as string
        try {
            std::stringstream strBuffer;
            strBuffer << f.rdbuf();
            // close file handlers
            f.close();
            std::string source = strBuffer.str();
            std::string name = GetFileName(path);
            int shaderType = _extToType.find(ext);
            resourceShaders.Create(name, source, shaderType);
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
    bool CanLoad(const std::string& ext) {
        for (const auto& ext : _extensions) {
            if (ext.compare(ext) == 0)
                return true;
        }
		return false;
	};

};