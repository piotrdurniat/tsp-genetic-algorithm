
#include "TSPAlgorithm.hpp"

TSPAlgorithm::TSPAlgorithm(GraphMatrix *graph)
{
    this->graph = graph;

    this->graphSize = graph->getVertexCount();

    this->currentPath = new int[graphSize];
    this->nextPath = new int[graphSize];
    this->bestFoundPath = new int[graphSize];
}

TSPAlgorithm::~TSPAlgorithm()
{
    delete[] currentPath;
    delete[] nextPath;
    delete[] bestFoundPath;
}

void TSPAlgorithm::setInitialPath()
{
    if (initialPathMode == Greedy)
    {
        initialPathGreedy();
    }
    else
    {
        initialPathInOrder();
    }
}

void TSPAlgorithm::initialPathInOrder()
{
    for (int i = 0; i < graphSize; i++)
    {
        this->currentPath[i] = i;
    }
}

void TSPAlgorithm::initialPathGreedy()
{
    // Array holding information about available vertices
    bool availableVerices[graphSize];

    for (int i = 1; i < graphSize; ++i)
    {
        availableVerices[i] = true;
    }

    currentPath[0] = 0;
    availableVerices[0] = false;

    for (int i = 1; i < graphSize; ++i)
    {

        int minPathWeight = INT_MAX;
        int closestVertex;
        for (int j = 1; j < graphSize; ++j)
        {
            if (availableVerices[j])
            {
                int edgeWeight = graph->getWeight(currentPath[i - 1], j);
                if (edgeWeight < minPathWeight)
                {
                    minPathWeight = edgeWeight;
                    closestVertex = j;
                }
            }
        }

        currentPath[i] = closestVertex;
        availableVerices[closestVertex] = false;
    }
}

int TSPAlgorithm::getGreedyPathWeight()
{
    initialPathGreedy();
    return getCurrentPathWeight();
}

int TSPAlgorithm::getInOrderPathWeight()
{
    initialPathInOrder();
    return getCurrentPathWeight();
}

int TSPAlgorithm::getCurrentPathWeight()
{
    int sum = 0;
    for (int i = 0; i < graphSize - 1; ++i)
    {
        sum += graph->getWeight(currentPath[i], currentPath[i + 1]);
    }
    sum += graph->getWeight(currentPath[graphSize - 1], 0);
    return sum;
}

int TSPAlgorithm::randomPathIndex()
{
    return rand() % graphSize;
}

bool TSPAlgorithm::nextPathEqualToCurrent()
{
    for (int i = 0; i < graphSize; ++i)
    {
        if (currentPath[i] != nextPath[i])
        {
            return false;
        }
    }
    return true;
}

void TSPAlgorithm::invert(int index1, int index2)
{
    if (index1 > index2)
    {
        std::swap(index1, index2);
    }

    for (int i = 0; i < graphSize; ++i)
    {
        if (i < index1 || i > index2)
        {
            nextPath[i] = currentPath[i];
        }
        else
        {
            nextPath[i] = currentPath[index2 - i + index1];
        }
    }

    nextPathWeight = currentPathWeight;

    // Path before invert: ... -> A -> B -> ... -> C -> D -> ...
    // Path after invert:  ... -> A -> C -> ... -> B -> D -> ...

    int A = index1 - 1;
    if (A == -1)
    {
        A = graphSize - 1;
    }
    int B = index1;
    int C = index2;
    int D = index2 + 1;
    if (D == graphSize)
    {
        D = 0;
    }

    nextPathWeight -= getWeight(A, B);
    nextPathWeight -= getWeight(C, D);

    nextPathWeight += getWeight(A, C);
    nextPathWeight += getWeight(B, D);

    // If the graph is directed then remove path weight from B to C
    // and add a reverse path weight (from C to B)
    if (graph->directed)
    {
        for (int i = B; i < C; ++i)
        {
            nextPathWeight -= getWeight(i, i + 1);
        }
        for (int i = C; i > B; --i)
        {
            nextPathWeight += getWeight(i, i - 1);
        }
    }
}

void TSPAlgorithm::swap(int index1, int index2)
{
    if (index1 > index2)
    {
        std::swap(index1, index2);
    }

    // Copy current path into new path
    for (int i = 0; i < graphSize; ++i)
    {
        nextPath[i] = currentPath[i];
    }

    // Swap vertex on index1 with vertex on index2
    nextPath[index1] = currentPath[index2];
    nextPath[index2] = currentPath[index1];

    nextPathWeight = currentPathWeight;

    // Path before swap: ... -> A1 -> B1 -> C1 -> ... -> A2 -> B2 -> C2 ->
    // Path after swap:  ... -> A1 -> B2 -> C1 -> ... -> A2 -> B1 -> C2 ->

    int A1 = index1 - 1;
    if (A1 == -1)
    {
        A1 = graphSize - 1;
    }
    const int B1 = index1;
    int C1 = index1 + 1;
    if (C1 == graphSize)
    {
        C1 = 0;
    }

    int A2 = index2 - 1;
    if (A2 == -1)
    {
        A2 = graphSize - 1;
    }
    const int B2 = index2;
    int C2 = index2 + 1;
    if (C2 == graphSize)
    {
        C2 = 0;
    }

    // Remove old edges
    nextPathWeight -= getWeight(A1, B1);
    nextPathWeight -= getWeight(B1, C1);

    if (C1 != B2)
    {
        nextPathWeight -= getWeight(A2, B2);
    }

    nextPathWeight -= getWeight(B2, C2);

    // Add new edges
    nextPathWeight += getNextWeight(A1, B1);
    nextPathWeight += getNextWeight(B1, C1);

    if (C1 != B2)
    {
        nextPathWeight += getNextWeight(A2, B2);
    }
    nextPathWeight += getNextWeight(B2, C2);
}

int TSPAlgorithm::getWeight(int index1, int index2)
{
    return graph->getWeight(currentPath[index1], currentPath[index2]);
}

int TSPAlgorithm::getNextWeight(int index1, int index2)
{
    return graph->getWeight(nextPath[index1], nextPath[index2]);
}

void TSPAlgorithm::printPath(int *path)
{
    for (int i = 0; i < graphSize; ++i)
    {
        printf("%i -> ", path[i]);
    }
    printf("%i\n", path[0]);
}

double TSPAlgorithm::randomDouble()
{
    return ((double)rand() / (RAND_MAX));
}

void TSPAlgorithm::copyPath(int *src, int *dest)
{
    for (int i = 0; i < graphSize; i++)
    {
        dest[i] = src[i];
    }
}

bool TSPAlgorithm::endCriterionIsMet()
{
    if (executionTimeLimit())
    {
        printf("The execution time limit has been reached\n");
        return true;
    }

    if (noImprovementItersLimit())
    {
        printf("The limit for number of iteraions without improvement has been reached\n");
        return true;
    }

    // if (pathWeight == optimum)
    // {
    //     // optimum reached
    //     return true;
    // }

    return false;
}

bool TSPAlgorithm::noImprovementItersLimit()
{
    return noImprovementIters >= maxNoImprovementIters;
}

bool TSPAlgorithm::executionTimeLimit()
{
    // printf("%lu ns\n", timer.getElapsedMillis());
    return timer.getElapsedMs() > maxExecutionTime;
}

void TSPAlgorithm::printImprovement(int iteration, int pathWeight)
{
    float prd = getPrd(pathWeight);
    printf("%4i %4i %.2f%%\n", iteration, pathWeight, prd);
}

float TSPAlgorithm::getPrd(int pathWeight)
{
    const int optimum = graph->getOptimum();
    return (100.0 * (pathWeight - optimum)) / optimum;
}
