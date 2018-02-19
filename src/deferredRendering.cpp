//================================================================================
//
//	Auter : KENSUKE WATANABE
//	Data  : 2018/02/10
//
//--------------------------------------------------------------------------------
#include "texManager.h"
#include "inputKeyboard.h"
#include "deferredRendering.h"
#include "manager.h"
#include "renderer.h"
#include "camera.h"
#include "lightCamera.h"
#include "scene2D.h"
#include "scene2DDeferred.h"
#include "scene2DBloom.h"
#include "scene2DPx.h"
#include "utility.h"

//--------------------------------------------------------------------------------
// デストラクタ
//--------------------------------------------------------------------------------
DefferdRendering::~DefferdRendering()
{
	// 終了処理
	Uninit();
}

//--------------------------------------------------------------------------------
// 初期化
//--------------------------------------------------------------------------------
void DefferdRendering::Init()
{
	// 変数の初期化
	m_posUV = D3DXVECTOR2(0.f, 0.f);
	m_widthUV = 1.f;
	m_heightUV = 1.f;
}

//--------------------------------------------------------------------------------
// 終了
//--------------------------------------------------------------------------------
void DefferdRendering::Uninit()
{
	for (int i = 0; i < MAX_RENDERTARGET; i++)
	{
		// テクスチャ
		if (m_renderTexture[i])
		{
			m_renderTexture[i]->Release();
			m_renderTexture[i] = nullptr;
		}
		// サーフェス
		if (m_renderSurface[i])
		{
			m_renderSurface[i]->Release();
			m_renderSurface[i] = nullptr;
		}
	}
	// テクスチャ
	if (m_renderTargetZBuffer)
	{
		m_renderTargetZBuffer->Release();
		m_renderTargetZBuffer = nullptr;
	}

	// シェーダーの解放
	if (m_pShaderSet)
	{
		// ピクセルシェーダー固定テーブル
		if (m_pShaderSet->pPConstantTable)
		{
			m_pShaderSet->pPConstantTable->Release();
			m_pShaderSet->pPConstantTable = nullptr;
		}
		// ピクセルシェーダー
		if (m_pShaderSet->pPixelShader)
		{
			m_pShaderSet->pPixelShader->Release();
			m_pShaderSet->pPixelShader = nullptr;
		}
		// 頂点シェーダー固定テーブル
		if (m_pShaderSet->pVConstantTable)
		{
			m_pShaderSet->pVConstantTable->Release();
			m_pShaderSet->pVConstantTable = nullptr;
		}
		// 頂点シェーダー
		if (m_pShaderSet->pVertexShader)
		{
			m_pShaderSet->pVertexShader->Release();
			m_pShaderSet->pVertexShader = nullptr;
		}

		delete m_pShaderSet;
		m_pShaderSet = nullptr;
	}
}

//--------------------------------------------------------------------------------
// 更新
//--------------------------------------------------------------------------------
void DefferdRendering::Update()
{
	// L押下で表示の切り替え
	if (CManager::GetKeyboard()->GetTrigger(DIK_L))
	{
		SwapDrawFlag();
	}
}

//--------------------------------------------------------------------------------
// VXシェーダーの設定
//--------------------------------------------------------------------------------
void DefferdRendering::SetVXShader(D3DXMATRIX mtxWorld)
{
	// スレッドセーフ(ロック)
#ifdef THREAD_ON
	std::unique_lock<std::recursive_mutex> locker = CManager::Locker();
#endif

	// D3Deviceの取得
	LPDIRECT3DDEVICE9 pDevice = CRenderer::GetDevice();
	// カメラ
	CCamera *pCamera = CManager::GetCamera();

	// 逆行列の転置行列
	D3DXMATRIX mtxWorldInverse;
	D3DXMATRIX mtxWIT;

	m_pShaderSet->pVConstantTable->SetValue(pDevice, "mtxWorld", mtxWorld, sizeof(D3DXMATRIX));

	D3DXMatrixInverse(&mtxWIT, nullptr, &mtxWorld);		// 逆行列
	D3DXMatrixTranspose(&mtxWIT, &mtxWIT);				// 転置行列
	m_pShaderSet->pVConstantTable->SetMatrix(pDevice, "mtxWIT", &mtxWIT);

	// マトリクスの設定
	D3DXMATRIX mtxWVP = (mtxWorld * pCamera->GetView() * pCamera->GetProj());
	m_pShaderSet->pVConstantTable->SetMatrix(pDevice, "mtxWVP", &mtxWVP);
	m_pShaderSet->pVConstantTable->SetMatrix(pDevice, "mtxW", &mtxWorld);
	m_pShaderSet->pVConstantTable->SetFloat(pDevice, "far", pCamera->GetFar());

	// 頂点シェーダーの設定
	pDevice->SetVertexShader(m_pShaderSet->pVertexShader);
}

//--------------------------------------------------------------------------------
// PXシェーダーの設定
//--------------------------------------------------------------------------------
void DefferdRendering::SetPXShader(D3DMATERIAL9* pMat)
{
	// スレッドセーフ(ロック)
#ifdef THREAD_ON
	std::unique_lock<std::recursive_mutex> locker = CManager::Locker();
#endif

	// D3Deviceの取得
	LPDIRECT3DDEVICE9 pDevice = CRenderer::GetDevice();

	// ピクセルシェーダーの設定
	//------------------------------------------------------------------------
	float power = pMat->Power + 50.f;
	m_pShaderSet->pPConstantTable->SetFloat(pDevice, "speclarPower", power);

	D3DXVECTOR3 modelEmissive(pMat->Emissive.r, pMat->Emissive.g, pMat->Emissive.b);
	m_pShaderSet->pPConstantTable->SetValue(pDevice, "modelEmissive", modelEmissive, sizeof(D3DXVECTOR3));

	D3DXVECTOR4 modelDiffuse = D3DXVECTOR4(pMat->Diffuse.r, pMat->Diffuse.g, pMat->Diffuse.b, pMat->Diffuse.a);
	m_pShaderSet->pPConstantTable->SetValue(pDevice, "modelDiffuse", modelDiffuse, sizeof(D3DXVECTOR4));

	// ピクセルシェーダーの設定
	pDevice->SetPixelShader(m_pShaderSet->pPixelShader);
}

//--------------------------------------------------------------------------------
// テクスチャ無しPXシェーダーの設定
//--------------------------------------------------------------------------------
void DefferdRendering::SetNoTexturePXShader(D3DMATERIAL9* pMat)
{
	// スレッドセーフ(ロック)
#ifdef THREAD_ON
	std::unique_lock<std::recursive_mutex> locker = CManager::Locker();
#endif

	// D3Deviceの取得
	LPDIRECT3DDEVICE9 pDevice = CRenderer::GetDevice();

	// ピクセルシェーダーの設定
	//------------------------------------------------------------------------
	float power = pMat->Power + 50.f;
	m_pShaderSet->pPConstantTableNoTex->SetFloat(pDevice, "speclarPower", power);
	m_pShaderSet->pPConstantTableNoTex->SetValue(pDevice, "resolution", D3DXVECTOR2(SCREEN_WIDTH, SCREEN_HEIGHT), sizeof(D3DXVECTOR2));

	D3DXVECTOR3 modelEmissive(pMat->Emissive.r, pMat->Emissive.g, pMat->Emissive.b);
	m_pShaderSet->pPConstantTableNoTex->SetValue(pDevice, "modelEmissive", modelEmissive, sizeof(D3DXVECTOR3));

	D3DXVECTOR4 modelDiffuse = D3DXVECTOR4(pMat->Diffuse.r, pMat->Diffuse.g, pMat->Diffuse.b, pMat->Diffuse.a);
	m_pShaderSet->pPConstantTableNoTex->SetValue(pDevice, "modelDiffuse", modelDiffuse, sizeof(D3DXVECTOR4));

	// ピクセルシェーダーの設定
	pDevice->SetPixelShader(m_pShaderSet->pPixelShaderNoTex);
}

//--------------------------------------------------------------------------------
// 書き込み用のレンダーターゲットへ切り替え
//--------------------------------------------------------------------------------
void DefferdRendering::DrawDeviceBegin()
{
	// スレッドセーフ(ロック)
#ifdef THREAD_ON
	std::unique_lock<std::recursive_mutex> locker = CManager::Locker();
#endif

	// D3Deviceの取得
	LPDIRECT3DDEVICE9 pDevice = CRenderer::GetDevice();

	// Zバッファ切り替え
	pDevice->SetDepthStencilSurface(m_renderTargetZBuffer);

	for (int i = 0; i < MAX_RENDERTARGET; i++)
	{
		// レンダーターゲットを消す
		pDevice->SetRenderTarget(0, m_renderSurface[i]);
		// バックバッファ＆Ｚバッファのクリア
		pDevice->Clear(0, NULL, (D3DCLEAR_TARGET | D3DCLEAR_ZBUFFER | D3DCLEAR_STENCIL), D3DCOLOR_RGBA(255, 255, 255, 255), 1.f, 0);
	}

	// サーフェス切り替え
	for (int i = 0; i < MAX_RENDERTARGET; i++)
	{
		pDevice->SetRenderTarget(i, m_renderSurface[i]);
	}
}

//--------------------------------------------------------------------------------
// レンダーターゲットを戻す
//--------------------------------------------------------------------------------
void DefferdRendering::DrawDeviceEnd()
{
	// スレッドセーフ(ロック)
#ifdef THREAD_ON
	std::unique_lock<std::recursive_mutex> locker = CManager::Locker();
#endif

	// D3Deviceの取得
	LPDIRECT3DDEVICE9 pDevice = CRenderer::GetDevice();

	for (int i = 0; i < MAX_RENDERTARGET; i++)
	{
		// レンダーターゲットを消す
		pDevice->SetRenderTarget(i, nullptr);
	}

	// レンダーターゲットを戻す
	pDevice->SetRenderTarget(0, CManager::GetRenderer()->GetRenderTarget()->GetRenderTargetSurface(0));

	// Zバッファを戻す
	pDevice->SetDepthStencilSurface(*CManager::GetRenderer()->GetZBuffer());
}

//--------------------------------------------------------------------------------
// バックバッファ＆Ｚバッファのクリア
//--------------------------------------------------------------------------------
void DefferdRendering::ClearBackBuffer()
{
	// スレッドセーフ(ロック)
#ifdef THREAD_ON
	std::unique_lock<std::recursive_mutex> locker = CManager::Locker();
#endif

	// D3Deviceの取得
	LPDIRECT3DDEVICE9 pDevice = CRenderer::GetDevice();

	// Zバッファ切り替え
	pDevice->SetDepthStencilSurface(m_renderTargetZBuffer);

	for (int i = 0; i < MAX_RENDERTARGET; i++)
	{
		// サーフェス切り替え
		pDevice->SetRenderTarget(0, m_renderSurface[i]);

		// バックバッファ＆Ｚバッファのクリア
		pDevice->Clear(0, NULL, (D3DCLEAR_TARGET | D3DCLEAR_ZBUFFER | D3DCLEAR_STENCIL), D3DCOLOR_RGBA(255, 255, 255, 255), 1.f, 0);
	}

	// Zバッファを戻す
	pDevice->SetDepthStencilSurface(CManager::GetRenderer()->GetRenderTarget()->GetRenderTargetZBuffer());

	// サーフェスを戻す
	pDevice->SetRenderTarget(0, CManager::GetRenderer()->GetRenderTarget()->GetRenderTargetSurface(1));
}

//--------------------------------------------------------------------------------
// テクスチャ合成
//--------------------------------------------------------------------------------
void DefferdRendering::DrawCombineTexture()
{
	// スレッドセーフ(ロック)
#ifdef THREAD_ON
	std::unique_lock<std::recursive_mutex> locker = CManager::Locker();
#endif
	// D3Deviceの取得
	LPDIRECT3DDEVICE9 pDevice = CRenderer::GetDevice();

	pDevice->SetVertexShader(nullptr);

	// Zバッファ切り替え
	pDevice->SetDepthStencilSurface(m_renderTargetZBuffer);

	// サーフェス切り替え
	pDevice->SetRenderTarget(0, m_combineSurface);

	// バックバッファ＆Ｚバッファのクリア
	pDevice->Clear(0, NULL, (D3DCLEAR_TARGET | D3DCLEAR_ZBUFFER | D3DCLEAR_STENCIL), D3DCOLOR_RGBA(255, 255, 255, 255), 1.f, 0);

	// ライトの位置のカメラの設定に変更
	CLightCamera *pLightCamera = CManager::GetLightCamera();
	pLightCamera->SetOrthoLH();

	CCamera *pCamera = CManager::GetCamera();
	// カメラ座標 - W
	D3DXVECTOR3 eye = pCamera->GetPos();
	m_pPConstantTableCombine->SetValue(pDevice, "posEyeW", eye, sizeof(D3DXVECTOR3));
	// ライト視線 - W
	D3DXVECTOR3 lightDirW = pLightCamera->GetDir();
	m_pPConstantTableCombine->SetValue(pDevice, "lightDirW", &lightDirW, sizeof(D3DXVECTOR3));

	// 点光源
	m_pPConstantTableCombine->SetValue(pDevice, "attenuation012", D3DXVECTOR3(0.f, 1.f, 0.f), sizeof(D3DXVECTOR3));
	D3DXVECTOR4 pointLightPos[4] =
	{
		D3DXVECTOR4(0.f, 3.f, -1.f, 0.f),
		D3DXVECTOR4(3.f, 0.5f, -2.f, 0.f),
		D3DXVECTOR4(-3.f, 0.5f, -2.f, 0.f),
		D3DXVECTOR4(0.f, 0.1f, 0.5f, 0.f)
	};
	m_pPConstantTableCombine->SetVectorArray(pDevice, "posPointLightW", pointLightPos, 4);
	D3DXVECTOR4 pointLightCol[4] =
	{
		D3DXVECTOR4(1.f, 0.f, 0.f, 1.f),
		D3DXVECTOR4(1.f, 1.f, 0.f, 1.f),
		D3DXVECTOR4(0.f, 1.f, 0.f, 1.f),
		D3DXVECTOR4(0.f, 0.f, 1.f, 1.f)
	};
	m_pPConstantTableCombine->SetVectorArray(pDevice, "colPointLight", pointLightCol, 4);

	// Gバッファ取得
	pDevice->SetTexture(0, m_renderTexture[0]);
	pDevice->SetTexture(1, m_renderTexture[1]);
	pDevice->SetTexture(2, m_renderTexture[2]);
	pDevice->SetTexture(3, m_renderTexture[3]);

	// ピクセルシェーダー設定
	pDevice->SetPixelShader(m_pPixelShaderCombine);
}

//--------------------------------------------------------------------------------
// レンダーターゲットを戻す
//--------------------------------------------------------------------------------
void DefferdRendering::DrawCombineEnd()
{
	// スレッドセーフ(ロック)
#ifdef THREAD_ON
	std::unique_lock<std::recursive_mutex> locker = CManager::Locker();
#endif

	// ブルームの設定
	UpdateBloom();

	// D3Deviceの取得
	LPDIRECT3DDEVICE9 pDevice = CRenderer::GetDevice();

	for (int i = 0; i < MAX_RENDERTARGET; i++)
	{
		// レンダーターゲットを消す
		pDevice->SetRenderTarget(i, nullptr);
	}

	pDevice->SetVertexShader(nullptr);
	pDevice->SetPixelShader(nullptr);

	// レンダーターゲットを戻す
	pDevice->SetRenderTarget(0, CManager::GetRenderer()->GetRenderTarget()->GetRenderTargetSurface(1));

	// Zバッファを戻す
	pDevice->SetDepthStencilSurface(CManager::GetRenderer()->GetRenderTarget()->GetRenderTargetZBuffer());
}

//--------------------------------------------------------------------------------
// テクスチャ合成
//--------------------------------------------------------------------------------
void DefferdRendering::DrawCombineBloom()
{
	// スレッドセーフ(ロック)
#ifdef THREAD_ON
	std::unique_lock<std::recursive_mutex> locker = CManager::Locker();
#endif
	// D3Deviceの取得
	LPDIRECT3DDEVICE9 pDevice = CRenderer::GetDevice();

	pDevice->SetVertexShader(nullptr);

	//----------------------------------------------------------------
	// Gバッファテクスチャに合成
	pDevice->SetDepthStencilSurface(m_renderTargetZBuffer);	// Zバッファ切り替え

	// デバイスにビューポートの設定
	D3DXVECTOR2 screenSize = CManager::GetRenderer()->GetScreenSize();
	D3DVIEWPORT9 viewData = { 0, 0, (DWORD)screenSize.x, (DWORD)screenSize.y, 0.0f, 1.0f };
	pDevice->SetViewport(&viewData);

	pDevice->SetRenderTarget(0, m_bloomSurface[3]);			// レンダーターゲット切り替え
	// バックバッファ＆Ｚバッファのクリア
	pDevice->Clear(0, NULL, (D3DCLEAR_TARGET | D3DCLEAR_ZBUFFER | D3DCLEAR_STENCIL), D3DCOLOR_RGBA(255, 255, 255, 255), 1.f, 0);

	pDevice->SetTexture(0, m_bloomTexture[0]);				// サンプラー設定
	pDevice->SetTexture(1, m_combineTexture);				// サンプラー設定
	pDevice->SetPixelShader(m_pPBloomCombine);				// ピクセルシェーダー設定
}

//--------------------------------------------------------------------------------
// レンダーターゲットを戻す
//--------------------------------------------------------------------------------
void DefferdRendering::DrawCombineBloomEnd()
{
	// スレッドセーフ(ロック)
#ifdef THREAD_ON
	std::unique_lock<std::recursive_mutex> locker = CManager::Locker();
#endif

	// D3Deviceの取得
	LPDIRECT3DDEVICE9 pDevice = CRenderer::GetDevice();

	pDevice->SetVertexShader(nullptr);
	pDevice->SetPixelShader(nullptr);

	// レンダーターゲットを戻す
	pDevice->SetRenderTarget(0, CManager::GetRenderer()->GetRenderTarget()->GetRenderTargetSurface(1));

	// Zバッファを戻す
	pDevice->SetDepthStencilSurface(CManager::GetRenderer()->GetRenderTarget()->GetRenderTargetZBuffer());
}

//--------------------------------------------------------------------------------
// ブルームの更新
//--------------------------------------------------------------------------------
void DefferdRendering::UpdateBloom()
{
	// スレッドセーフ(ロック)
#ifdef THREAD_ON
	std::unique_lock<std::recursive_mutex> locker = CManager::Locker();
#endif
	// D3Deviceの取得
	LPDIRECT3DDEVICE9 pDevice = CRenderer::GetDevice();
	D3DXVECTOR2 screenSize = CManager::GetRenderer()->GetScreenSize();
	D3DXVECTOR2 textureSize4 = D3DXVECTOR2(screenSize.x / 4.f, screenSize.y / 4.f);
	D3DXVECTOR2 textureSize16 = D3DXVECTOR2(screenSize.x / 16.f, screenSize.y / 16.f);

	// サンプラーステートの設定
	pDevice->SetSamplerState(0, D3DSAMP_ADDRESSU, D3DTADDRESS_MIRROR);	// テクスチャＵ値の繰り返し設定
	pDevice->SetSamplerState(0, D3DSAMP_ADDRESSV, D3DTADDRESS_MIRROR);	// テクスチャＶ値の繰り返し設定

	//----------------------------------------------------------------
	// 1/4テクスチャに書き込み
	pDevice->SetDepthStencilSurface(m_renderTargetZBuffer4);			// Zバッファ切り替え

	// デバイスにビューポートの設定
	D3DVIEWPORT9 viewData = { 0, 0, (DWORD)textureSize4.x, (DWORD)textureSize4.y, 0.0f, 1.0f };
	pDevice->SetViewport(&viewData);

	pDevice->SetRenderTarget(0, m_bloomSurface[0]);										// レンダーターゲット切り替え
	// バックバッファ＆Ｚバッファのクリア
	pDevice->Clear(0, NULL, (D3DCLEAR_TARGET | D3DCLEAR_ZBUFFER | D3DCLEAR_STENCIL), D3DCOLOR_RGBA(255, 255, 255, 0), 1.f, 0);

	pDevice->SetTexture(0, m_combineTexture);			// サンプラー設定
	m_pBloomPolygon[0]->Draw();							// 描画

	//----------------------------------------------------------------
	// 1/16テクスチャに書き込み
	pDevice->SetDepthStencilSurface(m_renderTargetZBuffer16);							// Zバッファ切り替え

	// デバイスにビューポートの設定
	viewData = { 0, 0, (DWORD)textureSize16.x, (DWORD)textureSize16.y, 0.0f, 1.0f };
	pDevice->SetViewport(&viewData);

	pDevice->SetRenderTarget(0, m_bloomSurface[1]);										// レンダーターゲット切り替え
	// バックバッファ＆Ｚバッファのクリア
	pDevice->Clear(0, NULL, (D3DCLEAR_TARGET | D3DCLEAR_ZBUFFER | D3DCLEAR_STENCIL), D3DCOLOR_RGBA(255, 255, 255, 0), 1.f, 0);

	pDevice->SetTexture(0, m_bloomTexture[0]);			// サンプラー設定
	m_pBloomPolygon[1]->Draw();							// 描画

	//----------------------------------------------------------------
	// 輝度抽出
	pDevice->SetRenderTarget(0, m_bloomSurface[2]);										// レンダーターゲット切り替え
	// バックバッファ＆Ｚバッファのクリア
	pDevice->Clear(0, NULL, (D3DCLEAR_TARGET | D3DCLEAR_ZBUFFER | D3DCLEAR_STENCIL), D3DCOLOR_RGBA(255, 255, 255, 0), 1.f, 0);

	pDevice->SetTexture(0, m_bloomTexture[1]);			// サンプラー設定
	pDevice->SetPixelShader(m_pPColorBrightPass);		// ピクセルシェーダー設定
	m_pBloomPolygon[2]->Draw();							// 描画

	for (int i = 0; i < 2; i++)
	{
		//----------------------------------------------------------------
		// ブルームH
		pDevice->SetRenderTarget(0, m_bloomSurface[1]);										// レンダーターゲット切り替え
	// バックバッファ＆Ｚバッファのクリア
		pDevice->Clear(0, NULL, (D3DCLEAR_TARGET | D3DCLEAR_ZBUFFER | D3DCLEAR_STENCIL), D3DCOLOR_RGBA(255, 255, 255, 0), 1.f, 0);

		pDevice->SetTexture(0, m_bloomTexture[2]);			// サンプラー設定
		m_pPCBloomH->SetValue(pDevice, "texSize", textureSize16, sizeof(D3DXVECTOR2));
		pDevice->SetPixelShader(m_pPBloomH);				// ピクセルシェーダー設定
		m_pBloomPolygon[1]->Draw();							// 描画

		//----------------------------------------------------------------
		// ブルームV
		pDevice->SetRenderTarget(0, m_bloomSurface[2]);										// レンダーターゲット切り替え
		// バックバッファ＆Ｚバッファのクリア
		pDevice->Clear(0, NULL, (D3DCLEAR_TARGET | D3DCLEAR_ZBUFFER | D3DCLEAR_STENCIL), D3DCOLOR_RGBA(255, 255, 255, 0), 1.f, 0);

		pDevice->SetTexture(0, m_bloomTexture[1]);			// サンプラー設定
		m_pPCBloomV->SetValue(pDevice, "texSize", textureSize16, sizeof(D3DXVECTOR2));
		pDevice->SetPixelShader(m_pPBloomV);				// ピクセルシェーダー設定
		m_pBloomPolygon[2]->Draw();							// 描画
	}

	//----------------------------------------------------------------
	// 1/4テクスチャに書き込み(ブルーム完成)
	pDevice->SetDepthStencilSurface(m_renderTargetZBuffer4);							// Zバッファ切り替え

	// デバイスにビューポートの設定
	viewData = { 0, 0, (DWORD)textureSize4.x, (DWORD)textureSize4.y, 0.0f, 1.0f };
	pDevice->SetViewport(&viewData);

	pDevice->SetRenderTarget(0, m_bloomSurface[0]);										// レンダーターゲット切り替え
		// バックバッファ＆Ｚバッファのクリア
	pDevice->Clear(0, NULL, (D3DCLEAR_TARGET | D3DCLEAR_ZBUFFER | D3DCLEAR_STENCIL), D3DCOLOR_RGBA(255, 255, 255, 0), 1.f, 0);

	pDevice->SetTexture(0, m_bloomTexture[2]);				// サンプラー設定
	m_pBloomPolygon[0]->Draw();								// 描画

	//----------------------------------------------------------------
	// 終了 - 設定を戻す

	// サンプラーステートの設定
	pDevice->SetSamplerState(0, D3DSAMP_ADDRESSU, D3DTADDRESS_WRAP);	// テクスチャＵ値の繰り返し設定
	pDevice->SetSamplerState(0, D3DSAMP_ADDRESSV, D3DTADDRESS_WRAP);	// テクスチャＶ値の繰り返し設定

	pDevice->SetPixelShader(nullptr);						// ピクセルシェーダーを取り敢えず初期化

	// デバイスにビューポートの設定
	pDevice->SetViewport(&CManager::GetViewPort());

	// レンダーターゲットを戻す
	pDevice->SetRenderTarget(0, CManager::GetRenderer()->GetRenderTarget()->GetRenderTargetSurface(1));

	// Zバッファを戻す
	pDevice->SetDepthStencilSurface(CManager::GetRenderer()->GetRenderTarget()->GetRenderTargetZBuffer());
}

//--------------------------------------------------------------------------------
// 作成処理
//--------------------------------------------------------------------------------
DefferdRendering *DefferdRendering::Create()
{
	// スレッドセーフ(ロック)
#ifdef THREAD_ON
	std::unique_lock<std::recursive_mutex> locker = CManager::Locker();
#endif

	// D3Deviceの取得
	LPDIRECT3DDEVICE9 pDevice = CRenderer::GetDevice();

	// DefferdRenderingの生成
	DefferdRendering *pRendering;
	pRendering = new DefferdRendering();

	pRendering->Init();

	// デプス・ステンシルサーフェスの生成 / 設定
	int screenSizeX = static_cast<int>(CManager::GetRenderer()->GetScreenSize().x);
	int screenSizeY = static_cast<int>(CManager::GetRenderer()->GetScreenSize().y);
	pDevice->CreateDepthStencilSurface(screenSizeX, screenSizeY, D3DFMT_D24S8, D3DMULTISAMPLE_NONE, 0, FALSE, &pRendering->m_renderTargetZBuffer, NULL);

	// デプス・ステンシルサーフェス 1/4
	pDevice->CreateDepthStencilSurface(screenSizeX / 4, screenSizeY / 4, D3DFMT_D24S8, D3DMULTISAMPLE_NONE, 0, FALSE, &pRendering->m_renderTargetZBuffer4, NULL);

	// デプス・ステンシルサーフェス 1//16
	pDevice->CreateDepthStencilSurface(screenSizeX / 16, screenSizeY / 16, D3DFMT_D24S8, D3DMULTISAMPLE_NONE, 0, FALSE, &pRendering->m_renderTargetZBuffer16, NULL);

	// ０番目のテクスチャ生成
	//--------------------------------------------------------------------------------
	pRendering->m_strFilePass[0] = "posW";
	// 空のテクスチャを生成
	D3DXCreateTexture(pDevice, screenSizeX, screenSizeY, 1, D3DUSAGE_RENDERTARGET, D3DFMT_A32B32G32R32F, D3DPOOL_DEFAULT, &pRendering->m_renderTexture[0]);
	// サーフェスの取得
	pRendering->m_renderTexture[0]->GetSurfaceLevel(0, &pRendering->m_renderSurface[0]);

	// １番目のテクスチャ生成
	//--------------------------------------------------------------------------------
	pRendering->m_strFilePass[1] = "color";
	// 空のテクスチャを生成
	D3DXCreateTexture(pDevice, screenSizeX, screenSizeY, 1, D3DUSAGE_RENDERTARGET, D3DFMT_A8R8G8B8, D3DPOOL_DEFAULT, &pRendering->m_renderTexture[1]);
	// サーフェスの取得
	pRendering->m_renderTexture[1]->GetSurfaceLevel(0, &pRendering->m_renderSurface[1]);

	// ２番目のテクスチャ生成
	//--------------------------------------------------------------------------------
	pRendering->m_strFilePass[2] = "normal";
	// 空のテクスチャを生成
	D3DXCreateTexture(pDevice, screenSizeX, screenSizeY, 1, D3DUSAGE_RENDERTARGET, D3DFMT_A8R8G8B8, D3DPOOL_DEFAULT, &pRendering->m_renderTexture[2]);
	// サーフェスの取得
	pRendering->m_renderTexture[2]->GetSurfaceLevel(0, &pRendering->m_renderSurface[2]);

	// ３番目のテクスチャ生成
	//--------------------------------------------------------------------------------
	pRendering->m_strFilePass[3] = "ambient";
	// 空のテクスチャを生成
	D3DXCreateTexture(pDevice, screenSizeX, screenSizeY, 1, D3DUSAGE_RENDERTARGET, D3DFMT_A8R8G8B8, D3DPOOL_DEFAULT, &pRendering->m_renderTexture[3]);
	// サーフェスの取得
	pRendering->m_renderTexture[3]->GetSurfaceLevel(0, &pRendering->m_renderSurface[3]);

	// 合成後のテクスチャ
	//--------------------------------------------------------------------------------
	// 空のテクスチャを生成
	D3DXCreateTexture(pDevice, screenSizeX, screenSizeY, 1, D3DUSAGE_RENDERTARGET, D3DFMT_A8R8G8B8, D3DPOOL_DEFAULT, &pRendering->m_combineTexture);
	// サーフェスの取得
	pRendering->m_combineTexture->GetSurfaceLevel(0, &pRendering->m_combineSurface);

	// ブルーム用テクスチャ(1/4)
	//--------------------------------------------------------------------------------
	// 空のテクスチャを生成
	D3DXCreateTexture(pDevice, screenSizeX / 4, screenSizeY / 4, 1, D3DUSAGE_RENDERTARGET, D3DFMT_A8R8G8B8, D3DPOOL_DEFAULT, &pRendering->m_bloomTexture[0]);
	// サーフェスの取得
	pRendering->m_bloomTexture[0]->GetSurfaceLevel(0, &pRendering->m_bloomSurface[0]);

	// ブルーム用テクスチャ(1/16) - 1
	//--------------------------------------------------------------------------------
	// 空のテクスチャを生成
	D3DXCreateTexture(pDevice, screenSizeX / 16, screenSizeY / 16, 1, D3DUSAGE_RENDERTARGET, D3DFMT_A8R8G8B8, D3DPOOL_DEFAULT, &pRendering->m_bloomTexture[1]);
	// サーフェスの取得
	pRendering->m_bloomTexture[1]->GetSurfaceLevel(0, &pRendering->m_bloomSurface[1]);

	// ブルーム用テクスチャ(1/16) - 2
	//--------------------------------------------------------------------------------
	// 空のテクスチャを生成
	D3DXCreateTexture(pDevice, screenSizeX / 16, screenSizeY / 16, 1, D3DUSAGE_RENDERTARGET, D3DFMT_A8R8G8B8, D3DPOOL_DEFAULT, &pRendering->m_bloomTexture[2]);
	// サーフェスの取得
	pRendering->m_bloomTexture[2]->GetSurfaceLevel(0, &pRendering->m_bloomSurface[2]);

	// ブルーム合成後のテクスチャ
	//--------------------------------------------------------------------------------
	// 空のテクスチャを生成
	D3DXCreateTexture(pDevice, screenSizeX, screenSizeY, 1, D3DUSAGE_RENDERTARGET, D3DFMT_A8R8G8B8, D3DPOOL_DEFAULT, &pRendering->m_bloomTexture[3]);
	// サーフェスの取得
	pRendering->m_bloomTexture[3]->GetSurfaceLevel(0, &pRendering->m_bloomSurface[3]);


	// シェーダーの設定
	//--------------------------------------------------------------------------------
	pRendering->CreateShader();

	// テクスチャマネージャ生成
	pRendering->m_pTexManager = new CTexManager;

	// 画面サイズの取得
	D3DXVECTOR2 size = CUtility::RatioToScrXY(D3DXVECTOR2(0.1f, 0.1f));

	for (int i = 0; i < MAX_RENDERTARGET; i++)
	{
		// テクスチャマネージャへの設定
		pRendering->m_pTexManager->SetTex(pRendering->m_strFilePass[i], pRendering->m_renderTexture[i]);

		// CScene2Dのポリゴン作成
		pRendering->m_pRenderPolygon[i] = CScene2D::Create(D3DXVECTOR3(size.x - 0.5f, size.y - 0.5f + (size.y * 2 * i), 0.f), D3DXVECTOR3(0.f, 0.f, 0.f),
			D3DXVECTOR3(size.x, size.y, 0.f), pRendering->m_strFilePass[i], 9, pRendering->m_pTexManager);
		pRendering->m_pRenderPolygon[i]->SetOverall();					// 遷移で消されない
		pRendering->m_pRenderPolygon[i]->SetCol(255, 255, 255, 255);
		pRendering->m_pRenderPolygon[i]->SetAlpha(false);
	}

	// 合成後のテクスチャ
	//--------------------------------------------------------------------------------
	char *texName = "combine";
	// テクスチャマネージャへの設定
	pRendering->m_pTexManager->SetTex(texName, pRendering->m_combineTexture);

	// Gバッファ作成用ポリゴン作成
	pRendering->m_pCombinePolygon = pRendering->m_pCombinePolygon->Create(D3DXVECTOR3(screenSizeX * 0.5f - 0.5f, screenSizeY * 0.5f - 0.5f, 0.f), D3DXVECTOR3(0.f, 0.f, 0.f),
		D3DXVECTOR3(screenSizeX * 0.5f, screenSizeY * 0.5f, 0.f), texName, 9, pRendering->m_pTexManager);
	pRendering->m_pCombinePolygon->SetOverall();					// 遷移で消されない
	pRendering->m_pCombinePolygon->SetCol(255, 255, 255, 255);
	pRendering->m_pCombinePolygon->SetAlpha(false);

	// Gバッファ描画用のポリゴン作成
	pRendering->m_pGPolygon = CScene2D::Create(D3DXVECTOR3(size.x - 0.5f, size.y - 0.5f + (size.y * 2 * 3), 0.f), D3DXVECTOR3(0.f, 0.f, 0.f),
		D3DXVECTOR3(size.x, size.y, 0.f), texName, 9, pRendering->m_pTexManager);
	pRendering->m_pGPolygon->SetOverall();				// 遷移で消されない
	pRendering->m_pGPolygon->SetCol(255, 255, 255, 255);
	pRendering->m_pGPolygon->SetAlpha(false);

	// ブルーム用のテクスチャ
	//--------------------------------------------------------------------------------
	// テクスチャマネージャへの設定
	char *texNameBloom[] =
	{
		"bloom1/4",
		"bloom1/16-1",
		"bloom1/16-2",
		"bloomCombine"
	};
	pRendering->m_pTexManager->SetTex(texNameBloom[0], pRendering->m_bloomTexture[0]);
	pRendering->m_pTexManager->SetTex(texNameBloom[1], pRendering->m_bloomTexture[1]);
	pRendering->m_pTexManager->SetTex(texNameBloom[2], pRendering->m_bloomTexture[2]);
	pRendering->m_pTexManager->SetTex(texNameBloom[3], pRendering->m_bloomTexture[3]);

	// ブルーム用ポリゴン作成
	pRendering->m_pBloomPolygon[0] = CScene2DPx::Create(D3DXVECTOR3(screenSizeX * 0.5f / 4 - 0.5f, screenSizeY * 0.5f / 4 - 0.5f, 0.f), D3DXVECTOR3(0.f, 0.f, 0.f),
		D3DXVECTOR3(screenSizeX * 0.5f / 4, screenSizeY * 0.5f / 4, 0.f), texName, 9, pRendering->m_pTexManager);
	pRendering->m_pBloomPolygon[0]->SetOverall();					// 遷移で消されない
	pRendering->m_pBloomPolygon[0]->SetCol(255, 255, 255, 255);
	pRendering->m_pBloomPolygon[0]->SetAlpha(false);
	pRendering->m_pBloomPolygon[0]->SetDrawFlag(false);

	pRendering->m_pBloomPolygon[1] = CScene2DPx::Create(D3DXVECTOR3(screenSizeX * 0.5f / 16 - 0.5f, screenSizeY * 0.5f / 16 - 0.5f, 0.f), D3DXVECTOR3(0.f, 0.f, 0.f),
		D3DXVECTOR3(screenSizeX * 0.5f / 16, screenSizeY * 0.5f / 16, 0.f), texNameBloom[0], 9, pRendering->m_pTexManager);
	pRendering->m_pBloomPolygon[1]->SetOverall();					// 遷移で消されない
	pRendering->m_pBloomPolygon[1]->SetCol(255, 255, 255, 255);
	pRendering->m_pBloomPolygon[1]->SetAlpha(false);
	pRendering->m_pBloomPolygon[1]->SetDrawFlag(false);

	pRendering->m_pBloomPolygon[2] = CScene2DPx::Create(D3DXVECTOR3(screenSizeX * 0.5f / 16 - 0.5f, screenSizeY * 0.5f / 16 - 0.5f, 0.f), D3DXVECTOR3(0.f, 0.f, 0.f),
		D3DXVECTOR3(screenSizeX * 0.5f / 16, screenSizeY * 0.5f / 16, 0.f), texNameBloom[1], 9, pRendering->m_pTexManager);
	pRendering->m_pBloomPolygon[2]->SetOverall();					// 遷移で消されない
	pRendering->m_pBloomPolygon[2]->SetCol(255, 255, 255, 255);
	pRendering->m_pBloomPolygon[2]->SetAlpha(false);
	pRendering->m_pBloomPolygon[2]->SetDrawFlag(false);

	// ブルーム合成後テクスチャ描画用のポリゴン作成
	pRendering->m_pBloomPolygon[3] = CScene2DPx::Create(D3DXVECTOR3(screenSizeX * 0.5f - 0.5f, screenSizeY * 0.5f - 0.5f, 0.f), D3DXVECTOR3(0.f, 0.f, 0.f),
		D3DXVECTOR3(screenSizeX * 0.5f, screenSizeY * 0.5f, 0.f), texNameBloom[2], 9, pRendering->m_pTexManager);
	pRendering->m_pBloomPolygon[3]->SetOverall();					// 遷移で消されない
	pRendering->m_pBloomPolygon[3]->SetCol(255, 255, 255, 255);
	pRendering->m_pBloomPolygon[3]->SetAlpha(false);
	pRendering->m_pBloomPolygon[3]->SetDrawFlag(false);

	// ブルーム描画用ポリゴン作成
	pRendering->m_pBloom = CScene2D::Create(D3DXVECTOR3(size.x - 0.5f, size.y - 0.5f + (size.y * 2 * 4), 0.f), D3DXVECTOR3(0.f, 0.f, 0.f),
		D3DXVECTOR3(size.x, size.y, 0.f), texNameBloom[0], 9, pRendering->m_pTexManager);
	pRendering->m_pBloom->SetOverall();					// 遷移で消されない
	pRendering->m_pBloom->SetCol(255, 255, 255, 255);
	pRendering->m_pBloom->SetAlpha(false);

//	pBloom = CScene2D::Create(D3DXVECTOR3(size.x * 3 - 0.5f, size.y * 3 - 0.5f, 0.f), D3DXVECTOR3(0.f, 0.f, 0.f),
//		D3DXVECTOR3(size.x, size.y, 0.f), texNameBloom[1], 9, pRendering->m_pTexManager);
//	pBloom->SetOverall();					// 遷移で消されない
//	pBloom->SetCol(255, 255, 255, 255);
//	pBloom->SetAlpha(false);
//
//	pBloom = CScene2D::Create(D3DXVECTOR3(size.x * 3 - 0.5f, size.y * 5 - 0.5f, 0.f), D3DXVECTOR3(0.f, 0.f, 0.f),
//		D3DXVECTOR3(size.x, size.y, 0.f), texNameBloom[2], 9, pRendering->m_pTexManager);
//	pBloom->SetOverall();					// 遷移で消されない
//	pBloom->SetCol(255, 255, 255, 255);
//	pBloom->SetAlpha(false);

	// ブルーム合成後テクスチャ作成用のポリゴン作成
	CScene2DBloom *pCombineBloom = CScene2DBloom::Create(D3DXVECTOR3(screenSizeX * 0.5f - 0.5f, screenSizeY * 0.5f - 0.5f, 0.f), D3DXVECTOR3(0.f, 0.f, 0.f),
		D3DXVECTOR3(screenSizeX * 0.5f, screenSizeY * 0.5f, 0.f), texNameBloom[3], 9, pRendering->m_pTexManager);
	pCombineBloom->SetOverall();					// 遷移で消されない
	pCombineBloom->SetCol(255, 255, 255, 255);
	pCombineBloom->SetAlpha(false);

	// ブルーム合成後テクスチャ描画用のポリゴン作成
	pRendering->m_combineBloomPolygon = CScene2D::Create(D3DXVECTOR3(screenSizeX * 0.5f - 0.5f, screenSizeY * 0.5f - 0.5f, 0.f), D3DXVECTOR3(0.f, 0.f, 0.f),
		D3DXVECTOR3(screenSizeX * 0.5f, screenSizeY * 0.5f, 0.f), texNameBloom[3], 1, pRendering->m_pTexManager);
	pRendering->m_combineBloomPolygon->SetOverall();					// 遷移で消されない
	pRendering->m_combineBloomPolygon->SetCol(255, 255, 255, 255);
	pRendering->m_combineBloomPolygon->SetAlpha(false);

	return pRendering;
}

//--------------------------------------------------------------------------------
// hlsl読み込み
//--------------------------------------------------------------------------------
void DefferdRendering::CreateShader()
{
	// スレッドセーフ(ロック)
#ifdef THREAD_ON
	std::unique_lock<std::recursive_mutex> locker = CManager::Locker();
#endif

	// D3Deviceの取得
	LPDIRECT3DDEVICE9 pDevice = CRenderer::GetDevice();

	// シェーダー用構造体
	m_pShaderSet = new SHADER_SET;

	// シェーダー読み込み用変数
	LPD3DXBUFFER err = NULL;
	LPD3DXBUFFER code = NULL;

	// 頂点シェーダーのコンパイル
	//----------------------------------------------------------------
	HRESULT hr = D3DXCompileShaderFromFile("hlsl/deferredVS.hlsl", NULL, NULL, "main", "vs_3_0", 0, &code, &err, &m_pShaderSet->pVConstantTable);
	if (FAILED(hr))
	{
		MessageBox(NULL, (char*)err->GetBufferPointer(), "D3DXCompileShaderFromFile(VS)", MB_OK);
		err->Release();
		return;
	}
	// 頂点シェーダーの生成
	hr = pDevice->CreateVertexShader((LPDWORD)code->GetBufferPointer(), &m_pShaderSet->pVertexShader);
	if (FAILED(hr))
	{
		MessageBox(NULL, "頂点シェーダー生成失敗", "CreateVertexShader(VS)", MB_OK);
		err->Release();
		return;
	}

	// ピクセルシェーダーのコンパイル
	//----------------------------------------------------------------
	hr = D3DXCompileShaderFromFile("hlsl/deferredPS.hlsl", NULL, NULL, "main", "ps_3_0", 0, &code, &err, &m_pShaderSet->pPConstantTable);
	if (FAILED(hr))
	{
		MessageBox(NULL, (char*)err->GetBufferPointer(), "D3DXCompileShaderFromFile(PS)", MB_OK);
		err->Release();
		return;
	}
	// ピクセルシェーダーの生成
	hr = pDevice->CreatePixelShader((LPDWORD)code->GetBufferPointer(), &m_pShaderSet->pPixelShader);
	if (FAILED(hr))
	{
		MessageBox(NULL, "ピクセルシェーダー生成失敗", "CreatePixelShader(PS)", MB_OK);
		err->Release();
		return;
	}

	// ピクセルシェーダーのコンパイル
	//----------------------------------------------------------------
	hr = D3DXCompileShaderFromFile("hlsl/deferredPS.hlsl", NULL, NULL, "NoTexture", "ps_3_0", 0, &code, &err, &m_pShaderSet->pPConstantTableNoTex);
	if (FAILED(hr))
	{
		MessageBox(NULL, (char*)err->GetBufferPointer(), "D3DXCompileShaderFromFile(PS)", MB_OK);
		err->Release();
		return;
	}
	// ピクセルシェーダーの生成
	hr = pDevice->CreatePixelShader((LPDWORD)code->GetBufferPointer(), &m_pShaderSet->pPixelShaderNoTex);
	if (FAILED(hr))
	{
		MessageBox(NULL, "ピクセルシェーダー生成失敗", "CreatePixelShader(PS)", MB_OK);
		err->Release();
		return;
	}

	// ピクセルシェーダーのコンパイル
	//----------------------------------------------------------------
	hr = D3DXCompileShaderFromFile("hlsl/deferredCombinePS.hlsl", NULL, NULL, "main", "ps_3_0", 0, &code, &err, &m_pPConstantTableCombine);
	if (FAILED(hr))
	{
		MessageBox(NULL, (char*)err->GetBufferPointer(), "D3DXCompileShaderFromFile(PS)", MB_OK);
		err->Release();
		return;
	}
	// ピクセルシェーダーの生成
	hr = pDevice->CreatePixelShader((LPDWORD)code->GetBufferPointer(), &m_pPixelShaderCombine);
	if (FAILED(hr))
	{
		MessageBox(NULL, "ピクセルシェーダー生成失敗", "CreatePixelShader(PS)", MB_OK);
		err->Release();
		return;
	}

	// ブルーム前の縮小用ピクセルシェーダーのコンパイル
	//----------------------------------------------------------------
	hr = D3DXCompileShaderFromFile("hlsl/ColorBrightPass.hlsl", NULL, NULL, "main", "ps_3_0", 0, &code, &err, &m_pPCColorBrightPass);
	if (FAILED(hr))
	{
		MessageBox(NULL, (char*)err->GetBufferPointer(), "D3DXCompileShaderFromFile(PS)", MB_OK);
		err->Release();
		return;
	}
	// ピクセルシェーダーの生成
	hr = pDevice->CreatePixelShader((LPDWORD)code->GetBufferPointer(), &m_pPColorBrightPass);
	if (FAILED(hr))
	{
		MessageBox(NULL, "ピクセルシェーダー生成失敗", "CreatePixelShader(PS)", MB_OK);
		err->Release();
		return;
	}

	// ブルームH
	//----------------------------------------------------------------
	hr = D3DXCompileShaderFromFile("hlsl/bloom.hlsl", NULL, NULL, "bloomH", "ps_3_0", 0, &code, &err, &m_pPCBloomH);
	if (FAILED(hr))
	{
		MessageBox(NULL, (char*)err->GetBufferPointer(), "D3DXCompileShaderFromFile(PS)", MB_OK);
		err->Release();
		return;
	}
	// ピクセルシェーダーの生成
	hr = pDevice->CreatePixelShader((LPDWORD)code->GetBufferPointer(), &m_pPBloomH);
	if (FAILED(hr))
	{
		MessageBox(NULL, "ピクセルシェーダー生成失敗", "CreatePixelShader(PS)", MB_OK);
		err->Release();
		return;
	}
	// ブルームW
	//----------------------------------------------------------------
	hr = D3DXCompileShaderFromFile("hlsl/bloom.hlsl", NULL, NULL, "bloomV", "ps_3_0", 0, &code, &err, &m_pPCBloomV);
	if (FAILED(hr))
	{
		MessageBox(NULL, (char*)err->GetBufferPointer(), "D3DXCompileShaderFromFile(PS)", MB_OK);
		err->Release();
		return;
	}
	// ピクセルシェーダーの生成
	hr = pDevice->CreatePixelShader((LPDWORD)code->GetBufferPointer(), &m_pPBloomV);
	if (FAILED(hr))
	{
		MessageBox(NULL, "ピクセルシェーダー生成失敗", "CreatePixelShader(PS)", MB_OK);
		err->Release();
		return;
	}
	// ブルーム合成用
	//----------------------------------------------------------------
	hr = D3DXCompileShaderFromFile("hlsl/bloomCombinePS.hlsl", NULL, NULL, "main", "ps_3_0", 0, &code, &err, &m_pPCBloomCombine);
	if (FAILED(hr))
	{
		MessageBox(NULL, (char*)err->GetBufferPointer(), "D3DXCompileShaderFromFile(PS)", MB_OK);
		err->Release();
		return;
	}
	// ピクセルシェーダーの生成
	hr = pDevice->CreatePixelShader((LPDWORD)code->GetBufferPointer(), &m_pPBloomCombine);
	if (FAILED(hr))
	{
		MessageBox(NULL, "ピクセルシェーダー生成失敗", "CreatePixelShader(PS)", MB_OK);
		err->Release();
		return;
	}

	// リリース
	if (err)err->Release();
	if (code)code->Release();
}
