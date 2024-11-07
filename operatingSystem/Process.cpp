#include "Process.h"

#include <chrono>
#include <iostream>
#include <thread>
#include <fstream>
#include <random>
#include <Windows.h>

#include "GlobalScheduler.h"
#include "Utils.h"

// Constructor
Process::Process(int pid, std::string processName, long long minIns, long long maxIns, int memorySize)
    : pid(pid), processName(processName), memorySize(memorySize) {
    timestampCreated = std::time(nullptr);
    totalLineOfInstruction = minIns + (std::rand() % (maxIns - minIns + 1)); // Randomize between min and max
    state = State::READY;
}

// Executes the process (basic implementation, assuming it increments the instruction counter)
void Process::execute() {
    if (state == State::RUNNING && currentLineOfInstruction < totalLineOfInstruction) {
        ++currentLineOfInstruction;
    }
    if (currentLineOfInstruction >= totalLineOfInstruction) {
        state = State::TERMINATED;
        timestampFinished = std::time(nullptr);
    }
}

// Executes for a specific quantum time
void Process::executeQuantum(int timeQuantum) {
    if (state == State::RUNNING) {
        for (int i = 0; i < timeQuantum && currentLineOfInstruction < totalLineOfInstruction; ++i) {
            ++currentLineOfInstruction;
        }
        if (currentLineOfInstruction >= totalLineOfInstruction) {
            state = State::TERMINATED;
            timestampFinished = std::time(nullptr);
        }
    }
}

// Getters
std::string Process::getName() const { return processName; }
bool Process::isFinished() const { return state == State::TERMINATED; }
long long Process::getRemainingTime() const { return totalLineOfInstruction - currentLineOfInstruction; }
long long Process::getCommandCounter() const { return currentLineOfInstruction; }
long long Process::getLinesOfCode() const { return totalLineOfInstruction; }
int Process::getPID() const { return pid; }
int Process::getCPUCoreID() const { return cpuCoreID; }
Process::State Process::getState() const { return state; }
int Process::getMemorySize() const { return memorySize; } // NEW: Returns the memory size of the process

// Timestamp methods
std::string Process::getTimestampStarted() const
{
    return convertTimestampToString(this->timestampStarted);
}

std::string Process::getTimestampFinished() const
{
    return convertTimestampToString(this->timestampFinished);
}

// Setters
void Process::setCPUCoreID(int coreID) { cpuCoreID = coreID; }
void Process::setMemoryBlockIndex(int index) { memoryBlockIndex = index; }
int Process::getMemoryBlockIndex() const { return memoryBlockIndex; }

// Updates the process state
void Process::update() {
    if (state == State::READY && timestampStarted == 0) {
        timestampStarted = std::time(nullptr);
    }
}

// Resets the ticks of instruction execution, useful for re-scheduling
void Process::resetTicksLineOfInstruction() {
    currentLineOfInstruction = 0;
    state = State::READY;
}







//OTHER CODE
/*
bool Process::allocateMemory() {
    if (!hasMemoryAllocated) {
        hasMemoryAllocated = MemoryManager::getInstance()->allocateMemory(name);
    }
    return hasMemoryAllocated;
}
void Process::deallocateMemory() {
    if (hasMemoryAllocated) {
        MemoryManager::getInstance()->deallocateMemory(name);
        hasMemoryAllocated = false;
    }
}
bool Process::hasMemory() const {
    return hasMemoryAllocated;
}
*/