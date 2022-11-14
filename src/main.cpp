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

    if (mode == "file_instance_test")
    {
        fileInstanceTest();
    }
    else if (mode == "random_instance_test")
    {
        randomInstanceTest();
    }

    else
    {
        printf("Wrong mode value.\n");
        return 1;
    }

    return 0;
}

void fileInstanceTest()
{
    const std::string inputDir = ini.GetValue("common", "input_dir", "./instances");
    const std::string outputDir = ini.GetValue("common", "output_dir", "./results");

    const int instanceCount = atoi(ini.GetValue("file_instance_test", "number_of_instances", "1"));

    for (int i = 0; i < instanceCount; i++)
    {
        const char *instanceTag = ("instance_" + std::to_string(i)).c_str();
        printf("\n%s:\n", instanceTag);

        const std::string instanceName = ini.GetValue(instanceTag, "instance", "UNKNOWN");
        const std::string outputFile = ini.GetValue(instanceTag, "output", "UNKNOWN");
        const int iterCount = atoi(ini.GetValue(instanceTag, "iterations", "1"));

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

        Tests::fileInstanceTest(graph, iterCount, instanceName, outputFilePath);

        printf("Finished.\n");
        printf("Results saved to file.\n");
        delete graph;
    }
}

void randomInstanceTest()
{
    printf("Random instance test\n\n");

    const std::string outputDir = ini.GetValue("common", "output_dir", "./results");

    const char *iniSection = "random_instance_test";
    const int minSize = atoi(ini.GetValue(iniSection, "min_size", "1"));
    const int maxSize = atoi(ini.GetValue(iniSection, "max_size", "1"));
    const int instanceCountPerSize = atoi(ini.GetValue(iniSection, "instance_num_per_size", "1"));
    const int iterCountPerInstance = atoi(ini.GetValue(iniSection, "iter_num_per_instance", "1"));
    const std::string outputFile = ini.GetValue(iniSection, "output", "UNKNOWN");
    const std::string outputFilePath = outputDir + "/" + outputFile;

    Tests::randomInstanceTest(minSize, maxSize, iterCountPerInstance, instanceCountPerSize, outputFilePath);
}
