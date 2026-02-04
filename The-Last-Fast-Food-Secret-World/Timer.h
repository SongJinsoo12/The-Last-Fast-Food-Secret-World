#pragma once
#include <chrono>

using namespace std;

class Timer
{
public:
	Timer();
	~Timer();
	void StartTimer();
	void UpdateTimer();
	bool CheckTimer(double p_time);

private:
	chrono::system_clock::time_point m_start;
	chrono::duration<double>m_time;
};

