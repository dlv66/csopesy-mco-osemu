#include <iostream>
#include <thread>
#include <chrono>
#include "Process.h"

std::string Process::getTimePeriod(int hour) const {  // Mark as const
    return (hour >= 12) ? " PM" : " AM";
}


std::string Process::getFormattedDate() const {
    std::string formattedDate;

    std::tm localTime;
    if (localtime_s(&localTime, &this->timestampCurrent) != 0) {
        return "Error: Unable to get local time";
    }
    else {
        // (MM/DD/YYYY, HH:MM:SS AM/PM) format
        formattedDate = std::to_string(localTime.tm_mon + 1) + "/" + std::to_string(localTime.tm_mday) + "/" + std::to_string(localTime.tm_year + 1900) + ", " +
            std::to_string(localTime.tm_hour % 12) + ":" + std::to_string(localTime.tm_min) + ":" + std::to_string(localTime.tm_sec) +
            this->getTimePeriod(localTime.tm_hour);
        return formattedDate;
    }
}




void Process::executeCommands() {
    this->state = Process::State::RUNNING;

    // Simulate executing 100 print commands
    for (int i = 0; i < this->totalLineOfInstruction; i++) {
        this->currentLineOfInstruction = i + 1;

        // Store the output string
        std::string commandOutput = processName +" Executing instruction " + std::to_string(currentLineOfInstruction) + " of " + std::to_string(totalLineOfInstruction);
        output.push_back(commandOutput);

        // Optionally print to the console (you can remove this if you don't want console output)
        std::cout << commandOutput << "\n";

        std::this_thread::sleep_for(std::chrono::milliseconds(50));  // Small delay for each command
    }

    this->state = Process::State::TERMINATED;
}
