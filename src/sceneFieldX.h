//================================================================================
//
//	title : classMake[ sceneFieldX.h ]			Auter : KENSUKE WATANABE
//												Data  : 2017/04/27
//
//--------------------------------------------------------------------------------
#ifndef _MESHFIELDX_H_
#define _MESHFIELDX_H_

#include "main.h"
#include <vector>
#include "scene3D.h"

struct XMODEL_VTX
{
	D3DXVECTOR3 pos;
	D3DXVECTOR3 nom;
	float nazo;
	D3DXVECTOR2 uv;
};

//--------------------------------------------------------------------------------
// クラス
//--------------------------------------------------------------------------------
class CSceneFieldX : public CScene3D
{
public:
	// 列挙対定義
	typedef enum
	{// ブロックのタイプ
		MESHFIELD_TEX_KUSA = 0,		// 草
		MESHFIELD_TEX_MAX
	}MESHFIELD_TEX;

	typedef struct
	{
		D3DXVECTOR3 p[3];
		D3DXVECTOR3 n[3];
		D3DXVECTOR2 uv[3];
	}FIELD_POLY;

	CSceneFieldX(int priority = 4) :CScene3D(priority) { m_objType = OBJTYPE::FIELDX; m_classType = CLASSTYPE::SCENE_FIELD; }
	~CSceneFieldX() {}

	void Init();
	void Uninit();
	void Update();
//	void Draw();
	void DrawDefferdRendering()override;	// デファードレンダリング用
	static CSceneFieldX *Create(const D3DXVECTOR3 &pos, const D3DXVECTOR3 &rot, const D3DXVECTOR3 &size, const std::string &strFilePass, int pry = 4);

	const D3DXVECTOR3 &GetCenterPos(){ return m_centerPos; }

	bool GetHeight(D3DXVECTOR3 pos, float* y, FIELD_POLY *pPoly);
	bool TriangleCloss(D3DXVECTOR3 pos, CSceneFieldX::FIELD_POLY *fieldPoly, D3DXVECTOR3 *nom);
	bool TriangleNearLine(D3DXVECTOR3 pos, CSceneFieldX::FIELD_POLY *fieldPoly, D3DXVECTOR3 *outPosS, D3DXVECTOR3 *outPosG);
	std::vector<FIELD_POLY*> *GetVecFieldPoly() { return &m_vecFieldPoly; }

private:
	std::vector<FIELD_POLY*> m_vecFieldPoly;

	D3DXVECTOR3 m_centerPos;
};

#endif