#include "RRScheduler.h"
#include "Utils.h"
#include <iostream>
#include <thread>
#include <chrono>
#include <mutex>

#include "GlobalScheduler.h"

RRScheduler::RRScheduler(int quantum, int delayExec, int nCores) : 
    AScheduler(SchedulingAlgorithm::RR), timeQuantum(quantum), delayPerExec(delayExec), IThread()
{
    std::cout << "RRScheduler created with quantum: " << quantum << ", delayExec: " << delayExec << ", nCores: " << nCores << std::endl;

    this->nCores = nCores;
    this->timeQuantum = quantum;
    this->delayPerExec = delayExec;

    for (int i = 0; i < nCores; i++) { // for all the cores
        coreList.push_back(Core(i)); // add core to the list
    }
}



/*
void RRScheduler::executeQuantum(int timeQuantum) {
    int currentTime = 0;

    while (GlobalScheduler::getInstance()->isRunning()) {
        for (int i = 0; i < nCores; i++) {

            // If the current core is empty/finished
            if (coreList[i].process == nullptr) {

                // if the core has a terminated process
                if (coreList[i].terminatedProcess != nullptr) {
                    this->terminatedProcessesList.push_back(coreList[i].terminatedProcess);
                    coreList[i].terminatedProcess = nullptr;
                }

                // if there are processes still in the waiting queue
                if (!this->activeProcessesList.empty()) {
                    std::shared_ptr<Process> newProcess = this->activeProcessesList.front(); // Get the process from in front of the queue (DOES NOT POP, JUST RETURNS)

                    // NEW PROCESS INTO CORE
                    coreList[i].setProcess(newProcess);
                    this->activeProcessesList.erase(this->activeProcessesList.begin());
                    coreList[i].startQuantum(timeQuantum);
                }
            }
            
            // Else if the current core STILL HAS A PROCESS
            else {
                if (!this->activeProcessesList.empty()) {
                    std::shared_ptr<Process> newProcess = this->activeProcessesList.front(); // Get the process from in front of the queue (DOES NOT POP, JUST RETURNS)

                    // if the core's current process is finished executing OR if the current process is ready for preemption
                    if (coreList[i].process->isFinished() || coreList[i].process->forPreemption(timeQuantum)) {

                        // OPTIONAL PART OF PREEMPTION - CURRENT PROCESS PREEMPTION
                        if (coreList[i].process->forPreemption(timeQuantum) && coreList[i].process != nullptr) {
                            coreList[i].process->resetTicksLineOfInstruction(); // Reset the ticks for the current process
                            coreList[i].process->update(); // To change state into PREEMPTED
                            this->activeProcessesList.push_back(coreList[i].process); // Move the current process to the back of the queue
                            // std::cout << "THIS IS HAPPENING";
                        }
                        // PREEMPTION PT.2 - NEW PROCESS INTO CORE
                        coreList[i].setProcess(newProcess);
                        this->activeProcessesList.erase(this->activeProcessesList.begin());
                        coreList[i].startQuantum(timeQuantum);
                    }

                }
            }

            // Simulate time passing (e.g., 1-second steps)
            std::this_thread::sleep_for(std::chrono::seconds(1));
        }
    }
}
*/

void RRScheduler::executeQuantum(int timeQuantum) {
    while (GlobalScheduler::getInstance()->isRunning()) {
        for (int i = 0; i < nCores; i++) {
            Core& core = coreList[i];

            // Handle terminated processes
            if (core.terminatedProcess != nullptr) {
                terminatedProcessesList.push_back(core.terminatedProcess);
                core.terminatedProcess = nullptr;
            }

            // Check for preemption: process has exceeded its quantum
            if (core.process != nullptr) {
                core.runQuantum(timeQuantum); // Let the core handle the quantum execution and preemption

                // If the core is now free, it means the process was preempted or finished
                if (!core.isRunningBool()) {
                    if (core.process != nullptr && !core.process->isFinished()) {
                        // Move preempted process back to the queue
                        activeProcessesList.push_back(core.process);
                        core.process->setCPUCoreID(-1);
                        core.process = nullptr;
                    }
                }
            }

            // Assign a new process to the core if it's empty and the queue is not empty
            if (core.process == nullptr && !activeProcessesList.empty()) {
                std::shared_ptr<Process> newProcess = this->activeProcessesList.front();
                this->activeProcessesList.erase(this->activeProcessesList.begin());
                core.setProcess(newProcess);
                core.startQuantum(timeQuantum);
            }

        }

        // Wait a short time to simulate time passing (adjust if needed)
        std::this_thread::sleep_for(std::chrono::milliseconds(100));
    }
}



/*
                        // TODO: Issue with update/start in Core.cpp or Process.cpp. Makes the process complete execution from start to finish, without any regard for potential preemption
                            // in essence, it's currently coded for FCFS only, not RR AND FCFS
                        coreList[i].process->update();                                      // update the status of the process
                        coreList[i].start();                                                // start the process execution



                        // thread to run the process?
                        std::thread([this, i, newProcess]() {
                            newProcess->execute();
                            // std::lock_guard<std::mutex> lock(queueMutex); // lock when accessing shared resources
                            if (newProcess->isFinished()) {
                                terminatedProcessesList.push_back(newProcess);
                                std::cout << "Terminating Process: " << newProcess->getName() << "\n";
                            }
                            coreList[i].process = nullptr; // clear process from core
                        }).detach(); // detach thread for independent execution
                    }
    
                }

            }

            // Simulate time passing (e.g., 1-second steps)
            std::this_thread::sleep_for(std::chrono::seconds(1));

        }

    }
}
*/



void RRScheduler::init()
{
    std::cout << "RRScheduler initialized." << std::endl;
}

// Runs the actual scheduler
void RRScheduler::run()
{
    this->executeQuantum(timeQuantum);
}

// Runs the actual scheduler, but with Quantum
void RRScheduler::runQuantum(int timeQuantum)
{
    this->executeQuantum(timeQuantum);
}

void RRScheduler::execute()
{

}