#include "FCFSScheduler.h"
#include "Utils.h"
#include <iostream>
#include <thread>
#include <chrono>

#include "GlobalScheduler.h"

FCFSScheduler::FCFSScheduler(int nCores) :
	AScheduler(SchedulingAlgorithm::FCFS, -1, FCFS_SCHEDULER_NAME),
	IThread()
{
    this->nCores = nCores;
    
    for (int i = 0; i < nCores; i++) { // for all the cores
        coreList.push_back(Core(i)); // add core to the list
    }
}

void FCFSScheduler::execute()
{
	
}

void FCFSScheduler::init()
{

}

// Adds a process to the queue
void FCFSScheduler::addProcess(std::shared_ptr<Process> process) {
    this->activeProcessesList.push_back(process);
}

// Runs the actual scheduler
void FCFSScheduler::run() {
    std::vector<std::thread> coreThreads;  // Thread for each core
    int currentTime = 0;  // Simulating the current time

    // Iterate over the available cores and run each in a separate thread
    while (GlobalScheduler::getInstance()->isRunning()) {
        for (int i = 0; i < nCores; i++)
{
            // if the current core is empty/finished
            if (coreList[i].process == nullptr) {

                // get the old finished process from the core
                std::shared_ptr<Process> terminatedProcess = coreList[i].process;
                // update the state of the process to terminated
				terminatedProcess->update(); 

            	this->terminatedProcessesList.push_back(terminatedProcess);
                coreList[i].process = nullptr; // set the core to empty

                // if there are processes in the waiting queue
                if (!this->activeProcessesList.empty()) {
                    std::shared_ptr<Process> process = this->activeProcessesList.front();  // Get the process from in front of the queue

                    if (coreList[i].process == nullptr) // Double-check if core is empty
                    {
                        std::shared_ptr<Process> process_2 = coreList[i].setProcess(process); // set the new process to the core
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
