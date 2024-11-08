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
    : AScheduler(SchedulingAlgorithm::RR), timeQuantum(quantum), delayPerExec(delayExec)/*, nCores(nCores)*/ {

    std::cout << "RRScheduler created with quantum: " << quantum << ", delayExec: " << delayExec << ", nCores: " << nCores << std::endl;
	this->nCores = nCores;
    // Initialize memory manager as a persistent instance
    memoryManager = std::make_shared<MemoryManager>();

    // Initialize each core and add it to coreList
    for (int i = 0; i < nCores; i++) {
        coreList.push_back(Core(i));
    }
}

void RRScheduler::executeQuantum(long long timeQuantum) {
    while (GlobalScheduler::getInstance()->isRunning()) {

        // Generate memory report after each quantum cycle
        memoryManager->generateReport(coreList);


        for (int i = 0; i < nCores; i++) {
            Core& core = coreList[i];

            // Check if the core has a process that needs to be preempted or completed
            if (core.process != nullptr && !core.isRunningBool()) {
                if (!core.process->isFinished()) {
                    // Process exceeded its quantum but is not yet finished: preempt it
                    this->activeProcessesList.push_back(core.process);
                    core.process->setCPUCoreID(-1);
                    /*std::cout << "Process " << core.process->getName()
                        << " preempted and returned to active queue.\n";*/
                }
                else {
                    // Process has completed
                    this->terminatedProcessesList.push_back(core.process);
                    //std::cout << "Process " << core.process->getName() << " completed.\n";
                }

                // Release memory for the process, whether it was preempted or completed
                memoryManager->releaseMemoryForProcess(core.process);
                core.process->setMemoryBlockIndex(-1);  // Mark as no memory assigned
                //std::cout << "Memory released for process: " << core.process->getName() << "\n";

                core.process = nullptr;  // Clear the core to allow new process assignment
            }

            // Assign a new process to the core if it's empty and the queue is not empty
            if (core.process == nullptr && !activeProcessesList.empty()) {
                std::shared_ptr<Process> newProcess = this->activeProcessesList.front();

                // Manually specify a starting index for memory allocation for testing purposes
                int framesNeeded = newProcess->getMemorySize() / MemoryManager::MEM_PER_FRAME;
                int manualStartIndex = (i * framesNeeded) % MemoryManager::FRAMES;

                if (memoryManager->allocateMemoryForProcess(newProcess, manualStartIndex)) {  // Attempt to allocate memory
                    newProcess->setMemoryBlockIndex(manualStartIndex);  // Set starting memory block index
                    this->activeProcessesList.erase(this->activeProcessesList.begin());
                    core.setProcess(newProcess);  // Set the new process to the core
                    /*std::cout << "Process " << newProcess->getName()
                        << " assigned to Core " << i
                        << " with starting memory index " << manualStartIndex << "\n";
                    std::cout << "Memory allocated from frame " << manualStartIndex
                        << " to frame " << (manualStartIndex + framesNeeded - 1) << "\n";*/

                    if (core.process) {
                        core.startQuantum(timeQuantum);
                    }
                    else {
                        std::cout << "Error: Process not set to Core " << core.coreID << " successfully.\n";
                    }
                }
                else {
                    /*std::cout << "Unable to schedule process " << newProcess->getName()
                        << " on Core " << i << " due to insufficient memory at index "
                        << manualStartIndex << ".\n";*/
                }
            }
        }

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






//OTHER CODE
/*
RRScheduler::RRScheduler()
    : AScheduler(SchedulingAlgorithm::ROUND_ROBIN) {
}

void RRScheduler::init() {
    quantumCount = 0;
}

void RRScheduler::execute() {
    size_t quantumCycles = GlobalConfig::getInstance()->getQuantumCycles();
    while (running) {
        std::this_thread::sleep_for(std::chrono::milliseconds(3000)); // Simulate waiting or other periodic tasks
        // Process management section
        {
            std::unique_lock<std::mutex> lock(queueMutex);
            // Check for processes in the ready queue
            if (!readyQueue.empty()) {
                auto process = readyQueue.front();
                readyQueue.pop();
                // Try to allocate memory if process doesn't have it
                if (!process->hasMemory()) {
                    if (!process->allocateMemory()) {
                        // If memory allocation failed, put process back at end of queue
                        readyQueue.push(process);
                        lock.unlock();
                        continue;
                    }
                }
                // Update process state to running
                process->setState(Process::RUNNING);
                // Execute for quantum duration
                size_t cycleCount = 0;
                while (cycleCount < quantumCycles && !process->isFinished() && running) {
                    process->executeCurrentCommand();
                    process->moveToNextLine();
                    cycleCount++;
                }
                // Check process completion
                if (process->isFinished()) {
                    process->setState(Process::FINISHED);
                    process->deallocateMemory();
                }
                else {
                    // If not finished, set back to ready and return to queue
                    process->setState(Process::READY);
                    readyQueue.push(process);
                }
            }
        }
        // Generate memory snapshot at end of quantum
        MemoryManager::getInstance()->generateMemorySnapshot(quantumCount++);
        // Simulate quantum timing
        std::this_thread::sleep_for(std::chrono::milliseconds(3000));
    }
    */


