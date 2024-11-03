#include "Core.h"

#include <iostream>
#include <memory>
#include <ostream>
#include <chrono>
#include <thread>
#include <Windows.h>

#include "GlobalScheduler.h"

//Constructor
Core::Core(int coreID): IThread(){
	this->coreID = coreID;
}

void Core::update(bool isRunning)
{
	this->isRunning = isRunning;
}

void Core::tick()
{
	this->cpuCycles++;
}

int Core::getCpuCycles() const
{
	return this->cpuCycles;
}


// Sets the new process to the core and returns the terminated process
void Core::setProcess(std::shared_ptr<Process> process) {
	if (!this->isRunning) {
		this->process = process;
		this->process->setCPUCoreID(this->coreID);
		this->update(true);
	}
}

bool Core::isRunningBool() const {
	return this->isRunning;
}

// FOR FCFSScheduler
void Core::run() {

	while(this->process != nullptr) // changed from while(true)
	{
		if(!this->process->isFinished())
		{
			this->process->execute();
		}
		else
		{
			this->update(false);
			this->terminatedProcess = this->process;
			this->process = nullptr;
			break;
			
		}

		this->process->update();
	}
}



// FOR RRScheduler
void Core::runQuantum(long long timeQuantum) {
	long long executedTime = 0; // overall time executed

	while (this->process != nullptr) {

		// if the process is NOT FINISHED
		if (!this->process->isFinished()) {

			// Execute the process for the time quantum or until it finishes
			long long remainingTime = this->process->getRemainingTime();
			long long timeToExecute = timeQuantum;

			// Adjust timeToExecute if remaining time is less than time quantum
			if (remainingTime < timeQuantum) {
				timeToExecute = remainingTime;
			}

			// Execute the process for the time quantum or until it finishes
			this->process->executeQuantum(timeToExecute);
			executedTime += timeToExecute;

			if (this->process && this->process->isFinished()) {
				this->terminatedProcess = this->process;
				std::cout << "[DEBUG] 1:" << this->terminatedProcess->getName() << std::endl;
				this->update(false);
				this->process = nullptr;
				break;
			} else if (!this->process)
			{
				std::cout << "Process is null" << std::endl;
			}

			// Check if the process needs preemption
			if (executedTime >= timeQuantum) {
				// std::cout << "Preempting process: " << this->process->getName() << std::endl;

				
				// NOTE: THE 6 LINES OF CODE BELOW THIS COMMENT ARE BREAKING THE ENTIRE PROGRAM
				this->process->update(); // To note preemption
				this->update(false); // Core is now available

				executedTime = 0;
				Sleep(50);
				break;
			
			}
		}

		// if the process IS ALREADY FINISHED
		else {
			this->terminatedProcess = this->process;
			std::cout << "[DEBUG] 2:" << this->terminatedProcess->getName() << std::endl;
			this->update(false);
			this->process = nullptr;
			break; // Exit if process finished
		}
		this->process->update();
	}
}