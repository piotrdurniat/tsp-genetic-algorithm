#ifndef INDIVIDUAL_HPP
#define INDIVIDUAL_HPP

#include "GraphMatrix.hpp"

class Individual
{
public:
    int vertexCount;
    int pathWeight;

    // Array holding vertices of the path
    int *path;

    GraphMatrix *graph;

    Individual();
    Individual(int vertexCount, GraphMatrix *graph);
    void setRandomPath();
    void printPath();
    int getPathWeight();
};

#endif
