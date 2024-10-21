#pragma once

#include <string>
#include <IETThread.h>
#include <Process.h>
#include <TypedefRepo.h>
#include <unordered_map>
#include <sstream>

static const std::string DEBUG_SCHEDULER_NAME = "DebugScheduler";
static const std::string FCFS_SCHEDULER_NAME = "FCFSScheduler";
static const std::string SJF_NONPREEMPTIVE_SCHEDULER_NAME = "SJF-NonPreempt-Scheduler";
static const std::string SJF_PREEMPTIVE_SCHEDULER_NAME = "SJF-Preempt-Scheduler";

class AScheduler : public IETTHread {

	public:	
		enum SchedulingAlgorithm {
			DEBUG,
			FCFS,
			SHORTEST_JOB_FIRST_NONPREEMPTIVE,
			SHORTEST_JOB_FIRST_PREEMPTIVE,
			ROUND_ROBIN
		};

		AScheduler(SchedulingAlgorithm schedulingAlgo, int pid, std::string processName);

		void addProcess(std::shared_ptr<Process> process);
		std::shared_ptr<Process> findProcess(std::string processName);
		void run() override;
		void stop();

		virtual void init() = 0;
		virtual void execute() = 0;

		struct ProcessInfo {
			int pid;
			std::string name;
			int cpuID;
			int currentLine; // originally lineCounter
			int linesOfCode; // for total lines
			int remainingTime; // for burst time
		};

};