#include "Animation.h"

Animation::Animation() : m_index(0), m_x(0), m_y(0), m_width(0), m_height(0)
{
}

Animation::~Animation()
{
}


int Animation::GetIndex()
{
	return m_index;
}

void Animation::SetIndex(int p_index)
{
	m_index = p_index;
}

int Animation::GetImageX()
{
	return m_x;
}

int Animation::GetImageY()
{
	return m_y;
}

int Animation::GetImageWidth()
{
	return m_width;
}

int Animation::GetImageHeight()
{
	return m_height;
}

void Animation::SetImageSize(int p_x, int p_y, int p_width, int p_height)
{
	m_x = p_x;
	m_y = p_y;
	m_width = p_width;
	m_height = p_height;
}

string Animation::GetId()
{
	return m_id;
}

void Animation::SetId(string p_id)
{
	m_id = p_id;
}