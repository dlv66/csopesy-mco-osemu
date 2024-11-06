#pragma once

#include "AScheduler.h"
#include "Core.h"
#include <queue>
#include <memory>
#include <thread>

// Forward declarations
class Process;
class MemoryManager;

class RRScheduler : public AScheduler, public IThread
{
private:
    std::queue<std::shared_ptr<Process>> processQueue;  // Queue of processes
    long long timeQuantum;  // quantum in CPU cycles
    long long delayPerExec; // delay between instruction executions
    int nCores;
    std::vector<Core> coreList;
    std::vector<std::shared_ptr<Process>> activeProcessesList;
    std::vector<std::shared_ptr<Process>> terminatedProcessesList;

    std::shared_ptr<MemoryManager> memoryManager; // Persistent MemoryManager instance

public:
    // Constructor
    RRScheduler(long long quantum, long long delayExec, int nCores);

    void init() override;
    void run() override;
    void runQuantum(long long timeQuantum) override;
    void execute() override;
    void executeQuantum(long long timeQuantum) override;
};
