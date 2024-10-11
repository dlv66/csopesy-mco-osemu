#pragma once
#include "Process.h"

class Core
{
	public:
		Process process;
		bool isBusy = false;
		
		// Constructor
		Core() {}

		Process setProcess(Process process);
		void startProcess();
};

