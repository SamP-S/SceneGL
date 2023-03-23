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


// Specular Weight (a.k.a. shininess  is specular.alpha)
struct material_t
{
	std::string name;
	vec3 ambient;
	vec3 diffuse;
	vec3 specular;
	float specularWeight;
	float dissolve;
	int illum;
};


// class Mesh
// {
// private:	
// 	uint VAO, VBO, EBO;

// public:

// 	std::vector<vertex_t> vertices;
// 	std::vector<uint> indices;
// 	material_t material;



// 	void Render(Shader shader)
// 	{
// 		glUniform3f(glGetUniformLocation(shader.ID, "material.ambient"), material.ambient.r, material.ambient.g, material.ambient.b);
// 		glUniform3f(glGetUniformLocation(shader.ID, "material.diffuse"), material.diffuse.r, material.diffuse.g, material.diffuse.b);
// 		glUniform3f(glGetUniformLocation(shader.ID, "material.specular"), material.specular.r, material.specular.g, material.specular.b);
// 		glUniform1f(glGetUniformLocation(shader.ID, "material.specularWeight"), material.specularWeight);
// 		glUniform1f(glGetUniformLocation(shader.ID, "material.dissolve"), material.dissolve);
// 		glUniform1i(glGetUniformLocation(shader.ID, "material.illum"), material.illum);
// 		glUniform1i(glGetUniformLocation(shader.ID, "material.isTex_ambient"), false);
// 		glUniform1i(glGetUniformLocation(shader.ID, "material.isTex_diffuse"), false);
// 		glUniform1i(glGetUniformLocation(shader.ID, "material.isTex_specular"), false);

// 		uint ambientIndex = 1, diffuseIndex = 1, specularIndex = 1;

//         for (uint i = 0; i < textures.size(); i++)
//         {
//             glActiveTexture(GL_TEXTURE0 + i); 
//             std::stringstream ss;
//             std::string number;
//             std::string name = textures[i].type;
            
// 			if (name == "tex_ambient") {
// 				ss << ambientIndex;
// 				ambientIndex++;
// 				glUniform1i(glGetUniformLocation(shader.ID, "material.isTex_ambient"), true);
// 			} else if (name == "tex_diffuse") {
// 				ss << diffuseIndex;
// 				diffuseIndex++;
// 				glUniform1i(glGetUniformLocation(shader.ID, "material.isTex_diffuse"), true);
// 			} else if (name == "tex_specular") {
// 				ss << specularIndex;
// 				specularIndex++;
// 				glUniform1i(glGetUniformLocation(shader.ID, "material.isTex_specular"), true);
// 			} else {
// 				std::cout << "WARNING: Texture Type Unknown: " << name << std::endl;
// 			}
            
//             number = ss.str();
//             glUniform1i(glGetUniformLocation(shader.ID, (name).c_str()), i);
// 			glBindTexture(GL_TEXTURE_2D, textures[i].ID);
//         }
        
//         // Draw mesh
//         glBindVertexArray(VAO);
//         glDrawElements(GL_TRIANGLES, indices.size(), GL_UNSIGNED_INT, 0);
// 		glBindVertexArray(0);
        
//         for (uint i = 0; i < textures.size(); i++)
//         {
//             glActiveTexture(GL_TEXTURE0 + i);
//             glBindTexture(GL_TEXTURE_2D, 0);
//         }
// 	}
// };



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
		std::vector<texture_t> textures;

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
	std::vector<Mesh> meshes;
	std::vector<texture_t> textures_loaded;

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