//================================================================================
//
//	Auter : KENSUKE WATANABE
//	Data  : 2018/02/10
//
//--------------------------------------------------------------------------------
#include "main.h"
#include <vector>
#include "utility.h"
#include "stencilShadow.h"
// Manager
#include "manager.h"
#include "texManager.h"
#include "modelManager.h"
// Game
#include "mainController.h"
#include "camera.h"
// Input
#include "inputKeyboard.h"
#include "inputXInput.h"
// Mode
#include "modeTitle.h"
#include "modeGame.h"
// Scene
#include "scene.h"
#include "scene2D.h"
#include "scene3D.h"
#include "sceneFont.h"
#include "scene3DPoly.h"
#include "sceneModel.h"
#include "sceneField.h"
#include "sceneSkyDome.h"
#include "sceneCircleShadow.h"
#include "effect3DPoly.h"
#include "gravityObj.h"
#include "sceneSkyBox.h"
#include "sceneBoxX.h"
// XAudio2
#include "xa2Manager.h"
#include "xa2SourceVoiceManager.h"
#include "xa2SourceVoice2D.h"
#include "xa2SoundResourceManager.h"
// ModeLoading
#include "modeLoading.h"

//================================================================================
// 定数定義
//--------------------------------------------------------------------------------
static const char* TEXNAME_BGCYLINDER = "data/TEXTURE/BG/WtoB.png";
static const char* TEXNAME_SKYBOX = "data/TEXTURE/skyBox/box001.jpg";
static const char* TEXNAME_UNDERLIGHT = "data/TEXTURE/whiteCircle.png";
static const char* TEXNAME_TITLE = "data/TEXTURE/Title/titlerogo.png";

static const char* MODELNAME_STANDARDCAR_BODY = "data/MODEL/car/Sbody.x";
static const char* MODELNAME_STANDARDCAR_TIRE = "data/MODEL/car/Stire.x";

// 車
static const D3DXVECTOR3 STANDARDCAR_RF = D3DXVECTOR3(0.35f, 0.1f, -0.50f);
static const D3DXVECTOR3 STANDARDCAR_RB = D3DXVECTOR3(0.35f, 0.1f, 0.67f);

static const int MAX_EFFECT = 512;		// エフェクトの最大数

CModeTitle::CModeTitle() : m_createEffectFrame(1),
	m_bR(true), m_bG(false), m_bB(false), m_gradR(0), m_gradG(0), m_gradB(0), m_titleCnt(0.f),
	m_pTitleLogo(nullptr)
{
	SetModeType(MODETYPE::TITLE); 
}

//================================================================================
// 初期化処理
//================================================================================
void CModeTitle::Init()
{
	Create(this);

	// 再生
	XA2SourceVoiceManager *pSoundObjectManager = CManager::GetXA2Manager()->GetSourceVoiceManager();
	pSoundObjectManager->GetXA2SourceVoice(this)->Play("data/BGM/BGM_Title.wav");
}

//================================================================================
// 生成処理
//================================================================================
void CModeTitle::Create(CModeTitle *pTitle)
{
	CCamera *pCam = CManager::GetCamera();
	pCam->Init();
	pCam->SetCamPos(D3DXVECTOR3(0.f, 2.f, 6.f) * CManager::GetMasterScale());
	pCam->SetCamPosAt(D3DXVECTOR3(0.f, 2.f, 0.f) * CManager::GetMasterScale());

	// スカイボックス
//	CSceneSkybox *pSceneSkybox = nullptr;
//	pSceneSkybox = pSceneSkybox->Create(D3DXVECTOR3(0.f, 0.f, 0.f), D3DXVECTOR3(0.f, 0.f, 0.f), 1300.f, TEXNAME_SKYBOX, 0);
//	pSceneSkybox->SetForward(false);
	CSceneBoxX::Create(D3DXVECTOR3(0.f, 0.f, 0.f), D3DXVECTOR3(0.f, D3DX_PI * 1.f, 0.f), D3DXVECTOR3(13.f, 13.f, 13.f), "data/MODEL/SkyBox/skyBox3.x", 0);
	
	// タイトルロゴ
	pTitle->m_pTitleLogo = CScene2D::Create(D3DXVECTOR3(CUtility::RatioToScrX(0.5f), CUtility::RatioToScrY(0.4f), 0.f), D3DXVECTOR3(0.f, 0.f, 0.f), D3DXVECTOR3(CUtility::RatioToScrX(0.3f), CUtility::RatioToScrY(0.15f), 0.f), TEXNAME_TITLE, 6);
	pTitle->m_pTitleLogo->SetCol(255, 255, 255, 255);
	pTitle->m_titleCnt = 0.f;

	// ボディ
	pTitle->m_pModel[0] = CSceneModel::Create(D3DXVECTOR3(0.f, 0.f, -1.f) * CManager::GetMasterScale(), D3DXVECTOR3(0.f, 0.f, 0.f), D3DXVECTOR3(1.5f, 1.5f, 1.5f), MODELNAME_STANDARDCAR_BODY, 4);
	pTitle->m_pModel[0]->SetBasePos(D3DXVECTOR3(0.f, 0.2f, 0.f) * CManager::GetMasterScale());
	pTitle->m_pModel[0]->SetParent(nullptr);

	// タイヤ
	// LF
	D3DXVECTOR3 tirePos = D3DXVECTOR3(-STANDARDCAR_RF.x, STANDARDCAR_RF.y, STANDARDCAR_RF.z);
	pTitle->m_pModel[1] = CSceneModel::Create(tirePos, D3DXVECTOR3(0.f, 0.f, 0.f), D3DXVECTOR3(1.f, 1.f, 1.f), MODELNAME_STANDARDCAR_TIRE, 4);
	pTitle->m_pModel[1]->SetParent(pTitle->m_pModel[0]);

	// RF
	tirePos = D3DXVECTOR3(STANDARDCAR_RF.x, STANDARDCAR_RF.y, STANDARDCAR_RF.z);
	pTitle->m_pModel[2] = CSceneModel::Create(tirePos, D3DXVECTOR3(0.f, 0.f, 0.f), D3DXVECTOR3(1.f, 1.f, 1.f), MODELNAME_STANDARDCAR_TIRE, 4);
	pTitle->m_pModel[2]->SetParent(pTitle->m_pModel[0]);

	// LB
	tirePos = D3DXVECTOR3(-STANDARDCAR_RB.x, STANDARDCAR_RB.y, STANDARDCAR_RB.z);
	pTitle->m_pModel[3] = CSceneModel::Create(tirePos, D3DXVECTOR3(0.f, 0.f, 0.f), D3DXVECTOR3(1.f, 1.f, 1.f), MODELNAME_STANDARDCAR_TIRE, 4);
	pTitle->m_pModel[3]->SetParent(pTitle->m_pModel[0]);

	// RB
	tirePos = D3DXVECTOR3(STANDARDCAR_RB.x, STANDARDCAR_RB.y, STANDARDCAR_RB.z);
	pTitle->m_pModel[4] = CSceneModel::Create(tirePos, D3DXVECTOR3(0.f, 0.f, 0.f), D3DXVECTOR3(1.f, 1.f, 1.f), MODELNAME_STANDARDCAR_TIRE, 4);
	pTitle->m_pModel[4]->SetParent(pTitle->m_pModel[0]);

	// ソースボイス作成
//	XA2LoadWave *pLoadWaveOnAll = CManager::GetXA2Manager()->GetLoadWaveOnAll();
	XA2LoadWave *pLoadWaveStreaming = CManager::GetXA2Manager()->GetLoadWaveStreaming();
	XA2SourceVoice2D *pXA2SourceVoice = new XA2SourceVoice2D;
	pXA2SourceVoice->Create("data/BGM/BGM_Title.wav", -1, pLoadWaveStreaming);
	// サウンドオブジェクト追加
	XA2SourceVoiceManager *pSoundObjectManager = CManager::GetXA2Manager()->GetSourceVoiceManager();
	pSoundObjectManager->SetXA2SoundObject(pXA2SourceVoice, pTitle);

	return;
}

//================================================================================
// 終了処理
//================================================================================
void CModeTitle::Uninit()
{
	// 全てのエフェクト表示を消す
	CEffect3DPoly::FlashAll();

	// シーン全解放
	CScene::FlagReleaseAll();

	// テクスチャリソース全解放
	CManager::GetTexManager()->ReleaseAll();

	// モデルリソース全解放
	CManager::GetModelManager()->ReleaseAll();

	// 登録済みソースボイス全解放
	XA2SourceVoiceManager *pSoundObjectManager = CManager::GetXA2Manager()->GetSourceVoiceManager();
	pSoundObjectManager->StopAndUninit();

	// サウンドリソース全解放
	CManager::GetXA2Manager()->GetSoundResourceManager()->Uninit();
}

//================================================================================
// 更新処理
//================================================================================
void CModeTitle::Update()
{
	CInputKeyboard *keyboard = CManager::GetKeyboard();
	CInputXinput *xInput = CManager::GetXInput();

	// 車を回転
	m_pModel[0]->AddRot(D3DXVECTOR3(0.f, 0.01f, 0.f));

	// タイトルを徐々に浮かび上がらせる
	m_titleCnt = min(255, m_titleCnt + 0.5f);
	m_pTitleLogo->SetCol(255, 255, 255, static_cast<int>(m_titleCnt));

	// 画面遷移
	if (keyboard)
	{
		if (keyboard->GetTrigger(DIK_RETURN))
		{
			CManager::LoadMode(new CModeGame);
			//CManager::SetMode(new CModeGame);
		}
	}
	if (xInput)
	{
		if (xInput->GetTrigger(XI_START) || xInput->GetTrigger(XI_A))
		{
			CManager::LoadMode(new CModeGame);
			//CManager::SetMode(new CModeGame);
		}
	}
}

//================================================================================
// 描画処理
//================================================================================
void CModeTitle::Draw()
{

}
