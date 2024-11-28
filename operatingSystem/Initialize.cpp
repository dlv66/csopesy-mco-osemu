#include "Initialize.h"

#include "GlobalScheduler.h"
#include <iostream>
#include <fstream>

Initialize::Initialize()
{
	myText = "";
	numCPU = 0;
	scheduler = "";
	quantumCycles = 0;
	batchProcessFreq = 0;
	minIns = 0;
	maxIns = 0;
	delayPerExec = 0;
	maxOverallMem = 0;
	memPerFrame = 0;
	minMemPerProc = 0;
	maxMemPerProc = 0;
}

void Initialize::start()
{
	std::ifstream configFile("config.txt");

	if (!configFile.is_open()) {
		std::cerr << "Failed to open config file." << std::endl;
		return;
	}

	while (getline(configFile, myText)) {
		if (myText.find("num-cpu") != std::string::npos) {
			numCPU = std::stoi(myText.substr(myText.find("num-cpu") + 8));
		}
		else if (myText.find("scheduler") != std::string::npos) {
			std::string schedulerValue = myText.substr(myText.find("scheduler") + 10);
			scheduler = schedulerValue.substr(1, schedulerValue.length() - 2);
		}
		else if (myText.find("quantum-cycles") != std::string::npos) {
			quantumCycles = std::stoll(myText.substr(myText.find("quantum-cycles") + 15));
		}
		else if (myText.find("batch-process-freq") != std::string::npos) {
			batchProcessFreq = std::stoll(myText.substr(myText.find("batch-process-freq") + 19));
		}
		else if (myText.find("min-ins") != std::string::npos) {
			minIns = std::stoll(myText.substr(myText.find("min-ins") + 8));
		}
		else if (myText.find("max-ins") != std::string::npos) {
			maxIns = std::stoll(myText.substr(myText.find("max-ins") + 8));
		}
		else if (myText.find("delay-per-exec") != std::string::npos) {
			delayPerExec = std::stoll(myText.substr(myText.find("delay-per-exec") + 15));
		}

		// NEW FOR MO2:
		else if (myText.find("max-overall-mem") != std::string::npos) {
			maxOverallMem = std::stoll(myText.substr(myText.find("max-overall-mem") + 16));
		}
		else if (myText.find("mem-per-frame") != std::string::npos) {
			memPerFrame = std::stoll(myText.substr(myText.find("mem-per-frame") + 14));
		}
		else if (myText.find("min-mem-per-proc") != std::string::npos) {
			minMemPerProc = std::stoll(myText.substr(myText.find("min-mem-per-proc") + 17));
		}
		else if (myText.find("max-mem-per-proc") != std::string::npos) {
			maxMemPerProc = std::stoll(myText.substr(myText.find("max-mem-per-proc") + 17));
		}
	}
	configFile.close();

}