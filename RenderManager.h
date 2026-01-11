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
		void SetImage(wstring p_path, string p_id, Rect load, Rect render);
		void RenderAll(Graphics* grap);

		void MoveImage(string p_id, Rect renderPos);
		void RemoveIamage(string p_id);
	};

}

extern GameImage_M::RenderManager g_renderManager;


