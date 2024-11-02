#include "Utils.h"

#include <ctime> 
#include <chrono>
#include <iomanip>
#include <sstream>

std::string convertTimestampToString(time_t now_time) {
    if (now_time == 0) { // time_t 0 corresponds to "1970-01-01 00:00:00" in UTC
        return "N/A";
    }

    std::ostringstream oss;

    std::tm local_time;
    localtime_s(&local_time, &now_time);

    oss << std::put_time(&local_time, "%Y-%m-%d %H:%M:%S");

    return oss.str();
}

time_t getCurrentTimestamp() {
    return std::time(nullptr); // Returns the current time as time_t
}


