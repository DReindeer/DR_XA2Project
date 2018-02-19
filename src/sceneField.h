//================================================================================
//
//	title : classMake[ MeshField.h ]			Auter : KENSUKE WATANABE
//												Data  : 2017/04/27
//
//--------------------------------------------------------------------------------
#ifndef _MESHFIELD_H_
#define _MESHFIELD_H_

#include "main.h"
#include <unordered_map>
#include <vector>
#include "scene3D.h"

//--------------------------------------------------------------------------------
// クラス
//--------------------------------------------------------------------------------
class CSceneField : public CScene3D
{
public:
	// 列挙対定義
	typedef enum
	{// ブロックのタイプ
		MESHFIELD_TEX_KUSA = 0,		// 草
		MESHFIELD_TEX_MAX
	}MESHFIELD_TEX;

	CSceneField(int priority = 4) :CScene3D(priority){ m_objType = OBJTYPE::FIELD; m_classType = CLASSTYPE::SCENE_FIELD; }
	~CSceneField() {}

	void Init();
	void Uninit();
	void Update();
	void Draw();
	static CSceneField *Create(const D3DXVECTOR3 &pos, const D3DXVECTOR3 &rot, const D3DXVECTOR3 &size, const std::string &strFilePass);

	HRESULT SetVtx();

	const std::vector<D3DXVECTOR3> &GetVecVtxPos() const { return m_vecVtxPos; }
	const std::vector<D3DXVECTOR3> &GetVecVtxNom() const { return m_vecVtxNom; }
	LPDIRECT3DINDEXBUFFER9 *GetIdxBuffer() { return &m_pIdxBuffer; }
	const int &GetNumFaceX() const { return m_numFaceX; }
	const int &GetNumFaceZ() const { return m_numFaceZ; }

	float GetHeight(D3DXVECTOR3 pos, bool *hit);
	bool TriangleCloss(D3DXVECTOR3 v12, D3DXVECTOR3 v20, D3DXVECTOR3 v01, D3DXVECTOR3 *nom);

private:
	int m_numVtx;
	int m_numIndex;
	int m_numFaceX, m_numFaceZ;
	D3DXVECTOR2 m_faceSize;
	std::vector<D3DXVECTOR3> m_vecVtxPos;
	std::vector<D3DXVECTOR3> m_vecVtxNom;
	
	LPDIRECT3DINDEXBUFFER9 m_pIdxBuffer;
};

#endif