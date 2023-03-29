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

	int ProcessMesh(std::string name, aiMesh *mesh) {
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

		for (int i = 0; i < scene->mNumMaterials; i++) {
			aiMaterial* aiMaterial = scene->mMaterials[i];
			
			Material* material = new Material(aiMaterial->GetName().C_Str());
			std::cout << "Material: " << aiMaterial->GetName().C_Str() << std::endl;
			aiColor3D diffuse = aiColor3D();
			std::cout << "diffuse: " << diffuse.r << "," << diffuse.g << "," << diffuse.b;
			aiMaterial->Get(AI_MATKEY_COLOR_DIFFUSE, diffuse);
			std::cout << ";\t" << diffuse.r << "," << diffuse.g << "," << diffuse.b << std::endl;
			for (int j = 0; j < aiMaterial->mNumProperties; j++) {
				aiMaterialProperty* property = aiMaterial->mProperties[j];
				std::cout << "\tProperty: " << property->mKey.C_Str() << std::endl;
				switch (property->mType) {
					case aiPTI_Float:
						std::cout << "\t\tFloat = " << *(float*)property->mData << std::endl;
						break;
					case aiPTI_Double:
						std::cout << "\t\tDouble = " << *(double*)property->mData << std::endl;
						break;
					case aiPTI_String:
						std::cout << "\t\tString = " << ((aiString*)property->mData)->C_Str() << std::endl;
						break;
					case aiPTI_Integer:
						std::cout << "\t\tInteger = " << *(int*)property->mData << std::endl;
						break;
					default:
						std::cout << "Buffer Data" << std::endl;
						break;
				}
				std::cout << "\t\tType = " << property->mType << std::endl;
				std::cout << "\t\tIndex = " << property->mIndex << std::endl;
				std::cout << "\t\tSemantic = " << property->mSemantic << std::endl;
				std::cout << std::endl;
			}
			// for (int j = 0; j < material->GetTextureCount(); j++) {
			// 	// aiTexture* texture = scene->mTextures[i];
			// 	// aiTextureType;
			// 	// material->GetTexture(aiTextureType)
			// 	// load texture
			// }
			// Load texture
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