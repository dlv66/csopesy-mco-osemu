#include "Process.h"

#include <chrono>
#include <iostream>
#include <thread>
#include <fstream>
#include <random>
#include <Windows.h>

#include "GlobalScheduler.h"
#include "Utils.h"

Process::Process(int pid, std::string processName, long long minIns, long long maxIns) {
	this->pid = pid;
	this->processName = processName;

	// Random number generator helper lines of code
	unsigned seed = std::chrono::system_clock::now().time_since_epoch().count();
	std::mt19937_64 rng(seed);

	this->totalLineOfInstruction = rng() % (maxIns - minIns + 1) + minIns;

	this->timestampCreated = getCurrentTimestamp();
}

std::string Process::getName() const
{
	return this->processName;

}

std::string Process::getTimestampStarted() const
{
	return convertTimestampToString(this->timestampStarted);
}

std::string Process::getTimestampFinished() const
{
	return convertTimestampToString(this->timestampFinished);
}

void Process::update() {
	if (this->isFinished()) {
		this->state = Process::State::TERMINATED;
		this->timestampFinished = getCurrentTimestamp();
	}
	else if (this->cpuCoreID != -1) {
		this->state = Process::State::RUNNING;
		this->timestampStarted = getCurrentTimestamp();
	}
	else {
		this->state = Process::State::READY;
	}
}


bool Process::isFinished() const {
	return this->currentLineOfInstruction >= this->totalLineOfInstruction;
}

void Process::resetTicksLineOfInstruction() {
	this->ticksLineOfInstruction = 0;
}

long long Process::getCommandCounter() const {
	return this->currentLineOfInstruction;
}

long long Process::getRemainingTime() const {
	return this->totalLineOfInstruction - this->currentLineOfInstruction;
}

long long Process::getLinesOfCode() const {
	return this->totalLineOfInstruction;
}

int Process::getPID() const {
	return this->pid;
}

int Process::getCPUCoreID() const {
	return this->cpuCoreID;
}

void Process::setCPUCoreID(int coreID) {
	this->cpuCoreID = coreID;
}

Process::State Process::getState() const {
	return this->state;
}

void Process::execute() { // FOR FCFS

	//std::ofstream outFile(this->processName + ".txt");

	//if (!outFile) {
	//	std::cerr << "Error: Could not open the file for writing." << std::endl;
	//	return;
	//}

	//outFile << "Process name: " << this->processName << std::endl;
	//outFile << "Logs: \n" << std::endl;

	for (long long i = 0; i < this->totalLineOfInstruction; i++)
	{
		this->currentLineOfInstruction++;
		//outFile << "(" << convertTimestampToString(getCurrentTimestamp()) << ")" << " Core:" << this->getCPUCoreID() << " 'Hello world from " << this->processName << "!'" << std::endl;
		Sleep(200);

		// CPU Ticks
		GlobalScheduler::getInstance()->scheduler->coreList[this->getCPUCoreID()].tick(); 
		GlobalScheduler::getInstance()->scheduler->delay(this->getCPUCoreID());
	}

	//outFile.close();

	Sleep(200);
}

void Process::executeQuantum(int timeQuantum) { // FOR RRScheduler
	//std::ofstream outFile(this->processName + ".txt");

	//if (!outFile) {
	//	std::cerr << "Error: Could not open the file for writing." << std::endl;
	//	return;
	//}

	//outFile << "Process name: " << this->processName << std::endl;
	//outFile << "Logs: \n" << std::endl;


	long long instructionsToRun;
	long long remainingInstructions = this->totalLineOfInstruction - this->currentLineOfInstruction;

	// RRScheduler-specific lines of code:
	if (timeQuantum > remainingInstructions) {
		// If time quantum is larger than the remaining instructions, just use the remaining instructions.
		instructionsToRun = remainingInstructions;
	}
	else {
		// Otherwise, use the maxInstructions value.
		instructionsToRun = timeQuantum;
	}


	for (long long i = 0; i < instructionsToRun; i++) {
		if (this->currentLineOfInstruction >= this->totalLineOfInstruction) {
			break;
		}

		this->currentLineOfInstruction++;
		this->ticksLineOfInstruction++;
		//outFile << "(" << convertTimestampToString(getCurrentTimestamp()) << ") Core:"
		//	<< this->getCPUCoreID() << " 'Hello world from "
		//	<< this->processName << "!'" << std::endl;

		// CPU Ticks
		GlobalScheduler::getInstance()->scheduler->coreList[this->getCPUCoreID()].tick();
		GlobalScheduler::getInstance()->scheduler->delay(this->getCPUCoreID());
		Sleep(200);
		
	}
	
	if (this->isFinished()) {
		this->state = Process::State::TERMINATED;
		this->timestampFinished = getCurrentTimestamp();
		//outFile.close();
	}
}