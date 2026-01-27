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
		//�̹� �ش� �̹����� �ε�Ǿ��ٸ� �ε�� �̹����� ��ȯ
		if (ImageCache.count(p_filepath))
			return ImageCache[p_filepath];

		//�ε���� ���� �̹����� ImageCache�� �߰�
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