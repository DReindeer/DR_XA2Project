//================================================================================
//
//	title : classMake[ sceneSkybox.h ]			Auter : KENSUKE WATANABE
//														Data  : 2017/04/27
//
//--------------------------------------------------------------------------------
#ifndef _SKYBOX_H_
#define _SKYBOX_H_

#include "main.h"
#include "scene3D.h"

//--------------------------------------------------------------------------------
// クラス
//--------------------------------------------------------------------------------
class CSceneSkybox : public CScene3D
{
public:
	CSceneSkybox(int priority = 1):CScene3D(priority) { m_classType = CLASSTYPE::SCENE_SKYDOME; }
	~CSceneSkybox() {}

	void Init();
	void Uninit();
	void Update();
	void Draw();
	void DrawDefferdRendering()override;	// デファードレンダリング用
	HRESULT SetVtx();

	static CSceneSkybox *Create(const D3DXVECTOR3 &pos, const D3DXVECTOR3 &rot, const float &size, const std::string &strFilePass, int pry = 4);

private:
	float m_moveTex;
	float m_size;
	int m_numVtx;
	int m_numIndex;
	
	LPDIRECT3DINDEXBUFFER9 m_pIdxBuffer;
};

#endif