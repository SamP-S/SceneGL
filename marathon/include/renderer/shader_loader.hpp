#pragma once

#include <filesystem>
#include <string>
#include <fstream>
#include <sstream>
#include <iostream>
#include <unordered_map>
#include <vector>

#include "ecs/asset.hpp"
#include "core/filepath.hpp"
#include "platform/opengl/opengl_shader.hpp"
#include "platform/opengl/opengl_shader_source.hpp"

//  GLSL standard shader file extensions
// .vert - a vertex shader
// .tesc - a tessellation control shader
// .tese - a tessellation evaluation shader
// .geom - a geometry shader
// .frag - a fragment shader
// .comp - a compute shader

class ShaderLoader : public IAssetLoader {
private:
    std::vector<std::string> _extensions = {".vert", ".tesc", ".tese", ".geom", ".frag", ".comp"};
    
    std::unordered_map<std::string, ShaderStage> _extToType = {
        {".vert", ShaderStage::VERTEX},
        {".tesc", ShaderStage::TESSELLATION_CONTROL},
        {".tese", ShaderStage::TESSELLATION_EVALUATION},
        {".geom", ShaderStage::GEOMETRY},
        {".frag", ShaderStage::FRAGMENT},
        {".comp", ShaderStage::COMPUTE}
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
                ShaderStage stage = it->second;
                std::shared_ptr<ShaderSource> shaderSource = assetManager.CreateAsset<OpenGLShaderSource>();
                shaderSource->name = name;
                shaderSource->path = filepath;
                shaderSource->source = source;
                shaderSource->stage = stage;
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