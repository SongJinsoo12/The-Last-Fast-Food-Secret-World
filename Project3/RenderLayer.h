#pragma once
#include <vector>
#include "ImageLoad.h"

namespace GameImage_M {
	class RenderLayer
	{
	private:
		vector < shared_ptr<ImageLoad>> m_ImageLayers;

		//우선순위 값
		int m_Priority;

	public:
		RenderLayer(int p_priority) : m_Priority(p_priority){}

		void AddImage(shared_ptr<ImageLoad> img);
	};
}

