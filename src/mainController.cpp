//================================================================================
//
//	Auter : KENSUKE WATANABE
//	Data  : 2018/02/10
//
//--------------------------------------------------------------------------------
#include "mainController.h"
#include "inputKeyboard.h"
#include "inputXInput.h"
#include "modeGame.h"
#include "modeTitle.h"
#include "camera.h"
#include "renderer.h"
#include "manager.h"
#include "debugStr.h"
#include "modelManager.h"
#include "texManager.h"
#include "motionManager.h"
#include "sceneFieldX.h"
#include "car.h"
#include "course.h"
// XAudio2
#include "xa2SourceVoiceManager.h"
#include "xa2SourceVoice2D.h"
#include "xa2Listener.h"

std::string CMainController::m_strName;

//================================================================================
// ����������
//--------------------------------------------------------------------------------
void CMainController::Init()
{

}

//================================================================================
// �I������
//--------------------------------------------------------------------------------
void CMainController::Uninit()
{

}

//================================================================================
// �X�V����
//--------------------------------------------------------------------------------
void CMainController::Update()
{
	CInputKeyboard *keyboard = CManager::GetKeyboard();	// �L�[�{�[�h�̎擾
	CInputXinput *xInput = CManager::GetXInput();

	// �Ԃ̎擾
	CCar *pCar = CCar::GetCar(m_strName);

	// ���[�h�擾
	CMode *pMode = CManager::GetMode();
	if (!pMode)return;
	if (pMode->GetModeType() == CMode::MODETYPE::LOADING)return;

	CModeGame *pModeGame = nullptr;
	bool noGoal = true;
	bool start = false;
	if (pMode->GetModeType() == CMode::GAME)
	{
		pModeGame = static_cast<CModeGame*>(CManager::GetMode());
		CCourse *pCourse = pModeGame->GetCourse();				// �R�[�X
		noGoal = pCourse->GetGoalCount() == 0;
		start = pCourse->GetReadyCount() == 0;

		if (pCar)
		{
			// �f�o�b�O�p�u�Ԉړ�����
			if(keyboard->GetPress(DIK_0)){
				pCar->SetPos(pCourse->GetCheckPoint(0));
			}
			else if (keyboard->GetPress(DIK_1)) {
				pCar->SetPos(pCourse->GetCheckPoint(1));
			}
			else if (keyboard->GetPress(DIK_2)) {
				pCar->SetPos(pCourse->GetCheckPoint(2));
			}
			else if (keyboard->GetPress(DIK_3)) {
				pCar->SetPos(pCourse->GetCheckPoint(3));
			}
			else if (keyboard->GetPress(DIK_4)) {
				pCar->SetPos(pCourse->GetCheckPoint(4));
			}
	
			// �Ԃ̍X�V
			pCar->Update();
			if (!start) {
				if (keyboard->GetTrigger(DIK_W) || xInput->GetRTrigger() > 42)
				{
					// SE_�A�N�Z��
					// -------- -------- -------- --------
					pCar->SoundEngin();
				}
				return;
			}
	
			// X,Z���ʏ�̕��s�ړ�
			if (keyboard->GetPress(DIK_RBRACKET) || xInput->GetPress(XI_X))	// �u���[�L
			{
				// �u���[�L
				pCar->Brake(); 
			}
			else
			{
				int rT = xInput->GetRTrigger();
				// ��
				if (keyboard->GetPress(DIK_W) && noGoal) { pCar->MoveForward(); }
				else if (rT > 0 && noGoal) { pCar->MoveForward(rT / 255.f); }
				// ��
				else if ((keyboard->GetPress(DIK_S) || xInput->GetPress(XI_Y)) && noGoal) { pCar->MoveBackward(); }
				// ����
				else { pCar->MoveFrictional(); }
			}
	
			int LX = xInput->GetStickLX();
	
			if (keyboard->GetPress(DIK_A) && noGoal) {
				pCar->TurnRight(); 
			}				// ��
			else if (keyboard->GetPress(DIK_D) && noGoal) {
				pCar->TurnLeft(); 
			}				// ��
			else if (LX > 2000 && noGoal){ 
				pCar->TurnRight(32767.f / LX); 
			}				// ��
			else if (LX < -2000 && noGoal) {
				pCar->TurnLeft(-32768.f / LX);
			}				// ��
	
			// �J���������_�ݒ�
			SetCamPosAt();
	
#if _DEBUG
			// �J����������
			CCamera *pCamera = CManager::GetCamera();
			// �X�N�V���ʒu�Ɉړ�
			if (keyboard->GetPress(DIK_P))
			{
				// �t�B�[���h�����`��
				CScene::DrawTypeOnly(CScene::CLASSTYPE::SCENE_FIELD);
				CSceneFieldX *pFieldX = (CSceneFieldX*)CScene::GetSceneClassType(CScene::CLASSTYPE::SCENE_FIELD);
				D3DXVECTOR3 size = pFieldX->GetSize();
	
				pCamera->SetCamPos(D3DXVECTOR3(size.x * 0.5f, 1000.f, size.z * -0.5f));
				pCamera->SetCamPosAt(D3DXVECTOR3(size.x * 0.5f, 0.f, size.z * -0.5f));
				pCar->SetPos(D3DXVECTOR3(size.x * 0.5f, 1000.f, size.z * -0.5f));
				pCamera->SetVecUp(D3DXVECTOR3(0.f, 0.f, -1.f));
			}
			else
			{
				CScene::DrawTypeOnly(CScene::CLASSTYPE::CLASSTYPE_NONE);
				pCamera->SetVecUp(D3DXVECTOR3(0.f, 1.f, 0.f));
			}
#endif
		}
	}
}

//================================================================================
// �`�揈��
//--------------------------------------------------------------------------------
void CMainController::Draw()
{

}

//================================================================================
// ���W
//--------------------------------------------------------------------------------
D3DXVECTOR3 CMainController::GetPos() const
{
	// �Ԃ̎擾
	CCar *pCar = CCar::GetCar(m_strName);
	if (pCar) return pCar->GetPos();
	else return D3DXVECTOR3(0.f, 0.f, 0.f);
}

//================================================================================
// 1f�O�̍��W
//--------------------------------------------------------------------------------
D3DXVECTOR3 CMainController::GetPosOld() const
{
	// �Ԃ̎擾
	CCar *pCar = CCar::GetCar(m_strName);
	if (pCar) return pCar->GetPosOld();
	else return D3DXVECTOR3(0.f, 0.f, 0.f);
}

//================================================================================
// ��]
//--------------------------------------------------------------------------------
D3DXVECTOR3 CMainController::GetRot() const
{
	// �Ԃ̎擾
	CCar *pCar = CCar::GetCar(m_strName);
	if (pCar) return pCar->GetRot();
	else return D3DXVECTOR3(0.f, 0.f, 0.f);
}

//================================================================================
// �ړ���
//--------------------------------------------------------------------------------
D3DXVECTOR3 CMainController::GetMove() const
{
	// �Ԃ̎擾
	CCar *pCar = CCar::GetCar(m_strName);
	if (pCar) return pCar->GetMove();
	else return D3DXVECTOR3(0.f, 0.f, 0.f);
}

float CMainController::GetMaxSpeed()const
{
	// �Ԃ̎擾
	CCar *pCar = CCar::GetCar(m_strName);
	if (pCar) return pCar->GetMaxSpeed();
	else return 0.f;
}

//================================================================================
// �J���������_�̐ݒ�
//--------------------------------------------------------------------------------
void CMainController::SetCamPosAt()
{
	// �Ԃ̎擾
	CCar *pCar = CCar::GetCar(m_strName);
	D3DXVECTOR3 camPosAt = pCar->GetCamPosAt();

	CCamera *pCamera = CManager::GetCamera();
	pCamera->SetLockAt(camPosAt);
	pCamera->SetMoveRotFlame(0.5f);
}