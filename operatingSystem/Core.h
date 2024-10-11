#pragma once
#include "Process.h"

class Core
{
	public:
		Process process;
		bool isBusy = false;
		int coreID;
		
		// Constructor
		Core(int coreID);

		Process setProcess(Process process);
		void startProcess();
};

