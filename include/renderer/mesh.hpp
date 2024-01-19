#pragma once

#include <vector>
#include <iostream>
#include <string>

#include "la_extended.h"
using namespace LA;
#include "gl_interface.hpp"
#include "ecs/resource.hpp"
#include "ecs/resource_manager.hpp"

// might need to add sub mesh support for future more complex meshes
// each sub-mesh can have its own material but ONLY 1 material
// multiple materials for a submesh need multiple sub-meshes

class Mesh : public Resource {
    private:
        bool _isGenerated = false;

    public:
        uint32_t vertexBO, normalBO, uvBO, colourBO, indicieBO = -1;
        uint32_t vao = 0;
        uint32_t material = 0;
        

    // todo add assimp loaded meshes
        Mesh(std::string name)
            : Resource(name) {
            std::cout << "Ok: Created empty mesh" << std::endl;
        }

        Mesh(std::string name, std::vector<vec3> vertices)
            : Resource(name) {
            _vertices = vertices;
            GenerateBuffers();
        }

        Mesh(std::string name, std::vector<vec3> vertices, std::vector<uint32_t> indicies)
            : Resource(name) {
            _vertices = vertices;
            _indicies = indicies;
            GenerateBuffers();
            std::cout << "Ok: Created array mesh" << std::endl;
        }

        Mesh(std::string name, std::vector<vec3> vertices, std::vector<vec3> normals, std::vector<uint32_t> indicies)
            : Resource(name) {
            _vertices = vertices;
            _normals = normals;
            _indicies = indicies;
            GenerateBuffers();
            std::cout << "Ok: Created array mesh" << std::endl;
        }

        ~Mesh() {
            // delete all opengl resources/buffers
        }

        std::vector<vec3> GetVertices() { return _vertices; }
        void SetVertices(std::vector<vec3> vertices) { 
            _vertices = vertices;
        }
        int GetVerticesSize() { return _vertices.size(); }

        std::vector<vec3> GetUVs() { return _uvs; }
        void SetUVs(std::vector<vec3> uvs) {
            if (uvs.size() != _vertices.size())
                std::cout << "Warning: Mesh::SetUV - UVs do not match count of vertices" << std::endl;
            else
                _uvs = uvs;
        }
        int GetUVsSize() { return _uvs.size(); }

        std::vector<vec3> GetNormals() { return _normals; }
        void SetNormals(std::vector<vec3> normals) { 
            if (normals.size() != _vertices.size())
                std::cout << "Warning: Mesh::SetNormals - Normals do not match count of vertices" << std::endl;
            else
                _normals = normals;
        }
        int GetNormalsSize() { return _normals.size(); } 

        std::vector<vec4> GetColours() { return _colours; }
        void SetColours(std::vector<vec4> colours) {
            if (colours.size() != _vertices.size())
                std::cout << "Warning: Mesh::SetColours - Colours do not match count of vertices" << std::endl;
            else
                _colours = colours;
        }
        int GetColoursSize() { return _colours.size(); }

        std::vector<uint32_t> GetIndicies() { return _indicies; }
        void SetIndicies(std::vector<uint32_t> indicies) {
            if (indicies.size() != _vertices.size())
                std::cout << "Warning: Mesh::SetIndicies - Indicies do not match count of vertices" << std::endl;
            else
                _indicies = indicies;
        }
        int GetIndiciesSize() { return _indicies.size(); }

        bool GetIsGenerated() {
            return _isGenerated;
        }
        
    private:

        std::vector<vec3> _vertices = std::vector<vec3>();
        std::vector<vec3> _uvs = std::vector<vec3>();
        std::vector<vec3> _normals = std::vector<vec3>();
        std::vector<vec4> _colours = std::vector<vec4>();
        std::vector<uint32_t> _indicies = std::vector<uint32_t>();

        /// NO VALIDATION OF CORRECT BUFFER CREATION/FILLING
        void GenerateBuffers() {
            this->vertexBO = GL_Interface::GenVertexBufferObj(&_vertices);
            this->indicieBO = GL_Interface::GenElementBufferObj(&_indicies);
            this->vao = GL_Interface::GenVertexArrayObject();
            GL_Interface::BindVertexBufferObj(this->vertexBO);
            GL_Interface::VertexAttribPtr(ATTRIB_LOC_POSITION, 3, TYPE_FLOAT);
            if (_uvs.size() > 0) {
                this->uvBO = GL_Interface::GenVertexBufferObj(&_uvs);
                GL_Interface::BindVertexBufferObj(this->uvBO);
                GL_Interface::VertexAttribPtr(ATTRIB_LOC_UV, 3, TYPE_FLOAT);
            }
            if (_normals.size() > 0) {
                this->normalBO = GL_Interface::GenVertexBufferObj(&_normals);
                GL_Interface::BindVertexBufferObj(this->normalBO);
                GL_Interface::VertexAttribPtr(ATTRIB_LOC_NORMAL, 3, TYPE_FLOAT);
            }
            if (_colours.size() > 0) {
                this->colourBO = GL_Interface::GenVertexBufferObj(&_colours);
                GL_Interface::BindVertexBufferObj(this->colourBO);
                GL_Interface::VertexAttribPtr(ATTRIB_LOC_COLOUR, 4, TYPE_FLOAT);
            }
            GL_Interface::BindElementBufferObj(this->indicieBO);
            _isGenerated = true;
        }
};

extern ResourceManager<Mesh> resourceMeshes;