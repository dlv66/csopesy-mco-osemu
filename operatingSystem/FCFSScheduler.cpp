#include "FCFSScheduler.h"
#include <iostream>
#include <thread>
#include <chrono>

FCFSScheduler::FCFSScheduler(int nCores) {
    this->nCores = nCores;
    this->processQueues = std::vector<Process>(nCores);
}

// TODO: Instantiates core list based on given number of cores
// STATUS: For testing
void FCFSScheduler::instantiateCoreList() {
    for(int i = 0; i < nCores; i++) { // for all the cores
        coreList.push_back(Core()); // add core to the list
    }
}

// TODO: Add a process to the queue
// STATUS: For testing
void FCFSScheduler::addProcess(const Process& process) {
    processQueues.push_back(process); // add process to queue
}

/* Commented out for now since no need to sort
// TODO: Sort process based on remaining instructions
// STATUS: Wrong (no sorting needed, just when processes are pushed at the same time. In which case, choose random?)
void FCFSScheduler::sortProcesses() {
    for(int i = 0; i < processQueues.size(); i++) { // for all the processes in the queue
        std::sort(processQueues[i].begin(), processQueues[i].end(), [](const Process& a, const Process& b) {
            return a.totalLineOfInstruction - a.currentLineOfInstruction > b.totalLineOfInstruction - b.currentLineOfInstruction;
        });
    }
}
*/

// TODO: Run scheduler
// STATUS: For testing
// Simulate FCFS Scheduler with threads (1 thread per core)
void FCFSScheduler::runFCFS() {
    std::vector<std::thread> coreThreads;  // Thread for each core
    int currentTime = 0;  // Simulating the current time

    // Iterate over the available cores and run each in a separate thread
    while (!processQueues.empty()) {
        for (int i = 0; i < nCores; i++) {
            if (coreList[i].isBusy == false && !processQueues.empty()) {  // Check if core is free
                Process process = processQueues.front();  // Get the first process in the queue
                
                // Check if process has arrived
                if (currentTime >= process.arrivalTime) {
                    if (coreList[i].setProcess(process)) {
                        processQueues.erase(processQueues.begin());  // Remove process from the queue

                        // Create a new thread to start the process on the core
                        coreThreads.push_back(std::thread([this, i]() {
                            coreList[i].startProcess();
                        }));

                        // Increment the simulated time
                        currentTime += process.burstTime;
                    }
                }
            }
        }

        // Simulate time passing (e.g., 1-second steps)
        std::this_thread::sleep_for(std::chrono::seconds(1));
        currentTime++;
    }

    // Join all the threads to ensure they complete
    for (std::thread &t : coreThreads) {
        if (t.joinable()) {
            t.join();
        }
    }
}