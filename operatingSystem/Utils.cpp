#include "Utils.h"

#include <ctime> 
#include <chrono>
#include <iomanip>
#include <sstream>

std::string convertTimestampToString(time_t now_time) {
    std::ostringstream oss;

    std::tm local_time;
    localtime_s(&local_time, &now_time);

    oss << std::put_time(&local_time, "%Y-%m-%d %H:%M:%S");

    return oss.str();
}

time_t getCurrentTimestamp() {
    return std::time(nullptr); // Returns the current time as time_t
}


