//================================================================================
//
//	Auter : KENSUKE WATANABE
//	Data  : 2018/02/10
//
//--------------------------------------------------------------------------------
#include "texManager.h"
#include "inputKeyboard.h"
#include "depthShadow.h"
#include "manager.h"
#include "renderer.h"
#include "camera.h"
#include "lightCamera.h"
#include "scene2D.h"
#include "utility.h"

//--------------------------------------------------------------------------------
// デストラクタ
//--------------------------------------------------------------------------------
DepthShadow::~DepthShadow()
{
	// 終了処理
	Uninit();
}

//--------------------------------------------------------------------------------
// 初期化
//--------------------------------------------------------------------------------
void DepthShadow::Init()
{
	// 変数の初期化
	m_posUV = D3DXVECTOR2(0.f, 0.f);
	m_widthUV = 1.f;
	m_heightUV = 1.f;
}

//--------------------------------------------------------------------------------
// 終了
//--------------------------------------------------------------------------------
void DepthShadow::Uninit()
{
	// テクスチャ
	if (m_depthShadowTexture)
	{
		m_depthShadowTexture->Release();
		m_depthShadowTexture = nullptr;
	}
	// サーフェス
	if (m_depthShadowSurface)
	{
		m_depthShadowSurface->Release();
		m_depthShadowSurface = nullptr;
	}
	// テクスチャ
	if (m_renderTargetZBuffer)
	{
		m_renderTargetZBuffer->Release();
		m_renderTargetZBuffer = nullptr;
	}

	// シェーダーの解放
	if (m_pDepthShadow)
	{
		// ピクセルシェーダー固定テーブル
		if (m_pDepthShadow->pPConstantTable)
		{
			m_pDepthShadow->pPConstantTable->Release();
			m_pDepthShadow->pPConstantTable = nullptr;
		}
		// ピクセルシェーダー
		if (m_pDepthShadow->pPixelShader)
		{
			m_pDepthShadow->pPixelShader->Release();
			m_pDepthShadow->pPixelShader = nullptr;
		}
		// 頂点シェーダー固定テーブル
		if (m_pDepthShadow->pVConstantTable)
		{
			m_pDepthShadow->pVConstantTable->Release();
			m_pDepthShadow->pVConstantTable = nullptr;
		}
		// 頂点シェーダー
		if (m_pDepthShadow->pVertexShader)
		{
			m_pDepthShadow->pVertexShader->Release();
			m_pDepthShadow->pVertexShader = nullptr;
		}
		// 頂点デクラレーション
		if (m_pDepthShadow->pVertexDeclaration)
		{
			m_pDepthShadow->pVertexDeclaration->Release();
			m_pDepthShadow->pVertexDeclaration = nullptr;
		}

		delete m_pDepthShadow;
		m_pDepthShadow = nullptr;
	}
}

//--------------------------------------------------------------------------------
// 更新
//--------------------------------------------------------------------------------
void DepthShadow::Update()
{
	// L押下で表示の切り替え
//	if (CManager::GetKeyboard()->GetTrigger(DIK_L))
//	{
//		SwapDrawFlag();
//	}
	m_pDepthShadowPolygon->SetDrawFlag(false);
}

//--------------------------------------------------------------------------------
// オブジェクト描画準備
//--------------------------------------------------------------------------------
void DepthShadow::DrawBigin(D3DXMATRIX mtxWorld)
{
	// スレッドセーフ(ロック)
#ifdef THREAD_ON
	std::unique_lock<std::recursive_mutex> locker = CManager::Locker();
#endif

	// D3Deviceの取得
	LPDIRECT3DDEVICE9 pDevice = CRenderer::GetDevice();

	// ライトの位置のカメラの設定に変更
	CLightCamera *pLightCamera = CManager::GetLightCamera();
	pLightCamera->SetOrthoLH();

	// 頂点シェーダーの定数バッファ用変数
	//--------------------------------------------------------------------------------
	// ライト取得
//	CLightCamera *pLightCamera = CManager::GetLightCamera();
	D3DXMATRIX mtxView = pLightCamera->GetMtxView();
	D3DXMATRIX mtxProj = pLightCamera->GetMtxProj();
	D3DXMATRIX mtxLightWV = mtxWorld * mtxView;
	m_pDepthShadow->pVConstantTable->SetMatrix(pDevice, "mtxLightWV", &mtxLightWV);
	D3DXMATRIX mtxLightWVP = mtxWorld * mtxView * mtxProj;
	m_pDepthShadow->pVConstantTable->SetMatrix(pDevice, "mtxLightWVP", &mtxLightWVP);
	m_pDepthShadow->pVConstantTable->SetFloat(pDevice, "lightFar", pLightCamera->GetFar());

	// 頂点シェーダーの設定
	//------------------------------------------------------------------------
	pDevice->SetVertexShader(m_pDepthShadow->pVertexShader);
	// ピクセルシェーダーの設定
	//------------------------------------------------------------------------
	pDevice->SetPixelShader(m_pDepthShadow->pPixelShader);
}

//--------------------------------------------------------------------------------
// オブジェクト描画終了
//--------------------------------------------------------------------------------
void DepthShadow::DrawEnd()
{
	// スレッドセーフ(ロック)
#ifdef THREAD_ON
	std::unique_lock<std::recursive_mutex> locker = CManager::Locker();
#endif

	// D3Deviceの取得
	LPDIRECT3DDEVICE9 pDevice = CRenderer::GetDevice();

	pDevice->SetPixelShader(nullptr);
	pDevice->SetVertexShader(nullptr);
}

//--------------------------------------------------------------------------------
// シャドウ書き込み用のレンダーターゲットへ切り替え
//--------------------------------------------------------------------------------
void DepthShadow::DrawDeviceBegin()
{
	// スレッドセーフ(ロック)
#ifdef THREAD_ON
	std::unique_lock<std::recursive_mutex> locker = CManager::Locker();
#endif

	// D3Deviceの取得
	LPDIRECT3DDEVICE9 pDevice = CRenderer::GetDevice();

	// Zバッファ切り替え
	pDevice->SetDepthStencilSurface(m_renderTargetZBuffer);

	// サーフェス切り替え
	pDevice->SetRenderTarget(0, m_depthShadowSurface);

	// デバイスにビューポートの設定
	D3DVIEWPORT9 viewData = { 0, 0, 4096, 4096, 0.0f, 1.0f };
	pDevice->SetViewport(&viewData);

	// バックバッファ＆Ｚバッファのクリア
	pDevice->Clear(0, NULL, (D3DCLEAR_TARGET | D3DCLEAR_ZBUFFER | D3DCLEAR_STENCIL), D3DCOLOR_RGBA(255, 255, 255, 255), 1.f, 0);
}

//--------------------------------------------------------------------------------
// レンダーターゲットを戻す
//--------------------------------------------------------------------------------
void DepthShadow::DrawDeviceEnd()
{
	// スレッドセーフ(ロック)
#ifdef THREAD_ON
	std::unique_lock<std::recursive_mutex> locker = CManager::Locker();
#endif

	// D3Deviceの取得
	LPDIRECT3DDEVICE9 pDevice = CRenderer::GetDevice();

	// サーフェスを戻す
	pDevice->SetRenderTarget(0, CManager::GetRenderer()->GetRenderTarget()->GetRenderTargetSurface(1));

	// Zバッファを戻す
	pDevice->SetDepthStencilSurface(*CManager::GetRenderer()->GetZBuffer());

	// ビューポートを戻す
	D3DVIEWPORT9 viewData = CManager::GetViewPort();
	pDevice->SetViewport(&viewData);
}

//--------------------------------------------------------------------------------
// バックバッファ＆Ｚバッファのクリア
//--------------------------------------------------------------------------------
void DepthShadow::ClearBackBuffer()
{
	// スレッドセーフ(ロック)
#ifdef THREAD_ON
	std::unique_lock<std::recursive_mutex> locker = CManager::Locker();
#endif

	// D3Deviceの取得
	LPDIRECT3DDEVICE9 pDevice = CRenderer::GetDevice();

	// Zバッファ切り替え
	pDevice->SetDepthStencilSurface(m_renderTargetZBuffer);

	// サーフェス切り替え
	pDevice->SetRenderTarget(0, m_depthShadowSurface);

	// バックバッファ＆Ｚバッファのクリア
	pDevice->Clear(0, NULL, (D3DCLEAR_TARGET | D3DCLEAR_ZBUFFER | D3DCLEAR_STENCIL), D3DCOLOR_RGBA(255, 255, 255, 255), 1.f, 0);

	// Zバッファを戻す
	pDevice->SetDepthStencilSurface(CManager::GetRenderer()->GetRenderTarget()->GetRenderTargetZBuffer());

	// サーフェスを戻す
	pDevice->SetRenderTarget(0, CManager::GetRenderer()->GetRenderTarget()->GetRenderTargetSurface(1));
}

//--------------------------------------------------------------------------------
// 作成処理
//--------------------------------------------------------------------------------
DepthShadow *DepthShadow::Create()
{
	// スレッドセーフ(ロック)
#ifdef THREAD_ON
	std::unique_lock<std::recursive_mutex> locker = CManager::Locker();
#endif

	// D3Deviceの取得
	LPDIRECT3DDEVICE9 pDevice = CRenderer::GetDevice();

	// DepthShadowの生成
	DepthShadow *pDepthShadow;
	pDepthShadow = new DepthShadow();

	pDepthShadow->Init();
	pDepthShadow->m_strFilePass = "DepthShadowTexture";

	// デプス・ステンシルサーフェスの生成 / 設定
	pDevice->CreateDepthStencilSurface(4096, 4096, D3DFMT_D24S8, D3DMULTISAMPLE_NONE, 0, FALSE, &pDepthShadow->m_renderTargetZBuffer, NULL);

	// 空のテクスチャを生成
	D3DXCreateTexture(pDevice, 4096, 4096, 1, D3DUSAGE_RENDERTARGET, D3DFMT_R32F, D3DPOOL_DEFAULT, &pDepthShadow->m_depthShadowTexture);

	// テクスチャマネージャへの設定
	pDepthShadow->m_pTexManager = new CTexManager;
	pDepthShadow->m_pTexManager->SetTex(pDepthShadow->m_strFilePass, pDepthShadow->m_depthShadowTexture);

	// サーフェスの取得
	pDepthShadow->m_depthShadowTexture->GetSurfaceLevel(0, &pDepthShadow->m_depthShadowSurface);

	// シェーダーの設定
	pDepthShadow->CreateShader();

	// CScene2Dのポリゴン作成
	D3DXVECTOR2 size = CUtility::RatioToScrXY(D3DXVECTOR2(0.1f, 0.1f));	// サイズ
	pDepthShadow->m_pDepthShadowPolygon = CScene2D::Create(D3DXVECTOR3(size.x, size.y, 0.f), D3DXVECTOR3(0.f, 0.f, 0.f),
		D3DXVECTOR3(size.x, size.y, 0.f), pDepthShadow->m_strFilePass, 9, pDepthShadow->m_pTexManager);
	pDepthShadow->m_pDepthShadowPolygon->SetOverall();					// 遷移で消されない
	pDepthShadow->m_pDepthShadowPolygon->SetCol(255, 255, 255, 255);
	pDepthShadow->m_pDepthShadowPolygon->SetAlpha(false);
	pDepthShadow->m_pDepthShadowPolygon->SetDrawFlag(true);

	return pDepthShadow;
}

//--------------------------------------------------------------------------------
// hlsl読み込み
//--------------------------------------------------------------------------------
void DepthShadow::CreateShader()
{
	// スレッドセーフ(ロック)
#ifdef THREAD_ON
	std::unique_lock<std::recursive_mutex> locker = CManager::Locker();
#endif

	// D3Deviceの取得
	LPDIRECT3DDEVICE9 pDevice = CRenderer::GetDevice();

	// シェーダー用構造体
	m_pDepthShadow = new DEPTHSHADOW_SHADER_SET;

	// シェーダー読み込み用変数
	LPD3DXBUFFER err = NULL;
	LPD3DXBUFFER code = NULL;

	// 頂点シェーダーのコンパイル
	//----------------------------------------------------------------
	HRESULT hr = D3DXCompileShaderFromFile("hlsl/depthShadowVS.hlsl", NULL, NULL, "main", "vs_3_0", 0, &code, &err, &m_pDepthShadow->pVConstantTable);
	if (FAILED(hr))
	{
		MessageBox(NULL, (char*)err->GetBufferPointer(), "D3DXCompileShaderFromFile(VS)", MB_OK);
		err->Release();
		return;
	}
	// 頂点シェーダーの生成
	hr = pDevice->CreateVertexShader((LPDWORD)code->GetBufferPointer(), &m_pDepthShadow->pVertexShader);
	if (FAILED(hr))
	{
		MessageBox(NULL, "頂点シェーダー生成失敗", "CreateVertexShader(VS)", MB_OK);
		err->Release();
		return;
	}

	// ピクセルシェーダーのコンパイル
	//----------------------------------------------------------------
	hr = D3DXCompileShaderFromFile("hlsl/depthShadowPS.hlsl", NULL, NULL, "main", "ps_3_0", 0, &code, &err, &m_pDepthShadow->pPConstantTable);
	if (FAILED(hr))
	{
		MessageBox(NULL, (char*)err->GetBufferPointer(), "D3DXCompileShaderFromFile(PS)", MB_OK);
		err->Release();
		return;
	}
	// ピクセルシェーダーの生成
	hr = pDevice->CreatePixelShader((LPDWORD)code->GetBufferPointer(), &m_pDepthShadow->pPixelShader);
	if (FAILED(hr))
	{
		MessageBox(NULL, "ピクセルシェーダー生成失敗", "CreatePixelShader(PS)", MB_OK);
		err->Release();
		return;
	}
	// リリース
	if (err)err->Release();
	if (code)code->Release();

	// デクラレーション
	m_pDepthShadow->pVertexDeclaration = nullptr;
//	D3DVERTEXELEMENT9 decl[] = {
//		// パイプライン, オフセット, 型, , セマンティクス, ,
//		{ 0, 0, D3DDECLTYPE_FLOAT3, D3DDECLMETHOD_DEFAULT, D3DDECLUSAGE_POSITION, 0 },
//		D3DDECL_END()	// 終端子マクロ(ターミネータ)
//	};
//	pDevice->CreateVertexDeclaration(decl, &m_pDepthShadow->pVertexDeclaration);
}
