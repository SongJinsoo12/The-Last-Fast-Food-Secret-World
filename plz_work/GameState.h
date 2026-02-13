#pragma once
#include "Windows.h"
#include <memory>
#include <vector>
#include "Singleton.h"
#include "CardManager.h"
#include "Timer.h"

#define STATE GameState_M::Context::GetInstance()

using namespace std;

namespace GameState_M {
	enum class E_InGameState
	{
		Lobby,
		Menu,
		DeckBuild,
		Stage,
		Shop,
		LuckyBox,
		InGame,
		InGameResult,
	};

	class State
	{
	public:
		//virtual ~State();

		virtual void Enter() = 0;
		virtual void Update(HDC p_hdc, HWND p_hwnd) = 0;
		virtual void Exit() = 0;
		void MonoGram(string p_image, string p_id, int p_x, int p_y);
	};

	class Context : public Singleton<Context>
	{
		friend class Singleton<Context>;

	private:
		shared_ptr<State> currentState;
		vector<shared_ptr<State>> StateVector;

	public:
		Context();

	public:
		void Init();
		void ChangeState(E_InGameState inputState);

		void Update(HDC p_hdc, HWND p_hwnd);
	};

	class Lobby :public State {
	public:
		void Enter() override;
		void Update(HDC p_hdc, HWND p_hwnd) override;
		void Exit() override;
	};

	class Menu :public State {
	public:
		void Enter() override;
		void Update(HDC p_hdc, HWND p_hwnd) override;
		void Exit() override;
	};

	class Stage :public State {
	public:
		void Enter() override;
		void Update(HDC p_hdc, HWND p_hwnd) override;
		void Exit() override;
	};

	class DeckBuild : public State {
	public:
		void Enter() override;
		void Update(HDC p_hdc, HWND p_hwnd) override;
		void Exit() override;
	};

	class Shop :public State {
	private:
		WCHAR text[128];
	public:
		void Enter() override;
		void Update(HDC p_hdc, HWND p_hwnd) override;
		void Exit() override;
	};

	class LuckyBox :public State {
	public:
		void Enter() override;
		void Update(HDC p_hdc, HWND p_hwnd) override;
		void Exit() override;
	};

	class InGame : public State {
	public:
		void Enter() override;
		void Update(HDC p_hdc, HWND p_hwnd) override;
		void Exit() override;

		void PlayAnimation(Timer& p_timer, Animation& p_effect, bool* p_isPlay);
	private:
		CardManager m_player, m_boss;
		Timer m_shiny, m_rip, m_skill, m_def;
		Animation m_shinyEffect, m_ripEffect, m_skillEffect, m_defEffect;
	};

	class InGameResult :public State {
	public:
		void Enter() override;
		void Update(HDC p_hdc, HWND p_hwnd) override;
		void Exit() override;
	};
}


