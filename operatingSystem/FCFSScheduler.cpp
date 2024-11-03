#include "FCFSScheduler.h"
#include "Utils.h"
#include <iostream>
#include <thread>
#include <chrono>

#include "GlobalScheduler.h"

FCFSScheduler::FCFSScheduler(int nCores, long long delayPerExec) :
	AScheduler(SchedulingAlgorithm::FCFS),
	IThread()
{
    this->nCores = nCores;
	this->delayPerExec = delayPerExec;
    
    for (int i = 0; i < nCores; i++) { // for all the cores
        coreList.push_back(Core(i)); // add core to the list
    }
}

void FCFSScheduler::execute()
{
	int currentTime = 0;  // Simulating the current time

    // Iterate over the available cores and run each in a separate thread
    while (GlobalScheduler::getInstance()->isRunning()) {

        // check for terminated processes
        for (int i = 0; i < nCores; i++) {
            if (this->coreList[i].terminatedProcess != nullptr) {
                this->terminatedProcessesList.push_back(this->coreList[i].terminatedProcess);
                this->coreList[i].terminatedProcess = nullptr;
            }
        }

		// If there are processes in the waiting queue
		if (!this->activeProcessesList.empty()) {
            std::shared_ptr<Process> process = this->activeProcessesList.front();
            for (int i = 0; i < nCores; i++)
            {
                // if the current core is empty/finished
                if (this->coreList[i].process == nullptr) {

                    this->coreList[i].setProcess(process); // set the new process to the core
                    this->activeProcessesList.erase(this->activeProcessesList.begin()); // remove the new process from the waiting queue
                    this->coreList[i].process->update();
                    this->coreList[i].start();

                    break;
                }

                // Simulate time passing (e.g., 1-second steps)
                Sleep(50);
            }
		}

        
    }
}

void FCFSScheduler::init()
{
    std::cout << "FCFSScheduler initialized." << std::endl;
}

// Runs the actual scheduler
void FCFSScheduler::run()
{
	this->execute();
}

void FCFSScheduler::runQuantum(long long timeQuantum) {
	// Not used in FCFS
}

void FCFSScheduler::executeQuantum(long long timeQuantum) {

}