#pragma once

#include <vector>
#include <iostream>
#include <string>

#include "platform/opengl/opengl.hpp"
#include "renderer/mesh.hpp"

//// TODO:
// think about renaming position to "vertex" or "fragment" for coordinate space to be inherant in the name
// modify to use an array of buffers instead of having 8 fixed buffers, glGenBuffers(n, &buffers)
// support uv4-uv7, bitangents
// implement using VertexAttribute configuration structs to make it flexible

enum OpenGLAttribLocs : uint32_t {
    POSITION = 0,
    NORMAL = 1,
    TANGENT = 2,
    COLOUR = 3,
    UV0 = 4,
    UV1 = 5,
    UV2 = 6,
    UV3 = 7
};

class OpenGLMesh : public Mesh {
public:
    // Set if no vertex attribute data, otherwise leave as 0
    uint32_t vertexCount = 0;

    OpenGLMesh(std::string name="Mesh");
    ~OpenGLMesh();

    bool IsUsable() override;
    
    void Draw() override;

private:
    bool _isGenerated = false;
    bool _isIndexed = false;

    uint32_t _vertexBuffer, _normalBuffer, _tangentBuffer, _colourBuffer, _indexBuffer = 0;
    uint32_t _uv0Buffer, _uv1Buffer, _uv2Buffer, _uv3Buffer = 0;
    uint32_t _vaBuffer = 0;

    template <typename T>
    uint32_t GenerateBuffer(GLenum target, const std::vector<T>& data) ;

    void Generate();
};
