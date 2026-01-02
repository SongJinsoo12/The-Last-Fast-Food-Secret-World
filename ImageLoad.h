#pragma once
#include <Windows.h>
#include <gdiplus.h>
#include <unordered_map>
#include <memory>
#include <string>

#pragma comment(lib, "Gdiplus.lib")

using namespace Gdiplus;
using namespace std;

namespace  GameImage_M {

	class Compoment {
	public:
		virtual void Update() = 0;
		virtual void Render(Graphics* grap) = 0;
	};

	class ImageLoad : public Compoment
	{
	protected:
		wstring path;
		string id;
		Rect loadRect;
		Rect renderRect;

		shared_ptr<Image> my_loadImage;
	public:

		void LoadI(wstring p_path, string p_Id, Rect p_load, Rect p_render);

		void Update() override;
		void Render(Graphics* grap) override;

		string GetID();
		void Move(Rect p_renderMovePos);
	};
}

