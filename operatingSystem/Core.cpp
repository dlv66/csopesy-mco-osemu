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

void Core::startProcess() {

	while(true)
	{
		if (this->process.state == Process::State::READY)
		{
			this->process.executeCommands();
		}

		if(this->process.state == Process::State::TERMINATED)
		{
			this->isBusy = false;
			break;
		}
	}
}