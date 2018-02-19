//================================================================================
//
//	title : classMake[ sceneSkyDome.h ]			Auter : KENSUKE WATANABE
//														Data  : 2017/04/27
//
//--------------------------------------------------------------------------------
#ifndef _SPHERE_H_
#define _SPHERE_H_

#include "main.h"
#include "scene3D.h"

#define SKY_DOME_R	( 1600.f )	// ドームの直径
//--------------------------------------------------------------------------------
// クラス
//--------------------------------------------------------------------------------
class CSceneSkydome : public CScene3D
{
public:
	CSceneSkydome(int priority = 4):CScene3D(priority) { m_classType = CLASSTYPE::SCENE_SKYDOME; }
	~CSceneSkydome() {}

	void Init();
	void Uninit();
	void Update();
	void Draw();
	HRESULT SetVtx(int slices, int stacks);

	static CSceneSkydome *Create(const D3DXVECTOR3 &pos, const D3DXVECTOR3 &rot, const D3DXVECTOR3 &scl, const std::string &strFilePass, int pry = 4);

private:
	D3DXVECTOR3 m_move;
	float m_moveTex;
	int m_numVtx;
	int m_numIndex;
	
	LPDIRECT3DINDEXBUFFER9 m_pIdxBuffer;
};

#endif