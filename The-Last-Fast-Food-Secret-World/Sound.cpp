#include "Sound.h"

Sound::Sound()
{
}

Sound::~Sound()
{
}

void Sound::InitSound()
{
	//엔진 시동
	m_soLoud.init();

	//소리 파일 업로드
	//m_sfxInfo.load()
	//m_bgm.load()
	//m_bgm.setLooping(true) 반복재생 가능 

}

void Sound::PlayBgm()
{
	//예시
	/*int handle = m_soLoud.play(m_bgm);
	m_soLoud.setVolume(handle, 0.3f);*/
}

void Sound::EndSound()
{
	//엔진 끄기
	m_soLoud.deinit();
}

