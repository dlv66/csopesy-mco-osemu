#include "IThread.h"
#include <thread>
#include <chrono>


void IThread::start()
{
	std::thread thread(&IThread::run, this);
	thread.detach();
}

void IThread::sleep(int milliseconds)
{
	std::this_thread::sleep_for(std::chrono::milliseconds(milliseconds));
}