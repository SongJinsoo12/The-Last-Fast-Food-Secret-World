#include <Windows.h>
#include <gdiplus.h>
#include <unordered_map>
#include <memory>
#include <string>
#pragma comment(lib, "Gdiplus.lib")

using namespace Gdiplus;
using namespace std;

class ImageManager {
private:
	unordered_map<wstring, shared_ptr<Image>> ImageCache;
public:
	shared_ptr<Image> GetImage(const wstring& filepath);

	void ImageSort();
};

class RenderItem {
public:
	wstring path;
	int x, y, w, h;
};

class ImageTool {
private:
	ULONG_PTR gdiToken;
	ImageManager imgManager;
	vector<RenderItem> items;

public:
	ImageTool();
	~ImageTool();

	void AddObject(const wstring& path, int x, int y, int w, int h);
	void Render(HDC hdc, RECT rect, HWND hwnd);

	RenderItem FindItem(const wstring& path)
	{
		RenderItem item;
		for (int i = 0; i < items.size(); i++)
		{
			if (items[i].path == path)
			{
				item = items[i];
				return item;
			}
		}
	}

	void RemoveObject(const wstring& path)
	{
		for (int i = 0; i < items.size(); i++)
		{
			if (items[i].path == path)
			{
				items.erase(items.begin() + i);
			}
		}
	}

	void MoveObject(const wstring& path, int dx, int dy)
	{
		RenderItem item = FindItem(path);
		RemoveObject(item.path);
		AddObject(item.path, item.x + dx, item.y + dy, item.w, item.h);
	}
};


