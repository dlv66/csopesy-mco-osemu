#pragma once
class IThread
{
public:
	IThread() = default;
	virtual ~IThread() = default;

	void start();
	static void sleep(int milliseconds);
	//virtual void stop() = 0;

protected:
	virtual void run() = 0;
};

