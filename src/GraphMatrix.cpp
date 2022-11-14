#include "GraphMatrix.hpp"
#include <iostream>
#include <vector>

GraphMatrix::GraphMatrix(int size)
{
    this->size = size;
    matrix = new int *[size];

    for (int i = 0; i < size; ++i)
    {
        matrix[i] = new int[size];
    }
}

void GraphMatrix::display()
{
    printf("   |");
    for (int i = 0; i < this->size; ++i)
    {
        printf(" %3i ", i);
    }
    printf("\n----");
    for (int i = 0; i < this->size; ++i)
    {
        printf("-----");
    }
    printf("\n");
    for (int i = 0; i < this->size; ++i)
    {
        printf("%2i |", i);
        for (int j = 0; j < size; ++j)
        {
            printf(" %3i ", matrix[i][j]);
        }
        printf("\n");
    }
    if (optimumIsKnown)
    {
        printf("Optimum: %i\n", optimum);
    }
    printf("Directed: %s", directed ? "true" : "false");
}

void GraphMatrix::addEdge(int u, int v, int weight)
{
    matrix[u][v] = weight;
}

int GraphMatrix::getVertexCount()
{
    return this->size;
}

std::vector<int> GraphMatrix::getAdj(int v)
{
    std::vector<int> adj;

    for (int i = 0; i < this->size; ++i)
    {
        if (matrix[v][i] != 0)
        {
            adj.push_back(i);
        }
    }
    return adj;
}

int GraphMatrix::getWeight(int u, int v)
{
    return matrix[u][v];
}

GraphMatrix::~GraphMatrix()
{
    for (int i = 0; i < size; ++i)
    {
        delete[] matrix[i];
    }
    delete[] matrix;
}

void GraphMatrix::setOptimum(int optimum)
{
    this->optimumIsKnown = true;
    this->optimum = optimum;
}

int GraphMatrix::getOptimum()
{
    return this->optimum;
}

bool GraphMatrix::isOptimumKnown()
{
    return this->optimumIsKnown;
}
