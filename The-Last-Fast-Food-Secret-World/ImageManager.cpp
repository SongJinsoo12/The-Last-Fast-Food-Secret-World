#include "ImageManager.h"
#include "ImageLoad.h"

namespace GameImage_M {
	ImageManager* ImageManager::Image_Instance = nullptr;

	void ImageManager::Release()
	{
		if (Image_Instance != nullptr)
			delete Image_Instance;

		Image_Instance = nullptr;
	}
	shared_ptr<Image> ImageManager::GetImage(const wstring& p_filepath)
	{
		//이미 해당 이미지가 로드되었다면 로드된 이미지로 반환
		if (ImageCache.count(p_filepath)) 
			return ImageCache[p_filepath];

		//로드되지 않은 이미지는 ImageCache에 추가
		shared_ptr<Image> newImage = make_shared<Image>(p_filepath.c_str());
		if (newImage->GetLastStatus() == Ok) {
			ImageCache[p_filepath] = newImage;
			return newImage;
		}

		return nullptr;
	}
	ImageManager::ImageManager()
	{
		GdiplusStartupInput si;
		GdiplusStartup(&m_GDIToken, &si, nullptr);
	}
	ImageManager::~ImageManager() 
	{
		ImageCache.clear();
		GdiplusShutdown(m_GDIToken);
	}
}
