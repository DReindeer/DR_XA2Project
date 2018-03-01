//================================================================================
//
//	Auter : KENSUKE WATANABE
//	Data  : 2017/12/17
//
//--------------------------------------------------------------------------------
// Game
#include "courseIroha.h"
#include "mainController.h"
#include "checkTime.h"
#include "miniMap.h"
// Scene
#include "scene2D.h"
#include "sceneModel.h"
#include "sceneCheckPoint.h"
#include "sceneFieldX.h"
// Mode
#include "modeGame.h"
// Utility
#include "math.h"
// XAudio2
#include "xa2SourceVoiceManager.h"
#include "xa2SourceVoice2D.h"

using namespace WMath;

// �C���n
static const char* MODELNAME_IROHA = "data/MODEL/FIELD/iroha/road.x";				// ��
static const char* MODELNAME_IROHA_TUTI = "data/MODEL/FIELD/iroha/tuti.x";			// �R
static const char* MODELNAME_IROHA_P0 = "data/MODEL/FIELD/iroha/p0.x";				// P0
static const char* MODELNAME_IROHA_P1 = "data/MODEL/FIELD/iroha/p1.x";				// P1
static const char* MODELNAME_IROHA_P2 = "data/MODEL/FIELD/iroha/p2.x";				// P2
static const char* MODELNAME_IROHA_P3 = "data/MODEL/FIELD/iroha/p3.x";				// P3
static const char* MODELNAME_IROHA_P4 = "data/MODEL/FIELD/iroha/p4.x";				// P4

static const char* TEXNAME_MINIMAP_IROHA = "data/TEXTURE/GameUI/minimap_iroha.png";	// UI

static const char* TEXNAME_SIGN = "data/TEXTURE/GameUI/SGsign.png";					// �X�^�[�g��/�S�[�����̃T�C��

static const int MAX_START_TIME = 360;

//================================================================================
// ������
//--------------------------------------------------------------------------------
void CourseIroha::Init()
{
	CCourse::Init();
}

//================================================================================
// �I��
//--------------------------------------------------------------------------------
void CourseIroha::Uninit()
{
	CCourse::Uninit();
}

//================================================================================
// �X�V
//--------------------------------------------------------------------------------
void CourseIroha::Update()
{
	CCourse::Update();
}

//================================================================================
// �`��
//--------------------------------------------------------------------------------
void CourseIroha::Draw()
{

}

//================================================================================
// ����
//--------------------------------------------------------------------------------
CourseIroha *CourseIroha::Create(int pry)
{
	CourseIroha *pCourse = new CourseIroha();
	pCourse->Init();

	// �R�[�X
	pCourse->m_pCourse = CSceneFieldX::Create(D3DXVECTOR3(0.f, 0.f, 0.f), D3DXVECTOR3(0.f, 0.f, 0.f), D3DXVECTOR3(1.f, 1.f, 1.f), MODELNAME_IROHA, pry);

	pCourse->m_mapWorldSize = D3DXVECTOR2(pCourse->m_pCourse->GetSize().x, pCourse->m_pCourse->GetSize().z);

	// �R�[�X�ɂ������Ă郂�f��
	CSceneModel *pFieldTuti = nullptr;
	pFieldTuti = pFieldTuti->Create(D3DXVECTOR3(0.f, 0.f, 0.f), D3DXVECTOR3(0.f, 0.f, 0.f), D3DXVECTOR3(1.f, 1.f, 1.f), MODELNAME_IROHA_TUTI, pry);

	// �X�^�[�g
	pCourse->m_checkPoint[0] = SceneCheckPoint::Create(D3DXVECTOR3(0.f, 0.f, 0.f), D3DXVECTOR3(0.f, 0.f, 0.f), D3DXVECTOR3(1.f, 1.f, 1.f), MODELNAME_IROHA_P0, pry);
	pCourse->m_checkPoint[0]->SetDrawFlag(false);
	pCourse->m_startPos = pCourse->m_checkPoint[0]->GetCenterPos();
	pCourse->m_startPos.y -= pCourse->m_checkPoint[0]->GetSize().y * (0.4f * CManager::GetMasterScale());

	// �`�F�b�N�|�C���g
	pCourse->m_checkPoint[1] = SceneCheckPoint::Create(D3DXVECTOR3(0.f, 0.f, 0.f), D3DXVECTOR3(0.f, 0.f, 0.f), D3DXVECTOR3(1.f, 1.f, 1.f), MODELNAME_IROHA_P1, pry);
	pCourse->m_checkPoint[1]->SetDrawFlag(false);
	pCourse->m_checkPoint[2] = SceneCheckPoint::Create(D3DXVECTOR3(0.f, 0.f, 0.f), D3DXVECTOR3(0.f, 0.f, 0.f), D3DXVECTOR3(1.f, 1.f, 1.f), MODELNAME_IROHA_P2, pry);
	pCourse->m_checkPoint[2]->SetDrawFlag(false);
	pCourse->m_checkPoint[3] = SceneCheckPoint::Create(D3DXVECTOR3(0.f, 0.f, 0.f), D3DXVECTOR3(0.f, 0.f, 0.f), D3DXVECTOR3(1.f, 1.f, 1.f), MODELNAME_IROHA_P3, pry);
	pCourse->m_checkPoint[3]->SetDrawFlag(false);

	// �S�[��
	pCourse->m_checkPoint[4] = SceneCheckPoint::Create(D3DXVECTOR3(0.f, 0.f, 0.f), D3DXVECTOR3(0.f, 0.f, 0.f), D3DXVECTOR3(1.f, 1.f, 1.f), MODELNAME_IROHA_P4, pry);
	pCourse->m_checkPoint[4]->SetDrawFlag(false);

	// �J�E���g�_�E���\�L
	//	D3DXVECTOR2 resize = D3DXVECTOR2(1920.f, 1080.f);
	//	D3DXVECTOR3 signPos = D3DXVECTOR3(resize.x * 0.5f, resize.y * 0.4f, 0.f);
	//	D3DXVECTOR3 countSize = D3DXVECTOR3(resize.x * 0.2f, resize.y * 0.2f, 0.f);
	D3DXVECTOR3 signPos = D3DXVECTOR3(CUtility::RatioToScrX(0.5f), CUtility::RatioToScrY(0.4f), 0.f);
	D3DXVECTOR3 countSize = D3DXVECTOR3(CUtility::RatioToScrX(0.2f), CUtility::RatioToScrY(0.2f), 0.f);

	pCourse->m_countDown = CScene2D::Create(signPos, D3DXVECTOR3(0.f, 0.f, 0.f), countSize, TEXNAME_SIGN, 9);
	pCourse->m_countDown->SetDrawFlag(false);
	pCourse->m_countDown->SetCol(255, 32, 32, 255);
	pCourse->m_countDown->SetTexturePos(D3DXVECTOR2(0.f, 0.f), D3DXVECTOR2(0.333333f, 0.333333f));

	D3DXVECTOR3 signSize = D3DXVECTOR3(CUtility::RatioToScrX(0.4f), CUtility::RatioToScrY(0.2f), 0.f);
	pCourse->m_sign = CScene2D::Create(signPos, D3DXVECTOR3(0.f, 0.f, 0.f), signSize, TEXNAME_SIGN, 9);
	pCourse->m_sign->SetCol(255, 32, 32, 255);
	pCourse->m_sign->SetTexturePos(D3DXVECTOR2(0.f, 0.333333f), D3DXVECTOR2(1.0f, 0.333333f));
	pCourse->m_sign->SetDrawFlag(false);

	// �~�j�}�b�v�̐���
	D3DXVECTOR2 mapPos = CUtility::RatioToScrXY(D3DXVECTOR2(0.15f, 0.8f));
	float mapLen = CUtility::RatioToScrX(0.08f);
	//	D3DXVECTOR2 mapPos = D3DXVECTOR2(resize.x * 0.15f, resize.y * 0.8f);
	//	float mapLen = resize.x * 0.1f;
	pCourse->m_pMiniMap = CMiniMap::Create(mapPos, 0.f, mapLen, pCourse->m_mapWorldSize, TEXNAME_MINIMAP_IROHA);
	
	
	// �\�[�X�{�C�X�쐬
	XA2LoadWave *pLoadWave = CManager::GetXA2Manager()->GetLoadWaveOnAll();
	XA2SourceVoice2D *pXA2SourceVoice = new XA2SourceVoice2D;
	pXA2SourceVoice->Create("data/SE/ready000.wav", 0, pLoadWave);			// ���f�B
	pXA2SourceVoice->Create("data/SE/start000.wav", 0, pLoadWave);			// �X�^�[�g
	pXA2SourceVoice->Create("data/SE/clear.wav", 0, pLoadWave);				// �N���A
	// �T�E���h�I�u�W�F�N�g�ǉ�
	CManager::GetXA2Manager()->GetSourceVoiceManager()->SetXA2SoundObject(pXA2SourceVoice, pCourse);

	return pCourse;
}