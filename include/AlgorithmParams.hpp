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

    int matingPoolSize;

    AlgorithmParams(){};

    AlgorithmParams(
        int maxExecutionTimeMs,
        int maxItersWithoutImprovment,
        float crossoverProbability,
        float mutationProbability,
        int populationCount)
    {
        if (populationCount % 2 == 1)
        {
            printf("Population count must be even.");
            exit(0);
        }

        this->maxExecutionTimeMs = maxExecutionTimeMs;
        this->maxItersWithoutImprovment = maxItersWithoutImprovment;
        this->crossoverProbability = crossoverProbability;
        this->mutationProbability = mutationProbability;
        this->populationCount = populationCount;

        this->matingPoolSize = populationCount / 2;
    }

    void printParams()
    {
        printf("Max execution time: %i\n", maxExecutionTimeMs);
        printf("Max iters w/out improv: %i\n", maxItersWithoutImprovment);
        printf("crossover probab: %.4f\n", crossoverProbability);
        printf("Mutation probab: %.4f\n", mutationProbability);
        printf("population count: %i\n", populationCount);
        printf("mating pool size: %i\n", matingPoolSize);
    }
};

#endif
