//================================================================================
//
//	Auter : KENSUKE WATANABE
//	Data  : 2017/04/27
//
//--------------------------------------------------------------------------------
//--------------------------------------------------------------------------------
// �C���N���[�h�t�@�C��
//--------------------------------------------------------------------------------
#include <math.h>
#include "main.h"
#include "manager.h"
#include "utility.h"
#include "sceneCylinder.h"
#include "effect3DPoly.h"
#include "bgCylinder.h"
#include "input.h"
#include "modeGame.h"
#include "texManager.h"
#include "renderer.h"
#include <random>

//================================================================================
// ������
//--------------------------------------------------------------------------------
void CBgCylinder::Init()
{
	// ������
	SetPos(D3DXVECTOR3(0.f, 0.f, 0.f));
	SetRot(D3DXVECTOR3(0.f, 0.f, 0.f));
	SetScl(D3DXVECTOR3(1.f, 1.f, 1.f));
	SetSize(D3DXVECTOR3(1.f, 1.f, 1.f));
	SetCol(0, 0, 0, 255);
	m_moveUV = D3DXVECTOR2(0.f, 0.f);
	m_strFilePass.clear();
}

//--------------------------------------------------------------------------------
// �I������
//--------------------------------------------------------------------------------
void CBgCylinder::Uninit()
{
	// �e�N���X�̏I��
	CSceneCylinder::Uninit();
}

//================================================================================
// �X�V����
//--------------------------------------------------------------------------------
void CBgCylinder::Update()
{
	// �X���b�h�Z�[�t(���b�N)
#ifdef THREAD_ON
	std::unique_lock<std::recursive_mutex> locker = CManager::Locker();
#endif

	int stacks = GetStacks();
	int slices = GetSlices();
	float r = 255.f / GetR();
	float g = 255.f / GetG();
	float b = 255.f / GetB();

	VERTEX_3D* pVtx;	// �o�b�t�@�����b�N����
	m_pVtxBuffer->Lock(0, 0, (void**)&pVtx, 0);
	for (int cntSlices = 0; cntSlices <= slices; cntSlices++)
	{
		for (int cntStacks = 0; cntStacks <= stacks; cntStacks++)
		{
			// �F
			pVtx[0].col = D3DXVECTOR4(r, g, b, 0.f);
			pVtx++;
		}
	}
	// ���b�N�̉���
	m_pVtxBuffer->Unlock();
}

//================================================================================
// �|���S���`��
//--------------------------------------------------------------------------------
void CBgCylinder::Draw()
{
	// �f�o�C�X�̎擾
	LPDIRECT3DDEVICE9 pDevice = CRenderer::GetDevice();

	pDevice->SetRenderState(D3DRS_LIGHTING, FALSE);						// ���C�g�\���I�t
	pDevice->SetSamplerState(0, D3DSAMP_ADDRESSU, D3DTADDRESS_CLAMP);	// �e�N�X�`���t�l�̌J��Ԃ��ݒ�
	pDevice->SetSamplerState(0, D3DSAMP_ADDRESSV, D3DTADDRESS_CLAMP);	// �e�N�X�`���u�l�̌J��Ԃ��ݒ�

																		// �e�̕`��
	CSceneCylinder::Draw();

	// �����_�[�X�e�[�g�̐ݒ��߂�
	pDevice->SetSamplerState(0, D3DSAMP_ADDRESSU, D3DTADDRESS_WRAP);	// �e�N�X�`���t�l�̌J��Ԃ��ݒ�
	pDevice->SetSamplerState(0, D3DSAMP_ADDRESSV, D3DTADDRESS_WRAP);	// �e�N�X�`���u�l�̌J��Ԃ��ݒ�
	pDevice->SetRenderState(D3DRS_LIGHTING, TRUE);						// ���C�g�̕\����L���ɂ���
}

//--------------------------------------------------------------------------------
// ���_�쐬
//--------------------------------------------------------------------------------
HRESULT CBgCylinder::SetVtx(int stacks, int slices)
{
	float phi = (D3DX_PI * 2) / stacks;			// �Ƃ̊p�x

	int numVtx = (slices + 1) * (stacks + 1);							// ���_��
	int numIndex = ((stacks + 1) * 2 * slices) + (slices - 1) * 2;		// �C���f�b�N�X��(�O�p�`�� + �ѕ�(�k�ރ|���S��))
	SetStacks(stacks);
	SetSlices(slices);
	SetNumVtx(numVtx);
	SetNumIndex(numIndex);

	// �X���b�h�Z�[�t(���b�N)
#ifdef THREAD_ON
	std::unique_lock<std::recursive_mutex> locker = CManager::Locker();
#endif

	// D3Device�̎擾
	LPDIRECT3DDEVICE9 pDevice = CManager::GetRenderer()->GetDevice();

	// ���_���̍쐬
	//---------------------
	if (FAILED(pDevice->CreateVertexBuffer(
		sizeof(VERTEX_3D) * numVtx,			// �������̃T�C�Y
		D3DUSAGE_WRITEONLY,					// �g�p���@(�������݂̂�)
		FVF_VERTEX_3D,						// �g�p���钸�_�t�H�[�}�b�g(���Ȃ�)
		D3DPOOL_MANAGED,					// �o�b�t�@�̊Ǘ����@(����ɕ������Ă����)
		&m_pVtxBuffer,							// 
		nullptr)))
	{
		// ���s������G���[��Ԃ�
		MessageBox(nullptr, "3D�pVtxBuffer�̍쐬�Ɏ��s���܂���", "�x��", MB_OK);
		return E_FAIL;
	}

	if (FAILED(pDevice->CreateIndexBuffer(
		sizeof(WORD) * numIndex,		// �o�b�t�@��
		D3DUSAGE_WRITEONLY,			// �g�p�t���O
		D3DFMT_INDEX16,				// �K�v�ȃo�b�t�@��
		D3DPOOL_MANAGED,			// �������̊Ǘ����@
		&m_pIdxBuffer,
		nullptr)))
	{
		// ���s������G���[��Ԃ�
		MessageBox(nullptr, "IdxBuffer�̍쐬�Ɏ��s���܂���", "�x��", MB_OK);
		return E_FAIL;
	}

	VERTEX_3D* pVtx;	// �o�b�t�@�����b�N����
	m_pVtxBuffer->Lock(0, 0, (void**)&pVtx, 0);

	// 1�u���b�N�̕��ƍ���
	D3DXVECTOR3 size = GetSize();

	// ���_�̐ݒ�
	float x, y, z;

	for (int cntSlices = 0; cntSlices <= slices; cntSlices++)
	{
		for (int cntStacks = 0; cntStacks <= stacks; cntStacks++)
		{
			// ���W
			x = cos(phi * cntStacks) * size.x,			// X
			y = cntSlices * size.y,						// Y
			z = sin(phi * cntStacks) * size.z;			// Z
			pVtx[0].pos = D3DXVECTOR3(x, y, z);
			// �@��
			pVtx[0].nom = D3DXVECTOR3(0.f, 1.f, 0.f);
			// �F
			pVtx[0].col = D3DXVECTOR4(1.f, 1.f, 1.f, 0.f);
			// �e�N�X�`�����W
			pVtx[0].tex = D3DXVECTOR2((1.f / stacks) * cntStacks, 1.f - ((1.f / slices) * cntSlices));
			pVtx++;
		}
	}
	// ���b�N�̉���
	m_pVtxBuffer->Unlock();

	// �C���f�b�N�X�o�b�t�@
	WORD *pIdx;			// �o�b�t�@�����b�N����
	m_pIdxBuffer->Lock(0, 0, (void**)&pIdx, 0);

	int nCnt = 0;
	for (int nCntY = 0; nCntY < slices; nCntY++)
	{
		if (nCntY != 0)	// ��ԏ��߂łȂ�������
		{
			// ��_�ł�
			pIdx[nCnt] = static_cast<WORD>((stacks + 1) * nCntY + (stacks));
			nCnt++;
		}
		// 2�_�ł�(�㉺)
		pIdx[nCnt] = static_cast<WORD>((stacks + 1) * (nCntY + 1));
		nCnt++;
		pIdx[nCnt] = static_cast<WORD>((stacks + 1) * nCntY);
		nCnt++;

		for (int nCntX = 0; nCntX < stacks; nCntX++)
		{
			// 2�_�ł�(�㉺)
			pIdx[nCnt] = static_cast<WORD>(((stacks + 1) * (nCntY + 1)) + (nCntX + 1));
			nCnt++;
			pIdx[nCnt] = static_cast<WORD>((stacks + 1) * nCntY + (nCntX + 1));
			nCnt++;
		}
		if (nCntY != slices - 1)	// �Ō�łȂ�������
		{
			// ��_�ł�
			pIdx[nCnt] = static_cast<WORD>(stacks + (nCntY * (stacks)));
			nCnt++;
		}
	}

	//���b�N�̉���
	m_pIdxBuffer->Unlock();

	return NOERROR;
}

//--------------------------------------------------------------------------------
// �쐬����
//--------------------------------------------------------------------------------
CBgCylinder *CBgCylinder::Create(const D3DXVECTOR3 &pos, const D3DXVECTOR3 &rot, const D3DXVECTOR3 &size, const std::string &strFilePass)
{
	// CBgCylinder�̐���
	CBgCylinder *pSceneCylinder;
	pSceneCylinder = new CBgCylinder(1);
	pSceneCylinder->Init();

	pSceneCylinder->SetPos(pos);
	pSceneCylinder->SetRot(rot);
	pSceneCylinder->SetSize(size);
	pSceneCylinder->m_strFilePass = strFilePass;
	pSceneCylinder->SetCol(255, 255, 255, 255);

	// ���_�ݒ菈��
	pSceneCylinder->SetVtx(32, 1);

	// �e�N�X�`���̃Z�b�g
	CManager::GetTexManager()->SetTex(strFilePass);

	return pSceneCylinder;
}