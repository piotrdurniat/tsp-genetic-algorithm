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
    ~Individual();
    Individual(int vertexCount, GraphMatrix *graph);
    void setRandomPath();
    void printPath();

    // Returns the path weigth
    int getPathWeight();

    // Calculates and updatest the path weight
    void updatePathWeight();
};

#endif
