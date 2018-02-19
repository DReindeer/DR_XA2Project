//================================================================================
//
//	Auter : KENSUKE WATANABE
//	Data  : 2018/02/10
//
//--------------------------------------------------------------------------------
#include "scene3D.h"
#include "scene3DPoly.h"
#include "manager.h"
#include "utility.h"
#include "input.h"
#include "modeGame.h"
#include "texManager.h"
#include "renderer.h"
#include "camera.h"
#include "depthShadow.h"

//--------------------------------------------------------------------------------
// ������
//--------------------------------------------------------------------------------
void CScene3DPoly::Init()
{
	// �ϐ��̏�����
	//---------------------
	SetPos(D3DXVECTOR3(0.f, 0.f, 0.f));		// ���W
	SetSize(D3DXVECTOR3(1.f, 1.f, 1.f));	// �T�C�Y
	SetRot(D3DXVECTOR3(0.f, 0.f, 0.f));		// ��]��
	SetScl(D3DXVECTOR3(1.f, 1.f, 1.f));		// �g�嗦
}

//--------------------------------------------------------------------------------
// �I��
//--------------------------------------------------------------------------------
void CScene3DPoly::Uninit()
{
	// Vtx�o�b�t�@�̉��
	if (m_pVtxBuffer != nullptr)
	{
		m_pVtxBuffer->Release();
		m_pVtxBuffer = nullptr;
	}
}

//--------------------------------------------------------------------------------
// �X�V
//--------------------------------------------------------------------------------
void CScene3DPoly::Update()
{
	// �X���b�h�Z�[�t(���b�N)
#ifdef THREAD_ON
	std::unique_lock<std::recursive_mutex> locker = CManager::Locker();
#endif

	// �o�b�t�@�����b�N�����z�A�h���X���擾
	VERTEX_3D* pVtx;
	m_pVtxBuffer->Lock(0, 0, (void**)&pVtx, 0);

	D3DXVECTOR3 size = GetSize();
	float r = 255.f / GetR();
	float g = 255.f / GetG();
	float b = 255.f / GetB();

	// ���_���W
	pVtx[0].pos = D3DXVECTOR3(-size.x * 0.5f, 0.f, size.z * 0.5f);
	pVtx[1].pos = D3DXVECTOR3(size.x * 0.5f, 0.f, size.z * 0.5f);
	pVtx[2].pos = D3DXVECTOR3(-size.x * 0.5f, 0.f, -size.z * 0.5f);
	pVtx[3].pos = D3DXVECTOR3(size.x * 0.5f, 0.f, -size.z * 0.5f);
	// �e�N�X�`�����W
	pVtx[0].tex = D3DXVECTOR2(0.f, 0.f);
	pVtx[1].tex = D3DXVECTOR2(1.f, 0.f);
	pVtx[2].tex = D3DXVECTOR2(0.f, 1.f);
	pVtx[3].tex = D3DXVECTOR2(1.f, 1.f);

	for (int i = 0; i < 4; i++)
	{
		pVtx[i].col = D3DXVECTOR4(r, g, b, 0.f);
		pVtx[i].nom = D3DXVECTOR3(0.f, 1.f, 0.f);
	}

	// �ݒ��o�b�t�@�̃A�����b�N
	m_pVtxBuffer->Unlock();
}
/*
//--------------------------------------------------------------------------------
// �`��
//--------------------------------------------------------------------------------
void CScene3DPoly::Draw()
{
	// �f�o�C�X�̎擾
	LPDIRECT3DDEVICE9 pDevice = CRenderer::GetDevice();

	// �ϐ��錾
	D3DXMATRIX mtxScl;			// �g��s��
	D3DXMATRIX mtxRot;			// ��]�s��
	D3DXMATRIX mtxPos;			// ���s�s��

	// ���[���h�}�g���N�X�̍쐬
	D3DXMatrixIdentity(&m_mtxWorld);		// �s���P�ʍs��ɂ���( �g�嗦1,��0�ŏ����� )
	D3DXMatrixIdentity(&mtxScl);			// 
	D3DXMatrixIdentity(&mtxRot);			// 
	D3DXMatrixIdentity(&mtxPos);			//

	// ���[���h�ϊ��s���ݒ肵�ĕϊ�
	D3DXVECTOR3 scl = GetScl();
	D3DXVECTOR3 rot = GetRot();
	D3DXVECTOR3 pos = GetPos();
	D3DXMatrixScaling(&mtxScl, scl.x, scl.y, scl.z);					// �g��s��
	D3DXMatrixRotationYawPitchRoll(&mtxRot, rot.y, rot.x, rot.z);		// ��]�s��
	D3DXMatrixTranslation(&mtxPos, pos.x, pos.y, pos.z);				// ���s�s��

	D3DXMatrixMultiply(&m_mtxWorld, &m_mtxWorld, &mtxScl);
	D3DXMatrixMultiply(&m_mtxWorld, &m_mtxWorld, &mtxRot);
	D3DXMatrixMultiply(&m_mtxWorld, &m_mtxWorld, &mtxPos);

	// �f�o�C�X�Ƀ��[���h�ϊ��s���ݒ�
	pDevice->SetTransform(D3DTS_WORLD, &m_mtxWorld);

	// ���_�t�H�[�}�b�g�̐ݒ�
	pDevice->SetFVF(FVF_VERTEX_3D);

	// �X�g���[���̐ݒ�
	pDevice->SetStreamSource(0, m_pVtxBuffer, 0, sizeof(VERTEX_3D));

	// �e�N�X�`���̐ݒ�
	pDevice->SetTexture(0, CManager::GetTexManager()->GetTexInterface(m_strFilePass));

	// �|���S���̕`�� // �|���S���̌`, �|���S���̐�, �擪�A�h���X, �T�C�Y
	pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, 0, NUM_POLYGON);
}
*/

//--------------------------------------------------------------------------------
// �f�t�@�[�h�����_�����O�p
//--------------------------------------------------------------------------------
void CScene3DPoly::DrawDefferdRendering()
{
	// �f�o�C�X�̎擾
	LPDIRECT3DDEVICE9 pDevice = CRenderer::GetDevice();

	// ���[���h�}�g���N�X�̍쐬
	D3DXMATRIX mtxScl;						// �g��s��
	D3DXMATRIX mtxRot;						// ��]�s��
	D3DXMATRIX mtxPos;						// ���s�s��

	D3DXMatrixIdentity(&m_mtxWorld);// 
	D3DXMatrixIdentity(&mtxScl);	// 
	D3DXMatrixIdentity(&mtxRot);	// 
	D3DXMatrixIdentity(&mtxPos);	// �s���P�ʍs��ɂ���( �g�嗦1,��0�ŏ����� )

	// ���[���h�ϊ��s���ݒ肵�ĕϊ�
	D3DXVECTOR3 scl = GetScl();
	D3DXMatrixScaling(&mtxScl, scl.x, scl.y, scl.z);						// �g��s��
	D3DXMatrixRotationYawPitchRoll(&mtxRot, m_rot.y, m_rot.x, m_rot.z);		// ��]�s��
	D3DXMatrixTranslation(&mtxPos, m_pos.x, m_pos.y, m_pos.z);				// ���s�s��

	D3DXMatrixMultiply(&m_mtxWorld, &m_mtxWorld, &mtxScl);
	D3DXMatrixMultiply(&m_mtxWorld, &m_mtxWorld, &mtxRot);
	D3DXMatrixMultiply(&m_mtxWorld, &m_mtxWorld, &mtxPos);

	// �f�o�C�X�Ƀ��[���h�ϊ��s���ݒ�
	pDevice->SetTransform(D3DTS_WORLD, &m_mtxWorld);

	// �A���t�@�e�X�g�̗L����
	pDevice->SetRenderState(D3DRS_ALPHATESTENABLE, TRUE);
	pDevice->SetRenderState(D3DRS_ALPHAFUNC, D3DCMP_GREATEREQUAL);
	// �s�����ɂ���l�̐ݒ�
	pDevice->SetRenderState(D3DRS_ALPHAREF, (DWORD)0xA0);

	// �J�n
	CManager::GetDepthShadow()->DrawBigin(m_mtxWorld);

	// ���_�t�H�[�}�b�g�̐ݒ�
	pDevice->SetFVF(FVF_VERTEX_3D);

	// �X�g���[���̐ݒ�
	pDevice->SetStreamSource(0, m_pVtxBuffer, 0, sizeof(VERTEX_3D));

	// �|���S���̕`�� // �|���S���̌`, �|���S���̐�, �擪�A�h���X, �T�C�Y
	pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, 0, NUM_POLYGON);

	// �I��
	CManager::GetDepthShadow()->DrawEnd();

	pDevice->SetRenderState(D3DRS_ALPHATESTENABLE, FALSE);
}

//--------------------------------------------------------------------------------
// �쐬����
//--------------------------------------------------------------------------------
CScene3DPoly *CScene3DPoly::Create(const D3DXVECTOR3 &pos, const D3DXVECTOR3 &rot, const D3DXVECTOR3 &size, const std::string &strFilePass, int priority)
{
	// CScene3DPoly�̐���
	CScene3DPoly *pScene3DPoly;
	pScene3DPoly = new CScene3DPoly(priority);
	pScene3DPoly->Init();

	pScene3DPoly->SetPos(pos);
	pScene3DPoly->SetRot(rot);
	pScene3DPoly->SetSize(size);
	pScene3DPoly->m_strFilePass = strFilePass;

	// �X���b�h�Z�[�t(���b�N)
#ifdef THREAD_ON
	std::unique_lock<std::recursive_mutex> locker = CManager::Locker();
#endif

	// D3Device�̎擾
	LPDIRECT3DDEVICE9 pDevice = CManager::GetRenderer()->GetDevice();

	// ���_���̍쐬
	//---------------------
	if (FAILED(pDevice->CreateVertexBuffer(
		sizeof(VERTEX_3D) * NUM_VERTEX,		// �������̃T�C�Y
		D3DUSAGE_WRITEONLY,					// �g�p���@(�������݂̂�)
		FVF_VERTEX_3D,						// �g�p���钸�_�t�H�[�}�b�g(���Ȃ�)
		D3DPOOL_MANAGED,					// �o�b�t�@�̊Ǘ����@(����ɕ������Ă����)
		&pScene3DPoly->m_pVtxBuffer,		// 
		nullptr)))
	{
		// ���s������G���[��Ԃ�
		MessageBox(nullptr, "3D�pVtxBuffer�̍쐬�Ɏ��s���܂���", "�x��", MB_OK);
		return nullptr;
	}

	// �e�N�X�`���̃Z�b�g
	CManager::GetTexManager()->SetTex(strFilePass);

	return pScene3DPoly;
}