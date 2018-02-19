//================================================================================
//
//	title : classMake[ sceneCylinder.h ]			Auter : KENSUKE WATANABE
//														Data  : 2017/05/12
//
//--------------------------------------------------------------------------------
#ifndef _SCENECYLINDER_H_
#define _SCENECYLINDER_H_

#include "main.h"
#include "scene3D.h"

//--------------------------------------------------------------------------------
// ƒNƒ‰ƒX
//--------------------------------------------------------------------------------
class CSceneCylinder : public CScene3D
{
public:
	CSceneCylinder(int priority = 4): CScene3D(priority){ m_classType = CLASSTYPE::SCENE_CYLINDER; }
	~CSceneCylinder() {}

	void Init();
	void Uninit();
	void Update();
	void Draw();
	HRESULT SetVtx(int stacks, int slices);

	static CSceneCylinder *Create(const D3DXVECTOR3 &pos, const D3DXVECTOR3 &rot, const D3DXVECTOR3 &size, const std::string &strFilePass);

	void SetStacks(const int stacks) { m_stacks = stacks; }
	void SetSlices(const int slices) { m_slices = slices; }
	const int &GetStacks()const { return m_stacks; }
	const int &GetSlices()const { return m_slices; }

private:
	D3DXVECTOR3 m_move;
	int m_stacks, m_slices;
};

#endif