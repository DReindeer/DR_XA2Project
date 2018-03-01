// ======== ======== ======== ======== ======== ======== ======== ========
//
//
//	title : レンダラー
//	Auter : KENSUKE WATANABE
//
//
// ======== ======== ======== ======== ======== ======== ======== ========
#ifndef _RENDERER_H_
#define _RENDERER_H_

#include "Wmath.h"

// ======== ======== ======== ======== ======== ======== ======== ========
// クラス定義
// -------- -------- -------- -------- -------- -------- -------- --------
class CRenderer
{
public:
	CRenderer();
	~CRenderer();

	HRESULT Init(HWND hWnd, bool bWindow);
	void Uninit(void);
	bool DrawBigin(void);
	void DrawEnd(void);

	static ID3D11DeviceContext *GetDeviceContext(void) { return m_pDeviceContext; }
	ID3D11Device *GetDevice(void){ return m_pDevice; }

	const XMFLOAT2 &GetScreenSize(void)const { return m_screenSize; }

	// スクリーンに対する座標orサイズを0.f~1.fの割合で指定
	static float RatioToScrX(const float &ratio) { return m_screenSize.x * ratio; }
	static float RatioToScrY(const float &ratio) { return m_screenSize.y * ratio; }
	static XMFLOAT2 RatioToScrXY(const XMFLOAT2 &ratio) { return XMFLOAT2(m_screenSize.x * ratio.x, m_screenSize.y * ratio.y); }

private:
	static XMFLOAT2 m_screenSize;

	// 11
	D3D11_VIEWPORT m_vp;		// ビューポート
	ID3D11Device *m_pDevice;
	static ID3D11DeviceContext *m_pDeviceContext;

	IDXGIDevice1 *m_pDXGI;
	IDXGIAdapter *m_pAdapter;
	IDXGIFactory *m_pDXGIFactory;
	IDXGISwapChain *m_pDXGISwpChain;
	ID3D11Texture2D *m_pBackBuffer;
	ID3D11RenderTargetView *m_pRenderTargetView;
	ID3D11DepthStencilView *m_pDepthStencilView;	// ステンシルバッファ
};

#endif
