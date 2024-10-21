#pragma once
#include <ctime>
#include <string>
#include <vector>

const int LINE_OF_INSTRUCTIONS = 100;

class Process
{
	public:
		enum class State {
			RUNNING, // Process is currently running
			WAITING, // Process is waiting for an event to occur
			READY, // Process is ready to be executed and is waiting to be assigned to a core
			TERMINATED // Process is done executing
		};

		Process(int pid, std::string processName);
		void execute();

		std::string getName() const;
		bool isFinished() const;
		int getRemainingTime() const;
		int getCommandCounter() const;
		int getLinesOfCode() const;
		int getPID() const;
		int getCPUCoreID() const;
		State getState() const;

		void setCPUCoreID(int coreID);
		void setState(State state);

		
		time_t timestampCreated = 0;
		time_t timestampStarted = 0;
		time_t timestampFinished = 0;

		int currentLineOfInstruction = 0;
		int totalLineOfInstruction = LINE_OF_INSTRUCTIONS;

	private:
		int pid = -1;
		std::string processName = "";
		int cpuCoreID = -1; // the cpu core where a process is assigned
		State state = State::READY;
};


