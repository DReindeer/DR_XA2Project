//================================================================================
//
//	Auter : KENSUKE WATANABE
//	Data  : 2017/12/17
//
//--------------------------------------------------------------------------------
// Game
#include "course.h"
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

static const int MAX_START_TIME = 360;

//================================================================================
// ������
//--------------------------------------------------------------------------------
void CCourse::Init()
{
	m_pCourse = nullptr;
	for (int i = 0; i < 5; i++)
	{
		m_checkPoint[i] = nullptr;
	}
	m_startPos = D3DXVECTOR3(0.f, 0.f, 0.f);
	m_mapWorldSize = D3DXVECTOR2(0.f, 0.f);

	m_goalCount = 0;				// �S�[����ɃJ�E���g�����l
	m_startCount = MAX_START_TIME;	// �X�^�[�g�O�ɃJ�E���g�����l
	m_readyCount = MAX_START_TIME;
	m_signDrawCount = 0;
	m_signDrawFlag = false;
}

//================================================================================
// �I��
//--------------------------------------------------------------------------------
void CCourse::Uninit()
{
	// �~�j�}�b�v���
	if (m_pMiniMap != nullptr)
	{
		m_pMiniMap->Uninit();
		delete m_pMiniMap;
		m_pMiniMap = nullptr;
	}
}

//================================================================================
// �X�V
//--------------------------------------------------------------------------------
void CCourse::Update()
{
	// Xaudio2
	XA2SourceVoiceManager *pSoundObjectManager = CManager::GetXA2Manager()->GetSourceVoiceManager();

	// �~�j�}�b�v�X�V
	m_pMiniMap->Update();

	// �`�F�b�N�|�C���g�ƃv���C���[�̓����蔻�� - 2�������W��̐����Ɛ����̓����蔻��
	if (m_currentCheckPoint < 4)
	{
		SEGMENT_SEGMENT_RESULT ssrRes;
		D3DXVECTOR3 playerPos = CManager::GetMainController()->GetPos();
		D3DXVECTOR3 playerPosOld = CManager::GetMainController()->GetPosOld();
		D3DXVECTOR3 checkPos[2];
		checkPos[0] = m_checkPoint[m_currentCheckPoint + 1]->GetPoint(0);
		checkPos[1] = m_checkPoint[m_currentCheckPoint + 1]->GetPoint(1);
		Math::SegmentVSSegment(playerPosOld, playerPos, checkPos[0], checkPos[1], &ssrRes);
		if (ssrRes.SegA_SegB_MinDist_Square < 0.5f * CManager::GetMasterScale())
		{
			m_currentCheckPoint++;
			if (m_currentCheckPoint >= 4)
			{
				m_goal = true;
				return;
			}
			CModeGame *pModeGame = static_cast<CModeGame*>(CManager::GetMode());
			if (!pModeGame)return;

			// SE_�S�[��
			// -------- -------- -------- --------
			pSoundObjectManager->GetXA2SourceVoice(this)->Play("data/SE/clear.wav");
		}
	}

	// �`�F�b�N�|�C���g�̎擾
	if (m_goal == true)
	{
		m_goalCount++;
	}

	// �ҋ@����
	m_startCount = max(--m_startCount, 0);
	if (m_startCount <= 0)
	{
		// �J�E���g�_�E��
		m_readyCount = max(--m_readyCount, 0);

		// �J�E���g�_�E���\��
		if (m_readyCount > 240) {
			m_countDown->SetDrawFlag(true);
			m_countDown->SetTexturePos(D3DXVECTOR2(0.f, 0.f), D3DXVECTOR2(0.333333f, 0.333333f));
			if (m_readyCount == 360 - 3)
			{
				// SE_���f�B�[
				// -------- -------- -------- --------
				pSoundObjectManager->GetXA2SourceVoice(this)->Play("data/SE/ready000.wav");
			}
		}
		else if (m_readyCount > 120)
		{
			m_countDown->SetDrawFlag(true);
			m_countDown->SetTexturePos(D3DXVECTOR2(0.333333f, 0.f), D3DXVECTOR2(0.333333f, 0.333333f));
			if (m_readyCount == 240 - 3)
			{
				// SE_���f�B�[
				// -------- -------- -------- --------
				pSoundObjectManager->GetXA2SourceVoice(this)->Play("data/SE/ready000.wav");
			}
		}
		else if (m_readyCount > 0)
		{
			m_countDown->SetDrawFlag(true);
			m_countDown->SetTexturePos(D3DXVECTOR2(0.666666f, 0.f), D3DXVECTOR2(0.333333f, 0.333333f));
			if (m_readyCount == 120 - 3)
			{
				// SE_���f�B�[
				// -------- -------- -------- --------
				pSoundObjectManager->GetXA2SourceVoice(this)->Play("data/SE/ready000.wav");
			}
		}
		else if (m_readyCount == 0 && m_signDrawFlag == false)
		{
			m_countDown->SetDrawFlag(false);	// �J�E���g�_�E����\��
			m_sign->SetDrawFlag(true);			// START�\��
			m_signDrawCount = 120;			// 
			m_signDrawFlag = true;
			// SE_�X�^�[�g
			// -------- -------- -------- --------
			pSoundObjectManager->GetXA2SourceVoice(this)->Play("data/SE/start000.wav");
		}
	}
	// �J�E���g�_�E��
	m_signDrawCount = max(--m_signDrawCount, 0);
	if (m_signDrawCount <= 0)m_sign->SetDrawFlag(false);			// START��\��

	if (m_goalCount <= 180 && m_goalCount > 0)
	{
		if (m_goalCount == 1)
		{
			// SE_�S�[��
			// -------- -------- -------- --------
			pSoundObjectManager->GetXA2SourceVoice(this)->Play("data/SE/clear.wav");
		}
		m_sign->SetDrawFlag(true);				// START��\��
		m_sign->SetCol(32, 32, 255, 255);
		m_sign->SetTexturePos(D3DXVECTOR2(0.f, 0.6666666f), D3DXVECTOR2(1.0f, 0.333333f));
	}
}

//================================================================================
// �`��
//--------------------------------------------------------------------------------
void CCourse::Draw()
{
	// �~�j�}�b�v�`��
	m_pMiniMap->Draw();
}

//================================================================================
// �`�F�b�N�|�C���g�̒��S���W�̎擾
//--------------------------------------------------------------------------------
const D3DXVECTOR3 &CCourse::GetCheckPoint(const int &index)
{
	if (index < 0 || index > 4)assert(!"�������s��");
	return m_checkPoint[index]->GetCenterPos();
}