#pragma once
#include "Process.h"

class Core
{
	public:
		Process process;
		bool isBusy = false;
		// Constructor
		Core() {}

		bool setProcess(Process process);
		void startProcess();
};

