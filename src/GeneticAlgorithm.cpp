#include "GeneticAlgorithm.hpp"
#include <algorithm>
#include <numeric>
#include <iterator>

GeneticAlgorithm::GeneticAlgorithm(GraphMatrix *graph, AlgorithmParams params)
{
    this->params = params;
    this->graph = graph;
    this->vertexCount = graph->getVertexCount();

    this->population = new Individual *[params.populationCount];
    this->nextGenPopulation = new Individual *[params.nextGenPopulationCount];
    this->matingPool = new Individual *[params.matingPoolSize];

    const int jointPopulCount = params.populationCount + params.nextGenPopulationCount;

    // Vector for holding a joint population of current population and next population during succession
    this->jointPopul.resize(jointPopulCount);

    for (int i = 0; i < params.populationCount; ++i)
    {
        population[i] = new Individual(vertexCount, graph);
    }
    for (int i = 0; i < params.nextGenPopulationCount; ++i)
    {
        nextGenPopulation[i] = new Individual(vertexCount, graph);
    }

    setMinError();
}

GeneticAlgorithm::~GeneticAlgorithm()
{
    for (int i = 0; i < params.populationCount; ++i)
    {
        delete population[i];
    }
    for (int i = 0; i < params.nextGenPopulationCount; ++i)
    {
        delete nextGenPopulation[i];
    }

    delete[] population;
    delete[] nextGenPopulation;
    delete[] matingPool;
}

void GeneticAlgorithm::setMinError()
{
    if (this->vertexCount <= 70)
    {
        this->params.minError = 0.0;
    }
    else if (this->vertexCount <= 350)
    {
        this->params.minError = 0.5;
    }
    else
    {
        this->params.minError = 1.5;
    }
}

void GeneticAlgorithm::checkPopulation(Individual **population, int populationCount)
{
    for (int i = 0; i < populationCount; i++)
    {
        if (!pathIsValid(population[i]->path))
        {
            printf("error\n");
        }
    }
}

void GeneticAlgorithm::copyPath(int *fromPath, int *toPath)
{
    std::copy(fromPath, fromPath + vertexCount, toPath);
}

void GeneticAlgorithm::copySection(int *fromArray, int *toArray, int index1, int index2)
{
    std::copy(fromArray + index1, fromArray + index2 + 1, toArray + index1);
}

Path GeneticAlgorithm::solveTSP()
{
    timer.start();

    // Crate random population
    initializePopulation();

    do
    {
        executeMutations();
        createMatingPool();
        executeCrossover();
        // printCurrentPopulationWeights();
        createNewPopulation();
        printBestPrd();
    } while (!endConditionIsMet());

    return getResult();
}

void GeneticAlgorithm::createNewPopulation()
{
    std::copy(population, population + params.populationCount, jointPopul.begin());
    std::copy(nextGenPopulation, nextGenPopulation + params.nextGenPopulationCount, jointPopul.begin() + params.populationCount);

    // Sort joint population from best individual to worst
    std::sort(jointPopul.begin(), jointPopul.end(), [](Individual *a, Individual *b)
              { return a->getPathWeight() < b->getPathWeight(); });

    // Copy first populationCount number of inviduals from joint population to the current population
    for (int i = 0; i < params.populationCount; i++)
    {
        copyPath(jointPopul[i]->path, population[i]->path);
        population[i]->pathWeight = jointPopul[i]->pathWeight;
    }

    fittestIndividual = jointPopul[0];
}

void GeneticAlgorithm::createMatingPool()
{
    // Create mating pool by tournament selection
    tournamentSelection();
}

Individual *fitterIndividual(Individual *individual1, Individual *individual2)
{
    return individual1->getPathWeight() < individual2->getPathWeight()
               ? individual1
               : individual2;
}

void GeneticAlgorithm::tournamentSelection()
{
    std::random_shuffle(population, population + params.populationCount);

    for (int i = 0; i < params.populationCount; i += 2)
    {
        Individual *contestant1 = population[i];
        Individual *contestant2 = population[i + 1];

        Individual *winner = fitterIndividual(contestant1, contestant2);

        matingPool[i / 2] = winner;
    }
}

bool GeneticAlgorithm::endConditionIsMet()
{
    // No individual generated yet
    if (fittestIndividual == NULL)
    {
        return false;
    }

    if (executionTimeLimit())
    {
        printf("Reached the execution time limit\n");
        return true;
    }

    // Reached minimum error
    float error = getError(fittestIndividual->getPathWeight());
    if (error <= params.minError)
    {
        printf("Reached minimum error: %.4f.\n", params.minError);
        return true;
    }

    return false;
}

void GeneticAlgorithm::initializePopulation()
{
    for (int i = 0; i < params.populationCount; ++i)
    {
        population[i]->setRandomPath();
        population[i]->updatePathWeight();
    }
}

void GeneticAlgorithm::executeCrossover()
{
    for (int i = 0; i < params.nextGenPopulationCount / 2; ++i)
    {
        // Get two random, unique parents from mating pool
        int index1 = randomInt(0, params.matingPoolSize - 2);
        int index2 = randomInt(index1 + 1, params.matingPoolSize - 1);

        Individual *parent1 = matingPool[index1];
        Individual *parent2 = matingPool[index2];

        Individual *child1 = nextGenPopulation[2 * i];
        Individual *child2 = nextGenPopulation[2 * i + 1];

        oxCrossover(parent1->path, parent2->path, child1->path, child2->path);
        child1->updatePathWeight();
        child2->updatePathWeight();
    }
}

void GeneticAlgorithm::fillMissingSection(int *parent, int *child, int index1, int index2)
{
    int childIndex = (index2 + 1) % vertexCount;
    int parentIndex = (index2 + 1) % vertexCount;

    while (childIndex != index1)
    {
        int value = parent[parentIndex];
        if (!sectionContains(value, child, index1, index2))
        {
            child[childIndex] = value;
            childIndex = (childIndex + 1) % vertexCount;
        }

        // Move to next parent index
        parentIndex = (parentIndex + 1) % vertexCount;
    }
}

int GeneticAlgorithm::randomInt(int min, int max)
{
    return min + rand() % (max - min + 1);
}

void GeneticAlgorithm::oxCrossover(int *parent1, int *parent2, int *child1, int *child2)
{
    int index1 = randomInt(0, vertexCount - 2);
    int index2 = randomInt(index1 + 1, vertexCount - 1);

    // sekcja dopasowania
    copySection(parent2, child1, index1, index2);
    copySection(parent1, child2, index1, index2);

    // uzupełnienie sekcji poza sekcją dopasowania
    fillMissingSection(parent1, child1, index1, index2);
    fillMissingSection(parent2, child2, index1, index2);
}

bool GeneticAlgorithm::sectionContains(int value, int *path, int index1, int index2)
{
    for (int i = index1; i <= index2; ++i)
    {
        if (path[i] == value)
        {
            return true;
        }
    }
    return false;
}

double GeneticAlgorithm::randomDouble()
{
    return (double)rand() / RAND_MAX;
}

void GeneticAlgorithm::executeMutations()
{
    for (int i = 0; i < params.populationCount; ++i)
    {
        if (params.mutationProbability > randomDouble())
        {
            int index1 = randomInt(0, vertexCount - 2);
            int index2 = randomInt(index1 + 1, vertexCount - 1);
            Individual *individual = population[i];
            inversionMutation(individual, index1, index2);
        }
    }
}

void GeneticAlgorithm::inversionMutation(Individual *individual, int index1, int index2)
{
    int *path = individual->path;
    std::reverse(path + index1, path + index2 + 1);
    individual->updatePathWeight();
}

bool GeneticAlgorithm::pathIsValid(int *path)
{
    int *pathCopy = new int[vertexCount];
    copyPath(path, pathCopy);
    std::sort(pathCopy, pathCopy + vertexCount);
    auto pos = std::adjacent_find(pathCopy, pathCopy + vertexCount);
    delete[] pathCopy;
    return pos == pathCopy + vertexCount;
}

float GeneticAlgorithm::getError(int pathWeight)
{
    const int optimum = graph->getOptimum();
    return (pathWeight - optimum) / (float)optimum;
}

float GeneticAlgorithm::getPrd(int pathWeight)
{
    return 100.0 * getError(pathWeight);
}

Path GeneticAlgorithm::getResult()
{
    int *bestPath = fittestIndividual->path;
    int bestPathWeight = fittestIndividual->getPathWeight();
    float prd = getPrd(bestPathWeight);
    return Path(bestPath, vertexCount, bestPathWeight, prd);
}

bool GeneticAlgorithm::executionTimeLimit()
{
    printf("elapsed: %lu ms\n", timer.getElapsedMs());
    return timer.getElapsedMs() > params.maxExecutionTimeMs;
}

// Printing functions

void GeneticAlgorithm::printCurrentPopulation()
{
    printf("Curr population: ");
    printPopulation(population, params.populationCount);
}

void GeneticAlgorithm::printNextPopulation()
{
    printf("Next population: ");
    printPopulation(population, params.populationCount);
}

void GeneticAlgorithm::printPopulation(Individual **population, int populationSize)
{
    printf("[");
    for (int i = 0; i < populationSize; ++i)
    {
        population[i]->printPath();
        printf(", ");
    }
    printf("]\n");
}

void GeneticAlgorithm::printPopulationWeights(Individual **population, int populationSize)
{
    printf("[");
    for (int i = 0; i < populationSize; ++i)
    {
        int weight = population[i]->getPathWeight();
        printf("%i, ", weight);
    }
    printf("]\n");
}

void GeneticAlgorithm::printArray(int *arr, int size)
{
    for (int i = 0; i < size; ++i)
    {
        printf("%i ", arr[i]);
    }
    printf("\n");
}

void GeneticAlgorithm::printCurrentPopulationWeights()
{
    printf("Curr population weights: ");
    printPopulationWeights(population, params.populationCount);
}

void GeneticAlgorithm::printBestPrd()
{
    if (fittestIndividual == NULL)
    {
        return;
    }
    int bestPathWeight = fittestIndividual->getPathWeight();
    float prd = getPrd(bestPathWeight);
    printf("prd: %.4f\n", prd);
}
