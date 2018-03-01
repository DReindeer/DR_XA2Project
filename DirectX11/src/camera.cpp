// ======== ======== ======== ======== ======== ======== ======== ========
//
//
//	title : �J����
//	Auter : KENSUKE WATANABE
//
//
// ======== ======== ======== ======== ======== ======== ======== ========
#include "main.h"
#include <algorithm>
#include <assert.h>
#include "camera.h"
#include "inputKeyboard.h"
#include "inputMouse.h"
#include "manager.h"
#include "renderer.h"
#include "scene.h"
#include "utility.h"

static const float CAM_MOVE_SPEED = 1.f;

// ======== ======== ======== ======== ======== ======== ======== ========
// �R���X�g���N�^
// -------- -------- -------- -------- -------- -------- -------- --------
CCamera::CCamera()
{
	m_pos = XMFLOAT3(0.f, 0.f, 0.f);		// ���W
	m_vecUp = XMFLOAT3(0.f, 0.f, 0.f);		// �J�����̏����
	m_posAt = XMFLOAT3(0.f, 0.f, 0.f);		// �����_
	m_moveRot = XMFLOAT3(0.f, 0.f, 0.f);	// ��]��
	m_rot = XMFLOAT3(0.f, 0.f, 0.f);
	m_len = 0.f;
	m_moveRotFlame = 0.98f;
	m_mtxView = XM::F4X4::Identity(&m_mtxView);	// �r���[���W
	m_pField = nullptr;
	m_pConstantBuffer = nullptr;
}

// ======== ======== ======== ======== ======== ======== ======== ========
// ������
// -------- -------- -------- -------- -------- -------- -------- --------
void CCamera::Init(void)
{
	m_pos = CAM_LEN;							// ���W
	m_vecUp = XMFLOAT3(0.f, 1.f, 0.f);			// �J�����̏����
	m_posAt = XMFLOAT3(0.f, 0.f, 0.f);			// �����_
	m_moveRot = XMFLOAT3(0.f, 0.f, 0.f);		// ��]��
	m_moveRotFlame = 0.98f;
	m_pField = nullptr;
	m_mapCap = false;
	m_atObjType = AT_OBJTYPE::NONE;

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
	XMFLOAT3 vec3 = XMFLOAT3(m_posAt.x - m_pos.x, 0.f, m_posAt.z - m_pos.z);
	m_len = sqrt(vec3.x * vec3.x + vec3.z * vec3.z);

	// �f�o�C�X�̎擾
	ID3D11DeviceContext* pDeviceContext = CRenderer::GetDeviceContext();
	ID3D11Device* pDevice = CManager::GetRenderer()->GetDevice();

	// �R���X�^���g�o�b�t�@�̍쐬
	D3D11_BUFFER_DESC bd;
	m_pConstantBuffer = nullptr;
	ZeroMemory(&bd, sizeof(bd));
	bd.ByteWidth = sizeof(ConstantBuffer);
	bd.Usage = D3D11_USAGE_DEFAULT;
	bd.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
	bd.CPUAccessFlags = 0;
	bd.MiscFlags = 0;
	bd.StructureByteStride = sizeof(float);
	if (FAILED(pDevice->CreateBuffer(&bd, nullptr, &m_pConstantBuffer)))
	{
		MessageBox(nullptr, "�R���X�^���g�o�b�t�@�̍쐬���s", "�x��", S_OK);
	}

	// �萔�o�b�t�@�쐬
	bd.Usage = D3D11_USAGE_DEFAULT;
	bd.ByteWidth = sizeof(CBufferModel);
	bd.CPUAccessFlags = 0;
	if (FAILED(pDevice->CreateBuffer(&bd, NULL, &m_pConstantBufferModel))) {
		MessageBox(nullptr, "�萔�o�b�t�@�쐬���s", "Err", MB_ICONSTOP);
		return;
	}
}

// ======== ======== ======== ======== ======== ======== ======== ========
// �I��
// -------- -------- -------- -------- -------- -------- -------- --------
void CCamera::Uninit(void)
{
	SAFE_RELEASE(m_pConstantBuffer);
}

// ======== ======== ======== ======== ======== ======== ======== ========
// �X�V
// -------- -------- -------- -------- -------- -------- -------- --------
void CCamera::Update(void)
{
	// �L�[�{�[�h
	CInputKeyboard *key = CManager::GetKeyboard();

	// �����_���W�̐ݒ�
	XMFLOAT3 rotObj = XMFLOAT3(0.f, 0.f, 0.f);
	XMFLOAT3 lookAt = XMFLOAT3(0.f, 0.f, 0.f);

	// �J������]
	if (key->GetPress(DIK_C))AddCamRoateY(0.02f);
	if (key->GetPress(DIK_Z))AddCamRoateY(-0.02f);
	if (key->GetPress(DIK_R))AddCamPosY(0.02f);
	if (key->GetPress(DIK_F))AddCamPosY(-0.02f);
	if (key->GetPress(DIK_E))m_len = max(1.f, m_len - 0.02f);
	if (key->GetPress(DIK_Q))m_len += (0.02f);

	// �J�����ʒu
	m_pos.x = m_len * cosf(m_rot.y) + m_posAt.x;
	m_pos.z = m_len * sinf(m_rot.y) + m_posAt.z;
}

// ======== ======== ======== ======== ======== ======== ======== ========
// ��]
// -------- -------- -------- -------- -------- -------- -------- --------
void CCamera::AddCamRoateY(const float &moveRot)
{
	// Y
	m_rot.y += moveRot;
}

// ======== ======== ======== ======== ======== ======== ======== ========
// ��]
// -------- -------- -------- -------- -------- -------- -------- --------
void CCamera::AddCamPosY(const float &move)
{
	// X
	m_pos.y += move;
}

// ======== ======== ======== ======== ======== ======== ======== ========
// �����_�ݒ�
// -------- -------- -------- -------- -------- -------- -------- --------
void CCamera::SetLockAt(XMFLOAT3 lookAt)
{
	m_posAt = lookAt;
}

//using DIRECTX;

// ======== ======== ======== ======== ======== ======== ======== ========
// �J�����ݒ�
// -------- -------- -------- -------- -------- -------- -------- --------
void CCamera::Set(void)
{
	// �f�o�C�X�̎擾
	ID3D11DeviceContext* pDeviceContext = CRenderer::GetDeviceContext();
	ID3D11Device* pDevice = CManager::GetRenderer()->GetDevice();

	// �X�N���[���T�C�Y�擾
	XMFLOAT2 screenSize = CManager::GetRenderer()->GetScreenSize();

	// �}�g���b�N�X�̏�����
	m_mtxWorld = XM::F4X4::Identity(&m_mtxWorld);		// ���[���h
	m_mtxView = XM::F4X4::Identity(&m_mtxView);			// �r���[

	// �r���[
	XMVECTOR hEye = XMLoadFloat3(&m_pos);			// �J�����̈ʒu
	XMVECTOR hAt = XMLoadFloat3(&m_posAt);			// �œ_�̈ʒu
	XMVECTOR hUp = XMLoadFloat3(&m_vecUp);			// ������x�N�g��
	XMMATRIX hWorld = XMLoadFloat4x4(&m_mtxWorld);	// ���[���h�ϊ��s��
	XMMATRIX hView = XMLoadFloat4x4(&m_mtxView);	// �r���[�ϊ��s��
	hView = XMMatrixLookAtLH(hEye, hAt, hUp);		// �r���[
	XMStoreFloat4x4(&m_mtxView, hView);				// �߂�

	//�����ˉe�ϊ��s��
	XMMATRIX hProjection = XMLoadFloat4x4(&m_mtxProj);
	hProjection = XMMatrixPerspectiveFovLH(ToRadian(60.0f), screenSize.x / screenSize.y, CAM_NEAR, CAM_FAR);
	XMStoreFloat4x4(&m_mtxProj, hProjection);		// �߂�

	//
	// �ϐ��̍X�V
	//
	ConstantBuffer hConstantBuffer;
	hConstantBuffer.mView = XMMatrixTranspose(hView);
	hConstantBuffer.mProjection = XMMatrixTranspose(hProjection);
	pDeviceContext->UpdateSubresource(m_pConstantBuffer, 0, nullptr, &hConstantBuffer, 0, 0);
	CBufferModel hBufferScene;
	hBufferScene.mWorld = XMMatrixTranspose(hWorld);
	m_lightPos = XMFLOAT4(10.0f, 40.0f, -70.0f, 1.0f);
	hBufferScene.mLight = m_lightPos;
	pDeviceContext->UpdateSubresource(m_pConstantBufferModel, 0, nullptr, &hBufferScene, 0, 0);

	//�R���e�L�X�g�ɐݒ�
	pDeviceContext->VSSetConstantBuffers(0, 1, &m_pConstantBuffer);
	pDeviceContext->VSSetConstantBuffers(1, 1, &m_pConstantBufferModel);
}
