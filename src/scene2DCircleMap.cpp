//================================================================================
//
//	Auter : KENSUKE WATANABE
//	Data  : 2018/02/10
//
//--------------------------------------------------------------------------------
#include "texManager.h"
#include "manager.h"
#include "modeGame.h"
#include "scene2DCircleMap.h"
#include "input.h"
#include "renderer.h"

//--------------------------------------------------------------------------------
// ������
//--------------------------------------------------------------------------------
void CScene2DCircleMap::Init()
{
	// �ϐ��̏�����
	m_pVtxBuffer = nullptr;
	m_pIdxBufferParAlpha[0] = nullptr;
	m_pIdxBufferParAlpha[1] = nullptr;
	SetPos(D3DXVECTOR3(0.f, 0.f, 0.f));		// ���W
	SetSize(D3DXVECTOR3(1.f, 1.f, 1.f));	// �T�C�Y
	SetRot(D3DXVECTOR3(0.f, 0.f, 0.f));		// ��]��
	SetScl(D3DXVECTOR3(1.f, 1.f, 1.f));		// �g�嗦
	m_posUV = D3DXVECTOR2(0.f, 0.f);
	m_widthUV = 1.f;
	m_heightUV = 1.f;
	m_rotUV = 0.f;
	m_len = 0;
	m_numOutVtx = 16;
	m_bInitDeviceLock = false;
}

//--------------------------------------------------------------------------------
// �I��
//--------------------------------------------------------------------------------
void CScene2DCircleMap::Uninit()
{
	// vtx�o�b�t�@�̉��
	if(m_pVtxBuffer != nullptr)
	{
		m_pVtxBuffer->Release();
		m_pVtxBuffer = nullptr;
	}
	// idx�o�b�t�@�̉��
	for (int i = 0; i < 2; i++)
	{
		if (m_pIdxBufferParAlpha[i] != nullptr)
		{
			m_pIdxBufferParAlpha[i]->Release();
			m_pIdxBufferParAlpha[i] = nullptr;
		}
	}
}

//--------------------------------------------------------------------------------
// �X�V
//--------------------------------------------------------------------------------
void CScene2DCircleMap::Update()
{
	// �X���b�h�Z�[�t(���b�N)
#ifdef THREAD_ON
	std::unique_lock<std::recursive_mutex> locker = CManager::Locker();
#endif

	// �X�V�����̏����ɍs��������(Device�̃��b�N.�A�����b�N)
	if (m_bInitDeviceLock == false)
	{
		InitDeviceLock();
		m_bInitDeviceLock = true;
	}

	// ���z�A�h���X���擾���邽�߂̕ϐ�
	VERTEX_2D* pVtx;
	// �o�b�t�@�����b�N�����z�A�h���X���擾����(0�̏ꏊ����0��(�S��)���b�N)
	m_pVtxBuffer->Lock(0, 0, (void**)&pVtx, 0);

	D3DXVECTOR3 pos = GetPos();
	D3DXVECTOR3 size = GetSize();
	D3DXVECTOR3 rot = GetRot();
	float ang = D3DX_PI * 2 / (m_numOutVtx);

	float alpha255Len = m_len * 0.8f;
	float alpha255LenUV = m_lenUV * 0.8f;

	pVtx[0].pos = D3DXVECTOR3(pos.x, pos.y, 0.f);		// ���_���W�̐ݒ�
	pVtx[0].tex = m_posUV;								// UV���W�̐ݒ�
	int r = GetR(); int g = GetB(); int b = GetB(); int a = GetA();
	pVtx[0].col = D3DCOLOR_RGBA(r, g, b, a);

	// ����
	for (int i = 0; i < m_numOutVtx + 1; i++)
	{
		// ���_���W�̐ݒ�
		pVtx[i + 1].pos = D3DXVECTOR3(cosf((ang * i) + rot.z) * alpha255Len + pos.x,
			sinf((ang * i) + rot.z) * alpha255Len + pos.y, 0.f);
		// UV���W�̐ݒ�
		pVtx[i + 1].tex = D3DXVECTOR2(cosf((ang * i) + m_rotUV) * alpha255LenUV + m_posUV.x,
			sinf((ang * i) + m_rotUV) * alpha255LenUV + m_posUV.y);
		// ���_�F�̐ݒ�
		pVtx[i + 1].col = D3DCOLOR_RGBA(r, g, b, a);
	}

	// �O��
	for (int i = 0; i < m_numOutVtx + 1; i++)
	{
		// ���_���W�̐ݒ�
		pVtx[i + 1 + m_numOutVtx + 1].pos = D3DXVECTOR3(cosf((ang * i) + rot.z) * m_len + pos.x,
			sinf((ang * i) + rot.z) * m_len + pos.y, 0.f);
		// UV���W�̐ݒ�
		pVtx[i + 1 + m_numOutVtx + 1].tex = D3DXVECTOR2(cosf((ang * i) + m_rotUV) * m_lenUV + m_posUV.x,
			sinf((ang * i) + m_rotUV) * m_lenUV + m_posUV.y);
		// ���_�F�̐ݒ�
		pVtx[i + 1 + m_numOutVtx + 1].col = D3DCOLOR_RGBA(r, g, b, static_cast<int>(a * 0.1f));
	}

	// �ݒ��o�b�t�@�̃A�����b�N
	m_pVtxBuffer->Unlock();
}

//--------------------------------------------------------------------------------
// �`��
//--------------------------------------------------------------------------------
void CScene2DCircleMap::Draw()
{
	// D3Device�̎擾
	LPDIRECT3DDEVICE9 pDevice = CRenderer::GetDevice();

	// �T���v���[�X�e�[�g�̐ݒ�
	pDevice->SetSamplerState(0, D3DSAMP_ADDRESSU, D3DTADDRESS_BORDER);	// �e�N�X�`���t�l�̌J��Ԃ��ݒ�
	pDevice->SetSamplerState(0, D3DSAMP_ADDRESSV, D3DTADDRESS_BORDER);	// �e�N�X�`���u�l�̌J��Ԃ��ݒ�
	
	// ���_�o�b�t�@���f�o�C�X�̃f�[�^ �X�g���[���Ƀo�C���h����
	pDevice->SetStreamSource(0, m_pVtxBuffer, 0, sizeof(VERTEX_2D));

	// ���_�t�H�[�}�b�g�̐ݒ�
	pDevice->SetFVF(FVF_VERTEX_2D);

	// �e�N�X�`���̐ݒ�
	pDevice->SetTexture(0, CManager::GetTexManager()->GetTexInterface(m_strFilePass));

	// �|���S���̕`��
	pDevice->DrawPrimitive(D3DPT_TRIANGLEFAN, 0, m_numOutVtx);

	// �C���f�b�N�X�ԍ����f�o�C�X�ɐݒ�
	pDevice->SetIndices(m_pIdxBufferParAlpha[0]);
	// �v���~�e�B�u�̐ݒ�
	pDevice->DrawIndexedPrimitive(D3DPT_TRIANGLESTRIP, 0, 0, NUM_POLYGON, 0, (m_numOutVtx + 1) * 2 - 2);

	// �T���v���[�X�e�[�g�̐ݒ�
	pDevice->SetSamplerState(0, D3DSAMP_ADDRESSU, D3DTADDRESS_WRAP);	// �e�N�X�`���t�l�̌J��Ԃ��ݒ�
	pDevice->SetSamplerState(0, D3DSAMP_ADDRESSV, D3DTADDRESS_WRAP);	// �e�N�X�`���u�l�̌J��Ԃ��ݒ�
}

//--------------------------------------------------------------------------------
// �쐬����
//--------------------------------------------------------------------------------
CScene2DCircleMap *CScene2DCircleMap::Create(const D3DXVECTOR3 &pos, const D3DXVECTOR3 &rot, float len, int numOutVtx, D3DXVECTOR2 posUV, float lenUV, const std::string &strFilePass, int priority)
{
	// CScene2DCircleMap�̐���
	CScene2DCircleMap *pScene2DCircleMap;
	pScene2DCircleMap = new CScene2DCircleMap(priority);
	pScene2DCircleMap->Init();

	pScene2DCircleMap->SetPos(pos);
	pScene2DCircleMap->SetRot(rot);
	pScene2DCircleMap->SetSize(D3DXVECTOR3(0.f, 0.f, 0.f));
	pScene2DCircleMap->m_posUV = posUV;
	pScene2DCircleMap->m_lenUV = lenUV;
	pScene2DCircleMap->m_len = len;
	pScene2DCircleMap->m_numOutVtx = max(16, numOutVtx);	// �Œ�16
	pScene2DCircleMap->m_strFilePass = strFilePass;

	// �X���b�h�Z�[�t(���b�N)
#ifdef THREAD_ON
	std::unique_lock<std::recursive_mutex> locker = CManager::Locker();
#endif

	// D3Device�̎擾
	LPDIRECT3DDEVICE9 pDevice = CManager::GetRenderer()->GetDevice();

	// ���_���̍쐬
	//---------------------
	if (FAILED(pDevice->CreateVertexBuffer(
		sizeof(VERTEX_2D) * (1 + (pScene2DCircleMap->m_numOutVtx + 1) * 2),			// �������̃T�C�Y
		D3DUSAGE_WRITEONLY,					// �g�p���@(�������݂̂�)
		FVF_VERTEX_2D,						// �g�p���钸�_�t�H�[�}�b�g(���Ȃ�)
		D3DPOOL_MANAGED,					// �o�b�t�@�̊Ǘ����@(����ɕ������Ă����)
		&pScene2DCircleMap->m_pVtxBuffer,	// 
		nullptr)))
	{
		// ���s������G���[��Ԃ�
		MessageBox(nullptr, "2D�pVtxBuffer�̍쐬�Ɏ��s���܂���", "�x��", MB_OK);
		return nullptr;
	}
	if (FAILED(pDevice->CreateIndexBuffer(
		sizeof(WORD) * (pScene2DCircleMap->m_numOutVtx + 1) * 2,		// �o�b�t�@��
		D3DUSAGE_WRITEONLY,					// �g�p�t���O
		D3DFMT_INDEX16,						// �K�v�ȃo�b�t�@��
		D3DPOOL_MANAGED,					// �������̊Ǘ����@
		&pScene2DCircleMap->m_pIdxBufferParAlpha[0],
		nullptr)))
	{
		// ���s������G���[��Ԃ�
		MessageBox(nullptr, "IdxBuffer�̍쐬�Ɏ��s���܂���", "�x��", MB_OK);
		return nullptr;
	}
	
	// �e�N�X�`���̃Z�b�g
	CManager::GetTexManager()->SetTex(strFilePass);

	return pScene2DCircleMap;
}

void CScene2DCircleMap::SetTexturePos(D3DXVECTOR2 uv, D3DXVECTOR2 length)
{
	m_posUV = uv;
	m_heightUV = length.y;
	m_widthUV = length.x;
}

//--------------------------------------------------------------------------------
// �X�V�����̍ŏ��ɍs������
//--------------------------------------------------------------------------------
void CScene2DCircleMap::InitDeviceLock()
{
	// ���z�A�h���X���擾���邽�߂̕ϐ�
	VERTEX_2D* pVtx;
	// �o�b�t�@�����b�N�����z�A�h���X���擾����(0�̏ꏊ����0��(�S��)���b�N)
	m_pVtxBuffer->Lock(0, 0, (void**)&pVtx, 0);

	D3DXVECTOR3 pos = GetPos();
	D3DXVECTOR3 size = GetSize();
	D3DXVECTOR3 rot = GetRot();
	float ang = D3DX_PI * 2 / (m_numOutVtx);

	float alpha255Len = m_len * 0.8f;
	float alpha255LenUV = m_lenUV * 0.8f;

	pVtx[0].pos = D3DXVECTOR3(pos.x, pos.y, 0.f);		// ���_���W�̐ݒ�
	pVtx[0].tex = m_posUV;								// UV���W�̐ݒ�
	pVtx[0].col = D3DCOLOR_RGBA(255, 255, 255, 255);	// ���_�F�̐ݒ�
	pVtx[0].rhw = 1.f;									// rhw�̐ݒ�
	for (int i = 0; i < m_numOutVtx + 1; i++)
	{
		// ���_���W�̐ݒ�
		pVtx[i + 1].pos = D3DXVECTOR3(cosf((ang * i) + rot.z) * alpha255Len + pos.x,
			sinf((ang * i) + rot.z) * alpha255Len + pos.y, 0.f);
		// UV���W�̐ݒ�
		pVtx[i + 1].tex = D3DXVECTOR2(cosf((ang * i) + m_rotUV) * alpha255LenUV + m_posUV.x,
			sinf((ang * i) + m_rotUV) * alpha255LenUV + m_posUV.y);
		// ���_�F�̐ݒ�
		pVtx[i + 1].col = D3DCOLOR_RGBA(255, 255, 255, 255);
		// rhw�̐ݒ�
		pVtx[i + 1].rhw = 1.f;
	}
	for (int i = 0; i < m_numOutVtx + 1; i++)
	{
		// ���_���W�̐ݒ�
		pVtx[i + 1 + m_numOutVtx + 1].pos = D3DXVECTOR3(cosf((ang * i) + rot.z) * m_len + pos.x,
			sinf((ang * i) + rot.z) * m_len + pos.y, 0.f);
		// UV���W�̐ݒ�
		pVtx[i + 1 + m_numOutVtx + 1].tex = D3DXVECTOR2(cosf((ang * i) + m_rotUV) * m_lenUV + m_posUV.x,
			sinf((ang * i) + m_rotUV) * m_lenUV + m_posUV.y);
		// ���_�F�̐ݒ�
		pVtx[i + 1 + m_numOutVtx + 1].col = D3DCOLOR_RGBA(255, 255, 255, 64);
		// rhw�̐ݒ�
		pVtx[i + 1 + m_numOutVtx + 1].rhw = 1.f;
	}

	// �ݒ��o�b�t�@�̃A�����b�N
	m_pVtxBuffer->Unlock();

	// �C���f�b�N�X
	WORD *pIdx;				// �o�b�t�@�����b�N����
	m_pIdxBufferParAlpha[0]->Lock(0, 0, (void**)&pIdx, 0);

	for (int i = 0; i < (m_numOutVtx + 1) * 2; i += 2)
	{
		pIdx[i] = static_cast<WORD>((i + 1) / 2 + 1);
		pIdx[i + 1] = static_cast<WORD>(pIdx[i] + (m_numOutVtx + 1));
	}

	m_pIdxBufferParAlpha[0]->Unlock();
}