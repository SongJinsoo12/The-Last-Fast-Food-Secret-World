#pragma once

#include "ImageLoad.h"
#include "ImageManager.h"

namespace GameImage_M {

	class RenderManager
	{
	private:
		vector<shared_ptr<ImageLoad>> m_RenderList;

	public:
		~RenderManager();

	public:
		void SetImage(wstring p_path, string p_id, Rect load, Rect render, bool isVisible);
		void RenderAll(Graphics* grap);

		void ImageVisible(string p_id, bool isVisible);
		void MoveImage(string p_id, Rect renderPos);
		void RemoveIDIamage(string p_id);
		void AllRemoveImage();
	};
}

extern GameImage_M::RenderManager g_renderManager;

