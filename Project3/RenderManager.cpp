#include "RenderManager.h"
#include <iostream>

namespace GameImage_M {
	RenderManager::RenderManager()
	{
		// 사이즈 설정
		m_RenderList.resize(static_cast<int>(LayerType::Max));
	}
	RenderManager::~RenderManager()
	{
		AllRemoveImage();
		ImageManager::GetInstance()->Release();
	}

	void RenderManager::SetImage(wstring p_path, string p_id, Rect load, Rect render
		, bool isVisible, LayerType p_layer)
	{
		if (m_IDMap.find(p_id) != m_IDMap.end()) return;

		shared_ptr<ImageLoad> newImage = make_shared<ImageLoad>();
		newImage->LoadI(p_path, p_id, load, render, isVisible);

		m_RenderList[(int)p_layer].push_back(newImage);
		m_IDMap[p_id] = p_layer;
	}

	void RenderManager::RenderAll(Graphics* grap)
	{
		for (int i = 0; i < (int)LayerType::Max; i++)
		{
			vector<shared_ptr<ImageLoad>>& currentLayer = m_RenderList[i];
			for (int j = 0; j < (int)currentLayer.size(); j++)
			{
				if (currentLayer[j]->GetVisible()) {
					currentLayer[j]->Render(grap);
				}
			}
		}
	}

	void RenderManager::ImageVisible(string p_id, bool isVisible)
	{
		shared_ptr<ImageLoad> img = FindImageID(p_id);
		if (img) img->IsVisibleImage(isVisible);
	}

	void RenderManager::MoveImage(string p_id, Rect renderPos)
	{
		shared_ptr<ImageLoad> img = FindImageID(p_id);
		if (img) img->Move(renderPos);
	}

	void RenderManager::RemoveIDIamage(string p_id)
	{
		if (m_IDMap.find(p_id) == m_IDMap.end()) return;

		LayerType layer = m_IDMap[p_id];
		vector<shared_ptr<ImageLoad>>& targetVec = m_RenderList[(int)layer];

		for (vector<shared_ptr<ImageLoad>>::iterator it= targetVec.begin(); 
			it != targetVec.end(); it++)
		{
			if ((*it)->GetID()==p_id){
				targetVec.erase(it);
				m_IDMap.erase(p_id);
				break;
			}
		}
	}

	void RenderManager::AllRemoveImage()
	{
		for (int i = 0; i < (int)LayerType::Max; i++)
		{
			m_RenderList[i].clear();
		}
		m_RenderList.clear();
		m_RenderList.shrink_to_fit();
		m_IDMap.clear();
	}

	shared_ptr<ImageLoad> RenderManager::FindImageID(string p_id)
	{
		if (m_IDMap.find(p_id) == m_IDMap.end()) return;

		LayerType layer = m_IDMap[p_id];
		vector<shared_ptr<ImageLoad>>& targetVec = m_RenderList[(int)layer];
		for (const shared_ptr<ImageLoad>& img : targetVec)
		{
			if (img->GetID() == p_id) 
				return img;
		}

		return nullptr;
	}

	void RenderManager::LayerMoveForward(string p_id)
	{
		if (m_IDMap.find(p_id) == m_IDMap.end()) return;
		vector<shared_ptr<ImageLoad>>& targetVec = m_RenderList[(int)m_IDMap[p_id]];

		for (int i = 0; i < (int)targetVec.size() - 1; i++)
		{
			if (targetVec[i]->GetID() == p_id) {
				swap(targetVec[i], targetVec[i + 1]);
				break;
			}
		}
	}

	void RenderManager::LayerMoveBackward(string p_id)
	{
		if (m_IDMap.find(p_id) == m_IDMap.end()) return;
		vector<shared_ptr<ImageLoad>>& targetVec = m_RenderList[(int)m_IDMap[p_id]];

		for (int i = 0; i < (int)targetVec.size() - 1; i++)
		{
			if (targetVec[i]->GetID() == p_id) {
				swap(targetVec[i], targetVec[i - 1]);
				break;
			}
		}
	}

	void RenderManager::LayerMoveToFont(string p_id)
	{
		if (m_IDMap.find(p_id) == m_IDMap.end()) return;
		LayerType layer = m_IDMap[p_id];
		vector<shared_ptr<ImageLoad>>& targetVec = m_RenderList[(int)layer];

		for (vector<shared_ptr<ImageLoad>>::iterator it=targetVec.begin();
			it != targetVec.end(); it++)
		{
			if ((*it)->GetID() == p_id) {
				shared_ptr<ImageLoad> temp = *it;
				targetVec.erase(it);
				targetVec.push_back(temp);
				break;
			}
		}
	}

	void RenderManager::LayerMoveToBack(string p_id)
	{
		if (m_IDMap.find(p_id) == m_IDMap.end()) return;
		LayerType layer = m_IDMap[p_id];
		vector<shared_ptr<ImageLoad>>& targetVec = m_RenderList[(int)layer];

		for (vector<shared_ptr<ImageLoad>>::iterator it = targetVec.begin();
			it != targetVec.end(); it++)
		{
			if ((*it)->GetID() == p_id) {
				shared_ptr<ImageLoad> temp = *it;
				targetVec.erase(it);
				targetVec.insert(targetVec.begin(), temp);
				break;
			}
		}
	}
}
