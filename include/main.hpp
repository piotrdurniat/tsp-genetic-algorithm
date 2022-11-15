#include "../lib/SimpleIni.h"
#include "AlgorithmParams.hpp"

CSimpleIniA ini;

int main(void);

// Tests on instances from files
void fileInstanceTest(std::string inputDir, std::string outputDir);

// Tests on random instances
void randomInstanceTest(std::string outputDir);

AlgorithmParams getAlorithmParams();
