//================================================================================
//
//	Auter : KENSUKE WATANABE
//	Data  : 2017/09/21
//
//--------------------------------------------------------------------------------
#ifndef _MINIMAP_H_
#define _MINIMAP_H_

#include "main.h"
#include <vector>
#include "sceneFieldX.h"

//--------------------------------------------------------------------------------
// ƒNƒ‰ƒX
//--------------------------------------------------------------------------------
class CScene2D;
class CScene2DCircleMap;

class CMiniMap
{
public:

	CMiniMap() {}
	~CMiniMap() {}

	void Init();
	void Uninit();
	void Update();
	void Draw();
	static CMiniMap *Create(const D3DXVECTOR2 &pos, const float &rot, const float &len, const D3DXVECTOR2 &mapWorldSize, std::string mapFilePass);

private:

	std::string m_filePass;
	D3DXVECTOR2 m_pos;
	float m_len;
	D3DXVECTOR2 m_wToMap;
	float m_rot;

	CScene2D *m_pPlayerPoint;
	CScene2DCircleMap *m_pMap; 
	CScene2DCircleMap *m_pMapFrame;
};

#endif

