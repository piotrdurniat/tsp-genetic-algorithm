#include "main.hpp"

#include <iostream>
#include "graphGenerator.hpp"
#include "FileUtils.hpp"
#include "Timer.hpp"
#include "tests.hpp"
#include "GraphMatrix.hpp"
#include "printColor.hpp"

int main(void)
{
    srand(1);

    ini.SetUnicode();

    SI_Error rc = ini.LoadFile("settings.ini");
    if (rc < 0)
    {
        printf("Could not load 'settings.ini' file\n");
        return 1;
    };

    const std::string mode = ini.GetValue("common", "mode", "UNKNOWN");
    const std::string inputDir = ini.GetValue("common", "input_dir", "./instances");
    const std::string outputDir = ini.GetValue("common", "output_dir", "./results");

    if (mode == "file_instance_test")
    {
        fileInstanceTest(inputDir, outputDir);
    }
    else if (mode == "random_instance_test")
    {
        randomInstanceTest(outputDir);
    }

    else
    {
        printf("Wrong mode value.\n");
        return 1;
    }

    return 0;
}

void fileInstanceTest(std::string inputDir, std::string outputDir)
{

    const int instanceCount = atoi(ini.GetValue("file_instance_test", "number_of_instances", "1"));

    for (int i = 0; i < instanceCount; i++)
    {
        const char *tag = ("instance_" + std::to_string(i)).c_str();
        printf("\n%s:\n", tag);

        const std::string instanceName = ini.GetValue(tag, "instance", "UNKNOWN");
        const std::string outputFile = ini.GetValue(tag, "output", "UNKNOWN");
        const int iterCount = atoi(ini.GetValue(tag, "iterations", "1"));

        const std::string inputFilePath = inputDir + "/" + instanceName;
        const std::string outputFilePath = outputDir + "/" + outputFile;

        printf("Input: %s\n", inputFilePath.c_str());
        printf("Output: %s\n", outputFilePath.c_str());
        printf("Iteration count: %i\n\n", iterCount);

        // Wczytanie grafu
        GraphMatrix *graph = FileUtils::loadGraph(inputFilePath);
        if (graph == NULL)
        {
            printf("File not found.\n");
            continue;
        }
        printf("Graph read from file:\n");
        graph->display();

        const auto params = getAlorithmParams();
        Tests::fileInstanceTest(graph, iterCount, instanceName, outputFilePath, params);

        printf("Finished.\n");
        printf("Results saved to file.\n");
        delete graph;
    }
}

void randomInstanceTest(std::string outputDir)
{
    printf("Random instance test\n\n");
    const char *tag = "random_instance_test";

    const int minSize = atoi(ini.GetValue(tag, "min_size", "1"));
    const int maxSize = atoi(ini.GetValue(tag, "max_size", "1"));
    const int instanceCountPerSize = atoi(ini.GetValue(tag, "instance_num_per_size", "1"));
    const int iterCountPerInstance = atoi(ini.GetValue(tag, "iter_num_per_instance", "1"));
    const auto outputFile = ini.GetValue(tag, "output", "UNKNOWN");

    const std::string outputFilePath = outputDir + "/" + outputFile;

    const auto params = getAlorithmParams();
    Tests::randomInstanceTest(minSize, maxSize, iterCountPerInstance, instanceCountPerSize, outputFilePath, params);
}

AlgorithmParams getAlorithmParams()
{
    const char *tag = "algorithm_params";

    int maxExecTimeMs = atoi(ini.GetValue(tag, "max_exec_time_ms", "30000"));
    int maxItersWoutImprov = atoi(ini.GetValue(tag, "max_iters_w_out_improv", "300000"));
    float crossoverProbab = std::stof(ini.GetValue(tag, "corssover_probability", "0.8"));
    float mutationProbab = std::stof(ini.GetValue(tag, "mutation_probability", "0.2"));
    int populationCount = atoi(ini.GetValue(tag, "population_count", "1000"));

    return AlgorithmParams(
        maxExecTimeMs,
        maxItersWoutImprov,
        crossoverProbab,
        mutationProbab,
        populationCount);
}
