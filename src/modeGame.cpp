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
// �萔��`
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

static const int MAX_EFFECT = 512;		// �G�t�F�N�g�̍ő吔

//================================================================================
// �R���X�g���N�^
//================================================================================
CModeGame::CModeGame() : m_pCourse(nullptr)
						, m_pSpeedMeter(nullptr)
						, m_pCheckTime(nullptr)
{
	SetModeType(MODETYPE::GAME);
}

//================================================================================
// �f�X�g���N�^
//================================================================================
CModeGame::~CModeGame()
{

}

//================================================================================
// ����������
//================================================================================
void CModeGame::Init()
{
	Create(this);

	// �Đ�
	XA2SourceVoiceManager *pSoundObjectManager = CManager::GetXA2Manager()->GetSourceVoiceManager();
	pSoundObjectManager->GetXA2SourceVoice(this)->Play("data/BGM/BGM_Game.wav");
}

//================================================================================
// ��������
//================================================================================
void CModeGame::Create(CModeGame *pGame)
{
	// �X�J�C�{�b�N�X
//	CSceneSkybox *pSceneSkybox = nullptr;
//	pSceneSkybox = pSceneSkybox->Create(D3DXVECTOR3(0.f, 0.f, 0.f), D3DXVECTOR3(0.f, 0.f, 0.f), 1300.f, TEXNAME_SKYBOX, 0);
//	pSceneSkybox->SetForward(false);
	CSceneBoxX::Create(D3DXVECTOR3(0.f, 0.f, 0.f), D3DXVECTOR3(0.f, 0.f, 0.f), D3DXVECTOR3(13.f, 13.f, 13.f), "data/MODEL/SkyBox/skyBox3.x", 0);

//	D3DXVECTOR2 resize = D3DXVECTOR2(1920.f, 1080.f);
	
	// �X�e�[�W�̐���
	pGame->m_pCourse = CourseAkina::Create(1);

	// �v���C���[, �Ԃ̍쐬
	CManager::GetMainController()->SetName(MODELNAME_CAR_BODY);
	CCar::Create(pGame->m_pCourse->GetStartPos(), D3DXVECTOR3(0.f, 0.f, 0.f), MODELNAME_CAR_BODY, 4);

	// ���[�^�[�̐���
	D3DXVECTOR2 meterPos = CUtility::RatioToScrXY(D3DXVECTOR2(0.85f, 0.8f));
//	D3DXVECTOR2 meterPos = D3DXVECTOR2(resize.x * 0.85f, resize.y * 0.8f);

	pGame->m_pSpeedMeter = pGame->m_pSpeedMeter->Create(meterPos, CUtility::RatioToScrX(0.08f));

	// �^�C���̐���
	D3DXVECTOR2 TimePos = CUtility::RatioToScrXY(D3DXVECTOR2(0.9f, 0.2f));
//	D3DXVECTOR2 TimePos = D3DXVECTOR2(resize.x * 0.9f, resize.y * 0.2f);
	pGame->m_pCheckTime = pGame->m_pCheckTime->Create(TimePos, CUtility::RatioToScrXY(D3DXVECTOR2(0.08f, 0.08f)));

	// �\�[�X�{�C�X�쐬
	XA2SourceVoice2D *pXA2SourceVoice = new XA2SourceVoice2D;
	pXA2SourceVoice->Create("data/BGM/BGM_Game.wav", -1, CManager::GetXA2Manager()->GetLoadWaveStreaming());

	// �T�E���h�I�u�W�F�N�g�ǉ�
	XA2SourceVoiceManager *pSoundObjectManager = CManager::GetXA2Manager()->GetSourceVoiceManager();
	pSoundObjectManager->SetXA2SoundObject(pXA2SourceVoice, pGame);

	// �J����
	CCamera *pCam = CManager::GetCamera();
	pCam->Init();
	pCam->SetatObjType(pCam->AT_OBJTYPE::CONTROL_PLAYER);

	return;
}

//================================================================================
// �I������
//================================================================================
void CModeGame::Uninit()
{
	// �Ԃ̉��
	CCar::ReleaseAll();

	// �S�ẴG�t�F�N�g�\��������
	CEffect3DPoly::FlashAll();

	// �V�[���S���
	CScene::FlagReleaseAll();

	// �e�N�X�`�����\�[�X�S���
	CManager::GetTexManager()->ReleaseAll();

	// ���f�����\�[�X�S���
	CManager::GetModelManager()->ReleaseAll();

	// �o�^�ς݃\�[�X�{�C�X�S���
	XA2SourceVoiceManager *pSoundObjectManager = CManager::GetXA2Manager()->GetSourceVoiceManager();
	pSoundObjectManager->StopAndUninit();

	// �T�E���h���\�[�X�S���
	CManager::GetXA2Manager()->GetSoundResourceManager()->Uninit();

	// �J���������_�^�C�v�̕ύX
	CCamera *pCam = CManager::GetCamera();
	pCam->Init();
	pCam->SetatObjType(pCam->AT_OBJTYPE::NONE);
}

//================================================================================
// �X�V����
//================================================================================
void CModeGame::Update()
{
	CInputKeyboard *keyboard = CManager::GetKeyboard();

	// ���[�^�[�X�V
	m_pSpeedMeter->Update();

	// ���Ԍv��
	m_pCheckTime->Update();

	// �`�F�b�N�|�C���g�X�V
	m_pCourse->Update();

	// ��ʑJ��
	if ((keyboard && keyboard->GetTrigger(DIK_RETURN)) || GetCourse()->GetGoalCount() > 180)
	{
		CManager::LoadMode(new CModeResult);
	}
}

//================================================================================
// �`�揈��
//================================================================================
void CModeGame::Draw()
{

}