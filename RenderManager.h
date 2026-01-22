#pragma once

#include "ImageLoad.h"
#include "ImageManager.h"
#include "Singleton.h"

#include <unordered_map>

namespace GameImage_M {

	enum class LayerType {
		Background=0,
		Field,
		Card,
		UI,
		Max,
	};

	class RenderManager : public Singleton<RenderManager>
	{
		friend class Singleton<RenderManager>;

	private:
		vector<vector<shared_ptr<ImageLoad>>> m_RenderList;
		unordered_map<string, LayerType> m_IDMap;

	public:
		RenderManager();
		virtual ~RenderManager();

	public:
		void SetImage(wstring p_path, string p_id, Rect load, Rect render
			, bool isVisible, LayerType p_layer);
		void RenderAll(Graphics* grap);

		void ImageVisible(string p_id, bool isVisible);
		void MoveImage(string p_id, Rect renderPos);
		void RemoveIDIamage(string p_id);
		void AllRemoveImage();

		shared_ptr<ImageLoad> FindImageID(string p_id);

		void LayerMoveForward(string p_id);
		void LayerMoveBackward(string p_id);
		void LayerMoveToFont(string p_id);
		void LayerMoveToBack(string p_id);
	};
}

