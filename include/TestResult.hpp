#ifndef TEST_RES_H
#define TEST_RES_H

#include <string>
#include "Path.hpp"

struct TestResult
{
    std::string instanceName;
    int vertexCount;
    unsigned long elapsedTime;
    Path path;
    // Percentage difference between the result weight and the optimal weight
    float prd;

    TestResult(std::string instanceName, int vertexCount, unsigned long elapsedTime, Path path, float prd)
    {
        this->instanceName = instanceName;
        this->vertexCount = vertexCount;
        this->elapsedTime = elapsedTime;
        this->path = path;
        this->prd = prd;
    }
};

#endif
