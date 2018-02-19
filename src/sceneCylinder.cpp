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
#include "scene3D.h"
#include "sceneCylinder.h"
#include "input.h"
#include "manager.h"
#include "texManager.h"
#include "renderer.h"

//================================================================================
// ������
//--------------------------------------------------------------------------------
void CSceneCylinder::Init()
{
	// ������
	SetPos(D3DXVECTOR3(0.f, 0.f, 0.f));
	SetRot(D3DXVECTOR3(0.f, 0.f, 0.f));
	SetScl(D3DXVECTOR3(1.f, 1.f, 1.f));
	SetSize(D3DXVECTOR3(1.f, 1.f, 1.f));
	m_move = D3DXVECTOR3(0.f, 0.f, 0.f);
	m_strFilePass.clear();
}

//--------------------------------------------------------------------------------
// �I������
//--------------------------------------------------------------------------------
void CSceneCylinder::Uninit()
{
	// �C���f�b�N�X�o�b�t�@�̉��
	if(m_pIdxBuffer != nullptr)
	{
		m_pIdxBuffer->Release();
		m_pIdxBuffer = nullptr;
	}

	// �e�N���X�̏I��
	CScene3D::Uninit();
}

//================================================================================
// �X�V����
//--------------------------------------------------------------------------------
void CSceneCylinder::Update()
{
	// �X���b�h�Z�[�t(���b�N)
#ifdef THREAD_ON
	std::unique_lock<std::recursive_mutex> locker = CManager::Locker();
#endif

	VERTEX_3D* pVtx;	// �o�b�t�@�����b�N����
	m_pVtxBuffer->Lock(0, 0, (void**)&pVtx, 0);

	// 1�u���b�N�̕��ƍ���
	D3DXVECTOR3 size = GetSize();

	// ���_�̐ݒ�
	float x, y, z;

	float phi = (D3DX_PI * 2) / m_stacks;			// �Ƃ̊p�x

	for (int cntSlices = 0; cntSlices <= m_slices; cntSlices++)
	{
		for (int cntStacks = 0; cntStacks <= m_stacks; cntStacks++)
		{
			// ���W
			x = cos(phi * cntStacks) * size.x,				// X
				y = (float)m_slices - cntSlices * size.y,	// Y
				z = sin(phi * cntStacks) * size.z;			// Z
			pVtx[0].pos = D3DXVECTOR3(x, y, z);
			// �@��
			pVtx[0].nom = D3DXVECTOR3(0.f, 1.f, 0.f);
			// �F
			pVtx[0].col = D3DXVECTOR4(1.f, 1.f, 1.f, 1.f);
			// �e�N�X�`�����W
			pVtx[0].tex = D3DXVECTOR2((1.f / m_stacks) * cntStacks, (1.f / m_slices) * cntSlices);
			pVtx++;
		}
	}
	// ���b�N�̉���
	m_pVtxBuffer->Unlock();


	WORD *pIdx;			// �o�b�t�@�����b�N����
	m_pIdxBuffer->Lock(0, 0, (void**)&pIdx, 0);

	int nCnt = 0;
	for (int nCntY = 0; nCntY < m_slices; nCntY++)
	{
		if (nCntY != 0)	// ��ԏ��߂���Ȃ�������
		{
			// ��_�ł�
			pIdx[nCnt] = static_cast<WORD>((m_stacks + 1) * nCntY + (m_stacks + 1));
			nCnt++;
		}
		// 2�_�ł�(�㉺)
		pIdx[nCnt] = static_cast<WORD>((m_stacks + 1) * nCntY);
		nCnt++;
		pIdx[nCnt] = static_cast<WORD>((m_stacks + 1) * (nCntY + 1));
		nCnt++;

		for (int nCntX = 0; nCntX < m_stacks; nCntX++)
		{
			// 2�_�ł�(�㉺)
			pIdx[nCnt] = static_cast<WORD>((m_stacks + 1) * nCntY + (nCntX + 1));
			nCnt++;
			pIdx[nCnt] = static_cast<WORD>(((m_stacks + 1) * (nCntY + 1)) + (nCntX + 1));
			nCnt++;
		}
		if (nCntY != m_slices - 1)	// �Ōザ��Ȃ�������
		{
			// ��_�ł�
			pIdx[nCnt] = static_cast<WORD>(m_stacks + (nCntY * (m_stacks + 1)));
			nCnt++;
		}
	}

	//���b�N�̉���
	m_pIdxBuffer->Unlock();
}

//================================================================================
// �|���S���`��
//--------------------------------------------------------------------------------
void CSceneCylinder::Draw()
{
	// �f�o�C�X�̎擾
	LPDIRECT3DDEVICE9 pDevice = CRenderer::GetDevice();

	D3DXVECTOR3 pos = GetPos();
	D3DXVECTOR3 rot = GetRot();
	D3DXVECTOR3 scl = GetScl();

	// ���[���h�}�g���N�X�̍쐬
	D3DXMATRIX mtxScl;						// �g��s��
	D3DXMATRIX mtxRot;						// ��]�s��
	D3DXMATRIX mtxPos;						// ���s�s��

	D3DXMatrixIdentity(&m_mtxWorld);		// �s���P�ʍs��ɂ���( �g�嗦1,��0�ŏ����� )
	D3DXMatrixIdentity(&mtxScl);			// 
	D3DXMatrixIdentity(&mtxRot);			// 
	D3DXMatrixIdentity(&mtxPos);			// 

	// ���[���h�ϊ��s���ݒ肵�ĕϊ�
	D3DXMatrixScaling(&mtxScl, scl.x, scl.y, scl.z);				// �g��s��
	D3DXMatrixRotationYawPitchRoll(&mtxRot, rot.y, rot.x, rot.z);	// ��]�s��
	D3DXMatrixTranslation(&mtxPos, pos.x, pos.y, pos.z);			// ���s�s��

	D3DXMatrixMultiply(&m_mtxWorld, &m_mtxWorld, &mtxScl);
	D3DXMatrixMultiply(&m_mtxWorld, &m_mtxWorld, &mtxRot);
	D3DXMatrixMultiply(&m_mtxWorld, &m_mtxWorld, &mtxPos);

	// �f�o�C�X�Ƀ��[���h�ϊ��s���ݒ�
	pDevice->SetTransform(D3DTS_WORLD, &m_mtxWorld);

	// ���_�t�H�[�}�b�g�̐ݒ�
	pDevice->SetFVF(FVF_VERTEX_3D);

	// �C���f�b�N�X�ԍ����f�o�C�X�ɐݒ�
	pDevice->SetIndices(m_pIdxBuffer);

	// �e�N�X�`�����p�C�v���C���ɃZ�b�g
	if(!m_strFilePass.empty())
	{
		pDevice->SetTexture(0, CManager::GetTexManager()->GetTexInterface(m_strFilePass));
	}else {pDevice->SetTexture(0, nullptr);}

	// ���_�o�b�t�@���f�o�C�X�̃f�[�^ �X�g���[���Ƀo�C���h����
	pDevice->SetStreamSource(0, m_pVtxBuffer, 0, sizeof(VERTEX_3D));

	pDevice->DrawIndexedPrimitive(D3DPT_TRIANGLESTRIP, 0, 0, NUM_POLYGON, 0, GetNumIndex() - 2);
}

//--------------------------------------------------------------------------------
// ���_�쐬
//--------------------------------------------------------------------------------
HRESULT CSceneCylinder::SetVtx(int stacks, int slices)
{
	m_slices = slices;
	m_stacks = stacks;

	int numVtx = ((slices + 1) * (stacks + 1));						// ���_��
	int numIndex = ((stacks + 1) * 2 * slices) + (slices - 1) * 2;	// �C���f�b�N�X��(�O�p�`�� + �ѕ�(�k�ރ|���S��))
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

	return NOERROR;
}

//--------------------------------------------------------------------------------
// �쐬����
//--------------------------------------------------------------------------------
CSceneCylinder *CSceneCylinder::Create(const D3DXVECTOR3 &pos, const D3DXVECTOR3 &rot, const D3DXVECTOR3 &size, const std::string &strFilePass)
{
	// CSceneCylinder�̐���
	CSceneCylinder *pSceneCylinder;
	pSceneCylinder = new CSceneCylinder();
	pSceneCylinder->Init();

	pSceneCylinder->SetPos(pos);
	pSceneCylinder->SetRot(rot);
	pSceneCylinder->SetSize(size);
	pSceneCylinder->m_strFilePass = strFilePass;

	// ���_�ݒ菈��
	pSceneCylinder->SetVtx(32, 1);
	
	// �e�N�X�`���̃Z�b�g
	CManager::GetTexManager()->SetTex(strFilePass);

	return pSceneCylinder;
}