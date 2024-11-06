#include "MemoryManager.h"
#include "Process.h"  // Include only in .cpp to access Process methods
#include <iostream>
#include <iomanip>

// Constructor initializes the memory frames as all empty
MemoryManager::MemoryManager() : frames(FRAMES, false) {}

// Allocates memory for a process. Returns true if successful, false if insufficient memory.
bool MemoryManager::allocateMemoryForProcess(std::shared_ptr<Process> process, int startIndex) {
    int requiredFrames = MEM_PER_PROC / MEM_PER_FRAME;

    // Check if there is enough room from startIndex to allocate memory
    if (startIndex < 0 || startIndex + requiredFrames > FRAMES) {
        std::cout << "Invalid starting index or not enough frames from the specified index." << std::endl;
        return false;
    }

    // Check if the frames from startIndex are free
    for (int i = startIndex; i < startIndex + requiredFrames; i++) {
        if (frames[i]) {
            std::cout << "Memory allocation failed: frames from " << startIndex
                << " to " << startIndex + requiredFrames - 1
                << " are not all free." << std::endl;
            return false;
        }
    }

    // Mark the frames as occupied
    for (int i = startIndex; i < startIndex + requiredFrames; i++) {
        frames[i] = true;
    }

    // Set process's memory block index and increment process count
    process->setMemoryBlockIndex(startIndex);
    processesInMemory++;
    std::cout << "Manually allocated memory for process " << process->getName()
        << " from frame " << startIndex << " to " << startIndex + requiredFrames - 1 << std::endl;
    return true;
}



// Releases memory allocated to a specific process by marking frames as free
void MemoryManager::releaseMemoryForProcess(std::shared_ptr<Process> process) {
    int blockIndex = process->getMemoryBlockIndex();
    if (blockIndex >= 0) {
        int releasedFrames = MEM_PER_PROC / MEM_PER_FRAME;
        for (int i = blockIndex; i < blockIndex + releasedFrames; i++) {
            frames[i] = false;  // Free the frames
        }
        process->setMemoryBlockIndex(-1);  // Reset the memory block index
        processesInMemory--;
        std::cout << "Released memory for process " << process->getName()
            << " from block index " << blockIndex << std::endl;
    }
}


// Calculates external fragmentation by finding the largest contiguous free block
int MemoryManager::calculateExternalFragmentation() const {
    int maxFreeBlock = 0;
    int contiguousFreeFrames = 0;

    for (bool frame : frames) {
        if (!frame) {
            contiguousFreeFrames++;
        }
        else {
            if (contiguousFreeFrames > maxFreeBlock) {
                maxFreeBlock = contiguousFreeFrames;
            }
            contiguousFreeFrames = 0;
        }
    }
    if (contiguousFreeFrames > maxFreeBlock) maxFreeBlock = contiguousFreeFrames;

    return maxFreeBlock * MEM_PER_FRAME;
}

// Function to generate a detailed report at each quantum cycle
void MemoryManager::generateReport(const std::vector<std::shared_ptr<Process>>& activeProcessesList) const {
    std::ofstream reportFile("memory_report_cycle.txt", std::ios::app);  // Open in append mode to add each cycle's info

    // Timestamp for the report
    std::time_t now = std::time(0);
    char buffer[26];  // Standard buffer size for date-time strings
    ctime_s(buffer, sizeof(buffer), &now);  // Use ctime_s for safe conversion
    reportFile << "\nTimestamp: " << buffer;

    // Number of processes currently in memory
    reportFile << "Processes in Memory: " << processesInMemory << "\n";

    // Calculate and display external fragmentation
    reportFile << "External Fragmentation: " << calculateExternalFragmentation() << " KB\n";

    reportFile << "\n----end---- = " << MAX_MEMORY << "\n\n";

    int currentAddress = MAX_MEMORY;
    int frameSizeInBytes = MEM_PER_FRAME;
    int processMemorySize = MEM_PER_PROC;

    for (int i = FRAMES - 1; i >= 0; ) {
        bool occupied = frames[i];
        int blockStart = i;

        // Identify contiguous occupied or free frames
        while (i >= 0 && frames[i] == occupied) {
            i--;
        }

        int blockEnd = blockStart;
        int blockStartAddress = currentAddress - ((blockEnd + 1) * frameSizeInBytes);
        int blockSize = (blockEnd - i) * frameSizeInBytes;
        currentAddress -= blockSize;

        if (occupied) {
            // Find the process occupying this block (assumes unique block for each process)
            for (const auto& process : activeProcessesList) {
                if (process->getMemoryBlockIndex() == i + 1) {
                    reportFile << blockStartAddress << "\n";
                    reportFile << "P" << process->getPID() << "\n";
                    reportFile << blockStartAddress + blockSize << "\n\n";
                    break;
                }
            }
        }
        else {
            // Print free memory block
            reportFile << blockStartAddress << "\n\n";
            reportFile << currentAddress << "\n\n";
        }
    }

    reportFile << "----start---- = 0\n";

    reportFile << "---------------------------------------------------\n";
    reportFile.close();
}


// Prints an ASCII representation of memory layout in 64-frame rows
void MemoryManager::printMemoryState(std::ofstream& reportFile) const {
    reportFile << "\nMemory Layout:\n";
    for (int i = 0; i < FRAMES; i++) {
        if (i % 64 == 0) reportFile << "\n";  // New line every 64 frames
        reportFile << (frames[i] ? "#" : ".");
    }
    reportFile << "\n";
}

