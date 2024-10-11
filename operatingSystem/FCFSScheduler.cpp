#include "FCFSScheduler.h"
#include "Utils.h"
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

void FCFSScheduler::addProcess(const Process& process) {
	processQueues.push_back(process);
}

void FCFSScheduler::runFCFS() {
    std::vector<std::thread> coreThreads;  // Thread for each core
    int currentTime = 0;  // Simulating the current time

    // Iterate over the available cores and run each in a separate thread
    while (terminatedProcesses.size() != 10) {
        for (int i = 0; i < nCores; i++) {
            if (coreList[i].isBusy == false) {
                Process terminatedProcess = coreList[i].process;
                if (terminatedProcess.processName != "Unnamed Process" && terminatedProcess.processName != "EMPTY") {
                    terminatedProcess.finishTimet = getCurrentTimestampString();
                    terminatedProcesses.push_back(terminatedProcess); // add the old process to the terminatedProcesses list
                    coreList[i].process.processName = "EMPTY";
                }

                if (!processQueues.empty()) {
                    Process process = processQueues.front();  // Get the first process in the queue

                    // Check if process has arrived
                    if (currentTime >= process.arrivalTime) {
                        if (!coreList[i].isBusy)
                        {

                            Process process_2 = coreList[i].setProcess(process);
                            processQueues.erase(processQueues.begin()); // remove the new process from the waiting queue


                            coreThreads.push_back(std::thread([this, i]() {
                                coreList[i].process.state = Process::State::READY;
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
       if(core.process.processName == "Unnamed Process"){
            std::cout << "NO PROCESS     (NO TIME)     Core: " << core.coreID << "\n";
       } else {
            std::cout << core.process.processName << "     (" << getCurrentTimestampString() << ")     Core: " << core.coreID << "     " << core.process.currentLineOfInstruction << " / " << core.process.totalLineOfInstruction << "\n";
       }
    }
}

void FCFSScheduler::printTerminatedProcesses() {
    for (int i = 0; i < terminatedProcesses.size(); i++) {
        std::cout << terminatedProcesses[i].processName << "     (" << terminatedProcesses[i].finishTimet << ")     FINISHED     " << terminatedProcesses[i].currentLineOfInstruction << " / " << terminatedProcesses[i].totalLineOfInstruction << "\n";
    }
}