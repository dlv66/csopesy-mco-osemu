#include "Utils.h"

#include <ctime> 
#include <chrono>
#include <iomanip>
#include <sstream>

std::string getCurrentTimestampString() {
    std::ostringstream oss;
    // Get the current time
    auto now = std::chrono::system_clock::now();
    std::time_t now_time = std::chrono::system_clock::to_time_t(now);

    std::tm local_time;
    localtime_s(&local_time, &now_time);

    oss << std::put_time(&local_time, "%Y-%m-%d %H:%M:%S");

    return oss.str();
}

time_t getCurrentTimestamp() {
    return std::time(nullptr); // Returns the current time as time_t
}


