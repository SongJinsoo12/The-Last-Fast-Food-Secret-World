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
	void Pause();
	void Resume();
	void SetTime(double p_time);
	bool GetIsPuase();

private:
	chrono::system_clock::time_point m_start, m_puase;
	chrono::duration<double>m_time, m_puaseTime;
	bool m_isStart, m_isPuase;
	int m_index;
};

