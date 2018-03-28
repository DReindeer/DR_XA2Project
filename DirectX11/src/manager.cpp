// ======== ======== ======== ======== ======== ======== ======== ========
//
//
//	title : マネージャ
//	Auter : KENSUKE WATANABE
//
//
// ======== ======== ======== ======== ======== ======== ======== ========
#include "main.h"
#include "manager.h"
#include "camera.h"
#include "InputKeyboard.h"
#include "InputMouse.h"
#include "inputXInput.h"
#include "renderer.h"
#include "texManager.h"
#include "modelManager.h"
#include "mainController.h"

#include "scene.h"
#include "scene3DPoly.h"
#include "scene2D.h"
#include "sceneBox.h"
#include "sceneXModel.h"
#include "sceneXModelListener.h"

// XAudio2
#include "xa2Manager.h"
#include "xa2SourceVoiceManager.h"
#include "xa2SourceVoice2D.h"
#include "xa2SoundResourceManager.h"


// ======== ======== ======== ======== ======== ======== ======== ========
// 定数定義
// -------- -------- -------- -------- -------- -------- -------- --------
static const int MAX_EFFECT = 512;		// エフェクトの最大数

static const char* TEX_TEST = "data/TEXTURE/NeoYurumu/NEOYurumu.png";
static const char* TEX_FIELD = "data/TEXTURE/FIELD/kusa.jpg";

// ======== ======== ======== ======== ======== ======== ======== ========
// 静的メンバ変数の宣言と初期化
// -------- -------- -------- -------- -------- -------- -------- --------
CRenderer *CManager::m_pRenderer = nullptr;
CCamera *CManager::m_pCamera = nullptr;
CInputKeyboard* CManager::m_pKeyboard = nullptr;
CInputMouse* CManager::m_pMouse = nullptr;
CInputXinput *CManager::m_pXInput = nullptr;
CTexManager *CManager::m_pTexManager = nullptr;
CXModelManager *CManager::m_pXModelManager = nullptr;
CMainController *CManager::m_pMainController = nullptr;
XA2Manager *CManager::m_pXA2Manager = nullptr;			// XA2soundマネージャ

// ======== ======== ======== ======== ======== ======== ======== ========
// 初期化
// -------- -------- -------- -------- -------- -------- -------- --------
HRESULT CManager::Init(HINSTANCE hInstance, HWND hWnd, bool bWindow)
{
	if (!XMVerifyCPUSupport()) {
		MessageBox(nullptr, "DirectXMath非対応CPU", "DirectXMath", MB_OK);
		return E_FAIL;
	}

	// レンダラーの生成
	m_pRenderer = new CRenderer();
	m_pRenderer->Init(hWnd, bWindow);

	// カメラ
	m_pCamera = new CCamera();
	m_pCamera->Init();

	// テクスチャマネージャ
	m_pTexManager = new CTexManager();
	m_pTexManager->Init();

	// モデルマネージャ
	m_pXModelManager = new CXModelManager();
	m_pXModelManager->Init();

	// メインコントローラ
	m_pMainController = new CMainController();
	m_pMainController->Init();

	// キーボード
	m_pKeyboard = new CInputKeyboard();
	m_pKeyboard->Init(hInstance, hWnd);
	// Xインプット
	m_pXInput = new CInputXinput();
	m_pXInput->Init(hInstance);
	// マウス
	m_pMouse = new CInputMouse();
	m_pMouse->Init(hInstance, hWnd);

	// レンダリングテスト
	CScene3DPoly *pField = CScene3DPoly::Create(XMFLOAT3(0.f, 0.f, 0.f), 
		XMFLOAT3(0.f, PI * -0.5f, 0.f), XMFLOAT3(10.f, 10.f, 10.f), TEX_FIELD, 4);
	CSceneXModelListener *pXModel = CSceneXModelListener::Create(XMFLOAT3(0.f, 0.5f, 0.f), 
		XMFLOAT3(0.f, PI * 0.5f, 0.f), XMFLOAT3(0.1f, 0.1f, 0.1f), "data/MODEL/car/body.x", 4);

	CScene2D *p2D = CScene2D::Create(XMFLOAT2(50.f, 50.f), 0.f, XMFLOAT2(50.f, 50.f), TEX_TEST, 5);

	// xaudio2
	m_pXA2Manager = new XA2Manager(hWnd);

	// ソースボイス作成
	XA2SourceVoice2D *pXA2SourceVoice = new XA2SourceVoice2D;
	pXA2SourceVoice->Create("data/BGM/BGM_Yurumu.ogg", -1, XA2Manager::GetLoadOggStreaming());

	// サウンドオブジェクト追加
	XA2SourceVoiceManager *pSoundObjectManager = XA2Manager::GetSourceVoiceManager();
	pSoundObjectManager->SetXA2SoundObject(pXA2SourceVoice, this);

	pXA2SourceVoice->Play("data/BGM/BGM_Yurumu.ogg");

	return S_OK;
}

// ------- 終了
void CManager::Uninit(void)
{
	// Scene全リリース
	CScene::ReleaseAll();

	// 登録済みソースボイス全解放
	XA2SourceVoiceManager *pSoundObjectManager = XA2Manager::GetSourceVoiceManager();
	pSoundObjectManager->StopAndUninit();

	// サウンドリソース全解放
	XA2Manager::GetSoundResourceManager()->Uninit();

	// xa2SoundManager
	if (m_pXA2Manager != nullptr)
	{
		delete m_pXA2Manager;
		m_pXA2Manager = nullptr;
	}

	// キーボード
	if (m_pKeyboard)
	{
		m_pKeyboard->Uninit();
		delete m_pKeyboard;
		m_pKeyboard = nullptr;
	}

	// メインコントローラ
	if (m_pMainController)
	{
		m_pMainController->Uninit();
		delete m_pMainController;
		m_pMainController = nullptr;
	}

	// テクスチャマネージャ
	if (m_pTexManager)
	{
		m_pTexManager->Uninit();
		delete m_pTexManager;
		m_pTexManager = nullptr;
	}

	// モデルマネージャ
	if (m_pXModelManager)
	{
		m_pXModelManager->Uninit();
		delete m_pXModelManager;
		m_pXModelManager = nullptr;
	}

	// Xインプット
	if (m_pXInput)
	{
		m_pXInput->Uninit();
		delete m_pXInput;
		m_pXInput = nullptr;
	}

	// マウス
	if (m_pMouse)
	{
		m_pMouse->Uninit();
		delete m_pMouse;
		m_pMouse = nullptr;
	}

	// カメラ
	if (m_pCamera != nullptr)
	{
		m_pCamera->Uninit();
		delete m_pCamera;
		m_pCamera = nullptr;
	}

	// レンダラー
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
	// Xインプット
	if (m_pXInput)m_pXInput->Update();
	// マウス
	if (m_pMouse)m_pMouse->Update(hWnd);

	// メインコントローラ
	if (m_pMainController)m_pMainController->Update();

	// カメラの更新
	if (m_pCamera)m_pCamera->Update();

	// scene全更新
	CScene::UpdateAll();

	// サウンド更新
	m_pXA2Manager->GetSourceVoiceManager()->Update();
}

// ------- 描画
void CManager::Draw(void)
{
	// 描画の開始
	if (m_pRenderer->DrawBigin() == false)return;

	// カメラのマトリクスの設定
	if(m_pCamera)m_pCamera->Set();

	// scene全描画
	CScene::DrawAll();

	// メインコントローラ
	if (m_pMainController)m_pMainController->Draw();

	// 描画の終了
	m_pRenderer->DrawEnd();
}
