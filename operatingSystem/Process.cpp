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

bool Process::isFinished() const {
	return this->currentLineOfInstruction >= this->totalLineOfInstruction;
}

int Process::getCommandCounter() const {
	return this->currentLineOfInstruction;
}

int Process::getRemainingTime() const {
	return this->totalLineOfInstruction - this->currentLineOfInstruction;
}

void Process::execute() {
	this->state = Process::State::RUNNING;
	this->timestampStarted = getCurrentTimestamp();

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
		Sleep(20);
	}

	outFile.close();
	this->state = Process::State::TERMINATED;

	Sleep(20);
}
