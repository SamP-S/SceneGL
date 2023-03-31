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
#include "transform.hpp"

#include "material.hpp"

#include "assimp/Importer.hpp"
#include "assimp/scene.h"
#include "assimp/postprocess.h"

class ModelNode {
	public:
		Transform trans = Transform();
		std::vector<ModelNode> children = std::vector<ModelNode>();
		std::vector<int> meshes = std::vector<int>();
};

class Model : public Resource {

	void ProcessNode(aiNode *node, ModelNode* modelNode, const aiScene *scene) {
		aiVector3D pos, rot, scl;
		node->mTransformation.Decompose(scl, rot, pos);
		modelNode->trans = Transform(	vec3({pos.x, pos.y, pos.z}),
										vec3({rot.x, rot.y, rot.z}),
										vec3({scl.x, scl.y, scl.z})
		);

		for (int i = 0; i < node->mNumMeshes; i++) {
			modelNode->meshes.push_back(meshes[node->mMeshes[i]]);
		}

		for (int i = 0; i < node->mNumChildren; i++) {
			modelNode->children.push_back(ModelNode());
			ProcessNode(node->mChildren[i], &modelNode->children.back(), scene);
		}
	}

	int ProcessTexture(std::string name, aiTexture* texture) {
		std::cout << "Texture: " << texture->mWidth << "x" << texture->mHeight;
		std::cout << " @ " << texture->mFilename.C_Str() << std::endl;
		return 0;
	}

	int ProcessMaterial(std::string name, aiMaterial* material) {
		std::string material_name = material->GetName().C_Str();
		std::cout << "Material: " << material_name << std::endl;
		for (int j = 0; j < material->mNumProperties; j++) {
			aiMaterialProperty* property = material->mProperties[j];
			std::cout << "\t" << property->mKey.C_Str() << "\t(" << property->mType;
			std::cout << ", " << property->mIndex << ", " << property->mSemantic << ") = \t";
			int size = 1;
			int k = 0;
			switch (property->mType) {
				case aiPTI_Float:
					size = property->mDataLength / sizeof(float);
					std::cout << *((float*)property->mData + k);
					for (k = 1; k < size; k++) {
						std::cout << "," << *((float*)property->mData + k);
					}
					std::cout << std::endl;
					break;
				case aiPTI_Double:
					size = property->mDataLength / sizeof(double);
					std::cout << *((double*)property->mData + k);
					for (k = 1; k < size; k++) {
						std::cout << "," << *((double*)property->mData + k);
					}
					std::cout << std::endl;
					break;
				case aiPTI_String:
					size = property->mDataLength;
					std::cout << size << " - " << ((aiString*)property->mData)->C_Str() << std::endl;
					break;
				case aiPTI_Integer:
					std::cout << *((int*)property->mData + k);
					for (k = 1; k < size; k++) {
						std::cout << "," << *((int*)property->mData + k);
					}
					std::cout << std::endl;
					break;
				default:
					std::cout << "Buffer Data" << std::endl;
					break;
			}
		}
	}

	int ProcessMesh(std::string name, aiMesh* mesh) {
		std::vector<vec3> vertices;
		std::vector<vec3> normals;
		std::vector<vec2> uvs;
		std::vector<vec3> colours;
		std::vector<uint32_t> indices;

		for (int i = 0; i < mesh->mNumVertices; i++) {
			vertices.push_back(vec3({mesh->mVertices[i].x, mesh->mVertices[i].y, mesh->mVertices[i].z}));
		
			if (mesh->mTextureCoords[0]) {
				uvs.push_back(vec2({mesh->mTextureCoords[0][i].x, mesh->mTextureCoords[0][i].y}));
			}
			if (!mesh->mNormals != NULL) {
				normals.push_back(vec3({mesh->mNormals[i].x, mesh->mNormals[i].y, mesh->mNormals[i].z}));
			}
			// CAUSES SEG FAULT
			// if (mesh->mColors != NULL) {
			// 	colours.push_back(vec3({mesh->mColors[0]->r, mesh->mColors[0]->g, mesh->mColors[0]->b}));
			// }
		}

		for (int i = 0; i < mesh->mNumFaces; i++) {
			for (int j = 0; j < mesh->mFaces[i].mNumIndices; j++) {
				indices.push_back(mesh->mFaces[i].mIndices[j]);
			}
		}
		
		return resourceMeshes.Add(new Mesh(name + "::" + mesh->mName.C_Str(), vertices, normals, uvs, colours, indices));
	}

	
public:
	bool isOk = false;
	std::string filePath;
	std::vector<int> meshes;
	std::vector<int> materials;
	std::vector<int> textures;
	ModelNode rootNode;
	
	Model(std::string name, std::string path) 
	: Resource(name) {
		filePath = path;
		Assimp::Importer importer;
		const aiScene *scene = importer.ReadFile(filePath, aiProcess_Triangulate | aiProcess_FlipUVs);
		if (!scene || scene->mFlags == AI_SCENE_FLAGS_INCOMPLETE || !scene->mRootNode) {
			std::cout << "ERROR: ASSIMP cannot load file: " << importer.GetErrorString() << std::endl;
			return;
		}

		for (int i = 0; i < scene->mNumTextures; i++) {
			aiTexture* texture = scene->mTextures[i];
			textures.push_back(ProcessTexture(name, texture));
		}

		for (int i = 0; i < scene->mNumMaterials; i++) {
			aiMaterial* material = scene->mMaterials[i];
			materials.push_back(ProcessMaterial(name, material));
		}

		for (int i = 0; i < scene->mNumMeshes; i++) {
			aiMesh* mesh = scene->mMeshes[i];
			meshes.push_back(ProcessMesh(name, mesh));
		}

		ProcessNode(scene->mRootNode, &rootNode, scene);
		isOk = true;
	}

};

ResourceManager<Model> resourceModels = ResourceManager<Model>();