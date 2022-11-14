#ifndef FILE_UTILS_H
#define FILE_UTILS_H

#include <string>
#include "GraphMatrix.hpp"
#include "TestResult.hpp"

class FileUtils
{
public:
    /**
     * @brief Loads graph from a file
     *
     * @param fileName Name of the file
     * @return Pointer to the newly created graph
     */
    static GraphMatrix *loadGraph(std::string fileName);

    /**
     * @brief Writes header for the result file of single instance test
     *
     * @param filePath Output file path
     */
    static void writeInstanceTestHeader(std::string filePath);

    /**
     * @brief Save results of the single instance test
     *
     * @param filePath Output file path
     * @param instance Name of the instance
     * @param executionTime Execution time of the algorithm
     * @param path The result path of the algorithm
     * @param isCorrect true if the result path weight is the same as in the instance file, otherwise false
     */
    static void appendTestResult(std::string filePath, TestResult testResult);

    static void appendRandomInstanceTestResult(std::string filePath, int vertexCount, unsigned long elapsedTime);
    static void writeRandomInstanceTestHeader(std::string filePath);

    /**
     * @brief Returns string representation of the path
     *
     * @param path Path vertices
     */
    static std::string pathToStr(std::vector<int> path);
};

#endif
