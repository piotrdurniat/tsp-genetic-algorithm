#ifndef GRAPH_MATRIX2_H
#define GRAPH_MATRIX2_H

#include <vector>

// Graph representation using Adjacency Matrix
class GraphMatrix
{
private:
    int **matrix;
    int size = 0;

public:
    bool optimumIsKnown = false;

    // By default graph is directed
    bool directed = true;

    // Weight of the optimal hamiltionian cycle
    int optimum = 0;
    /**
     * @brief Construct a new Graph Matrix object
     *
     * @param size size of the graph
     */
    GraphMatrix(int size);

    ~GraphMatrix();

    void setOptimum(int optimum);

    // Returns the weight of the optimal hamiltionian cycle
    int getOptimum();

    bool isOptimumKnown();

    // Adds a new vertex to the graph
    void addVertex();

    /**
     * @brief Adds a new edge to the graph from u to v, with weight 'weight'
     */
    void addEdge(int u, int v, int weight);

    // Prints the Ajacency Matrix of the graph to stdout
    void display();

    // Returns the number of vertices in the graph
    int getVertexCount();

    // Returns all adjacent vertices to the privided vertex
    std::vector<int> getAdj(int v);

    // Returns weight of an egde from u to v
    int getWeight(int u, int v);
};

#endif
