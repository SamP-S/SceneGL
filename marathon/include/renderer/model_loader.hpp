#pragma once

#include <filesystem>
#include <string>
#include <fstream>
#include <sstream>
#include <iostream>
#include <map>
#include <vector>

#include "core/filepath.hpp"
#include "ecs/asset.hpp"
#include "renderer/mesh.hpp"
#include "renderer/material.hpp"
#include "la_extended.h"
using namespace LA;

#include "platform/opengl/opengl_mesh.hpp"

#define TINYGLTF_IMPLEMENTATION
#define STB_IMAGE_IMPLEMENTATION
#define STB_IMAGE_WRITE_IMPLEMENTATION
// #define TINYGLTF_NOEXCEPTION // optional. disable exception handling.
#include "tiny_gltf.h"

//// TODO:
// remove opengl dependancy

class ModelLoader : public IAssetLoader {

private:
    std::vector<std::string> _extensions = {".gltf"};
	std::string _currentlyLoading = "";

	void ProcessMesh(const tinygltf::Mesh& mesh, const tinygltf::Model& model) {
		std::vector<vec3> vertices;
		std::vector<vec3> normals;
		std::vector<uint32_t> indices;

		for (const tinygltf::Primitive& primitive : mesh.primitives) {
			int posAttr = 0;
			int norAttr = 0;
			int indAttr = 0;
			try { 
				posAttr = primitive.attributes.at("POSITION");
				norAttr = primitive.attributes.at("NORMAL");
				indAttr = primitive.indices;
			} catch (const std::exception& e) {
				std::cout << "ERROR (ModelLoader): Exception caught " << e.what() << std::endl;
			}

			const tinygltf::Accessor& posAccessor = model.accessors[posAttr];
			const tinygltf::Accessor& norAccessor = model.accessors[norAttr];
			const tinygltf::Accessor& indAccessor = model.accessors[indAttr];

			const tinygltf::BufferView& posBufferView = model.bufferViews[posAccessor.bufferView];
			const tinygltf::BufferView& norBufferView = model.bufferViews[norAccessor.bufferView];
			const tinygltf::BufferView& indBufferView = model.bufferViews[indAccessor.bufferView];

			const tinygltf::Buffer& posBuffer = model.buffers[posBufferView.buffer];
			const tinygltf::Buffer& norBuffer = model.buffers[norBufferView.buffer];
			const tinygltf::Buffer& indBuffer = model.buffers[indBufferView.buffer];
			
			const float* posData = reinterpret_cast<const float*>(&posBuffer.data[posBufferView.byteOffset]);
			const float* norData = reinterpret_cast<const float*>(&norBuffer.data[norBufferView.byteOffset]);
			const uint16_t* indData = reinterpret_cast<const uint16_t*>(&indBuffer.data[indBufferView.byteOffset]);
			
			// std::cout << "IND TYPE: " << indAccessor.type << "; COMP: " << indAccessor.componentType << std::endl;

			for (size_t i = 0; i < posAccessor.count; i++) {
				float x = posData[i * 3];
				float y = posData[i * 3 + 1];
				float z = posData[i * 3 + 2];
				vertices.push_back(vec3({x, y, z}));
				// std::cout << i << "|\t" << x << ": " << y << ":" << z << std::endl;
			}

			for (size_t i = 0; i < norAccessor.count; i++) {
				float x = norData[i * 3];
				float y = norData[i * 3 + 1];
				float z = norData[i * 3 + 2];
				normals.push_back(vec3({x, y, z}));
				// std::cout<< i << "|\t" << x << ": " << y << ":" << z << std::endl;
			}

			for (size_t i = 0; i < indAccessor.count; i++) {
				// std::cout << i << "|\t" << indData[i] << std::endl;
				uint32_t index = static_cast<uint32_t>(indData[i]);
				indices.push_back(index);
			}
			// std::cout << vertices.size() << "|" << normals.size() << "|" << indices.size() << std::endl;
			std::shared_ptr<OpenGLMesh> meshGL = assetManager.CreateAsset<OpenGLMesh>();
			meshGL->name = mesh.name;
			meshGL->path = _currentlyLoading;
			meshGL->vertices = vertices;
			meshGL->normals = normals;
			meshGL->indices = indices;
		}
	}

	void ProcessMaterial(const tinygltf::Material& material, const tinygltf::Model& model) {
		// const std::vector<double>& baseD = material.pbrMetallicRoughness.baseColorFactor;
		// vec4 colour = vec4(std::vector<float>(baseD.begin(), baseD.end()));
		// float metallic = material.pbrMetallicRoughness.metallicFactor;
		// float roughness = material.pbrMetallicRoughness.roughnessFactor;
		// assetManager.CreateAsset<Material>(material.name, colour, metallic, roughness);
	}

public:

	ModelLoader()
		: IAssetLoader() {}

	bool Load(const std::string& filepath) {
		tinygltf::Model model;
		tinygltf::TinyGLTF loader;
		std::string err;
		std::string warn;
		
		bool result = loader.LoadASCIIFromFile(&model, &err, &warn, filepath);
		if (!result) {
			std::cout << "WARNING: Failed to load glTF file: " << err << std::endl;
			return false;
		}
		_currentlyLoading = filepath;
		for (const tinygltf::Material& material : model.materials) {
			ProcessMaterial(material, model);
		}

		for (const tinygltf::Mesh& mesh : model.meshes) {
			ProcessMesh(mesh, model);
		}
		_currentlyLoading = "";
		return true;
	}

	std::vector<std::string> GetSupportedExt() {
		return _extensions;
	}
    bool CanLoad(const std::string& filepath) {
		std::string test_ext = GetFileExtension(filepath);
		return (test_ext.compare(".gltf") == 0);
	};

};