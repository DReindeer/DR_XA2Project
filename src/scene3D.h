//================================================================================
//
//	title : ポリゴン表示プログラム[ CScene3D.h ]	Auter : KENSUKE WATANABE
//													Data  : 2016/05/30
//
//--------------------------------------------------------------------------------
#ifndef _SCENE3D_H_
#define _SCENE3D_H_

#include "main.h"
#include <string>
#include "scene.h"

//--------------------------------------------------------------------------------
// クラス
//--------------------------------------------------------------------------------
class CScene3D : public CScene
{
public:
	typedef struct
	{
		LPDIRECT3DVERTEXDECLARATION9 pVertexDeclaration;	// 頂点デクラレーション
		LPDIRECT3DVERTEXSHADER9 pVertexShader;				// 頂点シェーダー格納場所
		LPD3DXCONSTANTTABLE pVConstantTable;				// 頂点シェーダー用定数テーブル
		LPDIRECT3DPIXELSHADER9 pPixelShader;				// ピクセルシェーダー格納場所
		LPD3DXCONSTANTTABLE pPConstantTable;				// ピクセルシェーダー用定数テーブル
		LPDIRECT3DPIXELSHADER9 pPixelShaderNoTex;			// テクスチャ無しピクセルシェーダー格納場所
		LPD3DXCONSTANTTABLE pPConstantTableNoTex;			// テクスチャ無しピクセルシェーダー用定数テーブル
	}SHADER_SET;

	CScene3D(int priority = 4);
	~CScene3D();

	void Init() {}
	void Uninit() {}
	void Update() {}
	void Draw() {}

	const D3DXMATRIX &GetMtxWorld()const { return m_mtxWorld; }
	const float &GetR()const { return m_r; }
	const float &GetG()const { return m_g; }
	const float &GetB()const { return m_b; }
	const float &GetA()const { return m_a; }
	void SetCol(const float &r, const float &g, const float &b, const float &a) { m_r = r; m_g = g; m_b = b; m_a = a; }
	void AddCol(const float &r, const float &g, const float &b, const float &a) { m_r += r; m_g += g; m_b += b; m_a += a; }

	void SetNumVtx(const int &numVtx) { m_numVtx = numVtx; }
	void SetNumIndex(const int &numIndex) { m_numIndex = numIndex; }
	const int &GetNumVtx() { return m_numVtx; }
	const int &GetNumIndex() { return m_numIndex; }

	std::string GetFilePath()const { return m_strFilePass; }
	void SetFilePass(std::string strFilePass) { m_strFilePass = strFilePass; }

protected:
	std::string m_strFilePass;		// テクスチャFilePass
	D3DXMATRIX m_mtxWorld;			// ワールドマトリクス
	LPDIRECT3DINDEXBUFFER9 m_pIdxBuffer;
	D3DMATERIAL9 *m_pMat;
	static SHADER_SET *m_pShaderSet;	// シェーダー

private:
	void CreateShader();

	int m_numVtx;
	int m_numIndex;
	float m_r, m_g, m_b, m_a;
};

#endif
