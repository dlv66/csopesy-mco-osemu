#pragma once

#include "AScheduler.h"
#include "Core.h"
#include <queue>
#include <memory>
#include <vector>
#include <mutex>

// Round Robin Scheduler class for managing process scheduling
class RRScheduler : public AScheduler {
public:
    RRScheduler(int quantum, int delayExec, int nCores); // Constructor to initialize scheduler
    void addProcess(std::shared_ptr<Process> process) override; // Add a new process to the queue
    void run() override; // Start running the scheduler
    void init() override; // Initialize if needed

private:
    void execute(); // Execute all cores in separate threads
    void runCore(int coreID); // Main function for each core thread
    void printQueue() const;  // For debugging the queue state
    int findLowestIdleCore(); // Find the lowest idle core
    int findNextAvailableCore(); // Find the next available core using round-robin

    int timeQuantum; // Time quantum for each process
    int delayPerExec; // Delay per execution step
    int lastAssignedCore = -1; // Tracks last core assigned in round-robin
    int nCores; // Number of cores

    std::vector<Core> coreList; // List of core objects
    std::queue<std::shared_ptr<Process>> processQueue; // Queue of pending processes
    std::vector<std::shared_ptr<Process>> activeProcessesList; // List of active processes
    std::vector<std::shared_ptr<Process>> terminatedProcessesList; // List of terminated processes
    std::mutex queueMutex; // Mutex for managing access to process queue
};
