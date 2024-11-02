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