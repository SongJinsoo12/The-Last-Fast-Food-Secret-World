#pragma once

#include <Windows.h>
#include <gdiplus.h>
#include <unordered_map>
#include <memory>
#include <string>

#pragma comment(lib, "Gdiplus.lib")

using namespace Gdiplus;
using namespace std;

//���� �̸��� ���� �̹��� ����
namespace GameImage_M {
	class ImageManager
	{
	private:
		static ImageManager* Image_Instance;

	public:
		static ImageManager* GetInstance() {
			if (Image_Instance == nullptr) {
				Image_Instance = new ImageManager;
			}

			return Image_Instance;
		}
		void Release();

	private:
		ULONG_PTR m_GDIToken;
		unordered_map<wstring, shared_ptr<Image>> ImageCache;

	public:
		shared_ptr<Image> GetImage(const wstring& p_filepath);

		ImageManager();
		virtual ~ImageManager();
	};
}
