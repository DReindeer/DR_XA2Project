//================================================================================
//
//	title : classMake[ sceneFieldX.h ]			Auter : KENSUKE WATANABE
//												Data  : 2017/04/27
//
//--------------------------------------------------------------------------------
#ifndef _SCENEBOX_X_H_
#define _SCENEBOX_X_H_

#include "main.h"
#include <vector>
#include "scene3D.h"

//--------------------------------------------------------------------------------
// クラス
//--------------------------------------------------------------------------------
class CSceneBoxX : public CScene3D
{
public:

	CSceneBoxX(int priority = 4) :CScene3D(priority) { m_objType = OBJTYPE::MODEL; m_classType = CLASSTYPE::SCENE_SKYDOME; }
	~CSceneBoxX() {}

	void Init();
	void Uninit();
	void Update();
//	void Draw();
	void DrawDefferdRendering()override;	// デファードレンダリング用
	static CSceneBoxX *Create(const D3DXVECTOR3 &pos, const D3DXVECTOR3 &rot, const D3DXVECTOR3 &size, const std::string &strFilePass, int pry = 4);

	const D3DXVECTOR3 &GetCenterPos() { return m_centerPos; }
private:

	D3DXVECTOR3 m_centerPos;
};

#endif
