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
	
	this->scheduler = this->schedulerTable[schedulerName];
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
	this->scheduler->start();
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
	std::this_thread::sleep_for(std::chrono::milliseconds(1000));
}

void GlobalScheduler::addProcessToProcessTable(std::shared_ptr<Process> process)
{
	this->processTable[process->getName()] = process;
}

void GlobalScheduler::handleScreenLS() const
{
	std::cout << "Running Processes: " << std::endl;
	if(this->scheduler->activeProcessesList.empty())
	{
		std::cout << "No Active Processes" << std::endl;
	}
	else
	{
		for (auto& process : this->scheduler->activeProcessesList)
		{
			std::cout << std::setw(20) << process->getName()
				<< std::setw(40) << process->getTimestampStarted()
				<< "Core: " << std::setw(5) << process->getCPUCoreID() << std::setw(10)
				<< process->getCommandCounter() << "/" << process->getLinesOfCode() << std::endl;
		}
	}

	if (this->scheduler->terminatedProcessesList.empty())
	{
		std::cout << "No Finished Processes" << std::endl;
	}
	else
	{
		for (auto& process : this->scheduler->terminatedProcessesList)
		{
			std::cout << std::setw(20) << process->getName()
				<< std::setw(40) << process->getTimestampFinished()
				<< "Core: " << std::setw(10) << "Finished"
				<< process->getCommandCounter() << "/" << process->getLinesOfCode() << std::endl;
		}
	}
	
}