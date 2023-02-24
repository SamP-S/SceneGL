#include <vector>

#include "la.hpp"
#include "mesh.hpp"

int main() {
    std::vector<vec3> vertices = {
        -1.0f, -1.0f, 1.0f,
        1.0f, 1.0f, 1.0f,
        -1.0f, 1.0f, 1.0f,
        -1.0f, -1.0f, 1.0f,
        1.0f, -1.0f, 1.0f,
        1.0f, 1.0f, 1.0f
    };

    mesh m = mesh();
    m.SetVertices(vertices);
    std::cout << m.GetVertices().size() << std::endl;
}