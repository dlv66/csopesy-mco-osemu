#include "FCFSScheduler.h"
#include "Process.h"
#include <iostream>

// TODO: Add a process to the queue
// STATUS: For testing; incomplete (why the need to state nCore?)
void FCFSScheduler::addProcess(const Process& process, int nCore = 0) {
    if(nCore >= 0 && nCore < nCores) { // if core number is valid
        processQueues[nCore].push_back(process); // add process to queue
    } else {
        std::cerr << "Invalid core number.\n";
    }
}

// TODO: Sort process based on remaining instructions
// STATUS: Wrong (no sorting needed, just when processes are pushed at the same time. In which case, choose random?)
void FCFSScheduler::sortProcesses() {
    for(int i = 0; i < processQueues.size(); i++) { // for all the processes in the queue
        std::sort(processQueues[i].begin(), processQueues[i].end(), [](const Process& a, const Process& b) {
            return a.totalLineOfInstruction - a.currentLineOfInstruction > b.totalLineOfInstruction - b.currentLineOfInstruction;
        });
    }
}

// TODO: Run scheduler (check each core if they have a process. If not, pop the next process from the queue and start work)
// STATUS: Incomplete
void FCFSScheduler::runFCFS() {
    while(!processQueues[0].empty()){

    }
}