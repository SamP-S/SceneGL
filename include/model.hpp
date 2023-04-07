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
		Material* test = new Material("test material");
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
					test->Add(property->mKey.C_Str(), (float*)property->mData, size);
					break;
				case aiPTI_Double:
					size = property->mDataLength / sizeof(double);
					test->Add(property->mKey.C_Str(), (double*)property->mData, size);
					break;
				case aiPTI_String:
					size = property->mDataLength;
					test->Add(property->mKey.C_Str(), new std::string(property->mData, property->mDataLength));
					break;
				case aiPTI_Integer:
					size = property->mDataLength / sizeof(int);
					test->Add(property->mKey.C_Str(), (int*)property->mData, size);
					break;
				default:
					std::cout << "Buffer Data" << std::endl;
					break;
			}
		}
		std::cout << "end material" << std::endl;
	}

	int ProcessMesh(std::string name, aiMesh* mesh) {
		int primitive_type = mesh->mPrimitiveTypes;
		int uv_components = mesh->mNumUVComponents[0];
		int material = mesh->mMaterialIndex;
		std::vector<vec3> vertices;
		std::vector<vec3> normals;
		std::vector<vec3> uvs;
		std::vector<vec4> colours;
		std::vector<vec3> tangents;
		std::vector<vec3> bitangents;
		std::vector<uint32_t> indices;

		for (int i = 0; i < mesh->mNumVertices; i++) {
			vertices.push_back(vec3(((vec3*)mesh->mVertices)[i]));
		
			if (mesh->mTextureCoords[0] && mesh->HasTextureCoords(0)) {
				uvs.push_back(vec3(((vec3*)mesh->mTextureCoords[0])[i]));
			}
			if (!mesh->mNormals != NULL && mesh->HasNormals()) {
				normals.push_back(vec3(((vec3*)mesh->mNormals)[i]));
			}
			if (!mesh->mTangents != NULL && !mesh->mBitangents != NULL && mesh->HasTangentsAndBitangents()) {
				tangents.push_back(vec3(((vec3*)mesh->mTangents)[i]));
				bitangents.push_back(vec3(((vec3*)mesh->mBitangents)[i]));
			}
			if (mesh->mColors != NULL && mesh->HasVertexColors(0)) {
				colours.push_back(vec4(((vec4*)mesh->mColors[0])[i]));
			}
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

		std::cout << "Textures? = " << scene->HasTextures() << std::endl;
		for (int i = 0; i < scene->mNumTextures; i++) {
			textures.push_back(ProcessTexture(name, scene->mTextures[i]));
		}

		for (int i = 0; i < scene->mNumMaterials; i++) {
			materials.push_back(ProcessMaterial(name, scene->mMaterials[i]));
		}

		for (int i = 0; i < scene->mNumMeshes; i++) {
			meshes.push_back(ProcessMesh(name, scene->mMeshes[i]));
		}

		ProcessNode(scene->mRootNode, &rootNode, scene);
		isOk = true;
	}

};

ResourceManager<Model> resourceModels = ResourceManager<Model>();