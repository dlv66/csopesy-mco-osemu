#include "Process.h"

std::string Process::getTimePeriod(int hour) {
	return (hour >= 12) ? " PM" : " AM";
}

std::string Process::getFormattedDate() {
	std::string formattedDate;

	if (localtime_s(&this->date, &this->timestampCurrent) != 0) {
		return "Error: Unable to get local time";
	}
	else {
		// (MM/DD/YYYY, HH:MM:SS AM/PM) format
		formattedDate = std::to_string(date.tm_mon + 1) + "/" + std::to_string(date.tm_mday) + "/" + std::to_string(date.tm_year + 1900) + ", " +
			std::to_string(date.tm_hour % 12) + ":" + std::to_string(date.tm_min) + ":" + std::to_string(date.tm_sec) +
			this->getTimePeriod(date.tm_hour);
		return formattedDate;
	}
}

void Process::executeCommands() {
	this->state = Process::State::RUNNING;
	time(&this->timestampStart);

	// TODO: Run the process commands

	//once done
	this->state = Process::State::TERMINATED;
}