#pragma once

#include <string>
#include <unordered_map>
#include <sstream>

#include "IThread.h"
#include "Process.h"

static const std::string FCFS_SCHEDULER_NAME = "FCFSScheduler";
static const std::string RR_SCHEDULER_NAME = "RRScheduler";

// do we make a thread class?
class AScheduler : public IThread {

public:

	enum SchedulingAlgorithm {
		FCFS,
		ROUND_ROBIN
	};

	// a scheduler is instantiated with an initial process
	AScheduler(SchedulingAlgorithm schedulingAlgo, int pid, std::string processName);

	void addProcess(std::shared_ptr<Process> process);
	std::shared_ptr<Process> findProcess(std::string processName);
	void run() override;
	void stop();
	SchedulingAlgorithm schedulingAlgo;

	virtual void init() = 0; // the initializations of the algorithm
	virtual void execute() = 0; // the algorithm

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