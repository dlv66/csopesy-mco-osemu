#include "GlobalScheduler.h"
#include "Process.h"
#include <iostream>
#include <fstream>
#include <thread>
#include <chrono>
#include <iomanip>

#include "FCFSScheduler.h"
#include "RRScheduler.h"

GlobalScheduler* GlobalScheduler::sharedInstance = nullptr;

GlobalScheduler::GlobalScheduler(const Initialize& initConfig) : running(true) {
	// Choose the scheduler based on the config file's scheduler value
	if (initConfig.scheduler == "rr" || initConfig.scheduler == "RR") {
		// Instantiate Round-Robin Scheduler with quantum, delayExec, and numCPU from Initialize
		auto rrScheduler = std::make_shared<RRScheduler>(initConfig.quantumCycles, initConfig.delayPerExec, initConfig.numCPU);
		this->schedulerTable[RR_SCHEDULER_NAME] = rrScheduler;
		this->scheduler = rrScheduler;
		std::cout << "GlobalScheduler initialized with Round-Robin Scheduler.\n";
		this->scheduler->startQuantum(initConfig.quantumCycles);
	}
	else if (initConfig.scheduler == "fcfs" || initConfig.scheduler == "FCFS") {
		// Instantiate First-Come, First-Served Scheduler
		// Instantiate First-Come, First-Served Scheduler
		auto fcfsScheduler = std::make_shared<FCFSScheduler>(initConfig.numCPU, initConfig.delayPerExec);
		this->schedulerTable[FCFS_SCHEDULER_NAME] = fcfsScheduler;
		this->scheduler = fcfsScheduler;
		std::cout << "GlobalScheduler initialized with FCFS Scheduler.\n";
		this->scheduler->start();
	}
	else {
		std::cerr << "Unknown scheduler type in config: " << initConfig.scheduler << std::endl;
	}
}

void GlobalScheduler::initialize(const Initialize& initConfig) {
	if (!sharedInstance) {
		sharedInstance = new GlobalScheduler(initConfig);
	}
}

GlobalScheduler* GlobalScheduler::getInstance() {
	return sharedInstance;
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

void GlobalScheduler::runQuantum(int timeQuantum)
{
	this->scheduler->startQuantum(timeQuantum);
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

void GlobalScheduler::tickGlobal()
{
	this->globalCPUCycles++;
}

void GlobalScheduler::addProcessToProcessTable(std::shared_ptr<Process> process)
{
	this->processTable[process->getName()] = process;
}

void GlobalScheduler::handleReportUtil() const 
{
	std::ofstream reportUtilFile("csopesy-log.txt");

	reportUtilFile << "\n\n\n";
	reportUtilFile << "---------------------------------------------------------";
	reportUtilFile << "\n";

	reportUtilFile << "Running Processes: " << std::endl;
	if (this->scheduler->activeProcessesList.empty()) {
		reportUtilFile << "No Active Processes" << std::endl;
	}
	else {
		for (int i = 0; i < this->scheduler->nCores; i++) {
			if (this->scheduler->coreList[i].process != nullptr) {
				std::shared_ptr<Process> runningProcess = this->scheduler->coreList[i].process;
				reportUtilFile << std::setw(20) << std::left << runningProcess->getName()
					<< std::setw(40) << std::left << runningProcess->getTimestampStarted()
					<< "Core: " << std::setw(10) << std::left << runningProcess->getCPUCoreID()
					<< runningProcess->getCommandCounter() << "/" << runningProcess->getLinesOfCode() << std::endl;
			}
		}
	}

	reportUtilFile << "\nTerminated Processes: " << std::endl;
	if (this->scheduler->terminatedProcessesList.empty()) {
		reportUtilFile << "No Finished Processes" << std::endl;
	}
	else {
		for (auto& process : this->scheduler->terminatedProcessesList) {
			reportUtilFile << std::setw(20) << process->getName()
				<< std::setw(40) << process->getTimestampFinished()
				<< "Core: " << std::setw(10) << "Finished"
				<< process->getCommandCounter() << "/" << process->getLinesOfCode() << std::endl;
		}
	}

	reportUtilFile << "\nWaiting Processes: " << std::endl;
	for (auto& process : this->scheduler->activeProcessesList)
	{
		if (process != nullptr)
		{
			if (process->getCPUCoreID() == -1)
				reportUtilFile << std::setw(20) << std::left << process->getName()
				<< std::setw(40) << std::left << process->getTimestampStarted()
				<< "Core: " << std::setw(10) << std::left << "N/A"
				<< process->getCommandCounter() << "/" << process->getLinesOfCode() << std::endl;
		}
	}

	reportUtilFile << "\n";

	reportUtilFile.close();

	std::cout << "Report Util file created through file 'csopesy-log.txt'.\n";
}

void GlobalScheduler::getCPUUtilization() const
{
	// check cores being used
	int coresUsed = 0;
	for (int i = 0; i < this->scheduler->nCores; i++)
	{
		if (this->scheduler->coreList[i].process != nullptr && 
			this->scheduler->coreList[i].process->getState() == Process::State::RUNNING)
		{
			coresUsed++;
		}
	}
	double cpuUtilPercentage = 0.0;

	if (this->scheduler->nCores != 0) {
		cpuUtilPercentage = (static_cast<double>(coresUsed) / this->scheduler->nCores) * 100.0;
	}

	int coresAvailble = this->scheduler->nCores - coresUsed;

	std::cout << "CPU Utilization: " << cpuUtilPercentage << "%\n";
	std::cout << "Cores Used: " << coresUsed << "\n";
	std::cout << "Cores Available: " << coresAvailble << "\n";
}

void GlobalScheduler::handleScreenLS() const
{

	this->getCPUUtilization();

	std::cout << "\n\n\n";
	std::cout << "---------------------------------------------------------";
	std::cout << "\n";
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
					<< runningProcess->getCommandCounter() << "/" << runningProcess->getLinesOfCode();
			}
		}
	}
	std::cout << "\nWaiting Processes: " << std::endl;
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

	std::cout << "\nTerminated Processes: " << std::endl;
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

	std::cout << "\n";
}

void GlobalScheduler::startSchedulerTestInBackground(int minIns, int maxIns, int batchProcessFreq) {
	std::thread schedulerTestThread([this, minIns, maxIns, batchProcessFreq]() { handleSchedulerTest(minIns, maxIns, batchProcessFreq); });

	schedulerTestThread.detach(); // Detach the thread to run independently
}

void GlobalScheduler::handleSchedulerTest(int minIns, int maxIns, int batchProcessFreq)
{
	int i = 0;
	batchScheduler = true;

	while (batchScheduler) // while batchScheduler is true
	{

		// code for batch process freq
		tickGlobal(); // tick one cpu cycle

		for (i = 0; i < batchProcessFreq; i++) {

			// LOGIC ERROR: Every new enter of for loop creates "process0"
			std::string screenName = "process" + std::to_string(i);
			std::shared_ptr<Process> process = std::make_shared<Process>(1, screenName, minIns, maxIns); // create process
			std::shared_ptr <BaseScreen> screen = std::make_shared <BaseScreen>(process, screenName); // create screen for process
			GlobalScheduler::getInstance()->scheduler->addProcess(process); // add process to scheduler queue
			ConsoleManager::getInstance()->registerScreenNoCout(screen); // register screen to table of layouts/screens, NO COUT IMPORTANT.

		}

	}
}

void GlobalScheduler::handleSchedulerStop()
{
	batchScheduler = false;
	std::cout << "Dummy processes creation HALTED.\n";
}