#pragma once

#include "ImageLoad.h"
#include "ImageManager.h"
#include "Singleton.h"

namespace GameImage_M {

	class RenderManager : public Singleton<RenderManager>
	{
		friend class Singleton<RenderManager>;

	private:
		vector<shared_ptr<ImageLoad>> m_RenderList;

	//private:
	//	virtual ~RenderManager();

	public:
		virtual ~RenderManager();

	public:
		void SetImage(wstring p_path, string p_id, Rect load, Rect render, bool isVisible);
		void RenderAll(Graphics* grap);

		void ImageVisible(string p_id, bool isVisible);
		void MoveImage(string p_id, Rect renderPos);
		void RemoveIDIamage(string p_id);
		void AllRemoveImage();
	};
}
