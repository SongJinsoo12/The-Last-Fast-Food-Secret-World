#include "RenderLayer.h"

namespace GameImage_M {
	void RenderLayer::AddImage(shared_ptr<ImageLoad> img)
	{
		m_ImageLayers.push_back(img);
	}
}
