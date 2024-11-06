#include "RRScheduler.h"
#include "Process.h"        
#include "MemoryManager.h" 
#include "Utils.h"
#include <iostream>
#include <thread>
#include <chrono>
#include <mutex>
#include "GlobalScheduler.h"

// Constructor, function definitions, and logic remain the same

// Constructor with MemoryManager initialization
RRScheduler::RRScheduler(long long quantum, long long delayExec, int nCores)
    : AScheduler(SchedulingAlgorithm::RR), timeQuantum(quantum), delayPerExec(delayExec), nCores(nCores) {

    std::cout << "RRScheduler created with quantum: " << quantum << ", delayExec: " << delayExec << ", nCores: " << nCores << std::endl;

    // Initialize memory manager as a persistent instance
    memoryManager = std::make_shared<MemoryManager>();

    // Initialize each core and add it to coreList
    for (int i = 0; i < nCores; i++) {
        coreList.push_back(Core(i));
    }
}

void RRScheduler::executeQuantum(long long timeQuantum) {
    while (GlobalScheduler::getInstance()->isRunning()) {
        // Iterate over each core and manage process scheduling and memory
        for (int i = 0; i < nCores; i++) {
            Core& core = coreList[i];

            // Release memory for terminated processes
            if (core.terminatedProcess != nullptr) {
                memoryManager->releaseMemoryForProcess(core.terminatedProcess);
                terminatedProcessesList.push_back(core.terminatedProcess);
                core.terminatedProcess = nullptr;
                std::cout << "Memory released for terminated process.\n";
            }

            // Check if the core has a process that needs to be preempted or completed
            if (core.process != nullptr) {
                // If the process on this core has finished or its quantum is over
                if (!core.isRunningBool()) {
                    if (!core.process->isFinished()) {
                        // Process exceeded its quantum but is not yet finished: preempt it
                        activeProcessesList.push_back(core.process);
                        core.process->setCPUCoreID(-1);
                        std::cout << "Process " << core.process->getName()
                            << " preempted and returned to active queue.\n";
                    }
                    core.process = nullptr;  // Clear the core to allow new process assignment
                }
            }

            // Assign a new process to the core if it's empty and the queue is not empty
            if (core.process == nullptr && !activeProcessesList.empty()) {
                std::shared_ptr<Process> newProcess = activeProcessesList.front();

                // Manually specify a starting index for memory allocation for testing purposes
                int manualStartIndex = (i * (MemoryManager::MEM_PER_PROC / MemoryManager::MEM_PER_FRAME)) % MemoryManager::FRAMES;  // calculation
                if (memoryManager->allocateMemoryForProcess(newProcess, manualStartIndex)) {  // Attempt to allocate memory
                    activeProcessesList.erase(activeProcessesList.begin());
                    core.setProcess(newProcess);  // Set the new process to the core
                    std::cout << "Process " << newProcess->getName()
                        << " assigned to Core " << i << " with starting memory index " << manualStartIndex << "\n";

                    if (core.process) {
                        core.startQuantum(timeQuantum);
                    }
                    else {
                        std::cout << "Error: Process not set to Core " << core.coreID << " successfully.\n";
                    }
                }
                else {
                    std::cout << "Unable to schedule process " << newProcess->getName()
                        << " on Core " << i << " due to insufficient memory at index " << manualStartIndex << ".\n";
                }
            }
        }

        // Generate memory report after each quantum cycle
        memoryManager->generateReport(activeProcessesList);

        // Simulate the passage of one quantum cycle
        std::this_thread::sleep_for(std::chrono::milliseconds(950));
    }
}


void RRScheduler::init() {
    std::cout << "RRScheduler initialized." << std::endl;
}

void RRScheduler::run() {
    this->executeQuantum(timeQuantum);
}

void RRScheduler::runQuantum(long long timeQuantum) {
    this->executeQuantum(timeQuantum);
}

void RRScheduler::execute() {

}
