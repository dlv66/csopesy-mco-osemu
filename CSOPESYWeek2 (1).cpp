// Online C++ compiler to run C++ program online
#include <iostream>
#include <stdlib.h>
#include <cstdlib>
#include <string>

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