//================================================================================
//
//	title : ポリゴン表示プログラム[ CScene3DPolyPoly.h ]	Auter : KENSUKE WATANABE
//													Data  : 2016/05/30
//
//--------------------------------------------------------------------------------
#ifndef _SCENE3DPOLY_H_
#define _SCENE3DPOLY_H_

#include "main.h"
#include <string>
#include "scene3D.h"

//--------------------------------------------------------------------------------
// クラス
//--------------------------------------------------------------------------------
class CScene3DPoly : public CScene3D
{
public:
	CScene3DPoly(int priority = 4) :CScene3D(priority){}
	~CScene3DPoly() {}

	void Init();
	void Uninit();
	void Update();
//	void Draw();
	void DrawDefferdRendering()override;	// デファードレンダリング用
	static CScene3DPoly *CScene3DPoly::Create(const D3DXVECTOR3 &pos, const D3DXVECTOR3 &rot, const D3DXVECTOR3 &size, const std::string &strFilePass, int priority = 4);

private:
};

#endif
