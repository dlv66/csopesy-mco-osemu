#pragma once
#include <vector>
#include <memory>
#include <fstream>
#include <ctime>
#include <string>

class Process;

class MemoryManager {
public:
    MemoryManager();  // Constructor to initialize memory frames

    // Updated declaration to include startIndex parameter
    bool allocateMemoryForProcess(std::shared_ptr<Process> process, int startIndex);
    void releaseMemoryForProcess(std::shared_ptr<Process> process);   // Releases memory of a terminated process
    int calculateExternalFragmentation() const;                       // Calculates external fragmentation
    void generateReport(const std::vector<std::shared_ptr<Process>>& activeProcessesList) const;

    static const int MAX_MEMORY = 16384;     // Max memory in KB
    static const int MEM_PER_FRAME = 16;     // Frame size in KB
    static const int MEM_PER_PROC = 4096;    // Fixed memory required per process in KB
    static const int FRAMES = MAX_MEMORY / MEM_PER_FRAME;  // Number of frames in memory


private:
    std::vector<bool> frames;  // Tracks which frames are occupied
    int processesInMemory = 0; // Tracks the number of active processes in memory

    void printMemoryState(std::ofstream& reportFile) const; // Prints the ASCII memory layout
};
