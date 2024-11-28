#pragma once
#include <vector>
#include <memory>
#include <fstream>
#include <ctime>
#include <string>
#include "Core.h"

class Process;

class MemoryManager {
public:
    MemoryManager(long long maxOverallMem, 
                  long long memPerFrame);  // Constructor to initialize memory frames

    // Updated declaration to include startIndex parameter
    bool allocateMemoryForProcess(std::shared_ptr<Process> process, int startIndex);
    void releaseMemoryForProcess(std::shared_ptr<Process> process);   // Releases memory of a terminated process
    int calculateExternalFragmentation() const;                       // Calculates external fragmentation
    void generateReport(const std::vector<Core>& coreList) const;
	void addToBackingStore() const;                                   // Adds terminated processes to the backing store

    long long maxOverallMem;
    long long memPerFrame;

    // OLD CODE FROM WEEK 8 HOMEWORK:
    // static const int MAX_MEMORY = 16384;     // Max memory in KB
    // static const int MEM_PER_FRAME = 16;     // Frame size in KB

    static const int MEM_PER_PROC = 5120;    // Fixed memory required per process in KB
    
	// OLD CODE FROM WEEK 8 HOMEWORK:
    //static const int FRAMES = maxOverallMem / memPerFrame;  // Number of frames in memory

	int FRAMES = 0;  // Number of frames in memory

    static const std::string backingStoreFile; // Backing store file path

	void addToBackingStore(std::shared_ptr<Process> process); // Adds a process to the backing store
    std::shared_ptr<Process> fetchFromBackingStore(); // Fetches a process from the backing store
	bool isBackingStoreEmpty() const; // Checks if the backing store is empty
	std::string serializeProcess(const std::shared_ptr<Process>& process); // Serializes a process to a string
	std::shared_ptr<Process> deserializeProcess(const std::string& processString); // Deserializes a process from a string


private:

    std::vector<bool> frames;  // Tracks which frames are occupied
    int processesInMemory = 0; // Tracks the number of active processes in memory

    void printMemoryState(std::ofstream& reportFile) const; // Prints the ASCII memory layout
};









//OTHER CODE
/*
#pragma once
#include <vector>
#include <memory>
#include <string>
#include <fstream>
#include <chrono>
#include <sstream>
#include <iomanip>
#include <mutex>
class MemoryManager {
private:
    struct MemoryBlock {
        size_t startAddress;
        size_t size;
        std::string processName;
        bool isFree;
        MemoryBlock(size_t start, size_t s, bool free = true)
            : startAddress(start), size(s), isFree(free) {}
    };
    static MemoryManager* sharedInstance;
    std::mutex memoryMutex;
    // Memory configuration
    size_t totalMemory;
    size_t frameSize;
    size_t processMemorySize;
    size_t currentQuantum;
    std::vector<MemoryBlock> memoryBlocks;
    // Private constructor for singleton
    MemoryManager() = default;
    ~MemoryManager() = default;
    MemoryManager(const MemoryManager&) = delete;
    MemoryManager& operator=(const MemoryManager&) = delete;
public:
    static MemoryManager* getInstance();
    static void initialize();
    static void destroy();
    // Initialization
    void initMemory(size_t totalMem, size_t frameSize, size_t procMemSize);
    // Memory allocation/deallocation
    bool allocateMemory(const std::string& processName);
    void deallocateMemory(const std::string& processName);
    // Memory status
    size_t getTotalMemory() const { return totalMemory; }
    size_t getFreeMemory() const;
    size_t getExternalFragmentation() const;
    size_t getProcessCount() const;
    // Memory snapshot generation
    void generateMemorySnapshot(size_t quantumNumber);
    void incrementQuantum() { currentQuantum++; }
    size_t getCurrentQuantum() const { return currentQuantum; }
private:
    void mergeAdjacentFreeBlocks();
    std::string generateTimestamp() const;
    void printMemoryMap(std::ofstream& outFile) const;
    bool isContiguousBlockAvailable(size_t size) const;
};
*/
