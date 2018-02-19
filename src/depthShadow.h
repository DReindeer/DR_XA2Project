//================================================================================
//
//	title : ポリゴン表示プログラム[ CScene2D.h ]	Auter : KENSUKE WATANABE
//													Data  : 2016/05/30
//
//--------------------------------------------------------------------------------
#ifndef _SCENE2DEPTHSHADOW_H_
#define _SCENE2DEPTHSHADOW_H_

#include <string>
#include "scene2D.h"

//--------------------------------------------------------------------------------
// 前方宣言
//--------------------------------------------------------------------------------
class CTexManager;

//--------------------------------------------------------------------------------
// クラス
//--------------------------------------------------------------------------------
class DepthShadow
{
public:
	typedef struct
	{
		LPDIRECT3DVERTEXDECLARATION9 pVertexDeclaration;	// 頂点デクラレーション
		LPDIRECT3DVERTEXSHADER9 pVertexShader;				// 頂点シェーダー格納場所
		LPD3DXCONSTANTTABLE pVConstantTable;				// 頂点シェーダー用定数テーブル
		LPDIRECT3DPIXELSHADER9 pPixelShader;				// ピクセルシェーダー格納場所
		LPD3DXCONSTANTTABLE pPConstantTable;				// ピクセルシェーダー用定数テーブル
	}DEPTHSHADOW_SHADER_SET;

	DepthShadow() {}
	~DepthShadow();

	void Init();				// 初期化
	void Uninit();				// 終了
	void Update();				// 更新
	void DrawBigin(D3DXMATRIX mtxWorld);	// 描画
	void DrawEnd();				// 描画
	void DrawDeviceBegin();
	void DrawDeviceEnd();
	void ClearBackBuffer();
	static DepthShadow *Create();

	const LPDIRECT3DTEXTURE9 &GetTexture() { return m_depthShadowTexture; }

protected:
	D3DXVECTOR2 m_posUV;		// テクスチャ座標
	float m_widthUV;			// 幅
	float m_heightUV;			// 高さ
	std::string m_strFilePass;	// テクスチャFilePass

private:
	void CreateShader();

	void SwapDrawFlag() {
		m_drawFlag ^= 1;
		m_pDepthShadowPolygon->SetDrawFlag((bool)m_drawFlag);
	}

	CTexManager *m_pTexManager;
	DEPTHSHADOW_SHADER_SET *m_pDepthShadow;
	LPDIRECT3DTEXTURE9 m_depthShadowTexture;	// テクスチャ
	LPDIRECT3DSURFACE9 m_depthShadowSurface;	// サーフェス
	LPDIRECT3DSURFACE9 m_renderTargetZBuffer;	// レンダーターゲットにしたテクスチャのZバッファ格納用サーフェス
	D3DVIEWPORT9 m_viewOld;

	// 試し見ポリゴン
	int m_drawFlag = 1;
	CScene2D *m_pDepthShadowPolygon = nullptr;
};

#endif
