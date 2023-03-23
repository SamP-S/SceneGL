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
#include "la.hpp"

#include "assimp/Importer.hpp"
#include "assimp/scene.h"
#include "assimp/postprocess.h"


class Model : public Resource {


	void ProcessNode(aiNode *node, const aiScene *scene) {
		if (!node->mNumMeshes && node->mNumChildren > 0) {
			for (uint i = 0; i < node->mNumChildren; i++) {
				ProcessNode(node->mChildren[i], scene);
			}
		}

		for (uint i = 0; i < node->mNumMeshes; i++) {
			aiMesh* mesh = scene->mMeshes[node->mMeshes[i]];
			meshes.push_back(ProcessMesh(mesh));
		}
	}

	Mesh ProcessMesh(aiMesh *mesh) {
		std::vector<vec3> vertices;
		std::vector<vec3> normals;
		std::vector<vec2> uv;
		std::vector<vec3> colours;
		std::vector<uint> indices;

		for (uint i = 0; i < mesh->mNumVertices; i++) {
			vertices.push_back(vec3(mesh->mVertices[i]));
		
			if (mesh->mTextureCoords[0]) {
				uvs.push_back(vec2(mesh->mTextureCoords[0][i].x, mesh->mTextureCoords[0][i].y));
			}
			if (!mesh->mNormals != NULL) {
				normals.push_back(vec3(mesh->mNormals[i]));
			}
			if (mesh->mNormals != NULL) {
				colours.push_back(vec3(mesh->mColors[0]));
			}
		}

		for (uint i = 0; i < mesh->mNumFaces; i++) {
			for (uint j = 0; j < mesh->mFaces[i].mNumIndices; j++) {
				indices.push_back(mesh->mFaces[i].mIndices[j]);
			}
		}

		return Mesh(vertices, normals, uv, colours, indices);
	}

	
public:
	bool isOk = false;
	std::string name;
	std::string filePath;
	std::vector<int> meshes;

	Model(std::string name, std::string path) 
	: Resource(name) {
		filePath = path;
		Assimp::Importer importer;
		const aiScene *scene = importer.ReadFile(filePath, aiProcess_Triangulate | aiProcess_FlipUVs);

		if (!scene || scene->mFlags == AI_SCENE_FLAGS_INCOMPLETE || !scene->mRootNode) {
			std::cout << "ERROR: ASSIMP cannot load file: " << importer.GetErrorString() << std::endl;
			return false;
		}

		ProcessNode(scene->mRootNode, scene);

		for (int i = 0; i < meshes.size(); i++) {
			if (meshes[i].vertices.size() == 0)
				return false;
		}
		return true;
	}


};

ResourceManager<Model> models = ResourceManager<Model>();