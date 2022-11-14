#include <iostream>
#include <algorithm>

#include "Individual.hpp"

Individual::Individual()
{
}

Individual::Individual(int vertexCount, GraphMatrix *graph)
{
    this->graph = graph;
    this->vertexCount = vertexCount;
    this->path = new int[vertexCount];
}

Individual::~Individual()
{
    delete[] this->path;
}

void Individual::setRandomPath()
{
    for (int i = 0; i < vertexCount; ++i)
    {
        path[i] = i;
    }

    std::random_shuffle(path + 1, path + vertexCount);
}

void Individual::printPath()
{
    for (int i = 0; i < vertexCount; ++i)
    {
        printf("%i->", path[i]);
    }
    printf("%i", path[0]);
}

int Individual::getPathWeight()
{
    int sum = 0;
    for (int i = 0; i < vertexCount - 1; ++i)
    {
        // printf("path[i]    = %i\n", path[i]);
        // printf("path[i + 1] = %i\n", path[i + 1]);
        sum += graph->getWeight(path[i], path[i + 1]);
    }
    sum += graph->getWeight(path[vertexCount - 1], path[0]);
    return sum;
}
