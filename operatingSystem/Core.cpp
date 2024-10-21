#include "Core.h"

#include <memory>
#include <Windows.h>

//Constructor
Core::Core(int coreID): IThread(){
	this->coreID = coreID;
}

void Core::update(bool isRunning)
{
	this->isRunning = isRunning;

}


// Sets the new process to the core and returns the terminated process
std::shared_ptr<Process> Core::setProcess(std::shared_ptr<Process> process) {
	if (!this->isRunning) {
		std::shared_ptr<Process> terminatedProcess = this->process;
		this->process = process;
		this->update(true);
		return terminatedProcess;
	}
}

void Core::run() {

	while(true)
	{
		if (this->process->getState() == Process::State::READY)
		{
			this->process->execute();
			this->update(true);
		}

		if(this->process-> getState() == Process::State::TERMINATED)
		{
			this->update(false);
			break;
		}
	}
}