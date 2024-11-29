#include "MemoryManager.h"
#include "Process.h"  // Include only in .cpp to access Process methods
#include <iostream>
#include <iomanip>
#include <mutex>
#include <sstream> 
#include <vector>  
#include "Core.h"

const std::string MemoryManager::backingStoreFile = "backing_store.txt";

// Constructor initializes the memory frames as all empty
MemoryManager::MemoryManager(long long maxOverallMem, 
                             long long memPerFrame 
                             ) : frames(FRAMES), maxOverallMem(maxOverallMem), memPerFrame(memPerFrame)
{
    // Clear the backing store file on initialization
    std::ofstream ofs(backingStoreFile, std::ofstream::out | std::ofstream::trunc);
    ofs.close();
    FRAMES = maxOverallMem / memPerFrame;
    // set frames size
	frames.resize(FRAMES, false);
	if (maxOverallMem == memPerFrame)
	{
        this->memoryAllocator = MemoryAllocator::FlatMemory;
        FRAMES = maxOverallMem;
        frames.resize(FRAMES, false);
	}
	else
	{
        this->memoryAllocator = MemoryAllocator::Paging;
	}

}

void MemoryManager::allocate(std::shared_ptr<Process> process)
{
	if (this->memoryAllocator == MemoryAllocator::FlatMemory)
	{
		allocateFlatMemoryForProcess(process);
	}
	else
	{
		allocatePagingMemoryForProcess(process);
	}
}

void MemoryManager::initializePagingAllocator()
{
    for (int i = 0; i < FRAMES; ++i) {
        freeFrameList.push(i);

        // initialize the unordered map with frames
        frameMap[i] = -1;
    }

}


// Allocates memory for a process. Returns true if successful, false if insufficient memory.
bool MemoryManager::allocateFlatMemoryForProcess(std::shared_ptr<Process> process) {
	std::cout << "Allocating memory for process " << process->getName() << "...\n";
    int size = process->getMemorySize();
    int startIndex = process->getMemoryBlockIndex();

       // Check if there is enough room from startIndex to allocate memory
    if (size > FRAMES) {
        std::cout << "Invalid starting index or not enough frames from the specified index." << std::endl;
        return false;
    }

	// Check for blocks that can allocate memory
	for (int i = 0; i < FRAMES - size + 1; i++) {
		if (!frames[i]) {
			bool canAllocate = true;
			for (int j = i; j < i + size; j++) {
				if (frames[j]) {
					canAllocate = false;
					break;
				}
			}
			if (canAllocate) {
				startIndex = i;
				break;
			}
		}
	}


    if (startIndex >= 0)
    {
        // Set process's memory block index and increment process count
        process->setMemoryBlockIndex(startIndex);

    	// Mark the frames as occupied
		process->frameStart = startIndex;
		process->frameEnd = startIndex + size - 1;

        for (int i = startIndex; i < startIndex + size; i++) {
            frames[i] = true;
        }

        processesInMemory++;
        //std::cout << "Manually allocated memory for process " << process->getName()
        //    << " from frame " << startIndex << " to " << startIndex + size - 1 << std::endl;
        return true;

    }

	return false;
}

// Allocates memory for a process using the paging strategy
void* MemoryManager::allocatePagingMemoryForProcess(std::shared_ptr<Process> process) {
    int processId = process->getPID();
    int numFramesNeeded = process->getNumOfPages();

    // print pid and numFramesNeeded
    //std::cout << "Process ID: " << processId << " Number of Frames Needed: " << numFramesNeeded << std::endl;
    //std::cout << "Allocating " << numFramesNeeded << " frames for process " << processId << std::endl;

    if (numFramesNeeded > freeFrameList.size()) {
        //std::cout << "numFramesNeeded > freeFrameList.size()";
        return nullptr;
    }

    // Allocate frames for the process
    int frameIndex = allocateFrames(numFramesNeeded, processId);
    void* memoryPtr = &frameMap[frameIndex];
    process->setMemoryPtr(memoryPtr);

    processesInMemory++;

    return memoryPtr;
}

// Releases memory allocated to a specific process by marking frames as free
void MemoryManager::deallocateFlatMemoryForProcess(std::shared_ptr<Process> process) {
    int blockIndex = process->getMemoryBlockIndex();
    if (blockIndex >= 0) {
        int releasedFrames = process->getMemorySize() / memPerFrame;
        for (int i = blockIndex; i < process->frameEnd + 1; i++) {
            frames[i] = false;  // Free the frames
        }
        process->setMemoryBlockIndex(-1);  // Reset the memory block index
        processesInMemory--;
        /*std::cout << "Released memory for process " << process->getName()
            << " from block index " << blockIndex << std::endl;*/
    }
}

void MemoryManager::deallocatePagingMemoryForProcess(std::shared_ptr<Process> process)
{
    int processId = process->getPID();
    std::vector<int> framesToDeallocate;

    // Find frames allocated to the process
    for (const auto& entry : frameMap) {
        if (entry.second == processId) {
            framesToDeallocate.push_back(entry.first);
        }
    }

    if (framesToDeallocate.size() == 0) {
        return;
    }

    // Deallocate the frames
    for (int frameIndex : framesToDeallocate) {
        deallocateFrames(frameIndex);
    }

    // list deallocated frames
    /*for (int frameIndex : framesToDeallocate) {
        std::cout << "Deallocated Frame: " << frameIndex << std::endl;
    }*/

    process->setMemoryPtr(nullptr);

    // decrement processCount
    processesInMemory--;
}

int MemoryManager::allocateFrames(int numFrames, int processId) {
    std::vector<int> allocatedFrames;

    // Collect the required number of frames from the freeFrameList
    for (int i = 0; i < numFrames; ++i) {
        allocatedFrames.push_back(freeFrameList.front());
        freeFrameList.pop();

        setNumPagedIn(this->numPagedIn + 1);
    }

    // print allocated frames
    /*for (int frameIndex : allocatedFrames) {
        std::cout << "Allocated Frame: " << frameIndex << std::endl;
    }*/

    // Map allocated frames to the process ID
    for (int frameIndex : allocatedFrames) {
        frameMap[frameIndex] = processId;
    }

    // Return the index of the first allocated frame
    return allocatedFrames.front();
}

void MemoryManager::deallocateFrames(int frameIndex) {
    // Set frame to UNALLOCATED_FRAME to "deallocate"
    frameMap[frameIndex] = -1;

    // Add frame to the free frame list
    freeFrameList.push(frameIndex);

    // increment numPagedIn
    this->setNumPagedOut(this->numPagedOut + 1);
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

    return maxFreeBlock;
}
// Function to generate a detailed report at each quantum cycle
void MemoryManager::generateReport(const std::vector<Core>& coreList) const {
    std::ofstream reportFile("memory_report_cycle.txt", std::ios::app);  // Open in append mode to add each cycle's info

    // Timestamp for the report
    std::time_t now = std::time(0);
    char buffer[26];
    ctime_s(buffer, sizeof(buffer), &now);
    reportFile << "\nTimestamp: " << buffer;

    int processesInMemoryCount = 0;
    int externalFragmentation = calculateExternalFragmentation();


    // Temporary vector to hold each report entry for reverse output
    std::vector<std::string> reportEntries;

    // Iterate over coreList to capture running processes
    for (const auto& core : coreList) {
        if (core.process != nullptr) {  // Only consider processes running on cores
            processesInMemoryCount++;
            std::shared_ptr<Process> runningProcess = core.process;

            int blockStartAddr = runningProcess->getMemoryBlockIndex();
            int blockEndAddr = blockStartAddr + runningProcess->getMemorySize() -1;

            // Prepare the entry and add to the vector
            std::ostringstream entry;
            entry << blockEndAddr << "\n";
            entry << "P" << runningProcess->getPID() << "\n";
            entry << blockStartAddr << "\n\n";
            reportEntries.push_back(entry.str());

            // Debug output to verify process allocation in report
            /*std::cout << "Reporting running process " << runningProcess->getName() << " with PID "
                << runningProcess->getPID() << " from address " << blockStartAddr
                << " to " << blockEndAddr << "\n";*/
        }
    }

    reportFile << "Processes in Memory: " << processesInMemoryCount << "\n";
    reportFile << "External Fragmentation: " << externalFragmentation << " KB\n";
    reportFile << "\n----end---- = " << maxOverallMem << "\n\n";

    // Output entries in reverse order to match the descending format
    for (auto it = reportEntries.rbegin(); it != reportEntries.rend(); ++it) {
        reportFile << *it;
    }

    reportFile << "----start---- = 0\n";
    reportFile << "---------------------------------------------------\n";
    reportFile.close();

    // Update processes in memory count in the report
    //std::cout << "Processes in memory: " << processesInMemoryCount << "\n";
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

void MemoryManager::addToBackingStore(std::shared_ptr<Process> process) {
    std::ofstream ofs(MemoryManager::backingStoreFile, std::ios::app);
    if (ofs.is_open()) {
        ofs << serializeProcess(process) << "\n"; // Serialize process state
        //std::cout << "Process " << process->getName() << " added to backing store.\n";
    }
    else {
        std::cerr << "Error: Unable to open backing store file.\n";
    }
    ofs.close();
}

std::shared_ptr<Process> MemoryManager::fetchFromBackingStore() {
    std::ifstream ifs(MemoryManager::backingStoreFile);
    if (!ifs.is_open()) {
        std::cerr << "Error: Unable to open backing store file.\n";
        return nullptr;
    }

    std::string line;
    std::ostringstream remainingData;
    std::shared_ptr<Process> process = nullptr;

    if (std::getline(ifs, line)) {
        process = deserializeProcess(line); // Deserialize the first process
    }

    // Write remaining processes back to the file
    while (std::getline(ifs, line)) {
        remainingData << line << "\n";
    }
    ifs.close();

    std::ofstream ofs(MemoryManager::backingStoreFile, std::ios::out | std::ios::trunc);
    ofs << remainingData.str();
    ofs.close();

    if (process) {
        //std::cout << "Process " << process->getName() << " fetched from backing store.\n";
    }
    return process;
}

bool MemoryManager::isBackingStoreEmpty() const {
    std::ifstream ifs(MemoryManager::backingStoreFile);
    return ifs.peek() == std::ifstream::traits_type::eof();
}

std::string MemoryManager::serializeProcess(const std::shared_ptr<Process>& process) {
    // Example: Serialize process attributes as a comma-separated string
    std::ostringstream oss;
    oss << process->getPID() << "," << process->getName() << ","
        << process->getMemorySize() << "," << process->getCPUCoreID();
    return oss.str();
}

std::shared_ptr<Process> MemoryManager::deserializeProcess(const std::string& data) {
    // Example: Deserialize process attributes from a comma-separated string
    std::istringstream iss(data);
    std::string token;
    int id, memorySize, cpuCoreID;
    std::string name;

    if (std::getline(iss, token, ',')) id = std::stoi(token);
    if (std::getline(iss, token, ',')) name = token;
    if (std::getline(iss, token, ',')) memorySize = std::stoi(token);
    if (std::getline(iss, token, ',')) cpuCoreID = std::stoi(token);

    auto process = std::make_shared<Process>(id, name, memorySize);
    process->setCPUCoreID(cpuCoreID);
    return process;
}

void MemoryManager::setNumPagedIn(int numPagedIn) {
    this->numPagedIn = numPagedIn;
}

void MemoryManager::setNumPagedOut(int numPagedOut) {
    this->numPagedOut = numPagedOut;
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
void MemoryManager::initMemory(int totalMem, int frameSize, int procMemSize) {
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
            int remainingSize = block.size - processMemorySize;
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
int MemoryManager::getFreeMemory() const {
    int freeMemory = 0;
    for (const auto& block : memoryBlocks) {
        if (block.isFree) {
            freeMemory += block.size;
        }
    }
    return freeMemory;
}
int MemoryManager::getExternalFragmentation() const {
    int fragmentation = 0;
    for (const auto& block : memoryBlocks) {
        if (block.isFree && block.size < processMemorySize) {
            fragmentation += block.size;
        }
    }
    return fragmentation;
}
int MemoryManager::getProcessCount() const {
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
void MemoryManager::generateMemorySnapshot(int quantumNumber) {
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
bool MemoryManager::isContiguousBlockAvailable(int size) const {
    for (const auto& block : memoryBlocks) {
        if (block.isFree && block.size >= size) {
            return true;
        }
    }
    return false;
}
*/
