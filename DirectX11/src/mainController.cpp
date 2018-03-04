// ======== ======== ======== ======== ======== ======== ======== ========
//
//
//	title : ���C���R���g���[��
//	Auter : KENSUKE WATANABE
//
//
// ======== ======== ======== ======== ======== ======== ======== ========
#include "mainController.h"
#include "inputKeyboard.h"
#include "inputXInput.h"
#include "camera.h"
#include "renderer.h"
#include "manager.h"
#include "texManager.h"
#include "sceneBox.h"
#include "sceneXModelListener.h"
#include "XModelLoader.h"

static const float PLAYER_MOVE_SPEED = 0.005f;

std::string CMainController::m_strName;

// ======== ======== ======== ======== ======== ======== ======== ========
// ����������
// -------- -------- -------- -------- -------- -------- -------- --------
void CMainController::Init(void)
{
	m_pPlayer = nullptr;
}

// ======== ======== ======== ======== ======== ======== ======== ========
// �I������
// -------- -------- -------- -------- -------- -------- -------- --------
void CMainController::Uninit(void)
{

}

// ======== ======== ======== ======== ======== ======== ======== ========
// �X�V����
// -------- -------- -------- -------- -------- -------- -------- --------
void CMainController::Update(void)
{
	CInputKeyboard *key = CManager::GetKeyboard();	// �L�[�{�[�h�̎擾
	CInputXinput *xInput = CManager::GetXInput();

	CCamera *cam = CManager::GetCamera();

	// �����_���W�̐ݒ�
	for (int n = 0; n < MAX_SCENE_PRIORITY; n++)
	{
		std::list<CScene*> *lisScene = CScene::GetScene(n);
		for (CScene* pScene : *lisScene)
		{
			if (pScene == nullptr)continue;
			// �^�C�v�̎擾
			CScene::OBJTYPE objType = pScene->GetObjType();

			if (objType == CScene::OBJTYPE::LISTENER)
			{
				// �_�E���L���X�g
				m_pPlayer = (CSceneXModelListener*)pScene;
				break;
			}
		}
	}

	if (m_pPlayer)
	{
		float camY = cam->GetRot().y;

		XMFLOAT3 pos = m_pPlayer->GetPos();
		XMFLOAT3 size = m_pPlayer->GetSize();
		XMFLOAT3 scl = m_pPlayer->GetScl();
		XMFLOAT3 rot = m_pPlayer->GetRot();
		XMFLOAT3 move = m_pPlayer->GetMove();

		// X,Z���ʏ�̕��s�ړ�
		// -------- -------- -------- --------
		if (key->GetPress(DIK_W) && key->GetPress(DIK_A))
		{// ��+��
			move = XM::F3::Add(move, XMFLOAT3(cosf(camY - PI * 0.75f) * PLAYER_MOVE_SPEED, 0.0f, sinf(camY - PI * 0.75f) * PLAYER_MOVE_SPEED));
			rot = XM::F3::RotY(move);
		}
		else if (key->GetPress(DIK_W) && key->GetPress(DIK_D))
		{// ��+��
			move = XM::F3::Add(move, XMFLOAT3(cosf(camY + PI * 0.75f) * PLAYER_MOVE_SPEED, 0.0f, sinf(camY + PI * 0.75f) * PLAYER_MOVE_SPEED));
			rot = XM::F3::RotY(move);
		}
		else if (key->GetPress(DIK_S) && key->GetPress(DIK_A))
		{// ��+��
			move = XM::F3::Add(move, XMFLOAT3(cosf(camY - PI * 0.25f) * PLAYER_MOVE_SPEED, 0.0f, sinf(camY - PI * 0.25f) * PLAYER_MOVE_SPEED));
			rot = XM::F3::RotY(move);
		}
		else if (key->GetPress(DIK_S) && key->GetPress(DIK_D))
		{// ��+��
			move = XM::F3::Add(move, XMFLOAT3(cosf(camY + PI * 0.25f) * PLAYER_MOVE_SPEED, 0.0f, sinf(camY + PI * 0.25f) * PLAYER_MOVE_SPEED));
			rot = XM::F3::RotY(move);
		}
		else if (key->GetPress(DIK_W))
		{// ��
			move = XM::F3::Add(move, XMFLOAT3(cosf(camY - PI) * PLAYER_MOVE_SPEED, 0.0f, sinf(camY - PI) * PLAYER_MOVE_SPEED));
			rot = XM::F3::RotY(move);
		}
		else if (key->GetPress(DIK_S))
		{// ��
			move = XM::F3::Add(move, XMFLOAT3(cosf(camY) * PLAYER_MOVE_SPEED, 0.0f, sinf(camY) * PLAYER_MOVE_SPEED));
			rot = XM::F3::RotY(move);
		}
		else if (key->GetPress(DIK_A))
		{// ��
			move = XM::F3::Add(move, XMFLOAT3(cosf(camY - PI * 0.5f) * PLAYER_MOVE_SPEED, 0.0f, sinf(camY - PI * 0.5f) * PLAYER_MOVE_SPEED));
			rot = XM::F3::RotY(move);
		}
		else if (key->GetPress(DIK_D))
		{// ��
			move = XM::F3::Add(move, XMFLOAT3(cosf(camY + PI * 0.5f) * PLAYER_MOVE_SPEED, 0.0f, sinf(camY + PI * 0.5f) * PLAYER_MOVE_SPEED));
			rot = XM::F3::RotY(move);
		}

		// �ړ��ʂ𑫂�����
		// -------- -------- -------- --------
		if (fabs(move.x) > 0.f)move.x *= 0.90f;
		if (fabs(move.z) > 0.f)move.z *= 0.90f;
		pos = XM::F3::Add(pos, move);

		// �J���������_�ݒ�
		SetCamPosAt();

		// �ϐ��̍Đݒ�
		// -------- -------- -------- --------
		m_pPlayer->SetPos(pos);
		m_pPlayer->SetSize(size);
		m_pPlayer->SetScl(scl);
		m_pPlayer->SetRot(rot);
		m_pPlayer->SetMove(move);
	}
}

// ======== ======== ======== ======== ======== ======== ======== ========
// �`�揈��
// -------- -------- -------- -------- -------- -------- -------- --------
void CMainController::Draw(void)
{

}

// ======== ======== ======== ======== ======== ======== ======== ========
// ���W
// -------- -------- -------- -------- -------- -------- -------- --------
XMFLOAT3 CMainController::GetPos(void)
{
	if (m_pPlayer) return m_pPlayer->GetPos();
	else return XMFLOAT3(0.f, 0.f, 0.f);
}

// ======== ======== ======== ======== ======== ======== ======== ========
// ��]
// -------- -------- -------- -------- -------- -------- -------- --------
XMFLOAT3 CMainController::GetRot(void)
{
	if (m_pPlayer) return m_pPlayer->GetRot();
	else return XMFLOAT3(0.f, 0.f, 0.f);
}

// ======== ======== ======== ======== ======== ======== ======== ========
// �ړ���
// -------- -------- -------- -------- -------- -------- -------- --------
XMFLOAT3 CMainController::GetMove(void)
{
	if (m_pPlayer) return m_pPlayer->GetMove();
	else return XMFLOAT3(0.f, 0.f, 0.f);
}

// ======== ======== ======== ======== ======== ======== ======== ========
// �J���������_�̐ݒ�
// -------- -------- -------- -------- -------- -------- -------- --------
void CMainController::SetCamPosAt(void)
{
	XMFLOAT3 camPosAt = XMFLOAT3(0.f, 0.f, 0.f);
	if (m_pPlayer)
	{
		float len = max(0.0001f, -1.f / XM::F3::Length(m_pPlayer->GetMove()) * 2.f);
		XMVECTOR lookAt = XMLoadFloat3(&m_pPlayer->GetPos()) + (XMLoadFloat3(&XM::F3::LookAt(m_pPlayer->GetRot().y + PI * 0.5f))) * len;
		XMStoreFloat3(&camPosAt, lookAt);
	}
	CCamera *pCamera = CManager::GetCamera();
	pCamera->SetLockAt(camPosAt);
}