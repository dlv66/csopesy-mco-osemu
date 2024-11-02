#include "Core.h"

#include <iostream>
#include <memory>
#include <ostream>
#include <chrono>
#include <thread>
#include <Windows.h>

//Constructor
Core::Core(int coreID): IThread(){
	this->coreID = coreID;
}

void Core::update(bool isRunning)
{
	this->isRunning = isRunning;
}


// Sets the new process to the core and returns the terminated process
void Core::setProcess(std::shared_ptr<Process> process) {
	if (!this->isRunning) {
		this->process = process;
		this->process->setCPUCoreID(this->coreID);
		this->update(true);
	}
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
void Core::runQuantum(int timeQuantum) {

	// while this core has a process I THINK THIS IS THE PROBLEMATIC PART OF THE CODE.
	while (this->process != nullptr) {

		// if the process is NOT FINISHED
		if (this->process != nullptr && !this->process->isFinished()) {

			// plainly execute the process instructions; assume it will execute until the number set for timeQuantum
			this->process->executeQuantum(timeQuantum);
			
			// if the current process in the core is FOR PREEMPTION
			if (this->process->forPreemption(timeQuantum)) {
				std::cout << "umaabot dito"; // THIS IS ACTUALLY WORKING

				// LOGIC HERE NEEDS TO CHANGE
				this->process->update(); // TO NOTE PREEMPTION
				this->process = nullptr;
				this->update(false); // Core is now available
				std::this_thread::sleep_for(std::chrono::milliseconds(10));
				break;
			}
		}

		// if the process IS ALREADY FINISHED
		else {
			this->update(false);
			this->terminatedProcess = this->process;
			this->process = nullptr;
			break; // Exit if process finished
		}
		this->process->update();
	}
}