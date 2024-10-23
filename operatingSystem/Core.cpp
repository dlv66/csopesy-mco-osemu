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
void Core::setProcess(std::shared_ptr<Process> process) {
	if (!this->isRunning) {
		this->process = process;
		this->update(true);
	}
}

void Core::run() {

	while(true)
	{
		if(!this->process->isFinished())
		{
			this->process->execute();
		}
		else
		{
			this->update(false);
			this->terminatedProcess = this->process;
			this->process = nullptr;
			break;
			
		}

		this->process->update();
	}
}