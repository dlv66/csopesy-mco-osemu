#include "FCFSScheduler.h"
#include <iostream>

FCFSScheduler::FCFSScheduler(int nCores) {
    this->nCores = nCores;
    this->processQueues = std::vector<Process>(nCores);
}

// TODO: Instantiates core list based on given number of cores
// STATUS: For testing
void FCFSScheduler::instantiateCoreList() {
    for(int i = 0; i < nCores; i++) { // for all the cores
        coreList.push_back(Core()); // add core to the list
    }
}

// TODO: Add a process to the queue
// STATUS: For testing
void FCFSScheduler::addProcess(const Process& process) {
    processQueues.push_back(process); // add process to queue
}

/* Commented out for now since no need to sort
// TODO: Sort process based on remaining instructions
// STATUS: Wrong (no sorting needed, just when processes are pushed at the same time. In which case, choose random?)
void FCFSScheduler::sortProcesses() {
    for(int i = 0; i < processQueues.size(); i++) { // for all the processes in the queue
        std::sort(processQueues[i].begin(), processQueues[i].end(), [](const Process& a, const Process& b) {
            return a.totalLineOfInstruction - a.currentLineOfInstruction > b.totalLineOfInstruction - b.currentLineOfInstruction;
        });
    }
}
*/

// TODO: Run scheduler
// STATUS: For testing
void FCFSScheduler::runFCFS() {
    while(!processQueues.empty()){
        for(int i = 0; i < nCores; i++) { // for all the cores
            if(coreList[i].setProcess(processQueues[0]) == true) {
                coreList[i].startProcess(); // start the process
                processQueues.erase(processQueues.begin()); // pop the process from the front of the queue
            }
        }
    }
}