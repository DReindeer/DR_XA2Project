//================================================================================
//
//	Auter : KENSUKE WATANABE
//	Data  : 2018/02/10
//
//--------------------------------------------------------------------------------
#include "main.h"
#include <vector>
#include "renderer.h"
#include "debugStr.h"
#include "utility.h"
#include "stencilShadow.h"
// Manager
#include "manager.h"
#include "texManager.h"
#include "modelManager.h"
// Game
#include "mainController.h"
#include "camera.h"
#include "car.h"
#include "speedMeter.h"
#include "checkTime.h"
#include "course.h"
#include "courseAkina.h"
#include "courseIroha.h"
// Input
#include "inputKeyboard.h"
#include "inputXInput.h"
// Mode
#include "modeTitle.h"
#include "modeGame.h"
#include "modeResult.h"
// Scene
#include "scene.h"
#include "scene2D.h"
#include "scene3DPoly.h"
#include "sceneModel.h"
#include "sceneField.h"
#include "sceneFieldX.h"
#include "sceneSkyDome.h"
#include "sceneSkyBox.h"
#include "sceneBoxX.h"
#include "sceneCylinder.h"
#include "sceneCircleShadow.h"
#include "bgCylinder.h"
#include "effect3DPoly.h"
#include "gravityObj.h"
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
static const char* POLYGON_TEXTURENAME = "data/TEXTURE/wara.jpg";
static const char* TEXNAME_FIELD = "data/TEXTURE/FIELD/douro.jpg";
static const char* TEXNAME_SKYDOME = "data/TEXTURE/sky000.jpg";
static const char* TEXNAME_SKYBOX = "data/TEXTURE/skyBox/box001.jpg";
static const char* TEXNAME_CYLINDERMOUNTAIN = "data/TEXTURE/mountain000.png";
static const char* MODELNAME_HAKO = "data/MODEL/hako.x";
static const char* MODELNAME_KOYA = "data/MODEL/MD/koya.x";
static const char* TEXNAME_BGCYLINDER = "data/TEXTURE/BG/WtoB.png";

static const char* MODELNAME_CAR_BODY = "data/MODEL/car/body.x";
static const char* MOTIONNAME_MITY = "data/MODEL/mity/motion.txt";
static const char* MOTIONNAME_SHEEP = "data/MODEL/sheep/motion.txt";

// UI
static const char* TEXNAME_MINIMAP_IROHA = "data/TEXTURE/GameUI/minimap_iroha.png"; 

static const int MAX_EFFECT = 512;		// エフェクトの最大数

//================================================================================
// コンストラクタ
//================================================================================
CModeGame::CModeGame() : m_pCourse(nullptr)
						, m_pSpeedMeter(nullptr)
						, m_pCheckTime(nullptr)
{
	SetModeType(MODETYPE::GAME);
}

//================================================================================
// デストラクタ
//================================================================================
CModeGame::~CModeGame()
{

}

//================================================================================
// 初期化処理
//================================================================================
void CModeGame::Init()
{
	Create(this);

	// 再生
	XA2SourceVoiceManager *pSoundObjectManager = CManager::GetXA2Manager()->GetSourceVoiceManager();
	pSoundObjectManager->GetXA2SourceVoice(this)->Play("data/BGM/BGM_Game.wav");
}

//================================================================================
// 生成処理
//================================================================================
void CModeGame::Create(CModeGame *pGame)
{
	// スカイボックス
//	CSceneSkybox *pSceneSkybox = nullptr;
//	pSceneSkybox = pSceneSkybox->Create(D3DXVECTOR3(0.f, 0.f, 0.f), D3DXVECTOR3(0.f, 0.f, 0.f), 1300.f, TEXNAME_SKYBOX, 0);
//	pSceneSkybox->SetForward(false);
	CSceneBoxX::Create(D3DXVECTOR3(0.f, 0.f, 0.f), D3DXVECTOR3(0.f, 0.f, 0.f), D3DXVECTOR3(13.f, 13.f, 13.f), "data/MODEL/SkyBox/skyBox3.x", 0);

//	D3DXVECTOR2 resize = D3DXVECTOR2(1920.f, 1080.f);
	
	// ステージの生成
	pGame->m_pCourse = CourseAkina::Create(1);

	// プレイヤー, 車の作成
	CManager::GetMainController()->SetName(MODELNAME_CAR_BODY);
	CCar::Create(pGame->m_pCourse->GetStartPos(), D3DXVECTOR3(0.f, 0.f, 0.f), MODELNAME_CAR_BODY, 4);

	// メーターの生成
	D3DXVECTOR2 meterPos = CUtility::RatioToScrXY(D3DXVECTOR2(0.85f, 0.8f));
//	D3DXVECTOR2 meterPos = D3DXVECTOR2(resize.x * 0.85f, resize.y * 0.8f);

	pGame->m_pSpeedMeter = pGame->m_pSpeedMeter->Create(meterPos, CUtility::RatioToScrX(0.08f));

	// タイムの生成
	D3DXVECTOR2 TimePos = CUtility::RatioToScrXY(D3DXVECTOR2(0.9f, 0.2f));
//	D3DXVECTOR2 TimePos = D3DXVECTOR2(resize.x * 0.9f, resize.y * 0.2f);
	pGame->m_pCheckTime = pGame->m_pCheckTime->Create(TimePos, CUtility::RatioToScrXY(D3DXVECTOR2(0.08f, 0.08f)));

	// ソースボイス作成
	XA2SourceVoice2D *pXA2SourceVoice = new XA2SourceVoice2D;
	pXA2SourceVoice->Create("data/BGM/BGM_Game.wav", -1, CManager::GetXA2Manager()->GetLoadWaveStreaming());

	// サウンドオブジェクト追加
	XA2SourceVoiceManager *pSoundObjectManager = CManager::GetXA2Manager()->GetSourceVoiceManager();
	pSoundObjectManager->SetXA2SoundObject(pXA2SourceVoice, pGame);

	// カメラ
	CCamera *pCam = CManager::GetCamera();
	pCam->Init();
	pCam->SetatObjType(pCam->AT_OBJTYPE::CONTROL_PLAYER);

	return;
}

//================================================================================
// 終了処理
//================================================================================
void CModeGame::Uninit()
{
	// 車の解放
	CCar::ReleaseAll();

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

	// カメラ注視点タイプの変更
	CCamera *pCam = CManager::GetCamera();
	pCam->Init();
	pCam->SetatObjType(pCam->AT_OBJTYPE::NONE);
}

//================================================================================
// 更新処理
//================================================================================
void CModeGame::Update()
{
	CInputKeyboard *keyboard = CManager::GetKeyboard();

	// メーター更新
	m_pSpeedMeter->Update();

	// 時間計測
	m_pCheckTime->Update();

	// チェックポイント更新
	m_pCourse->Update();

	// 画面遷移
	if ((keyboard && keyboard->GetTrigger(DIK_RETURN)) || GetCourse()->GetGoalCount() > 180)
	{
		CManager::LoadMode(new CModeResult);
	}
}

//================================================================================
// 描画処理
//================================================================================
void CModeGame::Draw()
{

}