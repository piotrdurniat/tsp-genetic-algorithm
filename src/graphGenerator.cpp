#include "graphGenerator.hpp"

GraphMatrix *graphGenerator::getRandom(int verticesNum, int maxWeight)
{
    GraphMatrix *graph = new GraphMatrix(verticesNum);

    for (int i = 0; i < verticesNum; ++i)
    {
        for (int j = 0; j < verticesNum; ++j)
        {
            int weight = (j == i) ? -1 : getRandomInt(1, maxWeight);
            graph->addEdge(i, j, weight);
        }
    }
    return graph;
}

int graphGenerator::getRandomInt(int min, int max)
{
    return min + rand() % (max - min + 1);
}
