#pragma once
#include <string>
#include <unordered_map>
#include "Process.h"

class Screen{
    public:
        void handleScreenS(const std::string& sInput, std::unordered_map<std::string, Process>& processMap);
        void handleScreenR(const std::string& sInput, std::unordered_map<std::string, Process>& processMap);
};