#include "ImageLoad.h"
#include "ImageManager.h"

namespace GameImage_M {

	void ImageLoad::LoadI(wstring p_path, string p_Id, Rect p_load, Rect p_render)
	{
		//파일 이름을 포인터에 저장
		my_loadImage = ImageManager::GetInstance()->GetImage(p_path);

		id = p_Id;
		loadRect = p_load;
		renderRect = p_render;
	}
	void ImageLoad::Update()
	{

	}
	void ImageLoad::Render(Graphics* grap)
	{
		if (my_loadImage != nullptr && grap != nullptr) {
			grap->DrawImage(my_loadImage.get(), renderRect,
				loadRect.X, loadRect.Y, 
				loadRect.Width, loadRect.Height, 
				UnitPixel);
		}
	}
	string ImageLoad::GetID()
	{
		return id;
	}
	void ImageLoad::Move(Rect p_renderMovePos)
	{
		if (p_renderMovePos.Width == -1 || p_renderMovePos.Height == -1) {
			renderRect.X = p_renderMovePos.X;
			renderRect.Y = p_renderMovePos.Y;
		}
		else
			renderRect = p_renderMovePos;
	}
}

