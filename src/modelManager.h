//================================================================================
//
//	Auter : KENSUKE WATANABE
//	Data  : 2017/04/27
//
//--------------------------------------------------------------------------------
#ifndef _MODELMANAGER_H_
#define _MODELMANAGER_H_

#include "main.h"
#include <unordered_map>

typedef struct
{
	LPDIRECT3DVERTEXDECLARATION9 pVertexDeclaration;	// 頂点デクラレーション
	LPDIRECT3DVERTEXSHADER9 pVertexShader;				// 頂点シェーダー格納場所
	LPD3DXCONSTANTTABLE pVConstantTable;				// 頂点シェーダー用定数テーブル
	LPDIRECT3DPIXELSHADER9 pPixelShader;				// ピクセルシェーダー格納場所
	LPD3DXCONSTANTTABLE pPConstantTable;				// ピクセルシェーダー用定数テーブル
	LPDIRECT3DPIXELSHADER9 pPixelShaderNoTex;			// テクスチャ無しピクセルシェーダー格納場所
	LPD3DXCONSTANTTABLE pPConstantTableNoTex;			// テクスチャ無しピクセルシェーダー用定数テーブル
}MODEL_SHADER_SET;

//================================================================================
// クラス
//--------------------------------------------------------------------------------
class CModelManager
{
public:
	typedef struct
	{
		LPD3DXMESH pMesh;					// メッシュインターフェイスポインタ
		LPD3DXMESH pMeshClone;				// クローンメッシュインターフェイスポインタ
		LPD3DXBUFFER pBuffMat;				// マテリアルバッファ
		DWORD numMat;						// マテリアル数
		D3DXMATERIAL* pMat;					// マテリアル情報
	}MODEL;

	CModelManager(){}
	~CModelManager(){}

	void Init();
	void Uninit();
	
	bool SetModel(const std::string &strFilePass);
	MODEL *GetModel(const std::string &strFilePass);

	LPD3DXMESH GetModelMesh(const std::string &strFilePass);
	LPD3DXBUFFER GetModelBuffMat(const std::string &strFilePass);
	DWORD GetModelNumMat(const std::string &strFilePass);
	D3DXMATERIAL *GetModelMat(const std::string &strFilePass);
	
	bool Release(const std::string &strFilePass);			// 解放
	void ReleaseAll();										// 全開放

private:

	std::unordered_map<std::string, MODEL*> m_umapModel;	// ファイルパス
};

#endif