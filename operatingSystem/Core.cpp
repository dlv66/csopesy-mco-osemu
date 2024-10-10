#include "Core.h"
#include <Windows.h>

bool Core::setProcess(Process process) {
	if (this->isBusy) {
		return false;
	}
	else
	{
		process.state = Process::State::TERMINATED;
		this->process = process;
		this->isBusy = true;
		return true;
	}
}

void Core::checkProcessStatus() {
	while(true)
	{
		// TODO: Run the process command
		if(this->process.state == Process::State::RUNNING)
		{
			this->isBusy = false;
			break;
		}

		Sleep(10);
	}
}