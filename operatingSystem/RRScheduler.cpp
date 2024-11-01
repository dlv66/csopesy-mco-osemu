#include "RRScheduler.h"
#include "GlobalScheduler.h"
#include <iostream>
#include <thread>
#include <chrono>
#include <mutex>
#include <vector>

// Constructor for RRScheduler class, initializes the Round Robin Scheduler with specified quantum, delay, and core count.
RRScheduler::RRScheduler(int quantum, int delayExec, int nCores)
    : AScheduler(SchedulingAlgorithm::ROUND_ROBIN), timeQuantum(quantum), delayPerExec(delayExec), lastAssignedCore(-1) {
    // Output information about the created scheduler
    std::cout << "RRScheduler created with quantum: " << quantum
        << ", delayExec: " << delayExec << ", nCores: " << nCores << std::endl;
    this->nCores = nCores;

    // Initialize core list for each available core
    for (int i = 0; i < nCores; ++i) {
        coreList.push_back(Core(i));
    }
}

// Adds a new process to the scheduling queue and active processes list
void RRScheduler::addProcess(std::shared_ptr<Process> process) {
    std::lock_guard<std::mutex> lock(queueMutex);
    processQueue.push(process);
    activeProcessesList.push_back(process);
}

// Finds the next available core using a round-robin method to balance assignments across cores
int RRScheduler::findNextAvailableCore() {
    for (int i = 0; i < nCores; ++i) {
        int coreIndex = (lastAssignedCore + 1 + i) % nCores; // Round-robin calculation
        if (coreList[coreIndex].process == nullptr) { // Check if the core is idle
            lastAssignedCore = coreIndex;
            return coreIndex;
        }
    }
    return -1; // Return -1 if no available core is found
}

// Finds the core with the lowest ID that is currently idle
int RRScheduler::findLowestIdleCore() {
    for (int i = 0; i < nCores; ++i) {
        if (coreList[i].process == nullptr) {
            return i;
        }
    }
    return -1; // Return -1 if no idle core is available
}

// Core thread function to continuously execute processes assigned to a specific core
void RRScheduler::runCore(int coreID) {
    Core& core = coreList[coreID];
    while (GlobalScheduler::getInstance()->isRunning()) {
        std::shared_ptr<Process> process;

        {
            std::lock_guard<std::mutex> lock(queueMutex);
            // Assign a new process to core if idle
            if (core.process == nullptr && !processQueue.empty()) {
                process = processQueue.front();
                processQueue.pop();
                core.setProcess(process);
                process->setQuantumCounter(timeQuantum);
                process->setCPUCoreID(coreID);
                process->update(); // Update process state to RUNNING
                std::cout << "[INFO] Process " << process->getName() << " assigned to Core " << coreID << "\n";
            }
        }

        // Execute the assigned process if any
        if (core.process) {
            process = core.process;

            // Perform one step of execution if quantum and steps remain
            if (process->getQuantumCounter() > 0 && !process->isFinished()) {
                process->executeOneStep();
                process->decrementQuantumCounter();
                std::cout << "[INFO] Core " << coreID << " executed one step for process: " << process->getName()
                    << ". Remaining steps: " << process->getRemainingSteps() << "\n";
            }

            {
                std::lock_guard<std::mutex> lock(queueMutex);
                // Terminate or re-queue process based on completion or quantum expiration
                if (process->isFinished()) {
                    terminatedProcessesList.push_back(process);
                    core.process = nullptr;
                    process->update(); // Set state to TERMINATED
                    std::cout << "[INFO] Terminating Process: " << process->getName() << "\n";
                }
                else if (process->getQuantumCounter() == 0) {
                    if (processQueue.empty()) {
                        process->setQuantumCounter(timeQuantum);
                        process->update(); // Reset quantum and remain RUNNING
                        std::cout << "[INFO] Retaining Process on Core " << coreID << ": " << process->getName() << "\n";
                    }
                    else {
                        processQueue.push(process);
                        core.process = nullptr;
                        process->setCPUCoreID(-1);
                        process->update(); // Update state to READY for re-queuing
                        std::cout << "[INFO] Re-queuing Process: " << process->getName() << "\n";
                    }
                    printQueue(); // For debugging queue state
                }
            }
        }
        // Simulate execution delay
        std::this_thread::sleep_for(std::chrono::milliseconds(delayPerExec));
    }
}

// Starts the execution of each core in separate threads
void RRScheduler::execute() {
    std::vector<std::thread> coreThreads;
    for (int i = 0; i < nCores; ++i) {
        coreThreads.emplace_back(&RRScheduler::runCore, this, i);
    }
    // Wait for all threads to complete
    for (auto& thread : coreThreads) {
        if (thread.joinable()) {
            thread.join();
        }
    }
}

// Prints the current queue of processes for debugging purposes
void RRScheduler::printQueue() const {
    std::queue<std::shared_ptr<Process>> tempQueue = processQueue;
    std::cout << "[DEBUG] Queue Status: ";
    while (!tempQueue.empty()) {
        auto p = tempQueue.front();
        std::cout << p->getName() << " ";
        tempQueue.pop();
    }
    std::cout << "\n";
}

// Starts the scheduler
void RRScheduler::run() {
    execute();
}


void RRScheduler::init() {
}
