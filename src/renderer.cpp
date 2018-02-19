//================================================================================
//
//	Auter : KENSUKE WATANABE
//	Data  : 2018/02/10
//
//--------------------------------------------------------------------------------
#include "main.h"
#include "renderer.h"
#include "scene.h"
#include "manager.h"
#include "camera.h"
#include "inputKeyboard.h"
#include "debugStr.h"
#include "texManager.h"

#include "scene2D.h"
#include "mainController.h"

//================================================================================
// 静的メンバ変数
//--------------------------------------------------------------------------------
LPDIRECT3DDEVICE9 CRenderer::m_pD3DDevice = nullptr;

//================================================================================
// CFeedbackBlur::コンストラクタ
//--------------------------------------------------------------------------------
CFeedbackBlur::CFeedbackBlur()
{
	for (int i = 0; i < 2; i++)
	{
		m_renderTargetTexture[i] = nullptr;		// テクスチャ
		m_renderTargetSurface[i] = nullptr;		// サーフェス
		m_renderTarget2D[i] = nullptr;
	}
	m_renderTargetZBuffer = nullptr;

	// テクスチャマネージャの生成
	m_pTexManager = new CTexManager;
	m_pTexManager->Init();
}

//================================================================================
// CFeedbackBlur::デストラクタ
//--------------------------------------------------------------------------------
CFeedbackBlur::~CFeedbackBlur()
{
	for (int i = 0; i < 2; i++)
	{
		// サーフェス
		if (m_renderTargetSurface[i] != nullptr)
		{
			m_renderTargetSurface[i]->Release();
			m_renderTargetSurface[i] = nullptr;
		}
	}

	// Zバッファ格納用サーフェス
	if (m_renderTargetZBuffer != nullptr)
	{
		m_renderTargetZBuffer->Release();
		m_renderTargetZBuffer = nullptr;
	}

	// テクスチャマネージャ
	if (m_pTexManager != nullptr)
	{
		m_pTexManager->Uninit();
		delete m_pTexManager;
		m_pTexManager = nullptr;
	}
}

//================================================================================
// CRenderer::コンストラクタ
//--------------------------------------------------------------------------------
CRenderer::CRenderer() : m_screenSize(D3DXVECTOR2(SCREEN_WIDTH, SCREEN_HEIGHT)), m_HWnd(0)
{
	// 値のクリア
	m_pD3D = nullptr;			// Direct3Dオブジェクト
	m_pD3DDevice = nullptr;		// Deviceオブジェクト(描画に必要)

	m_pRenderTarget = nullptr;
	m_BackBufferSurface = nullptr;			// バックバッファ
	m_blur = 0.f;
}

//================================================================================
// 初期化処理
//================================================================================
HRESULT CRenderer::Init(HWND hWnd, bool bWindow)
{
	m_HWnd = hWnd;

	// Direct3Dオブジェクトの作成
	m_pD3D = Direct3DCreate9(D3D_SDK_VERSION);
	if (m_pD3D == nullptr)
	{
		return E_FAIL;
	}

	// 現在のディスプレイモードを取得
	D3DDISPLAYMODE d3ddm;
	if (FAILED(m_pD3D->GetAdapterDisplayMode(D3DADAPTER_DEFAULT, &d3ddm)))
	{
		return E_FAIL;
	}

	// (ウィンドウ作成後)解像度向上処理
	m_screenSize * 1.f;
	int screenX = static_cast<int>(m_screenSize.x);
	int screenY = static_cast<int>(m_screenSize.y);

	// デバイスのプレゼンテーションパラメータの設定
	D3DPRESENT_PARAMETERS d3dpp;
	ZeroMemory(&d3dpp, sizeof(d3dpp));								// ワークをゼロクリア
	d3dpp.BackBufferCount = 1;										// バックバッファの数
	d3dpp.BackBufferWidth = screenX;								// ゲーム画面サイズ(幅)
	d3dpp.BackBufferHeight = screenY;								// ゲーム画面サイズ(高さ)
	d3dpp.BackBufferFormat = d3ddm.Format;							// カラーモードの指定
	d3dpp.SwapEffect = D3DSWAPEFFECT_DISCARD;						// 映像信号に同期してフリップする
	d3dpp.EnableAutoDepthStencil = TRUE;							// デプスバッファ（Ｚバッファ）とステンシルバッファを作成
	d3dpp.AutoDepthStencilFormat = D3DFMT_D24S8;					// デプスバッファとして24bitを使う
	d3dpp.Windowed = bWindow;										// ウィンドウモード
	d3dpp.FullScreen_RefreshRateInHz = D3DPRESENT_RATE_DEFAULT;		// リフレッシュレート
	d3dpp.PresentationInterval = D3DPRESENT_INTERVAL_DEFAULT;		// インターバル

	// ディスプレイアダプタを表すためのデバイスを作成
	// 描画と頂点処理をハードウェアで行なう
	if (FAILED(m_pD3D->CreateDevice(D3DADAPTER_DEFAULT,
		D3DDEVTYPE_HAL,
		hWnd,
		D3DCREATE_HARDWARE_VERTEXPROCESSING | D3DCREATE_MULTITHREADED,
		&d3dpp, &m_pD3DDevice)))
	{
		// 上記の設定が失敗したら
		// 描画をハードウェアで行い、頂点処理はCPUで行なう
		if (FAILED(m_pD3D->CreateDevice(D3DADAPTER_DEFAULT,
			D3DDEVTYPE_HAL,
			hWnd,
			D3DCREATE_SOFTWARE_VERTEXPROCESSING | D3DCREATE_MULTITHREADED,
			&d3dpp, &m_pD3DDevice)))
		{
			// 上記の設定が失敗したら
			// 描画と頂点処理をCPUで行なう
			if (FAILED(m_pD3D->CreateDevice(D3DADAPTER_DEFAULT,
				D3DDEVTYPE_REF, hWnd,
				D3DCREATE_SOFTWARE_VERTEXPROCESSING | D3DCREATE_MULTITHREADED,
				&d3dpp, &m_pD3DDevice)))
			{
				// 生成失敗
				return E_FAIL;
			}
		}
	}

	// レンダーステートの設定
	m_pD3DDevice->SetRenderState(D3DRS_CULLMODE, D3DCULL_CCW);				// カリングを行わない
	m_pD3DDevice->SetRenderState(D3DRS_ZENABLE, TRUE);						// Zバッファを使用
	m_pD3DDevice->SetRenderState(D3DRS_ALPHABLENDENABLE, TRUE);				// αブレンドを行う
	m_pD3DDevice->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);		// αソースカラーの指定
	m_pD3DDevice->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA);	// αデスティネーションカラーの指定

	// サンプラーステートの設定
	m_pD3DDevice->SetSamplerState(0, D3DSAMP_ADDRESSU, D3DTADDRESS_WRAP);	// テクスチャＵ値の繰り返し設定
	m_pD3DDevice->SetSamplerState(0, D3DSAMP_ADDRESSV, D3DTADDRESS_WRAP);	// テクスチャＶ値の繰り返し設定
	m_pD3DDevice->SetSamplerState(0, D3DSAMP_MINFILTER, D3DTEXF_LINEAR);	// テクスチャ拡大時の補間設定
	m_pD3DDevice->SetSamplerState(0, D3DSAMP_MAGFILTER, D3DTEXF_LINEAR);	// テクスチャ縮小時の補間設定

	// テクスチャステージステートの設定
	m_pD3DDevice->SetTextureStageState(0, D3DTSS_ALPHAOP, D3DTOP_MODULATE);	// アルファブレンディング処理(初期値はD3DTOP_SELECTARG1)
	m_pD3DDevice->SetTextureStageState(0, D3DTSS_ALPHAARG1, D3DTA_TEXTURE);	// 最初のアルファ引数(初期値はD3DTA_TEXTURE、テクスチャがない場合はD3DTA_DIFFUSE)
	m_pD3DDevice->SetTextureStageState(0, D3DTSS_ALPHAARG2, D3DTA_DIFFUSE);	// ２番目のアルファ引数(初期値はD3DTA_DIFFUSE)
	
//	// アルファテストの有効化
//	m_pD3DDevice->SetRenderState(D3DRS_ALPHATESTENABLE, TRUE);
//	m_pD3DDevice->SetRenderState(D3DRS_ALPHAFUNC, D3DCMP_GREATEREQUAL);
//	// 不透明にする値の設定
//	m_pD3DDevice->SetRenderState(D3DRS_ALPHAREF, (DWORD)0x00000001);

	// レンダーターゲットの設定
	m_pRenderTarget = new CFeedbackBlur;

	CTexManager *pTexManager = m_pRenderTarget->GetTexManager();
	LPDIRECT3DTEXTURE9 tex0;
	LPDIRECT3DTEXTURE9 tex1;

	// 空のテクスチャの生成(バックバッファと同じサイズ...でないと、Zバッファを共有出来ない)
	D3DXCreateTexture(m_pD3DDevice, screenX, screenY, 1, D3DUSAGE_RENDERTARGET, D3DFMT_A8R8G8B8, D3DPOOL_DEFAULT, &tex0);				
	D3DXCreateTexture(m_pD3DDevice, screenX, screenY, 1, D3DUSAGE_RENDERTARGET, D3DFMT_A8R8G8B8, D3DPOOL_DEFAULT, &tex1); 
	m_pRenderTarget->SetRenderTargetTexture(&tex0, 0);
	m_pRenderTarget->SetRenderTargetTexture(&tex1, 1);
	pTexManager->SetTex("TEX_RenderTarget", tex0);
	pTexManager->SetTex("TEX_RenderTarget2", tex1);
	tex0->GetSurfaceLevel(0, &m_pRenderTarget->GetRenderTargetSurface(0));		// サーフェスの取得
	tex1->GetSurfaceLevel(0, &m_pRenderTarget->GetRenderTargetSurface(1));		// サーフェスの取得
	m_pD3DDevice->GetRenderTarget(0, &m_BackBufferSurface);						// 現在設定されているレンダーターゲット取得

	// デプス・ステンシルサーフェスの生成
	m_pD3DDevice->CreateDepthStencilSurface(screenX, screenY, D3DFMT_D24S8, D3DMULTISAMPLE_NONE, 0, FALSE, &m_pRenderTarget->GetRenderTargetZBuffer(), nullptr);
	m_pD3DDevice->GetDepthStencilSurface(&m_ZBuffer);
	
	CScene2D *pRenderTarget2D = CScene2D::Create(D3DXVECTOR3(screenX * 0.5f - 0.5f, screenY * 0.5f - 0.5f, 0.f), D3DXVECTOR3(0.f, 0.f, 0.f), D3DXVECTOR3(screenX * 0.5f, screenY * 0.5f, 0.f), "TEX_RenderTarget", 4, pTexManager);
	pRenderTarget2D->SetOverall();
	m_pRenderTarget->SetRenderTarget2D(pRenderTarget2D, 0);

	pRenderTarget2D = CScene2D::Create(D3DXVECTOR3(screenX * 0.5f - 0.5f, screenY * 0.5f - 0.5f, 0.f), D3DXVECTOR3(0.f, 0.f, 0.f), D3DXVECTOR3(screenX * 0.5f, screenY * 0.5f, 0.f), "TEX_RenderTarget2", 4, pTexManager);
	pRenderTarget2D->SetOverall();
	m_pRenderTarget->SetRenderTarget2D(pRenderTarget2D, 1);
	return S_OK;
}

//================================================================================
// 終了処理
//================================================================================
void CRenderer::Uninit()
{
	// レンダーターゲットの破棄
	if (m_pRenderTarget != nullptr)
	{
		delete m_pRenderTarget;
		m_pRenderTarget = nullptr;
	}

	// バックバッファの破棄
	if (m_BackBufferSurface != nullptr)
	{
		m_BackBufferSurface->Release();
		m_BackBufferSurface = nullptr;
	}

	// Direct3Dオブジェクトの破棄
	if(m_pD3D != nullptr)
	{
		m_pD3D->Release();
		m_pD3D = nullptr;
	}

	// デバイスの破棄
	if (m_pD3DDevice != nullptr)
	{
		m_pD3DDevice->Release();
		m_pD3DDevice = nullptr;
	}
}

//================================================================================
// 描画の先頭に行う処理 
//================================================================================
bool CRenderer::DrawBigin()
{
	// 画面サイズの再取得
	RECT rect;
	GetClientRect(m_HWnd, &rect);
	m_screenSize = D3DXVECTOR2((FLOAT)rect.right, (FLOAT)rect.bottom);

	m_pD3DDevice->SetDepthStencilSurface(m_BackBufferSurface);
	m_pD3DDevice->SetRenderTarget(0, m_pRenderTarget->GetRenderTargetSurface(1));
	CScene2D *pPolygon2D0 = m_pRenderTarget->GetRenderTarget2D(0);
	CScene2D *pPolygon2D1 = m_pRenderTarget->GetRenderTarget2D(1);
	pPolygon2D0->SetDrawFlag(false);
	pPolygon2D1->SetDrawFlag(false);
	pPolygon2D0->Update();
	pPolygon2D1->Update();

	// バックバッファ＆Ｚバッファのクリア
	m_pD3DDevice->Clear(0, NULL, (D3DCLEAR_TARGET | D3DCLEAR_ZBUFFER | D3DCLEAR_STENCIL), D3DCOLOR_RGBA(255, 255, 255, 255), 1.f, 0);
	
	// ワイヤーフレームの切り替え(切り替え時に使用するボタン)
//	DrawWireframe(DIK_1);

	return true;
}

//================================================================================
// 描画の後に行う処理 
//================================================================================
void CRenderer::DrawEnd()
{
	LPDIRECT3DTEXTURE9 tex0 = m_pRenderTarget->GetRenderTargetTexture(0);
	LPDIRECT3DTEXTURE9 tex1 = m_pRenderTarget->GetRenderTargetTexture(1);
	CScene2D *pPolygon2D0 = m_pRenderTarget->GetRenderTarget2D(0);
	CScene2D *pPolygon2D1 = m_pRenderTarget->GetRenderTarget2D(1);
	if (m_blur > 0.f)
	{
		m_pD3DDevice->SetTexture(0, tex0);
		//pPolygon2D0->SetCol(255, 255, 255, 255 * (1 - m_blur));
		//pPolygon2D0->SetScl(1.f + m_blur * 0.05f);
		//pPolygon2D0->SetDrawFlag(true);
		//pPolygon2D0->Draw();
	}
	else
	{
		pPolygon2D0->SetCol(255, 255, 255, 255);
		pPolygon2D0->SetScl(1.f);
	}

	// レンダーターゲットの切り替え
	m_pD3DDevice->SetRenderTarget(0, m_BackBufferSurface);

	// バックバッファ＆Ｚバッファのクリア
	m_pD3DDevice->Clear(0, NULL, (D3DCLEAR_TARGET | D3DCLEAR_ZBUFFER | D3DCLEAR_STENCIL), D3DCOLOR_RGBA(255, 255, 255, 255), 1.f, 0);

	m_pD3DDevice->SetTexture(0, tex1);
	
	pPolygon2D1->SetDrawFlag(true);
	pPolygon2D1->Draw();

	// 入れ替え
	LPDIRECT3DTEXTURE9 &tex = tex0;
	LPDIRECT3DSURFACE9 sfc = m_pRenderTarget->GetRenderTargetSurface(0);
	m_pRenderTarget->SetRenderTargetTexture(&m_pRenderTarget->GetRenderTargetTexture(1), 0);
	m_pRenderTarget->SetRenderTargetSurface(&m_pRenderTarget->GetRenderTargetSurface(1), 0);
	m_pRenderTarget->SetRenderTargetTexture(&tex, 1);
	m_pRenderTarget->SetRenderTargetSurface(&sfc, 1);

	CScene2D *pPolygon2D = m_pRenderTarget->GetRenderTarget2D(0);
	m_pRenderTarget->SetRenderTarget2D(m_pRenderTarget->GetRenderTarget2D(1), 0);
	m_pRenderTarget->SetRenderTarget2D(pPolygon2D, 0);
}

//================================================================================
// D3DDeviceの取得
//================================================================================
LPDIRECT3DDEVICE9 &CRenderer::GetDevice()
{
	return m_pD3DDevice;
}

//================================================================================
// ワイヤーフレームの切り替え
//================================================================================
void CRenderer::DrawWireframe(unsigned int key)
{
	// キーボードの取得
	CInputKeyboard *keyboard = CManager::GetKeyboard();
	if (!keyboard)return;
	// 描画方法の切り替え
	if (keyboard->GetPress(key))
	{
#ifdef _DEBUG
		m_pD3DDevice->SetRenderState(D3DRS_FILLMODE, D3DFILL_WIREFRAME);
#endif
	}
	else {
		m_pD3DDevice->SetRenderState(D3DRS_FILLMODE, D3DFILL_SOLID);
	}
}