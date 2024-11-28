#pragma once
#include <ctime>
#include <string>
#include <vector>
#include <memory>
#include <chrono>

class Process
{
	public:
		using TimePoint = std::chrono::time_point<std::chrono::system_clock>;

		Process(int pid = -1, std::string processName = "DefaultProcess",
				long long minIns = 1, long long maxIns = 1, 
				long long minMemPerProc = 1, long long maxMemPerProc = 1,
				int memPerFrame = 1);
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
		void setRunningState();
		State getState() const;

		std::string getTimestampStarted() const;
		std::string getTimestampFinished() const;

		void setCPUCoreID(int coreID);

		// Memory Management (Week 8 + MO2)
		void setMemoryBlockIndex(int index); // NEW: Set the memory block index for this process
		int getMemoryBlockIndex() const; // NEW: Get the memory block index

		void setMemoryRequired(int memoryRequired); // NEW: Set the memory required for this process
		int getMemoryRequired() const; // NEW: Get the memory required for this process

		void setMemoryPtr(void* ptr); // NEW: Set the memory pointer for this process
		void* getMemoryPtr(); // NEW: Get the memory pointer for this process

		int getMemorySize() const; // NEW: Get the memory size of the process

		TimePoint getMemoryAllocatedTime();
		void setMemoryAllocatedTime(TimePoint time);

		void setNumOfPages(int numOfPages);
		int getNumOfPages() const;

		bool isMemoryAllocatedTimeNull() const;

		//void setState(State state);


		void update();

		int frameStart;
		int frameEnd;

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

		// Memory Management (Week 8 + MO2)
		long long memorySize = 4096; // NEW: Memory size in KB, using MEM_PER_PROC as default
		int memoryRequired = 0; // NEW: Memory required for the process
		void* memoryPtr = nullptr; // NEW: Pointer to the allocated memory block
		TimePoint memoryAllocatedTime; // NEW: Time when the memory was allocated
		int numOfPages = 0; // NEW: Number of pages required for the process
		int memoryBlockIndex = -1; // NEW: Tracks the index of the allocated memory block, -1 if not allocated
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