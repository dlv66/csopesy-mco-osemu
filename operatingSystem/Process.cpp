#include "Process.h"

#include <chrono>
#include <iostream>
#include <thread>
#include <fstream>
#include <Windows.h>
#include "Utils.h"

Process::Process(int pid, std::string processName) {
	this->pid = pid;
	this->processName = processName;
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

void Process::update()
{
	if (this->isFinished())
	{
		this->state = Process::State::TERMINATED;
		this->timestampFinished = getCurrentTimestamp();
	}
	else if (this->cpuCoreID != -1)
	{
		this->state = Process::State::RUNNING;
		this->timestampStarted = getCurrentTimestamp();
	}
	else if (!this->isFinished() || this->cpuCoreID != -1) // LOGIC FOR PREEMPTION
	{
		this->state = Process::State::PREEMPTED;
	}
	else
	{
		this->state = Process::State::READY;
	}
}


bool Process::isFinished() const {
	return this->currentLineOfInstruction >= this->totalLineOfInstruction;
}

bool Process::forPreemption(int timeQuantum) const {
	return this->ticksLineOfInstruction == timeQuantum;
}

void Process::resetTicksLineOfInstruction() {
	this->ticksLineOfInstruction = 0;
}

int Process::getCommandCounter() const {
	return this->currentLineOfInstruction;
}

int Process::getRemainingTime() const {
	return this->totalLineOfInstruction - this->currentLineOfInstruction;
}

int Process::getLinesOfCode() const {
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

	std::ofstream outFile(this->processName + ".txt");

	if (!outFile) {
		std::cerr << "Error: Could not open the file for writing." << std::endl;
		return;
	}

	outFile << "Process name: " << this->processName << std::endl;
	outFile << "Logs: \n" << std::endl;

	for (int i = 0; i < this->totalLineOfInstruction; i++)
	{
		this->currentLineOfInstruction++;
		outFile << "(" << convertTimestampToString(getCurrentTimestamp()) << ")" << " Core:" << this->getCPUCoreID() << " 'Hello world from " << this->processName << "!'" << std::endl;
		Sleep(200);
	}

	outFile.close();

	Sleep(200);
}

void Process::executeQuantum(int timeQuantum) { // FOR RRScheduler
	std::ofstream outFile(this->processName + ".txt");

	if (!outFile) {
		std::cerr << "Error: Could not open the file for writing." << std::endl;
		return;
	}

	outFile << "Process name: " << this->processName << std::endl;
	outFile << "Logs: \n" << std::endl;


	int instructionsToRun;
	int remainingInstructions = this->totalLineOfInstruction - this->currentLineOfInstruction;



	// RRScheduler-specific lines of code:
	if (timeQuantum > remainingInstructions) {
		// If time quantum is larger than the remaining instructions, just use the remaining instructions.
		instructionsToRun = remainingInstructions;
	}
	else {
		// Otherwise, use the maxInstructions value.
		instructionsToRun = timeQuantum;
	}


	for (int i = 0; i < instructionsToRun; i++) {
		if (this->currentLineOfInstruction >= this->totalLineOfInstruction) {
			break;
		}

		this->currentLineOfInstruction++;
		this->ticksLineOfInstruction++;
		outFile << "(" << convertTimestampToString(getCurrentTimestamp()) << ") Core:"
			<< this->getCPUCoreID() << " 'Hello world from "
			<< this->processName << "!'" << std::endl;
		std::this_thread::sleep_for(std::chrono::milliseconds(200));
	}

	if (this->isFinished()) {
		this->state = Process::State::TERMINATED;
		this->timestampFinished = getCurrentTimestamp();
		outFile.close();
	}
}