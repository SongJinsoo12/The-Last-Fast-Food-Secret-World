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
	bool GetIsStart();
	void SetIsStart(bool p_isStart);
	int GetIndex();
	void SetIndex(int p_index);
	void PlusIndex();

private:
	chrono::system_clock::time_point m_start;
	chrono::duration<double>m_time;
	bool m_isStart;
	int m_index;
};

