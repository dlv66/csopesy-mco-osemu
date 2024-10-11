#include "FCFSScheduler.h"
#include <iostream>
#include <thread>
#include <chrono>

FCFSScheduler::FCFSScheduler(int nCores) {
    this->nCores = nCores;
    this->processQueues = std::vector<Process>();
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

// TODO: Run scheduler
// STATUS: For testing
// Simulate FCFS Scheduler with threads (1 thread per core)
void FCFSScheduler::runFCFS() {
    std::vector<std::thread> coreThreads;  // Thread for each core
    int currentTime = 0;  // Simulating the current time

    // Iterate over the available cores and run each in a separate thread
    while (!processQueues.empty()) {
        for (int i = 0; i < nCores; i++) {
            if (i >= coreList.size()) {
                std::cout << "Core index out of bounds: " << i << "\n";
                continue; // Skip this iteration
            }

            if (!coreList[i].isBusy && !processQueues.empty()) {  // Check if core is free
                Process process = processQueues.front();  // Get the first process in the queue

                // Check if process has arrived
                if (currentTime >= process.arrivalTime) {
                    if(!coreList[i].isBusy)
                    {
						Process terminatedProcess = coreList[i].setProcess(process); // set Core to new process, return the old process
                        processQueues.erase(processQueues.begin()); // remove the new process from the waiting queue

						terminatedProcess.state = Process::State::TERMINATED; // set the state of the old process to 'TERMINATED'
                        terminatedProcesses.push_back(terminatedProcess); // add the old process to the terminatedProcesses list
                        
                        coreThreads.push_back(std::thread([this, i]() {
                            coreList[i].process.state = Process::State::READY;
                            coreList[i].startProcess();

                        }));

                    // Increment the simulated time
                    currentTime += process.burstTime;
                }
            }
        }

        // Simulate time passing (e.g., 1-second steps)
        std::this_thread::sleep_for(std::chrono::seconds(1));
        currentTime++;
    }

    // Join all the threads to ensure they complete
    for (std::thread& t : coreThreads) {
        if (t.joinable()) {
            t.join();
        }
    }
}


void FCFSScheduler::printActiveProcesses() {
    for (int i = 0; i < coreList.size(); i++) {
        if (coreList[i].isBusy) {
            activeProcesses[i] = coreList[i].process;
        } else {
            activeProcesses[i] = Process(); // should be N/A
        }
    }
    for (int i = 0; i < activeProcesses.size(); i++) {
        std::cout << activeProcesses[i].processName << "\n";
    }
}

void FCFSScheduler::printTerminatedProcesses() {
    for (int i = 0; i < terminatedProcesses.size(); i++) {
        std::cout << terminatedProcesses[i].processName << "\n";
    }
}

