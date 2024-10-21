#pragma once
#include <memory>
#include <unordered_map>

#include "AScheduler.h"
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

	bool isRunning() const;
	std::shared_ptr<AScheduler> scheduler;

private:
	GlobalScheduler();
	~GlobalScheduler() = default;
	GlobalScheduler(GlobalScheduler const&) {};
	GlobalScheduler& operator=(GlobalScheduler const&) {};
	static GlobalScheduler* sharedInstance;

	ProcessTable processTable;
	ProcessTable activeProcessesTable;
	ProcessTable terminatedProcessesTable;

	SchedulerTable schedulerTable;
	bool running = true;
};


