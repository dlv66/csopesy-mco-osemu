#include "RRScheduler.h"
#include "GlobalScheduler.h"
#include <iostream>
#include <thread>
#include <chrono>

RRScheduler::RRScheduler(int quantum, int delayExec, int nCores)
    : AScheduler(SchedulingAlgorithm::ROUND_ROBIN), timeQuantum(quantum), delayPerExec(delayExec) {
    this->nCores = nCores;
    for (int i = 0; i < nCores; ++i) {
        this->coreList.push_back(Core(i));
    }
}

/*
RRScheduler::RRScheduler() : AScheduler(SchedulingAlgorithm::ROUND_ROBIN)
{
	
}
*/

void RRScheduler::execute() {
    while (GlobalScheduler::getInstance()->isRunning()) {
        for (int i = 0; i < nCores; i++) {
            // if the core is idle, assign a new process
            if (coreList[i].process == nullptr) {
                if (coreList[i].terminatedProcess != nullptr) {
                    this->terminatedProcessesList.push_back(coreList[i].terminatedProcess);
                    coreList[i].terminatedProcess = nullptr;
                }

                if (!processQueue.empty()) {
                    std::shared_ptr<Process> process = processQueue.front();
                    processQueue.pop();
                    coreList[i].setProcess(process);
                    coreList[i].process->update();
                    coreList[i].start();
                }
            }

            // simulate round-robin execution
            if (coreList[i].process != nullptr) {
                int executedInstructions = 0;
                while (executedInstructions < timeQuantum) {
                    coreList[i].process->execute();  // execute one instruction
                    executedInstructions++;

                    // simulate delay between instructions
                    // delays-per-exec are not part of the quantum
                    for (int j = 0; j < delayPerExec; ++j) {
                        // 1 CPU cycle is one run through the main loop
                        coreList[i].process->incrementCpuCycles();
                    }

                    if (coreList[i].process->isFinished()) {
                        terminatedProcessesList.push_back(coreList[i].process);
                        coreList[i].update(false);  // mark core as idle
                        break;
                    }

                    coreList[i].process->incrementCpuCycles();  // increment CPU cycle
                }

                // preempt process if not finished and requeue
                if (!coreList[i].process->isFinished()) {
                    processQueue.push(coreList[i].process);
                }

                // mark core as idle after quantum or process completion
                coreList[i].update(false);
                coreList[i].process = nullptr;
            }

            std::this_thread::sleep_for(std::chrono::seconds(1));  // simulate time passage
        }
    }
}

void RRScheduler::run() 
{
    this->execute();
}

void RRScheduler::init() 
{
    
}
