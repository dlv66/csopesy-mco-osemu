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
    this->processQueues = std::vector<std::shared_ptr<Process>>(nCores);
    
    for (int i = 0; i < nCores; i++) { // for all the cores
        coreList.push_back(Core(i)); // add core to the list
    }
}

// Adds a process to the queue
void FCFSScheduler::addProcess(std::shared_ptr<Process> process) {
	processQueues.push_back(process);
}

// Runs the actual scheduler
void FCFSScheduler::run() {
    std::vector<std::thread> coreThreads;  // Thread for each core
    int currentTime = 0;  // Simulating the current time

    // Iterate over the available cores and run each in a separate thread
    while (GlobalScheduler::getInstance()->activeProcessesTable.size() != 0) {
        for (int i = 0; i < nCores; i++) {
            if (coreList[i].process == nullptr) { // if the current core is empty/finished

                std::shared_ptr<Process> terminatedProcess = coreList[i].process; // get the old finished process from the core
                terminatedProcess->timestampFinished = getCurrentTimestamp();

            	GlobalScheduler::getInstance()->terminatedProcessesTable[terminatedProcess->getName()] = terminatedProcess;
                coreList[i].process = nullptr; // set the core to empty

                if (!processQueues.empty()) { // if there are processes in the waiting queue
                    std::shared_ptr<Process> process = processQueues.front();  // Get the process from in front of the queue

                    if (coreList[i].process == nullptr) // Double-check if core is empty
                    {
                        std::shared_ptr<Process> process_2 = coreList[i].setProcess(process); // set the new process to the core
                        processQueues.erase(processQueues.begin()); // remove the new process from the waiting queue

                        coreThreads.push_back(std::thread([this, i]() {
                            coreList[i].process->setState(Process::State::READY);
                            coreList[i].run();
                        }));
                    }
                }
            }

            // Simulate time passing (e.g., 1-second steps)
            std::this_thread::sleep_for(std::chrono::seconds(1));
        }

        // Join all the threads to ensure they complete
        for (std::thread& t : coreThreads) {
            if (t.joinable()) {
                t.detach();
            }
        }
    }
}
