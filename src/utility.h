
#ifndef _UTILITY_H_
#define _UTILITY_H_

#include "camera.h"
#include "manager.h"
#include "renderer.h"

class CUtility
{
public:
	// �X�N���[���ɑ΂�����Wor�T�C�Y��0.f~1.f�̊����Ŏw��
	static float RatioToScrX(const float &ratio) { return CManager::GetRenderer()->GetScreenSize().x * ratio; }
	static float RatioToScrY(const float &ratio) { return CManager::GetRenderer()->GetScreenSize().y * ratio; }
	static D3DXVECTOR2 RatioToScrXY(const D3DXVECTOR2 &ratio) { 
		return D3DXVECTOR2(CManager::GetRenderer()->GetScreenSize().x * ratio.x, CManager::GetRenderer()->GetScreenSize().y * ratio.y); 
	}

private:

};

#endif