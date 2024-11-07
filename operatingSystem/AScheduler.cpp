#include "AScheduler.h"

#include <iostream>

#include "GlobalScheduler.h"

AScheduler::AScheduler(SchedulingAlgorithm schedulingAlgo) : IThread() {
	this->schedulingAlgo = schedulingAlgo;
	//std::shared_ptr<Process> process = std::make_shared<Process>(pid, processName);
	//this->addProcess(std::shared_ptr<Process>(process));
}

void AScheduler::addProcess(std::shared_ptr<Process> process)
{
	GlobalScheduler::getInstance()->addProcessToProcessTable(process);
	this->activeProcessesList.push_back(process);
	auto it = std::find(activeProcessesList.begin(), activeProcessesList.end(), process);
	if (it == activeProcessesList.end()) {
		std::cout << "Process " << process->getName() << " not added in the list." << std::endl;
	}
}

std::shared_ptr<Process> AScheduler::findProcess(std::string processName) {
	return GlobalScheduler::getInstance()->processTable[processName];
}

void AScheduler::run() {
	this->init();
	this->execute();
}

void AScheduler::runQuantum(long long timeQuantum) {
	this->init();
	this->executeQuantum(timeQuantum);
}

void AScheduler::delay(int coreID) {
	for (long long i = 0; i < this->delayPerExec; i++) {
		this->coreList[coreID].tick();
		//std::cout << "Delaying " << i << std::endl;
		Sleep(100);
	}

}

void AScheduler::stop() {
	
}

void init()
{
	
}

void execute()
{

}