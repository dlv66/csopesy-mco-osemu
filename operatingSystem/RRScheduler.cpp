#include "RRScheduler.h"
#include "GlobalScheduler.h"
#include <iostream>
#include <thread>
#include <chrono>
#include <mutex>

RRScheduler::RRScheduler(int quantum, int delayExec, int nCores)
    : AScheduler(SchedulingAlgorithm::RR), timeQuantum(quantum), delayPerExec(delayExec) {
    std::cout << "RRScheduler created with quantum: " << quantum
        << ", delayExec: " << delayExec << ", nCores: " << nCores << std::endl;
    this->nCores = nCores;
    for (int i = 0; i < nCores; ++i) {
        this->coreList.push_back(Core(i));
    }
}

void RRScheduler::addProcessRR(std::shared_ptr<Process> process) {
    //std::cout << "Adding process " << process->getName() << " to the Round-Robin queue.\n";
    processQueue.push(process);
    this->activeProcessesList.push_back(process);
}

/*
RRScheduler::RRScheduler() : AScheduler(SchedulingAlgorithm::ROUND_ROBIN)
{

}
*/

std::mutex queueMutex;  // Mutex for synchronizing access to the queue

void RRScheduler::execute() {
    while (GlobalScheduler::getInstance()->isRunning()) {
        for (int i = 0; i < nCores; i++) {
            if (coreList[i].process == nullptr && !processQueue.empty()) {
                std::lock_guard<std::mutex> lock(queueMutex);
                if (!processQueue.empty()) {
                    std::shared_ptr<Process> process = processQueue.front();
                    processQueue.pop();
                    coreList[i].setProcess(process);
                    coreList[i].process->setCPUCoreID(i); // assign coreID to the process
                    std::cout << "Process " << process->getName() << " assigned to Core " << i << "\n";

                    // thread to run the process
                    std::thread([this, i, process]() {
                        process->execute();
                        std::lock_guard<std::mutex> lock(queueMutex); // lock when accessing shared resources
                        if (process->isFinished()) {
                            terminatedProcessesList.push_back(process);
                            std::cout << "Terminating Process: " << process->getName() << "\n";
                        }
                        coreList[i].process = nullptr; // clear process from core
                        }).detach(); // detach thread for independent execution
                }
            }
        }

        // sleep
        std::this_thread::sleep_for(std::chrono::milliseconds(10));
    }
}




void RRScheduler::run()
{
    this->execute();
}

void RRScheduler::init()
{

}
