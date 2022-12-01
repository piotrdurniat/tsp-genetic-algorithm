#include <iostream>
#include <algorithm>
#include <numeric>

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
    std::iota(path, path + vertexCount, 0);
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

void Individual::updatePathWeight()
{
    int sum = 0;
    for (int i = 0; i < vertexCount - 1; ++i)
    {
        sum += graph->getWeight(path[i], path[i + 1]);
    }
    sum += graph->getWeight(path[vertexCount - 1], path[0]);
    this->pathWeight = sum;
}

int Individual::getPathWeight()
{
    return pathWeight;
}
