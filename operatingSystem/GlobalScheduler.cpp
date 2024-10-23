#include "GlobalScheduler.h"
#include "Process.h"
#include <iostream>
#include <thread>
#include <chrono>
#include <iomanip>

#include "FCFSScheduler.h"

GlobalScheduler* GlobalScheduler::sharedInstance = nullptr;

GlobalScheduler::GlobalScheduler()
{
	this->running = true;

	const std::shared_ptr<FCFSScheduler> scheduler = std::make_shared<FCFSScheduler>(4);
	//const std::shared_ptr<RRScheduler> RRScheduler = std::make_shared<RRScheduler>();

	this->schedulerTable[FCFS_SCHEDULER_NAME] = scheduler;
	//this->schedulerTable[RR_SCHEDULER_NAME] = RRScheduler;

	// check what scheduler is set in the config file
	std::string schedulerName = "FCFSScheduler";
	this->schedulerTable[schedulerName];
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
	this->scheduler->run();
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
	this->scheduler->run();
}

void GlobalScheduler::addProcess(std::shared_ptr<Process> process) {
	if (!process) {
		throw std::invalid_argument("process is null");
	}
	if (!this->processTable) {
		throw std::runtime_error("processTable is not initialized");
	}
	this->processTable[process->getName()] = process;
}

void GlobalScheduler::handleScreenLS()
{
	std::cout << "Running Processes: " << std::endl;
	for (auto& process : this->scheduler->activeProcessesList)
	{
		std::cout	<< std::setw(20) << process->getName()
					<< std::setw(40) << process->getTimestampStarted()
					<< "Core: " << std::setw(5) << process->getCPUCoreID()
					<< process->getCommandCounter() << "/" << process->getLinesOfCode() << std::endl;
	}
	std::cout << "Finished Processes: " << std::endl;
	for (auto& process : this->scheduler->terminatedProcessesList)
	{
		std::cout << std::setw(20) << process->getName()
			<< std::setw(40) << process->getTimestampFinished()
			<< "Core: " << std::setw(10) << "Finished"
			<< process->getCommandCounter() << "/" << process->getLinesOfCode() << std::endl;
	}
}