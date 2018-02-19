//================================================================================
// 
// レンダラー処理 [renderer.h]
// Author : KENSUKE WATANABE
// 
//================================================================================
#ifndef _RENDERER_H_
#define _RENDERER_H_

//================================================================================
// ヘッダファイル
//--------------------------------------------------------------------------------
#include "main.h"

class CScene2D;
class CTexManager;

class CFeedbackBlur
{
public:
	CFeedbackBlur();
	~CFeedbackBlur();

	// 設定
	void SetRenderTargetTexture(LPDIRECT3DTEXTURE9 *renderTargetTexture, int n){ m_renderTargetTexture[n] = *renderTargetTexture; }
	void SetRenderTargetSurface(LPDIRECT3DSURFACE9 *renderTargetSurface, int n) { m_renderTargetSurface[n] = *renderTargetSurface; }
	void SetRenderTargetZBuffer(LPDIRECT3DSURFACE9 *renderTargetZBuffer) { m_renderTargetZBuffer = *renderTargetZBuffer; }
	void SetRenderTarget2D(CScene2D *renderTarget2D, int n) { m_renderTarget2D[n] = renderTarget2D; }
	// 取得
	LPDIRECT3DTEXTURE9 &GetRenderTargetTexture(int n) { return m_renderTargetTexture[n]; }
	LPDIRECT3DSURFACE9 &GetRenderTargetSurface(int n) { return m_renderTargetSurface[n]; }
	LPDIRECT3DSURFACE9 &GetRenderTargetZBuffer() { return m_renderTargetZBuffer; }
	CScene2D *GetRenderTarget2D(int n) { return m_renderTarget2D[n]; }

	CTexManager *GetTexManager() { return m_pTexManager; }

private:
	CTexManager *m_pTexManager;						// テクスチャマネージャ
	LPDIRECT3DTEXTURE9 m_renderTargetTexture[2];	// テクスチャ
	LPDIRECT3DSURFACE9 m_renderTargetSurface[2];	// サーフェス
	LPDIRECT3DSURFACE9 m_renderTargetZBuffer;		// レンダーターゲットにしたテクスチャのZバッファ格納用サーフェス
	CScene2D *m_renderTarget2D[2];					// scene2(板ポリ)
};

//================================================================================
// クラス定義
//--------------------------------------------------------------------------------
class CRenderer
{
public:
	CRenderer();
	~CRenderer() {}

	HRESULT Init(HWND hWnd, bool bWindow);
	void Uninit();
	bool DrawBigin();
	void DrawEnd();

	static void DrawWireframe(unsigned int key);
	void SetBlur(float blur) { m_blur = blur; }

	static LPDIRECT3DDEVICE9 &GetDevice();
	const D3DXVECTOR2 &GetScreenSize()const {
		return m_screenSize;
	}
	CFeedbackBlur *GetRenderTarget() { return m_pRenderTarget; }
	LPDIRECT3DSURFACE9 *GetZBuffer() { return &m_ZBuffer; }

private:
	LPDIRECT3D9 m_pD3D;							// Direct3Dオブジェクト
	static LPDIRECT3DDEVICE9 m_pD3DDevice;		// Deviceオブジェクト(描画に必要)
	D3DXVECTOR2 m_screenSize;

	// レンダーターゲット
	CFeedbackBlur *m_pRenderTarget;
	LPDIRECT3DSURFACE9 m_BackBufferSurface;		// バックバッファ
	LPDIRECT3DSURFACE9 m_ZBuffer;
	float m_blur;

	HWND m_HWnd;
};

#endif

