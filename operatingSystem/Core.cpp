#include "Core.h"
#include <Windows.h>

bool Core::setProcess(Process process) {
	if (this->isBusy) {
		return false;
	}
	else
	{
		this->process = process;
		this->isBusy = true;
		return true;
	}
}

void Core::checkProcessStatus() {
	while(true)
	{
		// Run the process command
		if(this->process.state == Process::State::RUNNING)
		{
			this->isBusy = false;
			break;
		}

		Sleep(10);
	}
}