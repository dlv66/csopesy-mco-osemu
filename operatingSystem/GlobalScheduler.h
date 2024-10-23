#pragma once
#include <memory>
#include <unordered_map>

#include "AScheduler.h"
#include "BaseScreen.h"
#include "ConsoleManager.h"
#include "Process.h"
#include "IThread.h"

class GlobalScheduler : public IThread
{
public:

	typedef std::unordered_map<std::string, std::shared_ptr<Process>> ProcessTable;
	typedef std::unordered_map<std::string, std::shared_ptr<AScheduler>> SchedulerTable;

	static GlobalScheduler* getInstance();
	static void initialize();
	static void destroy();

	void run() override;
	void stop();

	void tick() const;
	void addProcessToProcessTable(std::shared_ptr<Process> process);
	void addProcessToProcessTableNoCout(std::shared_ptr<Process> process);
	void handleScreenLS() const;
	void startSchedulerTestInBackground();
	void handleSchedulerStop();
	void handleSchedulerTest();

	bool isRunning() const;
	std::shared_ptr<AScheduler> scheduler;

	ProcessTable processTable;

private:
	GlobalScheduler();
	~GlobalScheduler() = default;
	GlobalScheduler(GlobalScheduler const&) {};
	GlobalScheduler& operator=(GlobalScheduler const&) {};
	static GlobalScheduler* sharedInstance;

	SchedulerTable schedulerTable;
	bool running = true;
	bool batchScheduler = false;
};


