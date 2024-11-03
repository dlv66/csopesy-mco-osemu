#pragma once
class IThread
{
public:
	IThread() = default;
	virtual ~IThread() = default;

	void start();
	void startQuantum(long long timeQuantum);
	static void sleep(int milliseconds);
	//virtual void stop() = 0;

protected:
	virtual void run() = 0;
	virtual void runQuantum(long long timeQuantum) = 0;
};

