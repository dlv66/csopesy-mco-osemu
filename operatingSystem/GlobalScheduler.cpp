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

void GlobalScheduler::addProcessToProcessTableNoCout(std::shared_ptr<Process> process)
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
		for (int i = 0; i < this->scheduler->nCores; i++)
		{
			if (this->scheduler->coreList[i].process != nullptr)
			{
				std::shared_ptr<Process> runningProcess = this->scheduler->coreList[i].process;
				std::cout << std::setw(20) << std::left << runningProcess->getName()
					<< std::setw(40) << std::left << runningProcess->getTimestampStarted()
					<< "Core: " << std::setw(10) << std::left << runningProcess->getCPUCoreID()
					<< runningProcess->getCommandCounter() << "/" << runningProcess->getLinesOfCode() << std::endl;
			}
		}
		for (auto& process : this->scheduler->activeProcessesList)
		{
			if (process != nullptr)
			{
				if (process->getCPUCoreID() == -1)
					std::cout << std::setw(20) << std::left << process->getName()
					<< std::setw(40) << std::left << process->getTimestampStarted()
					<< "Core: " << std::setw(10) << std::left << "N/A"
					<< process->getCommandCounter() << "/" << process->getLinesOfCode() << std::endl;
			}
		}
	}


	std::cout << "Terminated Processes: " << std::endl;
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

void GlobalScheduler::startSchedulerTestInBackground() {
	std::thread schedulerTestThread(&GlobalScheduler::handleSchedulerTest, this);
	schedulerTestThread.detach(); // Detach the thread to run independently
}

void GlobalScheduler::handleSchedulerTest()
{
	// “scheduler-test” behavior: Every X CPU cycles, a new process is generated and put into the ready queue for your CPU scheduler. This frequency can be set in the “config.txt.” 
	// As long as CPU cores are available, each process can be executed and be accessible via the “screen” command.

	int i = 0;
	batchScheduler = true;

	while (batchScheduler) // while batchScheduler is true
	{
		std::string screenName = "process" + std::to_string(i);
		std::shared_ptr<Process> process = std::make_shared<Process>(1, screenName); // create process
		std::shared_ptr <BaseScreen> screen = std::make_shared <BaseScreen>(process, screenName); // create screen for process
		GlobalScheduler::getInstance()->scheduler->addProcessNoCout(process); // add process to scheduler queue
		ConsoleManager::getInstance()->registerScreenNoCout(screen); // register screen to table of layouts/screens

		tick(); // wait for 1000ms

		i++; // increment i
	}
}

void GlobalScheduler::handleSchedulerStop()
{
	batchScheduler = false;
	std::cout << "Dummy processes creation HALTED.\n";
}