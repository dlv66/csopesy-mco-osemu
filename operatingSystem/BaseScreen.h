#pragma once
#include <memory>

#include "AConsole.h"
#include "Process.h"

class BaseScreen : public AConsole
{
public:
	BaseScreen(std::shared_ptr<Process> process, std::string processName);
	void onEnabled() override;
	void process() override;
	void display() override;

private:
	void printProcessInfo() const;
	std::shared_ptr<Process> attachedProcess;
	bool refreshed = false;
};

