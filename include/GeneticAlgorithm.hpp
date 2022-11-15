#ifndef GENETIC_ALGORITHM_HPP
#define GENETIC_ALGORITHM_HPP

#include <vector>
#include <climits>
#include <iostream>

#include "GraphMatrix.hpp"
#include "Individual.hpp"
#include "Path.hpp"
#include "Timer.hpp"
#include "AlgorithmParams.hpp"

class GeneticAlgorithm
{
private:
    AlgorithmParams params;

    GraphMatrix *graph;
    int vertexCount;

    // Array of individuals in the population
    Individual **population;
    // The next generation population
    Individual **nextGenPopulation;

    // Array of individuals in the mating pool
    Individual **matingPool;

    Individual *fittestIndividual = NULL;

    Timer timer;

public:
    GeneticAlgorithm(GraphMatrix *graph, AlgorithmParams params);
    ~GeneticAlgorithm();

    Path solveTSP();

    void initializePopulation();

    bool endConditionIsMet();

    void createMatingPool();

    // Generates the second population generation using crossover method
    void executeCrossover();

    // Mutates the inviduals of the second genertion population
    void executeMutations();

    // Generates 2 children from 2 parents using OX algorithm (Ordered Crossover)
    void oxCrossover(int *parent1, int *parent2, int *child1, int *child2);

    // Fills missing sections in child (from index2 to end, and from start to index1) with values from parent
    void fillMissingSection(int *parent, int *child, int index1, int index2);

    // creates a mating pool using the tournament selection algorithm
    void tournamentSelection();

    Individual *getRandomParent();

    // Returns true if the path contains the given value in section between index1 and index2 inclusive
    bool sectionContains(int value, int *path, int index1, int index2);

    void inversionMutation(int *path, int index1, int index2);

    // Creates new population by picking best individuals from current and next populations
    void createNewPopulation();

    Path getResult();

    float getPrd(int pathWeight);

    // Utils

    double randomDouble();

    /**
     * @brief Copy whole path `fromPath` into `toPath`
     */
    void copyPath(int *fromPath, int *toPath);

    /**
     * @brief Copy elements from fromArray, to toArray
     * starting from index index1 inclusive to index2 inclusive
     */
    void copySection(int *fromArray, int *toArray, int index1, int index2);

    // Printing functions

    void printCurrentPopulation();

    void printNextPopulation();

    void printPopulation(Individual **population, int populationSize);

    void printCurrentPopulationWeights();

    void printPopulationWeights(Individual **population, int populationSize);

    void printArray(int *arr, int size);

    void printBestPrd();

    // Testing functions

    // Checks if all inviduals in population have valid paths
    void checkPopulation(Individual **population, int populationCount);

    // Checks if path is a correct Hamiltonian path
    bool pathIsValid(int *path);

    /**
     * @brief Returns true when the execution time limit has been reached,
     * otherwise returns false
     */
    bool executionTimeLimit();
};

#endif
