//================================================================================
//
//	Auter : KENSUKE WATANABE
//	Data  : 2018/02/10
//
//--------------------------------------------------------------------------------
#include "main.h"
#include <vector>
#include "utility.h"
// Manager
#include "manager.h"
#include "texManager.h"
#include "modelManager.h"
// Game
#include "camera.h"
// Input
#include "inputKeyboard.h"
#include "inputXInput.h"
// Mode
#include "modeResult.h"
#include "modeTitle.h"
#include "modeGame.h"
// Scene
#include "scene.h"
#include "scene2D.h"
#include "scene3D.h"
#include "sceneModel.h"
#include "sceneField.h"
#include "sceneSkyDome.h"
#include "effect3DPoly.h"
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

static const int MAX_EFFECT = 512;		// エフェクトの最大数

//================================================================================
// デストラクタ
//================================================================================
CModeResult::~CModeResult()
{

}

//================================================================================
// 初期化処理
//================================================================================
void CModeResult::Init()
{
	Create(this);

	// 再生
	XA2SourceVoiceManager *pSoundObjectManager = CManager::GetXA2Manager()->GetSourceVoiceManager();
	pSoundObjectManager->GetXA2SourceVoice(this)->Play("data/BGM/BGM_Result.wav");
}

//================================================================================
// 生成処理
//================================================================================
void CModeResult::Create(CModeResult *pResult)
{
	CManager::GetCamera()->Init();

	// スカイボックス
	//	CSceneSkybox *pSceneSkybox = nullptr;
	//	pSceneSkybox = pSceneSkybox->Create(D3DXVECTOR3(0.f, 0.f, 0.f), D3DXVECTOR3(0.f, 0.f, 0.f), 1300.f, TEXNAME_SKYBOX, 0);
	//	pSceneSkybox->SetForward(false);
	CSceneBoxX::Create(D3DXVECTOR3(0.f, 0.f, 0.f), D3DXVECTOR3(0.f, 0.f, 0.f), D3DXVECTOR3(13.f, 13.f, 13.f), "data/MODEL/SkyBox/skyBox3.x", 0);

	// ソースボイス作成
	XA2SourceVoice2D *pXA2SourceVoice = new XA2SourceVoice2D;
	pXA2SourceVoice->Create("data/BGM/BGM_Result.wav", -1, CManager::GetXA2Manager()->GetLoadWaveStreaming());
	// サウンドオブジェクト追加
	XA2SourceVoiceManager *pSoundObjectManager = CManager::GetXA2Manager()->GetSourceVoiceManager();
	pSoundObjectManager->SetXA2SoundObject(pXA2SourceVoice, pResult);

	return;
}

//================================================================================
// 終了処理
//================================================================================
void CModeResult::Uninit()
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
void CModeResult::Update()
{
	CInputKeyboard *keyboard = CManager::GetKeyboard();
	CInputXinput *xInput = CManager::GetXInput();

	// 画面遷移
	if (keyboard)
	{
		if (keyboard->GetTrigger(DIK_RETURN))
		{
			CManager::LoadMode(new CModeTitle);
			//CManager::SetMode(new CModeTitle);
		}
	}
	if (xInput)
	{
		if (xInput->GetTrigger(XI_START) || xInput->GetTrigger(XI_A))
		{
			CManager::LoadMode(new CModeTitle);
			//CManager::SetMode(new CModeTitle);
		}
	}
}

//================================================================================
// 描画処理
//================================================================================
void CModeResult::Draw()
{

}
