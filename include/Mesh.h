#pragma once
#include "Vertex.h"
#include "AllocatedBuffer.h"
#include <vector>
namespace tgl {
    struct MeshPushConstants {
        glm::vec4 data;
        glm::mat4 renderMatrix;
    };

    struct Mesh {
        std::vector<Vertex> vertices;
        AllocatedBuffer allocatedBuffer;
    };
}