#pragma once
#include <ctime>
#include <string>
#include <vector>

class Process
{
	public:
		Process(int pid = -1, std::string processName = "DefaultProcess", long long minIns = 1, long long maxIns = 1);
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
};


