#include "GlobalScheduler.h"
#include "Process.h"
#include <iostream>
#include <thread>
#include <chrono>

#include "FCFSScheduler.h"

GlobalScheduler* GlobalScheduler::sharedInstance = nullptr;

GlobalScheduler::GlobalScheduler()
{
	this->running = true;

	//const std::shared_ptr<FCFSScheduler> FCFSScheduler = std::make_shared<FCFSScheduler>();
	//const std::shared_ptr<RRScheduler> RRScheduler = std::make_shared<RRScheduler>();

	//this->schedulerTable[FCFS_SCHEDULER_NAME] = FCFSScheduler;
	//this->schedulerTable[RR_SCHEDULER_NAME] = RRScheduler;
}

GlobalScheduler* GlobalScheduler::getInstance()
{
	return sharedInstance;
}

void GlobalScheduler::initialize()
{
	sharedInstance = new GlobalScheduler();
}

void GlobalScheduler::destroy()
{
	delete sharedInstance;
}

// TODO: Implement this function
void GlobalScheduler::run()
{

}

void GlobalScheduler::stop()
{
	this->running = false;
}

bool GlobalScheduler::isRunning() const
{
	return this->running;
}

void GlobalScheduler::tick() const
{
	this->scheduler->execute();
}

void GlobalScheduler::addProcess(std::shared_ptr<Process> process)
{
	this->processTable[process->getName()] = process;
}