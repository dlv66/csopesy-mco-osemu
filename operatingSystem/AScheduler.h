#pragma once

#include <string>
#include <unordered_map>
#include <sstream>

#include "Core.h"
#include "IThread.h"
#include "Process.h"

static const std::string FCFS_SCHEDULER_NAME = "FCFSScheduler";
static const std::string RR_SCHEDULER_NAME = "RRScheduler";

class AScheduler : public IThread {
<<<<<<< HEAD

public:

<<<<<<< HEAD
    enum SchedulingAlgorithm {
        FCFS,
        RR
    };
=======
	int nCores;
	std::vector<Core> coreList;
>>>>>>> parent of 9b0d641 (rr pt2)
=======

public:

	int nCores;
	std::vector<Core> coreList;
>>>>>>> parent of f7daf88 (Merge remote-tracking branch 'origin/RR_Testing')

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

	void addProcess(std::shared_ptr<Process> process);
	void addProcessNoCout(std::shared_ptr<Process> process);
	virtual void init() = 0; // the initializations of the algorithm
	virtual void execute() = 0; // the algorithm

	std::vector<std::shared_ptr<Process>> activeProcessesList;
	std::vector<std::shared_ptr<Process>> terminatedProcessesList;

	//struct ProcessInfo {
	//	int pid;
	//	std::string name;
	//	int cpuID;
	//	int currentLine; // originally lineCounter
	//	int linesOfCode; // for total lines
	//	int remainingTime; // for burst time
	//};

	friend class GlobalScheduler;

};