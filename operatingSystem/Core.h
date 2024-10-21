#pragma once
#include <memory>

#include "IThread.h"
#include "Process.h"

class Core : public IThread
{
public:

	// Constructor
	Core(int coreID);

	std::shared_ptr<Process> process;
	int coreID;

	void update(bool isRunning);

	void run() override;

	std::shared_ptr<Process> setProcess(std::shared_ptr<Process> process);
private:
	bool isRunning = false;
};

