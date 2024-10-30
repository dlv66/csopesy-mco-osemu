#include "GlobalScheduler.h"
#include "Process.h"
#include <iostream>
#include <thread>
#include <chrono>
#include <iomanip>
#include "RRScheduler.h"

GlobalScheduler* GlobalScheduler::sharedInstance = nullptr;

GlobalScheduler::GlobalScheduler() {
    this->running = true;
    const std::shared_ptr<RRScheduler> rrScheduler = std::make_shared<RRScheduler>(5, 0, 4);
    this->schedulerTable[RR_SCHEDULER_NAME] = rrScheduler;
    this->scheduler = rrScheduler;
    std::cout << "GlobalScheduler initialized with Round-Robin Scheduler.\n";
}

GlobalScheduler* GlobalScheduler::getInstance() {
    return sharedInstance;
}

void GlobalScheduler::initialize() {
    sharedInstance = new GlobalScheduler();
}

void GlobalScheduler::destroy() {
    delete sharedInstance;
}

void GlobalScheduler::run() {
    std::cout << "GlobalScheduler is running.\n";
    if (scheduler) {
        std::cout << "Calling scheduler->run() on " << (dynamic_cast<RRScheduler*>(scheduler.get()) ? "RRScheduler" : "Unknown Scheduler") << ".\n";
        scheduler->run();
    }
    else {
        std::cout << "Error: scheduler is not set.\n";
    }
}

void GlobalScheduler::stop() {
    this->running = false;
}

bool GlobalScheduler::isRunning() const {
    return this->running;
}

void GlobalScheduler::tick() const {
    std::this_thread::sleep_for(std::chrono::milliseconds(1000));
}

void GlobalScheduler::addProcessToProcessTable(std::shared_ptr<Process> process) {
    this->processTable[process->getName()] = process;
}

void GlobalScheduler::addProcessToProcessTableNoCout(std::shared_ptr<Process> process) {
    this->processTable[process->getName()] = process;
}

void GlobalScheduler::handleScreenLS() const {
    std::cout << "Running Processes: " << std::endl;
    if (this->scheduler->activeProcessesList.empty()) {
        std::cout << "No Active Processes" << std::endl;
    }
    else {
        for (int i = 0; i < this->scheduler->nCores; i++) {
            if (this->scheduler->coreList[i].process != nullptr) {
                std::shared_ptr<Process> runningProcess = this->scheduler->coreList[i].process;
                std::cout << std::setw(20) << std::left << runningProcess->getName()
                    << std::setw(40) << std::left << runningProcess->getTimestampStarted()
                    << "Core: " << std::setw(10) << std::left << runningProcess->getCPUCoreID()
                    << runningProcess->getCommandCounter() << "/" << runningProcess->getLinesOfCode() << std::endl;
            }
        }
        for (auto& process : this->scheduler->activeProcessesList) {
            if (process != nullptr) {
                if (process->getCPUCoreID() == -1)
                    std::cout << std::setw(20) << std::left << process->getName()
                    << std::setw(40) << std::left << process->getTimestampStarted()
                    << "Core: " << std::setw(10) << std::left << "N/A"
                    << process->getCommandCounter() << "/" << process->getLinesOfCode() << std::endl;
            }
        }
    }

    std::cout << "Terminated Processes: " << std::endl;
    if (this->scheduler->terminatedProcessesList.empty()) {
        std::cout << "No Finished Processes" << std::endl;
    }
    else {
        for (auto& process : this->scheduler->terminatedProcessesList) {
            std::cout << std::setw(20) << process->getName()
                << std::setw(40) << process->getTimestampFinished()
                << "Core: " << std::setw(10) << "Finished"
                << process->getCommandCounter() << "/" << process->getLinesOfCode() << std::endl;
        }
    }
}

void GlobalScheduler::startSchedulerTestInBackground() {
    std::thread schedulerTestThread(&GlobalScheduler::handleSchedulerTest, this);
    schedulerTestThread.detach(); // Detach the thread to run independently
}

void GlobalScheduler::handleSchedulerTest() {
    int i = 0;
    batchScheduler = true;

    while (batchScheduler) {
        std::string screenName = "process" + std::to_string(i);
        std::shared_ptr<Process> process = std::make_shared<Process>(1, screenName); // create process
        //std::cout << "GlobalScheduler: Creating process " << screenName << " and adding to scheduler.\n";
        scheduler->addProcess(process); // add process to scheduler queue
        tick(); // wait for 1000ms
        i++;
    }
}

void GlobalScheduler::handleSchedulerStop() {
    batchScheduler = false;
    std::cout << "Dummy processes creation HALTED.\n";
}
