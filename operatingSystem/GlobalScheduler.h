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
	void runQuantum(long long timeQuantum) override;
	void stop();

	void tick() const;
	void tickGlobal();
	void addProcessToProcessTable(std::shared_ptr<Process> process);
	void handleScreenLS() const;
	void handleReportUtil() const;
	void startSchedulerTestInBackground(long long minIns, long long maxIns, long long batchProcessFreq);
	void handleSchedulerStop();
	void handleSchedulerTest(long long minIns, long long maxIns, long long batchProcessFreq);
	void getCPUUtilization() const;
	void getCPUUtilizationReport(std::ofstream& reportUtilFile) const;
	void incrementProcessID();

	bool isRunning() const;
	std::shared_ptr<AScheduler> scheduler;

	ProcessTable processTable;

	int processID = 0;
	int globalCPUCycles = 0;

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


