#pragma once
#include <ctime>
#include <string>
#include <vector>

class Process {
public:
    Process(int pid = -1, std::string processName = "DefaultProcess");

    enum class State {
        RUNNING,
        WAITING,
        READY,
        TERMINATED
    };

    void executeOneStep();          // Execute a single instruction
    void setQuantumCounter(int quantum); // Set quantum time for the process
    int getQuantumCounter() const; // Retrieve the quantum counter
    void decrementQuantumCounter(); // Decrease the quantum counter
    std::string getName() const;
    bool isFinished() const;
    int getRemainingSteps() const; // Get remaining steps
    int getCommandCounter() const;
    int getLinesOfCode() const;
    int getPID() const;
    int getCPUCoreID() const;
    State getState() const;

    std::string getTimestampStarted() const;
    std::string getTimestampFinished() const;

    void setCPUCoreID(int coreID);
    void update();

    //void incrementCpuCycles(); 

private:
    int pid = -1;
    time_t timestampCreated = 0;
    time_t timestampStarted = 0;
    time_t timestampFinished = 0;
    std::string processName = "";
    int currentLineOfInstruction = 0;
    int totalLineOfInstruction = 100;
    int cpuCoreID = -1;
    State state = State::READY;
    int quantumCounter = 0; // Tracks quantum time remaining
};
