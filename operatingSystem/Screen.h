#pragma once
#include <string>
#include <unordered_map>
#include "Process.h"

class Screen {
    public:
        // Constructor
        Screen(); 

        void handleScreenS(const std::string& sInput, std::unordered_map<std::string, Process>& processMap);
        void handleScreenR(const std::string& sInput, std::unordered_map<std::string, Process>& processMap);
		void handleScreenLS(std::unordered_map<std::string, Process>& processMap, FCFSScheduler scheduler);
};