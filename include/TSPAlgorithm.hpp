#ifndef TSP_ALGORITHM_HPP
#define TSP_ALGORITHM_HPP

#include "GraphMatrix.hpp"
#include <iostream>
#include <climits>
#include <cmath>
#include <chrono>
#include "Timer.hpp"

enum InitialPathMode
{
    Greedy,
    InOrder,
};

enum NeighborMode
{
    Swap,
    Invert,
};

class TSPAlgorithm
{
protected:
    GraphMatrix *graph;
    int graphSize;
    // Array holding the vertices of the current path
    int *currentPath = NULL;
    int currentPathWeight;
    // Array holding the vertices of the next permutation of this path
    int *nextPath = NULL;
    int nextPathWeight;

    // Array holding the best found path yet
    int *bestFoundPath = NULL;
    int bestFoundPathWeight = INT_MAX;

    // Current number of iterations without improvement
    int noImprovementIters = 0;
    // The maximum number of iterations without improvement
    int maxNoImprovementIters;
    bool satisfactoryResult = false;

    int initialPathWeight;
    // The time when the algorithm started [ms]
    std::chrono::_V2::steady_clock::time_point startTime;

    // The maximum allowed time for the algorithm execution [ms]
    int maxExecutionTime = 30000;

    InitialPathMode initialPathMode = Greedy;
    NeighborMode neighborMode = Invert;
    Timer timer;

    /**
     * @brief Sets the initial path
     */
    void setInitialPath();

    /**
     * @brief Sets the initial path by simply putting vertices in order: 0, 1, 2, ...
     */
    void initialPathInOrder();

    /**
     * @brief Sets the initial path using the greedy method
     * Every newly added vertex is the closest one to the previous
     */
    void initialPathGreedy();

    int getCurrentPathWeight();

    int randomPathIndex();

    // Returns true if the next path is equal to current path
    bool nextPathEqualToCurrent();

    // Gets the new path by inverteing a path between index1 and index2 inclusive
    void invert(int index1, int index2);

    // Gets the new path by swaping vertex at index1 with vertex at index2
    void swap(int index1, int index2);

    /**
     * @brief Returns the edge weight between vertices from currentPath
     * at indices index1 and index2
     */
    int getWeight(int index1, int index2);

    /**
     * @brief Returns the edge weight between vertices from nextPath
     * at indices index1 and index2
     */
    int getNextWeight(int index1, int index2);

    void printPath(int *path);

    /**
     * @brief Returns a random double [0.0, 1.0]
     */
    double randomDouble();

    /**
     * @brief Copies the path from `src` path to `dest` path
     */
    void copyPath(int *src, int *dest);

    /**
     * @brief Checks if the end criterion is met
     *
     * @return true if the end criterion is met otherwise false
     */
    bool endCriterionIsMet();

    /**
     * @brief Prints 'iteration' number, 'pathWeight' and percentage difference between `pathWeight` and the optimum.
     */
    void printImprovement(int iteration, int pathWeight);

    /**
     * @brief Returns true when the limit of the number of iterations without improvement has been reached,
     * otherwise returns `false`
     */
    bool noImprovementItersLimit();

    /**
     * @brief Returns true when the execution time limit has been reached,
     * otherwise returns false
     */
    bool executionTimeLimit();

    /**
     * @brief Get the Percentage difference between best found path weight and the optimal path weight
     */
    float getPrd(int pathWeight);

public:
    TSPAlgorithm(GraphMatrix *graph);

    ~TSPAlgorithm();

    int getGreedyPathWeight();

    int getInOrderPathWeight();
};

#endif
