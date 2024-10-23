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
	std::cout << process->getName() << " added to " <<"activeProcessesList" << std::endl;
	sleep(100);
}

std::shared_ptr<Process> AScheduler::findProcess(std::string processName) {
	return GlobalScheduler::getInstance()->processTable[processName];
}

void AScheduler::run() {
	this->init();
	this->execute();
}

void AScheduler::stop() {
	
}

void init()
{
	
}

void execute()
{

}