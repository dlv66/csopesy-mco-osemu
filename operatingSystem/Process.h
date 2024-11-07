#pragma once
#include <ctime>
#include <string>
#include <vector>

class Process
{
	public:
		Process(int pid = -1, std::string processName = "DefaultProcess", long long minIns = 1, long long maxIns = 1, int memorySize = 4096);
		enum class State {
			RUNNING, // Process is currently running
			WAITING, // Process is waiting for an event to occur
			READY, // Process is ready to be executed and is waiting to be assigned to a core
			TERMINATED, // Process is done executing
			PREEMPTED // Process has been preempted
		};

		void execute();
		void executeQuantum(int timeQuantum);

		std::string getName() const;
		bool isFinished() const;
		void resetTicksLineOfInstruction();
		long long getRemainingTime() const;
		long long getCommandCounter() const;
		long long getLinesOfCode() const;
		int getPID() const;
		int getCPUCoreID() const;
		State getState() const;

		std::string getTimestampStarted() const;
		std::string getTimestampFinished() const;

		void setCPUCoreID(int coreID);

		void setMemoryBlockIndex(int index); // NEW: Set the memory block index for this process
		int getMemoryBlockIndex() const; // NEW: Get the memory block index
		int getMemorySize() const;           // NEW: Get the memory size of the process
		//void setState(State state);

		void update();

	private:
		int pid = -1;
		time_t timestampCreated = 0;
		time_t timestampStarted = 0;
		time_t timestampFinished = 0;
		std::string processName = "";
		long long ticksLineOfInstruction = 0;
		long long currentLineOfInstruction = 0;
		long long totalLineOfInstruction = 0;
		int cpuCoreID = -1; // the cpu core where a process is assigned
		State state = State::READY;

		long long memorySize = 4096; // NEW: Memory size in KB, using MEM_PER_PROC as default
		long long memoryBlockIndex = -1; // NEW: Tracks the index of the allocated memory block, -1 if not allocated
};






//OTHER CODE
/*
#pragma once
#include "ICommand.h"
#include "TypedefRepo.h"
#include "MemoryManager.h"
#include <chrono>
#include <string>
#include <vector>
@@ -44,6 +45,9 @@ class Process {
	// setters
	void setState(ProcessState state);
	void setCpuCoreId(int _cpuCoreId);
	bool allocateMemory();
	void deallocateMemory();
	bool hasMemory() const;

private:
	int pid;
@@ -59,4 +63,5 @@ class Process {
	// RequirementFlags requirements;
	// friend class ResourceEmulator
	friend class FCFSScheduler;
	bool hasMemoryAllocated = false;
};
*/