//================================================================================
//
//	Auter : KENSUKE WATANABE
//	Data  : 2018/01/15
//
//--------------------------------------------------------------------------------
#include "main.h"
#include <thread>
#include "inputKeyboard.h"
#include "texManager.h"
#include "utility.h"
#include "scene2D.h"
#include "modeLoading.h"
#include "debugStr.h"

static const char *pTexLoadingBg = "data/TEXTURE/loading/LoadingBg.png";
static const char *pTexLoadingCar = "data/TEXTURE/loading/LoadingCar.png";
static const char *pTexLoadingTire = "data/TEXTURE/loading/LoadingTire.png";
static const char *pTexLoadingRoad = "data/TEXTURE/loading/LoadingRoad.png";
static const char *pTexRogo = "data/TEXTURE/Title/titlerogo.png";

//--------------------------------------------------------------------------------
// �R���X�g���N�^
//--------------------------------------------------------------------------------
ModeLoading::ModeLoading(CMode *nextMode) : m_nextMode(nextMode), moveRot(0.f)
{
	// �^�O�Â�
	SetModeType(MODETYPE::LOADING);

	for (int i = 0; i < LOADING_MAX_POLYGON; i++)
	{
		m_loading2D[i] = nullptr;
	}
}

//--------------------------------------------------------------------------------
// �f�X�g���N�^
//--------------------------------------------------------------------------------
ModeLoading::~ModeLoading()
{
	if (CManager::GetLoadFlag() == false)
	{
		CManager::DestroyLoadingThread(m_nextMode);
	}
}

//--------------------------------------------------------------------------------
// ������
//--------------------------------------------------------------------------------
void ModeLoading::Init()
{
	// �X�N���[���T�C�Y
	D3DXVECTOR3 screenSize(CUtility::RatioToScrX(0.5f), CUtility::RatioToScrY(0.5f), 0.f);

	// �w�i
	m_loading2D[0] = CScene2D::Create(screenSize, D3DXVECTOR3(0.f, 0.f, 0.f), screenSize, pTexLoadingBg, 7);

	// ��
	m_loading2D[1] = CScene2D::Create(screenSize, D3DXVECTOR3(0.f, 0.f, 0.f), screenSize, pTexLoadingCar, 8);

	// �^�C��
	D3DXVECTOR3 tirePos = D3DXVECTOR3(CUtility::RatioToScrX(0.635f), CUtility::RatioToScrY(0.66f), 0.f);
	D3DXVECTOR3 tireSize = D3DXVECTOR3(CUtility::RatioToScrX(0.16f), CUtility::RatioToScrX(0.16f), 0.f);
	m_loading2D[2] = CScene2D::Create(tirePos, D3DXVECTOR3(0.f, 0.f, 0.f), tireSize, pTexLoadingTire, 9);

	// �� - �e���b�v�t��
	m_loading2D[3] = CScene2D::Create(screenSize, D3DXVECTOR3(0.f, 0.f, 0.f), screenSize, pTexLoadingRoad, 9);

	// ���S
	D3DXVECTOR3 rogoPos = D3DXVECTOR3(CUtility::RatioToScrX(0.85f), CUtility::RatioToScrY(0.15f), 0.f);
	m_loading2D[4] = CScene2D::Create(rogoPos, D3DXVECTOR3(0.f, 0.f, 0.f), screenSize * 0.25f, pTexRogo, 9);

	return;
}

//--------------------------------------------------------------------------------
// �I��
//--------------------------------------------------------------------------------
void ModeLoading::Uninit()
{
	for (int i = 0; i < LOADING_MAX_POLYGON; i++)
	{
		m_loading2D[i]->Release();
	}

	// �폜�t���O���オ����Scene�S����
	CScene::RemoveAll();
}

//--------------------------------------------------------------------------------
// �X�V
//--------------------------------------------------------------------------------
void ModeLoading::Update()
{
	// �^�C����]
	moveRot = max(-1.f, moveRot - 0.001f);
	m_loading2D[2]->AddRot(D3DXVECTOR3(0.f, 0.f, moveRot));

	// ���[�h�؂�ւ�
	if (CManager::GetLoadFlag() == true)
	{
		CManager::EndLoading(m_nextMode);
	}
}

//--------------------------------------------------------------------------------
// �`��
//--------------------------------------------------------------------------------
void ModeLoading::Draw()
{

}
