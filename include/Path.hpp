#ifndef PATH_H
#define PATH_H

#include <vector>

struct Path
{
    std::vector<int> vertices;
    int weight;
    float prd;

    Path() {}

    // Create path from standard array
    Path(int *verticesArr, int arrSize, int weight, float prd)
    {
        this->vertices.resize(arrSize);
        std::copy(verticesArr, verticesArr + arrSize, this->vertices.begin());
        this->weight = weight;
        this->prd = prd;
    }

    // Create path from vector
    Path(std::vector<int> vertices, int weight, float prd)
    {
        this->vertices = vertices;
        this->weight = weight;
        this->prd = prd;
    }
};

#endif
