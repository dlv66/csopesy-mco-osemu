#include "AScheduler.h"
#include "GlobalScheduler.h"

AScheduler::AScheduler(SchedulingAlgorithm schedulingAlgo, int pid, std::string processName) : IThread() {
	this->schedulingAlgo = schedulingAlgo;
	std::shared_ptr<Process> process = std::make_shared<Process>(pid, processName);
	this->addProcess(std::shared_ptr<Process>(process));
}

void AScheduler::addProcess(std::shared_ptr<Process> process) {
	GlobalScheduler::getInstance()->addProcess(process);
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