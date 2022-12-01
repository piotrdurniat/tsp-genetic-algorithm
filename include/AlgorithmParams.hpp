#ifndef GEN_ALG_PARAMS
#define GEN_ALG_PARAMS

#include <iostream>

struct AlgorithmParams
{
    int maxExecutionTimeMs;
    int maxItersWithoutImprovment;
    float crossoverProbability;
    float mutationProbability;
    // Number of individuals in the population and nextGenPopulation (Has to be even)
    int populationCount;
    int nextGenPopulationCount;
    int matingPoolSize;

    // Algorithms stops when reaching the min error
    float minError;

    AlgorithmParams(){};

    AlgorithmParams(
        int maxExecutionTimeMs,
        int maxItersWithoutImprovment,
        float crossoverProbability,
        float mutationProbability,
        int populationCount)
    {
        if (populationCount % 4 != 0)
        {
            printf("Population count must be divisible by 4.\n");
            exit(0);
        }
        if (crossoverProbability < 0 || crossoverProbability > 1.0)
        {
            printf("Crossover probability must be between 0 and 1\n");
            exit(0);
        }
        if (mutationProbability < 0 || mutationProbability > 1.0)
        {
            printf("Mutation probability must be between 0 and 1\n");
            exit(0);
        }

        this->maxExecutionTimeMs = maxExecutionTimeMs;
        this->maxItersWithoutImprovment = maxItersWithoutImprovment;

        this->crossoverProbability = crossoverProbability;
        this->mutationProbability = mutationProbability;

        this->populationCount = populationCount;
        this->nextGenPopulationCount = int(populationCount * crossoverProbability);
        this->matingPoolSize = populationCount / 2;

        this->minError = 0.0;
    }

    void print()
    {
        printf("max execution time: %i\n", maxExecutionTimeMs);
        printf("max iters w/out improv: %i\n", maxItersWithoutImprovment);
        printf("crossover probab: %.4f\n", crossoverProbability);
        printf("mutation probab: %.4f\n", mutationProbability);
        printf("population count: %i\n", populationCount);
        printf("next gen population count %i\n", nextGenPopulationCount);
        printf("mating pool size: %i\n", matingPoolSize);
        printf("min error: %.4f\n", minError);
    }
};

#endif
