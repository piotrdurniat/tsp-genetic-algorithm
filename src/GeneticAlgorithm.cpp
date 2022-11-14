#include "GeneticAlgorithm.hpp"
#include <algorithm>
#include <numeric>
#include <iterator>

GeneticAlgorithm::GeneticAlgorithm(GraphMatrix *graph)
{
    this->graph = graph;
    this->vertexCount = graph->getVertexCount();

    this->populationCount = 400;
    this->population = new Individual *[populationCount];

    this->nextGenPopulation = new Individual *[populationCount];

    this->matingPoolSize = populationCount / 2;
    this->matingPool = new Individual *[matingPoolSize];

    this->crossoverProbability = 0.8;
    this->mutationProbability = 0.05;
}

GeneticAlgorithm::~GeneticAlgorithm()
{
    delete[] population;
    delete[] nextGenPopulation;
    delete[] matingPool;
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
    // Crate random population
    initializePopulation();

    do
    {
        createMatingPool();
        executeCrossover();
        printCurrentPopulationWeights();
        createNewPopulation();
        // printBestPrd();
    } while (!endConditionIsMet());

    return getResult();
}

void GeneticAlgorithm::createNewPopulation()
{
    const int jointPopulCount = 2 * populationCount;

    // Create a joint population of current population and next population
    std::vector<Individual *> jointPopul(jointPopulCount);
    std::copy(population, population + populationCount, jointPopul.begin());
    std::copy(nextGenPopulation, nextGenPopulation + populationCount, jointPopul.begin() + populationCount);

    // Sort joint population from best individual to worst
    std::sort(jointPopul.begin(), jointPopul.end(), [](Individual *a, Individual *b)
              { return a->getPathWeight() < b->getPathWeight(); });

    // Copy first half of the joint population to the current population
    for (int i = 0; i < populationCount; i++)
    {
        copyPath(jointPopul[i]->path, population[i]->path);
    }

    fittestIndividual = jointPopul[0];
}

void GeneticAlgorithm::createMatingPool()
{
    // Create mating pool by tournament selection
    tournamentSelection();
}

void GeneticAlgorithm::tournamentSelection()
{
    // Array holding of contestants (their indices in population)
    int contestants[populationCount];

    std::iota(contestants, contestants + populationCount, 0);
    std::random_shuffle(contestants, contestants + populationCount);

    for (int i = 0; i < populationCount; i += 2)
    {
        int index1 = contestants[i];
        int index2 = contestants[i + 1];

        Individual *contestant1 = population[index1];
        Individual *contestant2 = population[index2];

        Individual *winner =
            contestant1->getPathWeight() < contestant2->getPathWeight()
                ? contestant1
                : contestant2;

        matingPool[i / 2] = winner;

        copyPath(winner->path, matingPool[i / 2]->path);
    }
}

bool GeneticAlgorithm::endConditionIsMet()
{
    // No indivudual generated yet
    if (fittestIndividual == NULL)
    {
        return false;
    }

    // Reached optimum
    if (fittestIndividual->getPathWeight() == graph->getOptimum())
    {
        return true;
    }

    return false;
}

void GeneticAlgorithm::initializePopulation()
{
    for (int i = 0; i < populationCount; ++i)
    {
        population[i] = new Individual(vertexCount, graph);
        population[i]->setRandomPath();
        nextGenPopulation[i] = new Individual(vertexCount, graph);
    }
}

void GeneticAlgorithm::executeCrossover()
{
    for (int i = 0; i < populationCount / 2; ++i)
    {
        Individual *parent1 = getRandomParent();
        Individual *parent2;
        do
        {
            parent2 = getRandomParent();
        } while (parent2 == parent1);

        Individual *child1 = nextGenPopulation[2 * i];
        Individual *child2 = nextGenPopulation[2 * i + 1];

        oxCrossover(parent1->path, parent2->path, child1->path, child2->path);
    }
}

Individual *GeneticAlgorithm::getRandomParent()
{
    int index = random() % matingPoolSize;
    return matingPool[index];
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

void GeneticAlgorithm::oxCrossover(int *parent1, int *parent2, int *child1, int *child2)
{
    // Pick first index [0, vertexCount - 2]
    int index1 = random() % (vertexCount - 1);
    // Pick second index [index1 + 1, vertexCount - 1]
    int index2 = index1 + 1 + random() % (vertexCount - index1 - 1);

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
    return ((double)rand() / (RAND_MAX));
}

void GeneticAlgorithm::executeMutations()
{
    for (int i = 0; i < populationCount; ++i)
    {
        int index1 = 1 + random() % (vertexCount - 2);
        int index2 = index1 + 1 + random() % (vertexCount - index1 - 1);

        int *path = nextGenPopulation[i]->path;

        if (mutationProbability > randomDouble())
        {
            inversionMutation(path, index1, index2);
        }
    }
}

void GeneticAlgorithm::inversionMutation(int *path, int index1, int index2)
{
    int *nextPath = new int[vertexCount];

    if (index1 > index2)
    {
        std::swap(index1, index2);
    }

    for (int i = 0; i < vertexCount; ++i)
    {
        if (i < index1 || i > index2)
        {
            nextPath[i] = path[i];
        }
        else
        {
            nextPath[i] = path[index2 - i + index1];
        }
    }

    std::copy(nextPath, nextPath + vertexCount, path);
}

bool GeneticAlgorithm::pathIsValid(int *path)
{
    int *pathCopy = new int[vertexCount];
    for (int i = 0; i < vertexCount; i++)
    {
        pathCopy[i] = path[i];
    }

    std::sort(pathCopy, pathCopy + vertexCount);
    auto pos = std::adjacent_find(pathCopy, pathCopy + vertexCount);
    return pos == pathCopy + vertexCount;
}

// Printing functions

void GeneticAlgorithm::printCurrentPopulation()
{
    printf("Curr population: ");
    printPopulation(population, populationCount);
}

void GeneticAlgorithm::printNextPopulation()
{
    printf("Next population: ");
    printPopulation(population, populationCount);
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
    printPopulationWeights(population, populationCount);
}

float GeneticAlgorithm::getPrd(int pathWeight)
{
    const int optimum = graph->getOptimum();
    return (100.0 * (pathWeight - optimum)) / optimum;
}

Path GeneticAlgorithm::getResult()
{
    int *bestPath = fittestIndividual->path;
    int bestPathWeight = fittestIndividual->getPathWeight();
    float prd = getPrd(bestPathWeight);
    return Path(bestPath, vertexCount, bestPathWeight, prd);
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
