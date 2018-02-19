//================================================================================
//
//	Auter : KENSUKE WATANABE
//	Data  : 2017/04/27
//
//--------------------------------------------------------------------------------
//--------------------------------------------------------------------------------
// �C���N���[�h�t�@�C��
//--------------------------------------------------------------------------------
#include "main.h"
#include <algorithm>
#include "camera.h"
#include "debugStr.h"
#include "inputKeyboard.h"
#include "inputMouse.h"
#include "renderer.h"
#include "manager.h"
#include "scene.h"
#include "mainController.h"
#include "sceneField.h"
#include "car.h"

#include "xa2Listener.h"

static const float CAM_MOVE_SPEED = 1.f;

//================================================================================
// �R���X�g���N�^
//--------------------------------------------------------------------------------
CCamera::CCamera() : m_near(CAM_NEAR * CManager::GetMasterScale()), m_far(CAM_FAR * CManager::GetMasterScale())
{
	D3DXMatrixIdentity(&m_mtxView);				// �r���[���W
	m_pField = nullptr;
}

//================================================================================
// ������
//--------------------------------------------------------------------------------
void CCamera::Init()
{
	m_atObjType = AT_OBJTYPE::NONE;
	m_moveRot = D3DXVECTOR3(0.f, 0.f, 0.f);		// ��]��
	m_moveRotFlame = 0.98f;
	m_camLen = CAM_LEN * CManager::GetMasterScale();
	m_pos = m_camLen;						// ���W
	m_posOld = m_pos;
	m_pField = nullptr;
	m_mapCap = false;
	m_atAngle = D3DX_PI / 4.f;

	// �����_���W�̐ݒ�
	for (int n = 0; n < MAX_SCENE_PRIORITY; n++)
	{
		std::list<CScene*> *lisScene = CScene::GetScene(n);
		for (CScene* pScene : *lisScene)
		{
			if (pScene == nullptr)continue;
			// �^�C�v�̎擾
			CScene::OBJTYPE objType = pScene->GetObjType();

			if (objType == CScene::OBJTYPE::FIELD)
			{
				// �_�E���L���X�g
				m_pField = ((CSceneField*)pScene);
				continue;
			}
		}
	}

	// �J�����̋����̑��
	D3DXVECTOR3 vec3 = D3DXVECTOR3(m_posAt.x - m_pos.x, 0.f, m_posAt.z - m_pos.z);
	m_len = sqrt(vec3.x * vec3.x + vec3.z * vec3.z);

	m_vecUp = D3DXVECTOR3(0.f, 1.f, 0.f);			// �J�����̏����

	// ���X�i�[
	if (!m_pListener)
	{
		m_pListener = new XA2Listener();
		m_pListener->SetCone(D3DX_PI * 0.75f, 1.1f, 0.8f);	// �R�[��
		m_pListener->SetPos(m_pos);							// ���W
		m_pListener->SetFront(m_posAt - m_pos);				// �O����
		m_pListener->SetTop(m_vecUp);						// �����
		m_pListener->SetVelocity(m_pos - m_posOld);			// �ړ���
	}
}

//================================================================================
// �I��
//--------------------------------------------------------------------------------
void CCamera::Uninit()
{
	// ���X�i�[
	if (m_pListener)
	{
		delete m_pListener;
		m_pListener = nullptr;
	}
}

//================================================================================
// ���W�̕ۑ�
//--------------------------------------------------------------------------------
void CCamera::UpdateBigin()
{
	m_posOld = m_pos;
}

//================================================================================
// �X�V
//--------------------------------------------------------------------------------
void CCamera::Update()
{
	// �����_���W�̐ݒ�
	D3DXVECTOR3 rotObj = D3DXVECTOR3(0.f, 0.f, 0.f);
	D3DXVECTOR3 lookAt = D3DXVECTOR3(0.f, 0.f, 0.f);

	if (m_atObjType == AT_OBJTYPE::CONTROL_PLAYER)
	{
		std::string name = CMainController::GetName();
		if (name.empty())return;
		CCar *pCar = CCar::GetCar(name);
		rotObj = pCar->GetRot();
		lookAt = pCar->GetCamPosAt();
		lookAt.y += m_camLen.y;
		m_posAt = lookAt;

		//======================================
		// �J�����͏�Ƀv���C���[�̓��̌��
		//--------------------------------------
		m_rot.y = rotObj.y * -1.f + D3DX_PI * 0.5f + m_moveRot.y;
		m_pos = m_posAt - D3DXVECTOR3(cosf(m_rot.y) * m_camLen.z, 0.f, sinf(m_rot.y) * m_camLen.z);
		m_moveRot.y += (0.f - m_moveRot.y) * m_moveRotFlame;
	}

	// �J�����̋����̑��
	D3DXVECTOR3 vec3 = D3DXVECTOR3(m_posAt.x - m_pos.x, 0.f, m_posAt.z - m_pos.z);
	m_len = sqrt(vec3.x * vec3.x + vec3.z * vec3.z);

	// ���X�i�[�X�V
	//--------------------------------------------------------------------------------
	m_pListener->SetPos(m_pos);							// ���W

	D3DXVECTOR3 vecRight, vecFront = m_posAt - m_pos;	// �O����
	m_pListener->SetFront(vecFront);					// 

	D3DXVec3Cross(&vecRight, &m_vecUp, &vecFront);		// �E����
	D3DXVec3Cross(&m_vecUp, &vecFront, &vecRight);		// �����
	D3DXVec3Normalize(&m_vecUp, &m_vecUp);				// 
	m_pListener->SetTop(m_vecUp);						// 

	D3DXVECTOR3 move = m_pos - m_posOld;				// �ړ���
	m_pListener->SetVelocity(move);						//
}

//================================================================================
// ��]
//--------------------------------------------------------------------------------
void CCamera::AddCamRoate(const float &moveRot)
{
	// Y
	m_moveRot.y += moveRot;
	m_pos.x = m_len * cosf(m_moveRot.y) + m_posAt.x;
	m_pos.z = m_len * sinf(m_moveRot.y) + m_posAt.z;
}

//================================================================================
// �����_�ݒ�
//--------------------------------------------------------------------------------
void CCamera::SetLockAt(D3DXVECTOR3 lookAt)
{
	lookAt.y += m_camLen.y;
	m_posAt = lookAt;
}

//================================================================================
// �J�����ݒ�
//--------------------------------------------------------------------------------
void CCamera::Set()
{
	// �f�o�C�X�̎擾
	LPDIRECT3DDEVICE9 pDevice = CRenderer::GetDevice();

	D3DXMatrixLookAtLH(&m_mtxView, &m_pos, &m_posAt, &m_vecUp);	// �r���[�ϊ��s��

	// �f�o�C�X�Ƀr���[�ϊ��s���ݒ�
	pDevice->SetTransform(D3DTS_VIEW, &m_mtxView);

	D3DXVECTOR2 screenSize = CManager::GetRenderer()->GetScreenSize();

	// �v���W�F�N�V�����s��̍쐬
	if (1)
	{
		// �p�[�X�y�N�e�B�u�s��̍쐬
		D3DXMatrixPerspectiveFovLH(&m_mtxProj,		// �v���W�F�N�V�����s��̃A�h���X
			m_atAngle,								// �����p
			(float)screenSize.x / screenSize.y,		// �A�X�y�N�g�� (�X�N���[���̃T�C�Y�Ŋ���)
			m_near,								// near
			m_far);								// far
	}
	else
	{
		// �r���[�ϊ��s��
		CMainController *pPlayer = CManager::GetMainController();
		D3DXVECTOR3 playerPos = pPlayer->GetPos();
		D3DXVECTOR3 pos = playerPos + D3DXVECTOR3(1.f, 64.f, 24.f);
		D3DXVECTOR3 rot = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		D3DXVECTOR3 lightVecAt = playerPos - pos;
		D3DXVec3Normalize(&lightVecAt, &lightVecAt);
		D3DXVECTOR3 vecRight, vecUp = D3DXVECTOR3(0.f, 1.f, 0.f);
		D3DXVec3Cross(&vecRight, &lightVecAt, &vecUp);
		D3DXVec3Cross(&vecUp, &vecRight, &lightVecAt);
		D3DXVec3Normalize(&vecUp, &vecUp);
		D3DXMatrixLookAtLH(&m_mtxView, &pos, &playerPos, &vecUp);

		// �f�o�C�X�Ƀr���[�ϊ��s���ݒ�
		pDevice->SetTransform(D3DTS_VIEW, &m_mtxView);

		// �I���\�[�s��̍쐬
		D3DXMatrixOrthoLH(&m_mtxProj, 32.f, 32.f, 0.1f, 200.f);
	}

	// �f�o�C�X�Ƀv���W�F�N�V�����ϊ��s���ݒ�
	pDevice->SetTransform(D3DTS_PROJECTION, &m_mtxProj);
}

//================================================================================
// �X�N���[�����W�����[���h���W�ɕϊ�
//--------------------------------------------------------------------------------
D3DXVECTOR3* CCamera::CalcScreenToWorld(
	D3DXVECTOR3* pout,	// �o��
	int Sx,				// �X�N���[��X���W
	int Sy,				// �X�N���[��Y���W
	float fZ,			// �ˉe��Ԃł�Z�l�i0�`1�j
	int Screen_w,		// �X�N���[����
	int Screen_h,		// �X�N���[������
	const D3DXMATRIX &View,	// �r���[�s��
	const D3DXMATRIX &Prj)	// �v���W�F�N�V�����s��
{
	// �e�s��̋t�s����Z�o
	D3DXMATRIX InvView, InvPrj, VP, InvViewport;
	D3DXMatrixInverse(&InvView, nullptr, &View);
	D3DXMatrixInverse(&InvPrj, nullptr, &Prj);
	D3DXMatrixIdentity(&VP);
	VP._11 = Screen_w / 2.f;
	VP._22 = -Screen_h / 2.f;
	VP._41 = Screen_w / 2.f;
	VP._42 = Screen_h / 2.f;
	D3DXMatrixInverse(&InvViewport, nullptr, &VP);

	// �t�ϊ�
	D3DXMATRIX tmp = InvViewport * InvPrj * InvView;
	D3DXVECTOR3 VecS = D3DXVECTOR3(static_cast<float>(Sx), static_cast<float>(Sy), static_cast<float>(fZ));
	D3DXVec3TransformCoord(pout, &VecS, &tmp);

	return pout;
}