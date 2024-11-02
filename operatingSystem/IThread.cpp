#include "IThread.h"
#include <thread>
#include <chrono>


void IThread::start()
{
	std::thread thread(&IThread::run, this); // TODO: May need refactoring to accomodate FCFS and RR?
	thread.detach();
}

void IThread::startQuantum(int timeQuantum)
{
	std::thread thread([this, timeQuantum]() { runQuantum(timeQuantum); });
	thread.detach();
}

void IThread::sleep(int milliseconds)
{
	std::this_thread::sleep_for(std::chrono::milliseconds(milliseconds));
}