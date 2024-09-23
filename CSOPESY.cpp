// Online C++ compiler to run C++ program online
#include <iostream>
#include <stdlib.h>
#include <cstdlib>
#include <string>
#include <ctime>

class process {
    public:
        std::string processName;
        int currentLineOfInstruction;
        int totalLineOfInstruction;
        time_t timestamp;
        struct tm * date;
    
        process(std::string processName, int currentLineOfInstruction, int totalLineOfInstruction){
            this->processName = processName;
            this->currentLineOfInstruction = currentLineOfInstruction;
            this->totalLineOfInstruction = totalLineOfInstruction;
            time(&this->timestamp);
        }

        std::string getTimePeriod(int hour){
            if(hour > 12){
                return " PM";
            } else {
                return " AM";
            }
        }
        
        std::string getFormattedDate(){

            std::string formattedDate;

            this->date = localtime(&this->timestamp);
            // (MM/DD/YYYY, HH:MM:SS AM/PM) format
            formattedDate = std::to_string(date->tm_mon + 1) + "/" + std::to_string(date->tm_mday) + "/" + std::to_string(date->tm_year + 1900) + ", " + 
                            std::to_string(date->tm_hour) + ":" + std::to_string(date->tm_min) + ":" + std::to_string(date->tm_sec) +
                            this->getTimePeriod(date->tm_hour);

            return formattedDate;
        }
};


int main() {
    int nClear = 0;
    
    // Header text
    std::cout << "  __  ___   __  ___  ___  ___  _  _ \n";
    std::cout << " / _)/ __) /  \\(  ,\\(  _)/ __)( \\/ ) \n";
    std::cout << "( (_ \\__ \\( () )) _/ ) _)\\__ \\ \\  / \n";
    std::cout << " \\__)(___/ \\__/(_)  (___)(___/(__/ \n\n";

    std::cout << "By MP Group 10: Dela Cruz, Estrera, Fernandez, Villamiel\n\n";
    
    while (nClear == 0) {
        // Write C++ code here
        std::string sInput = "test";
                                
        // Asking for text input
        std::cout << "Please type in a command: ";
        std::cin >> sInput;
        
        // Input validation and conditions
        if(sInput != "clear" && sInput != "exit")
        {
            if(sInput == "initialize" || sInput == "screen" || sInput == "scheduler-test" || sInput == "scheduler-stop" || sInput == "report-util")
            {
                std::cout << "'" + sInput + "'" + " command recognized. Doing something.\n\n";
            }
            else 
            {
                std::cout << "'" + sInput + "'" + " command NOT recognized. Please try again.\n\n";
            }
        } 
        else if(sInput == "clear")
        {
            system("CLS");
            main();
        } 
        else if(sInput == "exit")
        {
            nClear = 1;
        }
    };
     
    if(nClear == 1){
        exit(0); 
    }
}