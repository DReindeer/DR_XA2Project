//================================================================================
//
//	title : ポリゴン表示プログラム[ CScene2D.h ]	Auter : KENSUKE WATANABE
//													Data  : 2016/05/30
//
//--------------------------------------------------------------------------------
#ifndef _DEFERRD_RENDERING_H_
#define _DEFERRD_RENDERING_H_

#include <string>
#include "scene2D.h"

static const int MAX_RENDERTARGET = 4;
static const int MAX_BLOOM = 2;

//--------------------------------------------------------------------------------
// 前方宣言
//--------------------------------------------------------------------------------
class CTexManager;
class CScene2DDeferred;
class CScene2DPx;

//--------------------------------------------------------------------------------
// クラス
//--------------------------------------------------------------------------------
class DefferdRendering
{
public:
	typedef struct
	{
		LPDIRECT3DVERTEXSHADER9 pVertexShader;				// 頂点シェーダー格納場所
		LPD3DXCONSTANTTABLE pVConstantTable;				// 頂点シェーダー用定数テーブル
		LPDIRECT3DPIXELSHADER9 pPixelShader;				// ピクセルシェーダー格納場所
		LPD3DXCONSTANTTABLE pPConstantTable;				// ピクセルシェーダー用定数テーブル
		LPDIRECT3DPIXELSHADER9 pPixelShaderNoTex;			// テクスチャ無しピクセルシェーダー格納場所
		LPD3DXCONSTANTTABLE pPConstantTableNoTex;			// テクスチャ無しピクセルシェーダー用定数テーブル
	}SHADER_SET;

	DefferdRendering() : m_pCombinePolygon(nullptr){}
	~DefferdRendering();

	void Init();											// 初期化
	void Uninit();											// 終了
	void Update();											// 更新

	// モデル用
	void SetVXShader(D3DXMATRIX mtxWorld);					// VXシェーダーの設定
	void SetPXShader(D3DMATERIAL9* pMat);					// PXシェーダーの設定
	void SetNoTexturePXShader(D3DMATERIAL9* pMat);			// テクスチャ無しPXシェーダーの設定

	void DrawDeviceBegin();									// レンダーターゲットの切り替え
	void DrawDeviceEnd();									// レンダーターゲットを戻す
	void ClearBackBuffer();									// Zバッファクリア

	void DrawCombineTexture();
	void DrawCombineEnd();

	void DrawCombineBloom();
	void DrawCombineBloomEnd();

	static DefferdRendering *Create();						// 生成

	const LPDIRECT3DTEXTURE9 &GetTexture(const int &i) { return m_renderTexture[i]; }
	const LPDIRECT3DSURFACE9 &GetSurface(const int &i) { return m_renderSurface[i]; }

protected:
	D3DXVECTOR2 m_posUV;									// テクスチャ座標
	float m_widthUV;										// 幅
	float m_heightUV;										// 高さ
	std::string m_strFilePass[MAX_RENDERTARGET];			// テクスチャFilePass

private:
	void CreateShader();

	void SwapDrawFlag() { 
		m_drawFlag ^= 1;
		m_pGPolygon->SetDrawFlag((bool)m_drawFlag);
		m_pBloom->SetDrawFlag((bool)m_drawFlag);
		for (int i = 0; i < MAX_RENDERTARGET; ++i) { m_pRenderPolygon[i]->SetDrawFlag((bool)m_drawFlag); }
	}

	LPDIRECT3DSURFACE9 m_renderTargetZBuffer;				// テクスチャのZバッファ格納用サーフェス
	LPDIRECT3DSURFACE9 m_renderTargetZBuffer4;				// テクスチャのZバッファ格納用サーフェス1/4
	LPDIRECT3DSURFACE9 m_renderTargetZBuffer16;				// テクスチャのZバッファ格納用サーフェス1/16

	CTexManager *m_pTexManager;
	SHADER_SET *m_pShaderSet;
	LPDIRECT3DTEXTURE9 m_renderTexture[MAX_RENDERTARGET];	// テクスチャ
	LPDIRECT3DSURFACE9 m_renderSurface[MAX_RENDERTARGET];	// サーフェス

	CScene2DDeferred *m_pCombinePolygon;					// 合成後ポリゴン
	LPDIRECT3DTEXTURE9 m_combineTexture;					// テクスチャ
	LPDIRECT3DSURFACE9 m_combineSurface;					// サーフェス
	LPD3DXCONSTANTTABLE m_pPConstantTableCombine;			// ピクセルシェーダー用定数テーブル
	LPDIRECT3DPIXELSHADER9 m_pPixelShaderCombine;			// ピクセルシェーダー格納場所

	void UpdateBloom();										// ブルームの更新
	CScene2DPx *m_pBloomPolygon[4];							// ブルーム合成後ポリゴン
	CScene2DPx *m_pCombineBuloom;							// 合成後ポリゴン
	LPDIRECT3DTEXTURE9 m_bloomTexture[4];					// テクスチャ
	LPDIRECT3DSURFACE9 m_bloomSurface[4];					// サーフェス

	LPD3DXCONSTANTTABLE m_pPCColorBrightPass;				// 輝度ピクセルシェーダー用定数テーブル
	LPDIRECT3DPIXELSHADER9 m_pPColorBrightPass;				// 輝度ピクセルシェーダー格納場所

	LPD3DXCONSTANTTABLE m_pPCBloomH;						// ブルームHピクセルシェーダー用定数テーブル
	LPDIRECT3DPIXELSHADER9 m_pPBloomH;						// ブルームピクセルシェーダー格納場所
	LPD3DXCONSTANTTABLE m_pPCBloomV;						// ブルームVピクセルシェーダー用定数テーブル
	LPDIRECT3DPIXELSHADER9 m_pPBloomV;						// ブルームピクセルシェーダー格納場所

	LPD3DXCONSTANTTABLE m_pPCBloomCombine;					// ブルーム合成ピクセルシェーダー用定数テーブル
	LPDIRECT3DPIXELSHADER9 m_pPBloomCombine;				// ブルーム合成ピクセルシェーダー格納場所

	// 試し見ポリゴン
	int m_drawFlag = 1;
	CScene2D *m_pRenderPolygon[MAX_RENDERTARGET] = { nullptr };
	CScene2D *m_pGPolygon = nullptr;			// Gバッファ描画用
	CScene2D *m_pBloom = nullptr;				// ブルーム描画用
	CScene2D *m_combineBloomPolygon = nullptr;	// 合成後(最終)描画用
};

#endif
