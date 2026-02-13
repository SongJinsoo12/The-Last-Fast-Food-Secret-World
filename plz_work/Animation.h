#pragma once

#include <string>
using namespace std;

class Animation
{
public:
	Animation();
	~Animation();

	int GetIndex();
	void SetIndex(int p_index);
	int GetImageX(); int GetImageY(); int GetImageWidth(); int GetImageHeight();
	void SetImageSize(int p_x, int p_y, int p_width, int p_height);
	string GetId();
	void SetId(string p_id);

private:
	int m_index;
	int m_x, m_y, m_width, m_height;
	string m_id;
};