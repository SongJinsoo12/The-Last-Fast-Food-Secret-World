#include "Timer.h"

Timer::Timer() : m_time(0), m_puaseTime(0), m_isStart(false), m_isPuase(false), m_index(0)
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

int Timer::GetIndex()
{
	return m_index;
}

void Timer::SetIndex(int p_index)
{
	m_index = p_index;
}

void Timer::PlusIndex()
{
	m_index++;
}

void Timer::Pause()
{
	if (!m_isPuase)
	{
		m_puase = chrono::system_clock::now();
		m_puaseTime = m_time;
		m_isPuase = true;
	}
}

void Timer::Resume()
{
	if (m_isPuase)
	{
		m_time = m_puaseTime;
		m_isPuase = false;
	}
	m_puaseTime = chrono::system_clock::now() - m_puase;
    m_start = m_start + std::chrono::duration_cast<std::chrono::system_clock::duration>(m_puaseTime);
}

void Timer::SetTime(double p_time)
{
	m_time = chrono::duration<double>(p_time);
}

bool Timer::GetIsPuase()
{
	return m_isPuase;
}
