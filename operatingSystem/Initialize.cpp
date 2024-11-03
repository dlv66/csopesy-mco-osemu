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
	}
	configFile.close();

	// TODO: Find way to use the variables for the other parts of the operating system
	// GlobalScheduler::getInstance()->initialize(numCPU, scheduler, quantumCycles, batchProcessFreq, minIns, maxIns, delayPerExec);
}