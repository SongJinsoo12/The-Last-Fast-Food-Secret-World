#include "Timer.h"

Timer::Timer() : m_time(0), m_isStart(false)
{
}

Timer::~Timer()
{
}

//타이머 시작
void Timer::StartTimer()
{
	m_start = chrono::system_clock::now();
}

//타이머 업데이트
void Timer::UpdateTimer()
{
	m_time = chrono::system_clock::now() - m_start;
}

//타이머 확인
bool Timer::CheckTimer(double p_time)
{
	if (m_time.count() >= p_time)
		return true;
	else
		return false;
}

bool Timer::GetIsStart()
{
	return m_isStart;
}

void Timer::SetIsStart(bool p_isStart)
{
	m_isStart = p_isStart;
}
