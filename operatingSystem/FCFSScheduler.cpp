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
        coreList.push_back(Core(i)); // add core to the list
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
            if (coreList[i].isBusy == false && !processQueues.empty()) {  // Check if core is free
                Process process = processQueues.front();  // Get the first process in the queue

                // Check if process has arrived
                if (currentTime >= process.arrivalTime) {
                    if (!coreList[i].isBusy)
                    {
                        Process terminatedProcess = coreList[i].setProcess(process); // set Core to new process, return the old process
                        processQueues.erase(processQueues.begin()); // remove the new process from the waiting queue

                        terminatedProcess.state = Process::State::TERMINATED; // set the state of the old process to 'TERMINATED'

                        if(terminatedProcess.processName != "Unnamed Process"){
                            terminatedProcesses.push_back(terminatedProcess); // add the old process to the terminatedProcesses list
                        }
                        //terminatedProcesses.push_back(terminatedProcess); // add the old process to the terminatedProcesses list

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
}

void FCFSScheduler::printActiveProcesses() {

    for (auto core : coreList)
    {
        /*
		if (!core.isBusy)
		{
			std::cout << "Core " << core.coreID << core.process.processName << "\n";
		}
		else if (core.isBusy)
		{
			std::cout << core.process.processName << "     (" << core.process.executeTime << ")     Core: " << core.coreID << "     " << core.process.currentLineOfInstruction << " / " << core.process.totalLineOfInstruction << "\n";
		}
        */
       if(core.process.processName == "Unnamed Process"){
            std::cout << "NO PROCESS     (NO TIME)     Core: " << core.coreID << "\n";
       } else {
            std::cout << core.process.processName << "     (" << core.process.executeTimet << ")     Core: " << core.coreID << "     " << core.process.currentLineOfInstruction << " / " << core.process.totalLineOfInstruction << "\n";
       }
    }
}

void FCFSScheduler::printTerminatedProcesses() {
    for (int i = 0; i < terminatedProcesses.size(); i++) {
        std::cout << terminatedProcesses[i].processName << "\n";
    }
}