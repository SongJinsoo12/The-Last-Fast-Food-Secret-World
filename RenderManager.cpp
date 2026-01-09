#include "RenderManager.h"
#include <iostream>

namespace GameImage_M {
	RenderManager::~RenderManager()
	{
		m_RenderList.clear();
		ImageManager::GetInstance()->Release();
	}
	void RenderManager::SetImage(wstring p_path, string p_id, Rect load, Rect render)
	{
		for (vector<shared_ptr<ImageLoad>>::iterator it = m_RenderList.begin() ; 
			it != m_RenderList.end(); )
		{
			if ((*it)->GetID() == p_id) {
				cout << "ID Áßº¹ °ª Á¸Àç" << endl;
				return;
			}
			else
				++it;
		}

		shared_ptr<ImageLoad> newImage = make_shared<ImageLoad>();
		newImage->LoadI(p_path, p_id, load, render);
		m_RenderList.push_back(newImage);
	}
	void RenderManager::RenderAll(Graphics* grap)
	{
		for (size_t i = 0; i < m_RenderList.size(); i++)
		{
			shared_ptr<ImageLoad> p_Image = m_RenderList[i];
			if (p_Image != nullptr) {
				p_Image->Render(grap);
			}
		}
	}
	void RenderManager::MoveImage(string p_id, Rect renderPos)
	{
		for (size_t i = 0; i < m_RenderList.size(); i++)
		{
			if (m_RenderList[i]->GetID() == p_id) {
				m_RenderList[i]->Move(renderPos);
				break;
			}
		}
	}
	void RenderManager::RemoveIamage(string p_id)
	{
		for (size_t i = 0; i < m_RenderList.size(); i++)
		{
			if (m_RenderList[i]->GetID() == p_id) {
				m_RenderList.erase(m_RenderList.begin() + i);
				break;
			}
		}
	}
}

