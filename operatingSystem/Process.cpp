#include "Process.h"
#include <iostream>
#include <chrono>
#include <Windows.h>
#include "Utils.h"

Process::Process(int pid, std::string processName) {
    this->pid = pid;
    this->processName = processName;
    this->timestampCreated = getCurrentTimestamp();
    this->state = State::READY; 
}

// Executes a single step of the process
void Process::executeOneStep() {
    if (!isFinished()) {
        this->currentLineOfInstruction++;
        update(); // Updates state based on progress
    }
}

// Checks if the process has finished executing
bool Process::isFinished() const {
    return this->currentLineOfInstruction >= this->totalLineOfInstruction;
}

// Gets remaining steps for completion
int Process::getRemainingSteps() const {
    return this->totalLineOfInstruction - this->currentLineOfInstruction;
}

// Sets the quantum counter to manage time slice
void Process::setQuantumCounter(int quantum) {
    this->quantumCounter = quantum;
}

// Gets the current quantum counter value
int Process::getQuantumCounter() const {
    return this->quantumCounter;
}

// Decreases the quantum counter after a step
void Process::decrementQuantumCounter() {
    if (this->quantumCounter > 0) {
        --this->quantumCounter;
    }
}

std::string Process::getName() const {
    return this->processName;
}

int Process::getCommandCounter() const {
    return this->currentLineOfInstruction;
}

int Process::getLinesOfCode() const {
    return this->totalLineOfInstruction;
}

int Process::getPID() const {
    return this->pid;
}

int Process::getCPUCoreID() const {
    return this->cpuCoreID;
}

std::string Process::getTimestampStarted() const {
    return convertTimestampToString(this->timestampStarted);
}

std::string Process::getTimestampFinished() const {
    return convertTimestampToString(this->timestampFinished);
}

void Process::setCPUCoreID(int coreID) {
    this->cpuCoreID = coreID;
}

// Updates the process state based on its execution progress
void Process::update() {
    if (this->isFinished()) {
        this->state = Process::State::TERMINATED;
        this->timestampFinished = getCurrentTimestamp();
    }
    else if (this->cpuCoreID != -1 && this->quantumCounter > 0) {
        this->state = Process::State::RUNNING;
        if (this->timestampStarted == 0) {
            this->timestampStarted = getCurrentTimestamp();
        }
    }
    else {
        this->state = Process::State::READY;
    }
}

Process::State Process::getState() const {
    return this->state;
}
