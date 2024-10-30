#pragma once

#include <string>
#include <unordered_map>
#include <sstream>
#include <memory>
#include <vector>

#include "Core.h"
#include "IThread.h"
#include "Process.h"

static const std::string FCFS_SCHEDULER_NAME = "FCFSScheduler";
static const std::string RR_SCHEDULER_NAME = "RRScheduler";

class AScheduler : public IThread {
public:
    int nCores;
    std::vector<Core> coreList;

    enum SchedulingAlgorithm {
        FCFS,
        ROUND_ROBIN
    };

    // a scheduler is instantiated with an initial process
    AScheduler(SchedulingAlgorithm schedulingAlgo);

    std::shared_ptr<Process> findProcess(std::string processName);
    void run() override;
    void stop();
    SchedulingAlgorithm schedulingAlgo;

    // Add virtual keyword here
    virtual void addProcess(std::shared_ptr<Process> process) = 0;
    void addProcessNoCout(std::shared_ptr<Process> process);
    virtual void init() = 0; // the initializations of the algorithm
    virtual void execute() = 0; // the algorithm

    std::vector<std::shared_ptr<Process>> activeProcessesList;
    std::vector<std::shared_ptr<Process>> terminatedProcessesList;

    friend class GlobalScheduler;
};
