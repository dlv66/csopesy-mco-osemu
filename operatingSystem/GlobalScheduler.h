#pragma once
#include <memory>
#include <unordered_map>

#include "AScheduler.h"
#include "BaseScreen.h"
#include "ConsoleManager.h"
#include "Process.h"
#include "IThread.h"
#include "Initialize.h"

class GlobalScheduler : public IThread
{
public:

	typedef std::unordered_map<std::string, std::shared_ptr<Process>> ProcessTable;
	typedef std::unordered_map<std::string, std::shared_ptr<AScheduler>> SchedulerTable;

	static GlobalScheduler* getInstance();
	static void initialize(const Initialize& initConfig);
	static void destroy();

	void run() override;
	void runQuantum(int timeQuantum) override;
	void stop();

	void tick() const;
	void addProcessToProcessTable(std::shared_ptr<Process> process);
	void handleScreenLS() const;
	void handleReportUtil() const;
	void startSchedulerTestInBackground(int minIns, int maxIns);
	void handleSchedulerStop();
	void handleSchedulerTest(int minIns, int maxIns);

	bool isRunning() const;
	std::shared_ptr<AScheduler> scheduler;

	ProcessTable processTable;

private:
	GlobalScheduler(const Initialize& initConfig);
	~GlobalScheduler() = default;
	GlobalScheduler(GlobalScheduler const&) {};
	GlobalScheduler& operator=(GlobalScheduler const&) {};
	static GlobalScheduler* sharedInstance;

	SchedulerTable schedulerTable;
	bool running = true;
	bool batchScheduler = false;
};


