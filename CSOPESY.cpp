#include <iostream>
#include <stdlib.h>
#include <cstdlib>
#include <string>
#include <ctime>
#include <unordered_map>

class process {
public:
    std::string processName;
    int currentLineOfInstruction;
    int totalLineOfInstruction;
    time_t timestamp;
    struct tm* date;

    // Default constructor
    process() : processName("Unnamed Process"), currentLineOfInstruction(0), totalLineOfInstruction(0) {
        time(&this->timestamp);
    }

    // Custom constructor
    process(std::string processName, int currentLineOfInstruction, int totalLineOfInstruction) {
        this->processName = processName;
        this->currentLineOfInstruction = currentLineOfInstruction;
        this->totalLineOfInstruction = totalLineOfInstruction;
        time(&this->timestamp);
    }

    std::string getTimePeriod(int hour) {
        return (hour >= 12) ? " PM" : " AM";
    }

    std::string getFormattedDate() {
        std::string formattedDate;

        if ((&this->timestamp) == nullptr) {
            return "Error: Unable to get local time";
        }
        else {
            this->date = localtime(&this->timestamp);
            // (MM/DD/YYYY, HH:MM:SS AM/PM) format
            formattedDate = std::to_string(date->tm_mon + 1) + "/" + std::to_string(date->tm_mday) + "/" + std::to_string(date->tm_year + 1900) + ", " +
                std::to_string(date->tm_hour % 12) + ":" + std::to_string(date->tm_min) + ":" + std::to_string(date->tm_sec) +
                this->getTimePeriod(date->tm_hour);
            return formattedDate;
        }
    }
};

std::unordered_map<std::string, process> processMap; // To store active processes

int main() {
    int nClear = 0;

    while (nClear == 0) {
        // Header text
        std::cout << "  __  ___   __  ___  ___  ___  _  _ \n";
        std::cout << " / _)/ __) /  \\(  ,\\(  _)/ __)( \\/ ) \n";
        std::cout << "( (_ \\__ \\( () )) _/ ) _)\\__ \\ \\  / \n";
        std::cout << " \\__)(___/ \\__/(_)  (___)(___/(__/ \n\n";

        std::cout << "By MP Group 10: Dela Cruz, Estrera, Fernandez, Villamiel\n\n";

        // Asking for text input
        std::string sInput;
        std::cout << "Please type in a command: ";
        std::getline(std::cin, sInput); // changed to getline to capture the entire input including spaces

        // Input validation and conditions
        if (sInput != "clear" && sInput != "exit") {
            if (sInput == "initialize" || sInput == "scheduler-test" || sInput == "scheduler-stop" || sInput == "report-util") {
                std::cout << "'" + sInput + "'" + " command recognized. Doing something.\n\n";
            }
            else if (sInput.find("screen -s") == 0) {
                system("CLS");
                std::string sCommand = "screen -s";
                std::string processName;

                // Get the process name
                int pos = sInput.find(sCommand);
                processName = sInput.substr(pos + 10); // Extracting process name

                // Get the current line of instruction
                int currentLine = rand() % 1000;

                // Get the total line of instruction
                int totalLine;
                do {
                    totalLine = rand() % 1000;
                } while (totalLine < currentLine);

                // Instantiate the process object
                process newProcess(processName, currentLine, totalLine);
                processMap[processName] = newProcess; // Store the process in the map

                // Print the process details
                std::cout << "Process Name: " + newProcess.processName + "\n\n";
                std::cout << "Current instruction line: " << newProcess.currentLineOfInstruction << "\n";
                std::cout << "Lines of code: " << newProcess.totalLineOfInstruction << "\n";
                std::cout << "Timestamp: " << newProcess.getFormattedDate() << "\n\n";

                // Command loop for the screen session
                std::string screenInput;
                while (true) {
                    std::cout << "Enter a command ('exit' to return to the main menu): ";
                    std::getline(std::cin, screenInput);
                    if (screenInput == "exit") {
                        break;  // Break out of the inner loop to return to the main menu
                    }
                    else {
                        std::cout << "'" << screenInput << "' command is not recognized.\n";
                    }
                }

                system("CLS");
            }
            else if (sInput.find("screen -r") == 0) {
                std::string sCommand = "screen -r";
                std::string processName;

                // Get the process name
                int pos = sInput.find(sCommand);
                processName = sInput.substr(pos + 10); // Extracting process name

                // Check if the process exists
                if (processMap.find(processName) != processMap.end()) {
                    process existingProcess = processMap[processName];

                    // Print the process details
                    std::cout << "Reattaching to process:\n";
                    std::cout << "Process Name: " + existingProcess.processName + "\n";
                    std::cout << "Current instruction line: " << existingProcess.currentLineOfInstruction << " / " << existingProcess.totalLineOfInstruction << "\n";
                    std::cout << "Timestamp: " << existingProcess.getFormattedDate() << "\n\n";
                }
                else {
                    std::cout << "No detached screen process found with the name '" << processName << "'.\n\n";
                }
            }
            else {
                std::cout << "'" + sInput + "'" + " command NOT recognized. Please try again.\n\n";
            }
        }
        else if (sInput == "clear") {
            system("CLS");
            main();
        }
        else if (sInput == "exit") {
            nClear = 1;
        }
    };

    if (nClear == 1) {
        exit(0);
    }
}
