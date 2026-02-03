//#include "Gdi.h"
//
//shared_ptr<Image> ImageManager::GetImage(const wstring& filepath) {
//	unordered_map<wstring, shared_ptr<Image>>::iterator it = ImageCache.find(filepath);
//
//	if (it != ImageCache.end()) {
//		return it->second;
//	}
//
//	shared_ptr<Image> newImage = make_shared<Image>(filepath.c_str());
//	if (newImage->GetLastStatus() == Ok) {
//		ImageCache[filepath] = newImage;
//		return newImage;
//	}
//	return nullptr;
//}
//
//void ImageManager::ImageSort() {
//	unordered_map<wstring, shared_ptr<Image>>::iterator it = ImageCache.begin();
//	while (it != ImageCache.end()) {
//		if (it->second.use_count() <= 1) {
//			it = ImageCache.erase(it);
//		}
//		else {
//			++it;
//		}
//	}
//}
//
//ImageTool::ImageTool() {
//	GdiplusStartupInput si;
//	GdiplusStartup(&gdiToken, &si, nullptr);
//}
//
//ImageTool::~ImageTool() {
//	GdiplusShutdown(gdiToken);
//}
//
//void ImageTool::AddObject(const wstring& Path, int x, int y, int w, int h) {
//	RenderItem item;
//	item.path = Path; item.x = x; item.y = y; item.w = w;item.h = h;
//	items.push_back(item);
//}
//
//void ImageTool::Render(HDC hdc, RECT rect, HWND hwnd) {
//	int width = rect.right - rect.left;
//	int height = rect.bottom - rect.top;
//
//	HDC memDC = CreateCompatibleDC(hdc);
//	HBITMAP hMemBitmap = CreateCompatibleBitmap(hdc, width, height);
//	HBITMAP hOldBitmap = (HBITMAP)SelectObject(memDC, hMemBitmap);
//
//	HBRUSH hBackground = CreateSolidBrush(RGB(255, 255, 255));
//	FillRect(memDC, &rect, hBackground);
//	DeleteObject(hBackground);
//
//	{
//		Graphics graphics(memDC);
//		graphics.SetInterpolationMode(InterpolationModeHighQualityBicubic);
//
//		for (size_t i = 0; i < items.size(); ++i)
//		{
//			const RenderItem& item = items[i];
//			shared_ptr<Image>img = imgManager.GetImage(item.path);
//
//			if (img != nullptr) {
//				graphics.DrawImage(img.get(), item.x, item.y, item.w, item.h);
//			}
//		}
//	}
//
//	BitBlt(hdc, 0, 0, width, height, memDC, 0, 0, SRCCOPY);
//
//	SelectObject(memDC, hOldBitmap);
//	DeleteObject(hMemBitmap);
//	DeleteDC(memDC);
//}
//
//void ImageTool::RemoveObject(const wstring& Path) {
//	//
//}