#pragma once
#include <ctime>
#include <string>
#include <vector>

class Process
{
	public:
		Process(int pid = -1, std::string processName = "DefaultProcess", int minIns = 1, int maxIns = 1);
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
		int getRemainingTime() const;
		int getCommandCounter() const;
		int getLinesOfCode() const;
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
		int ticksLineOfInstruction = 0;
		int currentLineOfInstruction = 0;
		int totalLineOfInstruction = 100;
		int cpuCoreID = -1; // the cpu core where a process is assigned
		State state = State::READY;
};


