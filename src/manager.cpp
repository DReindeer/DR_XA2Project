//================================================================================
//
//	Auter : KENSUKE WATANABE
//	Data  : 2018/02/10
//
//--------------------------------------------------------------------------------
#include "main.h"
#include "manager.h"
#include "mode.h"
#include "modeTitle.h"
#include "modeGame.h"
#include "camera.h"
#include "light.h"
#include "InputKeyboard.h"
#include "InputMouse.h"
#include "inputJoyStick.h"
#include "inputXInput.h"
#include "modelManager.h"
#include "texManager.h"
#include "fontManager.h"
#include "sceneFont.h"
#include "renderer.h"
#include "debugStr.h"
#include "scene.h"
#include "effect3DPoly.h"
#include "mainController.h"
#include "motionManager.h"
#include "modeLoading.h"
#include "depthShadow.h"
#include "deferredRendering.h"
#include "lightCamera.h"
#include "xa2Manager.h"
#include "xa2SourceVoiceManager.h"

//================================================================================
// 定数定義
//--------------------------------------------------------------------------------
static const int MAX_EFFECT = 512;		// エフェクトの最大数

//================================================================================
// 静的メンバ変数の宣言と初期化
//--------------------------------------------------------------------------------
CMode *CManager::m_pMode = nullptr;
CMode *CManager::m_pLoadingMode = nullptr;
CRenderer *CManager::m_pRenderer = nullptr;
CInputKeyboard* CManager::m_pKeyboard = nullptr;
CInputMouse* CManager::m_pMouse = nullptr;
CInputJoyStick* CManager::m_pJoyStick = nullptr;
CInputXinput *CManager::m_pXInput = nullptr;
CModelManager *CManager::m_pModelManager = nullptr;		// モデルマネージャ
CTexManager *CManager::m_pTexManager = nullptr;			// テクスチャマネージャ
CFontManager *CManager::m_pFontManager = nullptr;
CMotionManager *CManager::m_pMotionManager = nullptr;
XA2Manager *CManager::m_pXA2Manager = nullptr;			// XA2soundマネージャ
CCamera *CManager::m_pCamera = nullptr;					// ライト
CLight *CManager::m_pLight = nullptr;					// カメラ
CLightCamera *CManager::m_pLightCamera = nullptr;		// カメラ
CMainController *CManager::m_pMainController = nullptr;
DepthShadow *CManager::m_pDepthShadow = nullptr;		// 深度バッファシャドウ
DefferdRendering *CManager::m_defferdRendering = nullptr;	// デファードレンダリング
const float CManager::m_masterScale = 1.f;				// world空間上のスケール感を一律に変更

D3DVIEWPORT9 CManager::m_viewPortOld;

bool CManager::m_loadingNow = false;
bool CManager::m_loadingFlag = false; 
std::thread CManager::m_threadLoading;
std::recursive_mutex CManager::m_mutex;

//================================================================================
// 初期化
//--------------------------------------------------------------------------------
HRESULT CManager::Init(HINSTANCE hInstance, HWND hWnd, bool bWindow)
{
	// テクスチャマネージャの生成
	m_pTexManager = new CTexManager;
	m_pTexManager->Init();

	// レンダラーの生成
	m_pRenderer = new CRenderer;
	m_pRenderer->Init(hWnd, bWindow);

	// カメラ
	m_pCamera = new CCamera();
	m_pCamera->Init();

	Create(hInstance, hWnd);

	return S_OK;
}

void CManager::Create(HINSTANCE hInstance, HWND hWnd)
{
	// モデルマネージャの生成
	m_pModelManager = new CModelManager;
	m_pModelManager->Init();

	// フォントマネージャ
//	m_pFontManager = new CFontManager;
//	m_pFontManager->Init(hInstance);

	// キーボード
	m_pKeyboard = new CInputKeyboard();
	m_pKeyboard->Init(hInstance, hWnd);
	// ジョイスティック
	m_pJoyStick = new CInputJoyStick();
	m_pJoyStick->Init(hInstance, hWnd);
	// Xインプット
	m_pXInput = new CInputXinput();
	m_pXInput->Init(hInstance);
	// マウス
	m_pMouse = new CInputMouse();
	m_pMouse->Init(hInstance, hWnd);

	// xaudio2
	m_pXA2Manager = new XA2Manager(hWnd);

	// ライト
//	m_pLight = new CLight();
//	m_pLight->Init();

	// ライトカメラ
	m_pLightCamera = new CLightCamera();
	m_pLightCamera->Init();

	// メインコントローラ
	m_pMainController = new CMainController();
	m_pMainController->Init();

	// 3Dエフェクト生成
	CEffect3DPoly::Create(MAX_EFFECT, TEXNAME_EFFECT_WTOBLINE);

	// 深度バッファシャドウ表示用のポリゴン
	m_pDepthShadow = DepthShadow::Create();

	// ディファードレンダリング
	m_defferdRendering = m_defferdRendering->Create();

	// ビューポートの取得
	m_pRenderer->GetDevice()->GetViewport(&m_viewPortOld);

	return;
}

// ------- 終了
void CManager::Uninit()
{
	if (m_pDepthShadow)
	{
		m_pDepthShadow->Uninit();
		delete m_pDepthShadow;
		m_pDepthShadow = nullptr;
	}

	if (m_defferdRendering)
	{
		m_defferdRendering->Uninit();
		delete m_defferdRendering;
		m_defferdRendering = nullptr;
	}

	// モードの終了
	if (m_pMode != nullptr)
	{
		m_pMode->Uninit();
		delete m_pMode;
		m_pMode = nullptr;
	}

	// Scene全リリース
	CScene::ReleaseAll();

	// デバッグ文字リリース
#ifdef _DEBUG
//	CDebugStr::ReleaseAll();
#endif

	// メインコントローラ
	if (m_pCamera != nullptr)
	{
		m_pMainController->Uninit();
		delete m_pMainController;
		m_pMainController = nullptr;
	}

	// カメラの破棄
	if (m_pCamera != nullptr)
	{
		m_pCamera->Uninit();
		delete m_pCamera;
		m_pCamera = nullptr;
	}

	if (m_pLightCamera)
	{
		m_pLightCamera->Uninit();
		delete m_pLightCamera;
		m_pLightCamera = nullptr;
	}

	// キーボードの破棄
	if (m_pKeyboard)
	{
		m_pKeyboard->Uninit();
		delete m_pKeyboard;
		m_pKeyboard = nullptr;
	}

	// Xインプットの破棄
	if (m_pXInput)
	{
		m_pXInput->Uninit();
		delete m_pXInput;
		m_pXInput = nullptr;
	}

	// ジョイスティックの破棄
	if (m_pJoyStick)
	{
		m_pJoyStick->Uninit();
		delete m_pJoyStick;
		m_pJoyStick = nullptr;
	}

	// マウスの破棄
	if (m_pMouse)
	{
		m_pMouse->Uninit();
		delete m_pMouse;
		m_pMouse = nullptr;
	}

	// モデルマネージャの破棄
	if (m_pModelManager != nullptr)
	{
		m_pModelManager->Uninit();
		delete m_pModelManager;
		m_pModelManager = nullptr;
	}

	// テクスチャマネージャの破棄
	if (m_pTexManager != nullptr)
	{
		m_pTexManager->Uninit();
		delete m_pTexManager;
		m_pTexManager = nullptr;
	}

	// モーションマネージャの破棄
	if (m_pMotionManager != nullptr)
	{
		m_pMotionManager->ReleaseAll();
		delete m_pMotionManager;
		m_pMotionManager = nullptr;
	}

	if (m_pFontManager != nullptr)
	{
		m_pFontManager->Uninit();
		delete m_pFontManager;
		m_pFontManager = nullptr;
	}

	// xa2SoundManager
	if (m_pXA2Manager != nullptr)
	{
		delete m_pXA2Manager;
		m_pXA2Manager = nullptr;
	}

	// レンダラーの破棄
	if (m_pRenderer != nullptr)
	{
		m_pRenderer->Uninit();
		delete m_pRenderer;
		m_pRenderer = nullptr;
	}
}

// ------- 更新
void CManager::Update(HWND hWnd)
{
	// キーボード
	if (m_pKeyboard)m_pKeyboard->Update();
	// ジョイスティック
	if (m_pJoyStick)m_pJoyStick->Update(); 
	// Xインプット
	if (m_pXInput)m_pXInput->Update();
	// マウス
	if (m_pMouse)m_pMouse->Update(hWnd);

	// カメラの前更新
	if (m_pCamera != nullptr)m_pCamera->UpdateBigin();

	// 各モードの更新
	m_pMode->Update();

	// メインコントローラ
	m_pMainController->Update();

	// scene全更新
	CScene::UpdateAll();

	// カメラの更新
	if (m_pCamera != nullptr)m_pCamera->Update();

	// ライトの更新
	m_pLightCamera->Update();

	// レンダリング系フラグ更新
	m_pDepthShadow->Update();
	m_defferdRendering->Update();

	// サウンド更新
	m_pXA2Manager->GetSourceVoiceManager()->Update();
}

// ------- 描画
void CManager::Draw()
{
	// Direct3Dによる描画の開始
	m_pRenderer->GetDevice()->BeginScene();

	// デファードレンダリングの設定
	//--------------------------------------------------------------------------------
	if (m_defferdRendering)
	{
		m_defferdRendering->DrawDeviceBegin();

		// 各モードの描画
		m_pMode->Draw();

		// 深度バッファシャドウを行うオブジェクトを全描画
		CScene::DrawDefferdRenderingAll();

		// デファードレンダリング終了
		m_defferdRendering->DrawDeviceEnd();
	}
	//--------------------------------------------------------------------------------

	// 描画の開始
	if (m_pRenderer->DrawBigin() == false)return;

	// カメラのマトリクスの設定
	m_pCamera->Set();

	// 各モードの描画
	m_pMode->Draw();

	// scene全描画
	CScene::DrawAll();

	// 描画開始
	m_pRenderer->DrawEnd();

	// Direct3Dによる描画の終了
	m_pRenderer->GetDevice()->EndScene();

	// バックバッファとフロントバッファの入れ替え
	m_pRenderer->GetDevice()->Present(nullptr, nullptr, nullptr, nullptr);
}

void CManager::LoadMode(CMode *mode)
{
	m_loadingNow = true;

	// 前回モードの終了処理
	if (m_pMode != nullptr)
	{
		m_pMode->Uninit();
		delete m_pMode;
		m_pMode = nullptr;
	}

#ifdef THREAD_ON
	// モードロードの初期化
	m_pMode = new ModeLoading(mode);
	m_pMode->Init();

	// サブスレッドで引数のモードの初期化処理
	m_threadLoading = std::thread(InitMode, mode);

	// ロード中のモードを一時保存
	m_pLoadingMode = mode;
#else
	SetMode(mode);
#endif
}

// -------- モード初期化
void CManager::InitMode(CMode *mode)
{
	if (mode != nullptr)
	{
		mode->Init();
	}

	m_loadingFlag = true;
}

// 
void CManager::EndLoading(CMode *mode)
{
	// モードロードの終了処理
	if (m_pMode != nullptr)
	{
		m_pMode->Uninit();
		delete m_pMode;
	}

	// サブスレッドの終了を待機
	m_threadLoading.join();

	// ロード中のモードをNULLに変更
	m_pLoadingMode = nullptr;

	// モード切り替え
	m_pMode = mode;

	m_loadingFlag = false;
	m_loadingNow = false;
}

void CManager::DestroyLoadingThread(CMode *mode)
{
	// モードロードの終了処理
	if (mode != nullptr)
	{
		mode->Uninit();
		delete mode;
	}

	// ロード中のモードをNULLに変更
	m_pLoadingMode = nullptr;

	// サブスレッドの終了を待機
	m_threadLoading.join();

	m_loadingFlag = false;
	m_loadingNow = false;
}

// -------- モード変更
void CManager::SetMode(CMode *mode)
{
	// モードの終了処理
	if (m_pMode != nullptr)
	{
		m_pMode->Uninit();
		delete m_pMode;
	}

	m_pMode = mode;
	if (m_pMode != nullptr)
	{
		m_pMode->Init();
	}
}

