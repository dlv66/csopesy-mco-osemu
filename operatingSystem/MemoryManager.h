#pragma once
#include <vector>
#include <memory>
#include <fstream>
#include <ctime>
#include <queue>
#include <string>
#include <unordered_map>

#include "Core.h"

class Process;

class MemoryManager {
public:
    MemoryManager(long long maxOverallMem, 
                  long long memPerFrame);  // Constructor to initialize memory frames

    // Updated declaration to include startIndex parameter
    enum MemoryAllocator {
        FlatMemory,
        Paging
    };

	void initializePagingAllocator(); // Initializes the Paging memory allocator

	MemoryAllocator memoryAllocator = Paging; // Default memory allocation strategy
    bool allocateFlatMemoryForProcess(std::shared_ptr<Process> process);
    void deallocateFlatMemoryForProcess(std::shared_ptr<Process> process);   // Releases memory of a terminated process

    void* allocatePagingMemoryForProcess(std::shared_ptr<Process> process);  // Allocates memory for a process
	void deallocatePagingMemoryForProcess(std::shared_ptr<Process> process); // Releases memory of a terminated process

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

	int numPagedIn = 0; // Number of processes paged in
	int numPagedOut = 0; // Number of processes paged out

	void addToBackingStore(std::shared_ptr<Process> process); // Adds a process to the backing store
    std::shared_ptr<Process> fetchFromBackingStore(); // Fetches a process from the backing store
	bool isBackingStoreEmpty() const; // Checks if the backing store is empty
	std::string serializeProcess(const std::shared_ptr<Process>& process); // Serializes a process to a string
	std::shared_ptr<Process> deserializeProcess(const std::string& processString); // Deserializes a process from a string

	void setMemoryAllocator(MemoryAllocator allocator); // Sets the memory allocation strategy
	void allocate(std::shared_ptr<Process> process); // Allocates memory for a process using the selected strategy

	// frameMap for Paging memory allocation
	std::unordered_map<int, int> frameMap; // Maps frame index to process ID
    std::queue<int> freeFrameList;

	int getNumPagedIn() const; // Returns the number of processes paged in
	int getNumPagedOut() const; // Returns the number of processes paged out
	void setNumPagedIn(int numPagedIn); // Sets the number of processes paged in
	void setNumPagedOut(int numPagedOut); // Sets the number of processes paged out

    int allocateFrames(int numFrames, int processId);
    void deallocateFrames(int frameIndex);

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
        int startAddress;
        int size;
        std::string processName;
        bool isFree;
        MemoryBlock(int start, int s, bool free = true)
            : startAddress(start), size(s), isFree(free) {}
    };
    static MemoryManager* sharedInstance;
    std::mutex memoryMutex;
    // Memory configuration
    int totalMemory;
    int frameSize;
    int processMemorySize;
    int currentQuantum;
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
    void initMemory(int totalMem, int frameSize, int procMemSize);
    // Memory allocation/deallocation
    bool allocateMemory(const std::string& processName);
    void deallocateMemory(const std::string& processName);
    // Memory status
    int getTotalMemory() const { return totalMemory; }
    int getFreeMemory() const;
    int getExternalFragmentation() const;
    int getProcessCount() const;
    // Memory snapshot generation
    void generateMemorySnapshot(int quantumNumber);
    void incrementQuantum() { currentQuantum++; }
    int getCurrentQuantum() const { return currentQuantum; }
private:
    void mergeAdjacentFreeBlocks();
    std::string generateTimestamp() const;
    void printMemoryMap(std::ofstream& outFile) const;
    bool isContiguousBlockAvailable(int size) const;
};
*/
