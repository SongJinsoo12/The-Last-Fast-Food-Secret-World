#pragma once

#include "soloud.h"
#include "soloud_wav.h"
#include "soloud_wavstream.h"

using namespace SoLoud;

class Sound
{
public:
	Sound();
	virtual ~Sound();

	void InitSound();
	void PlayBgm();
	void EndSound();

	static Sound* Instance();

private:
	Soloud m_soLoud; //엔진
	Wav m_sfxInfo; //효과음
	WavStream m_bgm; //배경음악
protected:
	static Sound* m_Instance;
};

