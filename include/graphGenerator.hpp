#ifndef GRAPH_GENERATOR_H
#define GRAPH_GENERATOR_H

#include <iostream>
#include "GraphMatrix.hpp"

namespace graphGenerator
{
    // Generates and returns a complete, directed, weighted graph
    GraphMatrix *getRandom(int verticesNum, int maxWeight);

    // Returns random int from min (inclusive) to max (inclusive)
    int getRandomInt(int min, int max);
}

#endif
