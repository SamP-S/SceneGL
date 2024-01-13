#pragma once

#include <filesystem>
#include <string>
#include <fstream>
#include <sstream>
#include <iostream>
#include <map>
#include <vector>

#include "mesh.hpp"
#include "la_extended.h"
using namespace LA;

#define TINYGLTF_IMPLEMENTATION
#define STB_IMAGE_IMPLEMENTATION
#define STB_IMAGE_WRITE_IMPLEMENTATION
// #define TINYGLTF_NOEXCEPTION // optional. disable exception handling.
#include "tiny_gltf.h"


class ModelLoader : public AssetLoader {

private:
    std::vector<std::string> _extensions = {".gltf"};

	void ProcessMesh(const tinygltf::Mesh& mesh, const tinygltf::Model& model) {
	std::vector<vec3> vertices;
	std::vector<vec3> normals;
	std::vector<uint32_t> indices;

	for (const tinygltf::Primitive& primitive : mesh.primitives) {
		int posAttr = 0;
		try { 
			posAttr = primitive.attributes.at("POSITION");
		} catch (const std::exception& e) {
			std::cout << "ERROR (ModelLoader): Exception caught " << e.what() << std::endl;
		}

		const tinygltf::Accessor& posAccessor = model.accessors[posAttr];
		const tinygltf::BufferView& posBufferView = model.bufferViews[posAccessor.bufferView];
		const tinygltf::Buffer& posBuffer = model.buffers[posBufferView.buffer];
		
		const float* posData = reinterpret_cast<const float*>(&posBuffer.data[posAccessor.byteOffset]);
		for (size_t i = 0; i < posAccessor.count; i++) {
			vertices.push_back(vec3({posData[i * 3], posData[i * 3 + 1], posData[i * 3 + 2]}));
		}

		resourceMeshes.Create(mesh.name, vertices);
	}
}


public:

	ModelLoader()
		: AssetLoader() {}

	bool Load(const std::string& path) {
		tinygltf::Model model;
		tinygltf::TinyGLTF loader;
		std::string err;
		std::string warn;
		
		bool result = loader.LoadASCIIFromFile(&model, &err, &warn, path);
		if (!result) {
			std::cout << "WARNING: Failed to load glTF file: " << err << std::endl;
			return false;
		}

		for (const tinygltf::Mesh& mesh : model.meshes) {
			ProcessMesh(mesh, model);
		}
		return true;
	}

	std::vector<std::string> GetExtensions() {
		return _extensions;
	}
    bool CanLoad(const std::string& ext) {
		return (ext.compare(".gltf") == 0);
	};

};