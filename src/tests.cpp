#include "tests.hpp"

#include <iostream>
#include "Timer.hpp"
#include "graphGenerator.hpp"
#include "FileUtils.hpp"
#include "printColor.hpp"
#include "TestResult.hpp"
#include "Path.hpp"
#include "GeneticAlgorithm.hpp"

void Tests::fileInstanceTest(GraphMatrix *graph, int iterCount, std::string instanceName, std::string outputPath)
{
    FileUtils::writeInstanceTestHeader(outputPath);
    Timer timer;

    for (int i = 0; i < iterCount; ++i)
    {
        timer.start();
        GeneticAlgorithm alg(graph);
        Path path = alg.solveTSP();
        const unsigned long elapsedTime = timer.getElapsedNs();

        TestResult testResult(instanceName, graph->getVertexCount(), elapsedTime, path, path.prd);

        FileUtils::appendTestResult(outputPath, testResult);
    }
}

void Tests::randomInstanceTest(int minSize, int maxSize, int iterCountPerInstance, int instanceCountPerSize, std::string outputPath)
{
    FileUtils::writeRandomInstanceTestHeader(outputPath);
    Timer timer;
    GraphMatrix *graph;
    printf("%i, %i\n", iterCountPerInstance, instanceCountPerSize);

    for (int vertexCount = minSize; vertexCount <= maxSize; vertexCount++)
    {
        srand(1);
        // Average time for all instances of this size
        long unsigned averageTime = 0;
        for (int i = 0; i < instanceCountPerSize; ++i)
        {
            graph = graphGenerator::getRandom(vertexCount, 10);

            for (int j = 0; j < iterCountPerInstance; ++j)
            {
                timer.start();
                GeneticAlgorithm alg(graph);
                Path path = alg.solveTSP();
                averageTime += timer.getElapsedNs();
            }
            delete graph;
            graph = NULL;
        }
        averageTime /= iterCountPerInstance * instanceCountPerSize;
        FileUtils::appendRandomInstanceTestResult(outputPath, vertexCount, averageTime);
    }
    printf("Done. Saved to file.\n");
}

void Tests::testAlgorithm(std::vector<std::string> instances)
{

    for (std::string instanceName : instances)
    {
        GraphMatrix *graph;
        graph = FileUtils::loadGraph(instanceName);

        GeneticAlgorithm alg(graph);
        Path path = alg.solveTSP();
        const bool correctRes = path.weight == graph->optimum;

        printf("%14s", instanceName.c_str());

        if (correctRes)
        {
            printColor(" PASS\n", GREEN, BOLD, BOLD);
        }
        else
        {
            printColor(" FAIL\n", RED, BOLD, BOLD);
        }

        delete graph;
        graph = NULL;
    }
}
