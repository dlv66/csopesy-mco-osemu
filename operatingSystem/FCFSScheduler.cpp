#include "FCFSScheduler.h"
#include "Utils.h"
#include <iostream>
#include <thread>
#include <chrono>

#include "GlobalScheduler.h"

FCFSScheduler::FCFSScheduler(int nCores) :
	AScheduler(SchedulingAlgorithm::FCFS),
	IThread()
{
    this->nCores = nCores;
    
    for (int i = 0; i < nCores; i++) { // for all the cores
        coreList.push_back(Core(i)); // add core to the list
    }
}

void FCFSScheduler::execute()
{
	int currentTime = 0;  // Simulating the current time

    // Iterate over the available cores and run each in a separate thread
    while (GlobalScheduler::getInstance()->isRunning()) {
        for (int i = 0; i < nCores; i++)
        {
            // if the current core is empty/finished
            if (coreList[i].process == nullptr) {

				if (coreList[i].terminatedProcess != nullptr)
				{
                    this->terminatedProcessesList.push_back(coreList[i].terminatedProcess);
					coreList[i].terminatedProcess = nullptr;
				}

                // if there are processes in the waiting queue
                if (!this->activeProcessesList.empty()) {
                    std::shared_ptr<Process> process = this->activeProcessesList.front();  // Get the process from in front of the queue

                    if (coreList[i].process == nullptr) // Double-check if core is empty
                    {
                        coreList[i].setProcess(process); // set the new process to the core
                        this->activeProcessesList.erase(this->activeProcessesList.begin()); // remove the new process from the waiting queue
                        coreList[i].process->update();
                        coreList[i].start();
                    }
                }
            }

            // Simulate time passing (e.g., 1-second steps)
            std::this_thread::sleep_for(std::chrono::seconds(1));
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
