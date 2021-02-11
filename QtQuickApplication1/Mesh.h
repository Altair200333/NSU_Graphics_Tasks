#pragma once
#include <vector>

#include "Vertex.h"

class Mesh final
{
public:
	std::vector<Vertex> vertices;
	std::vector<unsigned int> indices;

    Mesh() = default;
    Mesh(const std::vector<Vertex>& vertices, const std::vector<unsigned int>& indices)
    {
        this->vertices = vertices;
        this->indices = indices;
    }
};
