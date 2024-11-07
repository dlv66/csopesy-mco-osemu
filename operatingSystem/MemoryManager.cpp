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














//OTHER CODE
/*
#include "MemoryManager.h"
#include <algorithm>
#include <filesystem>
MemoryManager* MemoryManager::sharedInstance = nullptr;
MemoryManager* MemoryManager::getInstance() {
    return sharedInstance;
}
void MemoryManager::initialize() {
    if (sharedInstance == nullptr) {
        sharedInstance = new MemoryManager();
    }
}
void MemoryManager::destroy() {
    delete sharedInstance;
    sharedInstance = nullptr;
}
void MemoryManager::initMemory(size_t totalMem, size_t frameSize, size_t procMemSize) {
    std::lock_guard<std::mutex> lock(memoryMutex);
    totalMemory = totalMem;
    this->frameSize = frameSize;
    processMemorySize = procMemSize;
    currentQuantum = 0;
    // Initialize with one free block spanning all memory
    memoryBlocks.clear();
    memoryBlocks.emplace_back(0, totalMemory, true);
}
bool MemoryManager::allocateMemory(const std::string& processName) {
    std::lock_guard<std::mutex> lock(memoryMutex);
    // First-fit allocation
    for (auto& block : memoryBlocks) {
        if (block.isFree && block.size >= processMemorySize) {
            // If block is exactly the right size
            if (block.size == processMemorySize) {
                block.isFree = false;
                block.processName = processName;
                return true;
            }
            // Split block
            size_t remainingSize = block.size - processMemorySize;
            block.size = processMemorySize;
            block.isFree = false;
            block.processName = processName;
            // Create new free block with remaining space
            memoryBlocks.emplace(
                std::next(std::find_if(memoryBlocks.begin(), memoryBlocks.end(),
                    [&](const MemoryBlock& b) { return b.startAddress == block.startAddress; })),
                block.startAddress + processMemorySize,
                remainingSize,
                true
            );
            return true;
        }
    }
    return false;
}
void MemoryManager::deallocateMemory(const std::string& processName) {
    std::lock_guard<std::mutex> lock(memoryMutex);
    for (auto it = memoryBlocks.begin(); it != memoryBlocks.end(); ++it) {
        if (!it->isFree && it->processName == processName) {
            it->isFree = true;
            it->processName.clear();
            mergeAdjacentFreeBlocks();
            break;
        }
    }
}
size_t MemoryManager::getFreeMemory() const {
    size_t freeMemory = 0;
    for (const auto& block : memoryBlocks) {
        if (block.isFree) {
            freeMemory += block.size;
        }
    }
    return freeMemory;
}
size_t MemoryManager::getExternalFragmentation() const {
    size_t fragmentation = 0;
    for (const auto& block : memoryBlocks) {
        if (block.isFree && block.size < processMemorySize) {
            fragmentation += block.size;
        }
    }
    return fragmentation;
}
size_t MemoryManager::getProcessCount() const {
    return std::count_if(memoryBlocks.begin(), memoryBlocks.end(),
        [](const MemoryBlock& block) { return !block.isFree; });
}
void MemoryManager::mergeAdjacentFreeBlocks() {
    auto it = memoryBlocks.begin();
    while (it != memoryBlocks.end() && std::next(it) != memoryBlocks.end()) {
        if (it->isFree && std::next(it)->isFree) {
            it->size += std::next(it)->size;
            memoryBlocks.erase(std::next(it));
        }
        else {
            ++it;
        }
    }
}
std::string MemoryManager::generateTimestamp() const {
    auto now = std::chrono::system_clock::now();
    auto now_c = std::chrono::system_clock::to_time_t(now);
    std::tm tm_buf;
    localtime_s(&tm_buf, &now_c);
    std::stringstream ss;
    ss << std::put_time(&tm_buf, "%m/%d/%Y %H:%M:%S");
    return ss.str();
}
void MemoryManager::generateMemorySnapshot(size_t quantumNumber) {
    std::lock_guard<std::mutex> lock(memoryMutex);
    std::stringstream filename;
    filename << "memory_stamp_" << std::setw(2) << std::setfill('0') << quantumNumber << ".txt";
    std::ofstream outFile(filename.str());
    if (!outFile.is_open()) {
        throw std::runtime_error("Could not create memory snapshot file");
    }
    // Write header information
    outFile << "Timestamp: " << generateTimestamp() << "\n";
    outFile << "Number of processes in memory: " << getProcessCount() << "\n";
    outFile << "Total external fragmentation in KB: " << getExternalFragmentation() / 1024 << "\n\n";
    // Write memory map
    outFile << "----end---- = " << totalMemory << "\n";
    // Print each memory block
    for (const auto& block : memoryBlocks) {
        outFile << block.startAddress + block.size << "\n";
        if (!block.isFree) {
            outFile << block.processName << "\n";
        }
    }
    outFile << "----start----- = 0\n";
    outFile.close();
}
bool MemoryManager::isContiguousBlockAvailable(size_t size) const {
    for (const auto& block : memoryBlocks) {
        if (block.isFree && block.size >= size) {
            return true;
        }
    }
    return false;
}
*/
