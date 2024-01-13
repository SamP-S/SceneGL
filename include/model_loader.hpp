#pragma once

#include <filesystem>
#include <string>
#include <fstream>
#include <sstream>
#include <iostream>
#include <map>
#include <vector>

#include "resource.hpp"
#include "mesh.hpp"
#include "la_extended.h"
using namespace LA;
#include "transform.hpp"
#include "texture.hpp"
#include "material.hpp"

#define TINYGLTF_IMPLEMENTATION
#define STB_IMAGE_IMPLEMENTATION
#define STB_IMAGE_WRITE_IMPLEMENTATION
// #define TINYGLTF_NOEXCEPTION // optional. disable exception handling.
#include "tiny_gltf.h"


class ModelLoader : public AssetLoader {

private:
    std::vector<std::string> _extensions = {".gltf"};

	int ProcessMesh(const tinygltf::Mesh& mesh, const tinygltf::Model& model) {
		int primitive_type = mesh.primitives[0].mode;
		int uv_components = mesh.primitives[0].attributes.count("TEXCOORD_0");
		int material = mesh.primitives[0].material;
		std::vector<vec3> vertices;
		std::vector<vec3> normals;
		std::vector<vec3> uvs;
		std::vector<vec4> colours;
		std::vector<vec3> tangents;
		std::vector<vec3> bitangents;
		std::vector<uint32_t> indices;

		const tinygltf::Accessor& positionAccessor = model.accessors[mesh.primitives[0].attributes.at("POSITION")];
		const tinygltf::BufferView& positionBufferView = model.bufferViews[positionAccessor.bufferView];
		const tinygltf::Buffer& positionBuffer = model.buffers[positionBufferView.buffer];
		const float* positionData = reinterpret_cast<const float*>(&positionBuffer.data[positionBufferView.byteOffset + positionAccessor.byteOffset]);

		for (size_t i = 0; i < positionAccessor.count; i++) {
			vertices.push_back(vec3({positionData[i * 3], positionData[i * 3 + 1], positionData[i * 3 + 2]}));
		}

		if (uv_components > 0) {
			const tinygltf::Accessor& uvAccessor = model.accessors[mesh.primitives[0].attributes.at("TEXCOORD_0")];
			const tinygltf::BufferView& uvBufferView = model.bufferViews[uvAccessor.bufferView];
			const tinygltf::Buffer& uvBuffer = model.buffers[uvBufferView.buffer];
			const float* uvData = reinterpret_cast<const float*>(&uvBuffer.data[uvBufferView.byteOffset + uvAccessor.byteOffset]);

			for (size_t i = 0; i < uvAccessor.count; i++) {
				uvs.push_back(vec3({uvData[i * uv_components], uvData[i * uv_components + 1], 0.0f}));
			}
		}

		if (mesh.primitives[0].attributes.count("NORMAL") > 0) {
			const tinygltf::Accessor& normalAccessor = model.accessors[mesh.primitives[0].attributes.at("NORMAL")];
			const tinygltf::BufferView& normalBufferView = model.bufferViews[normalAccessor.bufferView];
			const tinygltf::Buffer& normalBuffer = model.buffers[normalBufferView.buffer];
			const float* normalData = reinterpret_cast<const float*>(&normalBuffer.data[normalBufferView.byteOffset + normalAccessor.byteOffset]);

			for (size_t i = 0; i < normalAccessor.count; i++) {
				normals.push_back(vec3({normalData[i * 3], normalData[i * 3 + 1], normalData[i * 3 + 2]}));
			}
		}

		// Send to resource manager
		resourceMeshes.Create(mesh.name, vertices, normals, uvs, colours, indices);
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

};