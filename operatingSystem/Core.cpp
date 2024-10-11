#include "Core.h"
#include <Windows.h>

//Constructor
Core::Core(int coreID) {
	this->coreID = coreID;
}

// Sets the new process to the core and returns the terminated process
Process Core::setProcess(Process process) {
	if (!this->isBusy) {
		Process terminatedProcess = this->process;
		this->process = process;
		this->isBusy = true;
		return terminatedProcess;
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