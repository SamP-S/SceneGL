#include <vector>
#include <iostream>
#include <string>

#include "la.hpp"
#include "gl_interface.hpp"
#include "resource.hpp"

using namespace LA;

// might need to add sub mesh support for future more complex meshes
// each sub-mesh can have its own material but ONLY 1 material
// multiple materials for a submesh need multiple sub-meshes


const std::vector<vec3> quadVertices = {
            {-1.0f, -1.0f, 1.0f},
            {1.0f,   1.0f, 1.0f},
            {-1.0f,  1.0f, 1.0f},
            {-1.0f, -1.0f, 1.0f},
            {1.0f,  -1.0f, 1.0f},
            {1.0f,   1.0f, 1.0f}
        };

const std::vector<vec3> cubeVertices = {
            {-1.0f, -1.0f, -1.0f},  // A = 0
            {-1.0f, -1.0f,  1.0f},  // B = 1
            {-1.0f,  1.0f, -1.0f},  // C = 2
            {-1.0f,  1.0f,  1.0f},  // D = 3
            { 1.0f, -1.0f, -1.0f},  // E = 4
            { 1.0f, -1.0f,  1.0f},  // F = 5
            { 1.0f,  1.0f, -1.0f},  // G = 6
            { 1.0f,  1.0f,  1.0f}   // H = 7
        };

const std::vector<uint32_t> cubeIndicies = {
    0, 4, 6, 0, 6, 2,   // -z
    5, 1, 3, 5, 3, 7,   // +z
    0, 1, 5, 0, 5, 4,   // -y
    2, 6, 7, 2, 7, 3,   // +y
    0, 2, 3, 0, 3, 1,   // -x
    4, 5, 7, 4, 7, 6
};

const std::vector<vec3> cubeColours = {
    {0.0f, 0.0f, 0.0f},  // Black
    {0.0f, 0.0f, 1.0f},  // Blue
    {0.0f, 1.0f, 0.0f},  // Green
    {0.0f, 1.0f, 1.0f},  // Cyan
    {1.0f, 0.0f, 0.0f},  // Red
    {1.0f, 0.0f, 1.0f},  // Magenta
    {1.0f, 1.0f, 0.0f},  // Orange
    {1.0f, 1.0f, 1.0f}   // White
};

class Mesh : public Resource {


    public:
        uint32_t vertexBO = 0;
        uint32_t colourBO = 0;
        uint32_t indicieBO = 0;
        uint32_t vao = 0;

    // todo add assimp loaded meshes
        Mesh(std::string name)
            : Resource(name) {
            std::cout << "Ok: Created empty mesh" << std::endl;
        }

        Mesh(std::string name, std::vector<vec3> vertices)
            : Resource(name) {
            Clear();
            _vertices = vertices;
            GenerateBuffers();
        }

        Mesh(std::string name, std::vector<vec3> vertices, std::vector<vec3> colours, std::vector<uint32_t> indicies)
            : Resource(name) {
            Clear();
            _vertices = vertices;
            _colours = colours;
            _indicies = indicies;
            GenerateBuffers();
            std::cout << "Ok: Created array mesh" << std::endl;
        }

        ~Mesh() {
            // delete all opengl resources/buffers
        }

        std::vector<vec3> GetVertices() { return _vertices; }
        void SetVertices(std::vector<vec3> vertices) { 
            Clear();
            _vertices = vertices;
        }
        std::vector<vec2> GetUVs() { return _uv; }
        void SetUVs(std::vector<vec2> uv) {
            if (uv.size() != _vertices.size())
                std::cout << "Warning: Mesh::SetUV - UVs do not match count of vertices" << std::endl;
            else
                _uv = uv;
        }
        std::vector<vec3> GetNormals() { return _normals; }
        void SetNormals(std::vector<vec3> normals) { 
            if (normals.size() != _vertices.size())
                std::cout << "Warning: Mesh::SetNormals - Normals do not match count of vertices" << std::endl;
            else
                _normals = normals;
        }
        std::vector<vec3> GetColours() { return _colours; }
        void SetColours(std::vector<vec3> colours) {
            if (colours.size() != _vertices.size())
                std::cout << "Warning: Mesh::SetColours - Colours do not match count of vertices" << std::endl;
            else
                _colours = colours;
        }
        std::vector<uint32_t> GetIndicies() { return _indicies; }
        void SetIndicies(std::vector<uint32_t> indicies) {
            if (indicies.size() != _vertices.size())
                std::cout << "Warning: Mesh::SetIndicies - Indicies do not match count of vertices" << std::endl;
            else
                _indicies = indicies;
        }

    private:

        std::vector<vec3> _vertices = std::vector<vec3>();
        std::vector<vec2> _uv = std::vector<vec2>();
        std::vector<vec3> _normals = std::vector<vec3>();
        std::vector<vec3> _colours = std::vector<vec3>();
        std::vector<uint32_t> _indicies = std::vector<uint32_t>();

        void Clear() {
            _vertices.clear();
            _uv.clear();
            _normals.clear();
            _colours.clear();
            _indicies.clear();
        }

        /// NO VALIDATION OF CORRECT BUFFER CREATION/FILLING
        void GenerateBuffers() {
            this->vertexBO = GL_Interface::GenVertexBufferObj(&_vertices);
            this->colourBO = GL_Interface::GenVertexBufferObj(&_colours);
            this->indicieBO = GL_Interface::GenElementBufferObj(&_indicies);
            this->vao = GL_Interface::GenVertexArrayObject();
            GL_Interface::BindVertexBufferObj(this->vertexBO);
            GL_Interface::VertexAttribPtr(ATTRIB_LOC_POSITION, 3, TYPE_FLOAT);
            if (_colours.size() > 0) {
                GL_Interface::BindVertexBufferObj(this->colourBO);
                GL_Interface::VertexAttribPtr(ATTRIB_LOC_COLOUR, 3, TYPE_FLOAT);
            }
            GL_Interface::BindElementBufferObj(this->indicieBO);
        }


};