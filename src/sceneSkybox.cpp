//================================================================================
//
//	Auter : KENSUKE WATANABE
//	Data  : 2017/04/27
//
//--------------------------------------------------------------------------------
#include <math.h>
#include "main.h"
#include "sceneSkybox.h"
#include "input.h"
#include "manager.h"
#include "texManager.h"
#include "renderer.h"
#include "mainController.h"
#include "deferredRendering.h"

//================================================================================
// �萔
//--------------------------------------------------------------------------------
static const int NUM_VTX = 18;			// ���_��
static const int NUM_IDX = 22;			// �C���f�b�N�X��

//================================================================================
// ������
//--------------------------------------------------------------------------------
void CSceneSkybox::Init( void )
{
	CScene3D::Init();

	// ������
	SetPos(D3DXVECTOR3(0.f, 0.f, 0.f));
	SetRot(D3DXVECTOR3(0.f, 0.f, 0.f));
	SetScl(D3DXVECTOR3(1.f, 1.f, 1.f));
	m_moveTex = 0.000f;
	m_strFilePass.clear();
	m_numVtx = NUM_VTX;
	m_numIndex = NUM_IDX;
}

//--------------------------------------------------------------------------------
// �I������
//--------------------------------------------------------------------------------
void CSceneSkybox::Uninit( void )
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
void CSceneSkybox::Update( void )
{
	AddRot(D3DXVECTOR3(0.f, m_moveTex, 0.f));
	SetPos(CManager::GetCamera()->GetPos());
}

//================================================================================
// �|���S���`��
//--------------------------------------------------------------------------------
void CSceneSkybox::Draw( void )
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

	pDevice->SetRenderState(D3DRS_LIGHTING, FALSE);			// ���C�g�\���I�t

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

	pDevice->DrawIndexedPrimitive(D3DPT_TRIANGLESTRIP, 0, 0, NUM_POLYGON, 0, m_numIndex - 2);

	// �����_�[�X�e�[�g�̐ݒ��߂�
	pDevice->SetRenderState(D3DRS_LIGHTING, TRUE);			// ���C�g�̕\����L���ɂ���
}

//--------------------------------------------------------------------------------
// �f�t�@�[�h�����_�����O�p
//--------------------------------------------------------------------------------
void CSceneSkybox::DrawDefferdRendering()
{
	// �f�o�C�X�̎擾
	LPDIRECT3DDEVICE9 pDevice = CRenderer::GetDevice();

//	pDevice->SetRenderState(D3DRS_FILLMODE, D3DFILL_WIREFRAME);

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

	D3DMATERIAL9 matDef;
	pDevice->GetMaterial(&matDef);

	// �f�t�@�[�h�����_�����O�̎擾
	DefferdRendering *pDefferdRendering = CManager::GetDefferdRendering();

	// ���_�V�F�[�_�[�̐ݒ�
	//------------------------------------------------------------------------
	pDevice->SetVertexDeclaration(m_pShaderSet->pVertexDeclaration);
	pDefferdRendering->SetVXShader(m_mtxWorld);

	// �f�o�C�X�Ƀ}�e���A����ݒ�
	pDevice->SetMaterial(m_pMat);

	// �s�N�Z���V�F�[�_�[�̐ݒ�
	//------------------------------------------------------------------------
	if (!m_strFilePass.empty())
	{
		LPDIRECT3DTEXTURE9 pTex = CManager::GetTexManager()->GetTexInterface(m_strFilePass);
		pDevice->SetTexture(0, pTex);
		pDefferdRendering->SetPXShader(m_pMat);
	}
	else 
	{ 
		pDevice->SetTexture(0, nullptr);
		pDefferdRendering->SetNoTexturePXShader(m_pMat);
	}

	// �C���f�b�N�X�ԍ����f�o�C�X�ɐݒ�
	pDevice->SetIndices(m_pIdxBuffer);

	// ���_�o�b�t�@���f�o�C�X�̃f�[�^ �X�g���[���Ƀo�C���h����
	pDevice->SetStreamSource(0, m_pVtxBuffer, 0, sizeof(VERTEX_3D));

	pDevice->DrawIndexedPrimitive(D3DPT_TRIANGLESTRIP, 0, 0, NUM_POLYGON, 0, m_numIndex - 2);

	// �����_�[�X�e�[�g�̐ݒ��߂�
//	pDevice->SetRenderState(D3DRS_FILLMODE, D3DFILL_SOLID);

	pDevice->SetMaterial(&matDef);	// ���ɖ߂�

	pDevice->SetPixelShader(nullptr);
	pDevice->SetVertexShader(nullptr);
}

//--------------------------------------------------------------------------------
// ���_�쐬
//--------------------------------------------------------------------------------
HRESULT CSceneSkybox::SetVtx()
{
	// �X���b�h�Z�[�t(���b�N)
#ifdef THREAD_ON
	std::unique_lock<std::recursive_mutex> locker = CManager::Locker();
#endif

	LPDIRECT3DDEVICE9 pDevice = CRenderer::GetDevice();

	// ���_�o�b�t�@���쐬����--------------------------------
	if (FAILED(pDevice->CreateVertexBuffer(sizeof(VERTEX_3D) * NUM_VTX,
		D3DUSAGE_WRITEONLY, FVF_VERTEX_3D, D3DPOOL_MANAGED, &m_pVtxBuffer, nullptr)))
	{
		return E_FAIL;
	}
	VERTEX_3D* pVtx;	// �o�b�t�@�����b�N����
	m_pVtxBuffer->Lock(0, 0, (void**)&pVtx, 0);

	D3DXVECTOR3 scl = GetScl();

	// ����
	pVtx[0].pos = D3DXVECTOR3(m_size, -m_size, -m_size);			// ���W
	pVtx[0].tex = D3DXVECTOR2(0.f, 0.6666666f);			// �e�N�X�`�����W
	pVtx[1].pos = D3DXVECTOR3(m_size, m_size, -m_size);			// ���W
	pVtx[1].tex = D3DXVECTOR2(0.f, 0.3333333f);			// �e�N�X�`�����W

	pVtx[2].pos = D3DXVECTOR3(-m_size, -m_size, -m_size);		// ���W
	pVtx[2].tex = D3DXVECTOR2(0.25f, 0.6666666f);		// �e�N�X�`�����W
	pVtx[3].pos = D3DXVECTOR3(-m_size, m_size, -m_size);			// ���W
	pVtx[3].tex = D3DXVECTOR2(0.25f, 0.3333333f);		// �e�N�X�`�����W

	pVtx[4].pos = D3DXVECTOR3(-m_size, -m_size, m_size);			// ���W
	pVtx[4].tex = D3DXVECTOR2(0.5f, 0.6666666f);		// �e�N�X�`�����W
	pVtx[5].pos = D3DXVECTOR3(-m_size, m_size, m_size);			// ���W
	pVtx[5].tex = D3DXVECTOR2(0.5f, 0.3333333f);		// �e�N�X�`�����W

	pVtx[6].pos = D3DXVECTOR3(m_size, -m_size, m_size);			// ���W
	pVtx[6].tex = D3DXVECTOR2(0.75f, 0.6666666f);		// �e�N�X�`�����W
	pVtx[7].pos = D3DXVECTOR3(m_size, m_size, m_size);			// ���W
	pVtx[7].tex = D3DXVECTOR2(0.75f, 0.3333333f);		// �e�N�X�`�����W

	pVtx[8].pos = D3DXVECTOR3(m_size, -m_size, -m_size);			// ���W
	pVtx[8].tex = D3DXVECTOR2(1.f, 0.6666666f);			// �e�N�X�`�����W
	pVtx[9].pos = D3DXVECTOR3(m_size, m_size, -m_size);			// ���W
	pVtx[9].tex = D3DXVECTOR2(1.f, 0.3333333f);			// �e�N�X�`�����W

														// ���
	pVtx[10].pos = D3DXVECTOR3(-m_size, m_size, -m_size);		// ���W
	pVtx[10].tex = D3DXVECTOR2(0.25f, 0.3333333f);		// �e�N�X�`�����W
	pVtx[11].pos = D3DXVECTOR3(m_size, m_size, -m_size);			// ���W
	pVtx[11].tex = D3DXVECTOR2(0.25f, 0.f);				// �e�N�X�`�����W
	pVtx[12].pos = D3DXVECTOR3(-m_size, m_size, m_size);			// ���W
	pVtx[12].tex = D3DXVECTOR2(0.5f, 0.3333333f);		// �e�N�X�`�����W
	pVtx[13].pos = D3DXVECTOR3(m_size, m_size, m_size);			// ���W
	pVtx[13].tex = D3DXVECTOR2(0.5f, 0.f);				// �e�N�X�`�����W

														// ����
	pVtx[14].pos = D3DXVECTOR3(-m_size, -m_size, m_size);		// ���W
	pVtx[14].tex = D3DXVECTOR2(0.25f, 0.6666666f);		// �e�N�X�`�����W
	pVtx[15].pos = D3DXVECTOR3(m_size, -m_size, m_size);			// ���W
	pVtx[15].tex = D3DXVECTOR2(0.25f, m_size);				// �e�N�X�`�����W
	pVtx[16].pos = D3DXVECTOR3(-m_size, -m_size, -m_size);		// ���W
	pVtx[16].tex = D3DXVECTOR2(0.5f, 0.6666666f);		// �e�N�X�`�����W
	pVtx[17].pos = D3DXVECTOR3(m_size, -m_size, -m_size);		// ���W
	pVtx[17].tex = D3DXVECTOR2(0.5f, m_size);				// �e�N�X�`�����W


	for (int i = 0; i < m_numVtx; i++)
	{
		pVtx[i].nom = D3DXVECTOR3(0.f, 1.f, 0.f);		// �@��
		pVtx[i].col = D3DXVECTOR4(1.f, 1.f, 1.f, 0.f);	// �F
	}

	// ���b�N�̉���
	m_pVtxBuffer->Unlock();

	// �C���f�b�N�X�o�b�t�@���쐬����--------------------------------
	if (FAILED(pDevice->CreateIndexBuffer(
		sizeof(WORD) * m_numIndex,			// �o�b�t�@��
		D3DUSAGE_WRITEONLY,					// �g�p�t���O
		D3DFMT_INDEX16,						// �K�v�ȃo�b�t�@��
		D3DPOOL_MANAGED,					// �������̊Ǘ����@
		&m_pIdxBuffer,
		nullptr)))
	{
		return E_FAIL;
	}

	WORD *pIdx;			// �o�b�t�@�����b�N����
	m_pIdxBuffer->Lock(0, 0, (void**)&pIdx, 0);

	pIdx[0] = static_cast<WORD>(0);
	pIdx[1] = static_cast<WORD>(1);
	pIdx[2] = static_cast<WORD>(2);
	pIdx[3] = static_cast<WORD>(3);
	pIdx[4] = static_cast<WORD>(4);
	pIdx[5] = static_cast<WORD>(5);
	pIdx[6] = static_cast<WORD>(6);
	pIdx[7] = static_cast<WORD>(7);
	pIdx[8] = static_cast<WORD>(8);
	pIdx[9] = static_cast<WORD>(9);
	pIdx[10] = static_cast<WORD>(9);
	pIdx[11] = static_cast<WORD>(10);
	pIdx[12] = static_cast<WORD>(10);
	pIdx[13] = static_cast<WORD>(11);
	pIdx[14] = static_cast<WORD>(12);
	pIdx[15] = static_cast<WORD>(13);
	pIdx[16] = static_cast<WORD>(13);
	pIdx[17] = static_cast<WORD>(14);
	pIdx[18] = static_cast<WORD>(14);
	pIdx[19] = static_cast<WORD>(15);
	pIdx[20] = static_cast<WORD>(16);
	pIdx[21] = static_cast<WORD>(17);

	//���b�N�̉���
	m_pIdxBuffer->Unlock();

	// �f�t���[�Y�F
	m_pMat = new D3DMATERIAL9;
	m_pMat->Diffuse.a = 0.f;
	m_pMat->Diffuse.r = 1.f;
	m_pMat->Diffuse.g = 1.f;
	m_pMat->Diffuse.b = 1.f;
	m_pMat->Specular.a = 1.f;
	m_pMat->Specular.r = 1.f;
	m_pMat->Specular.g = 1.f;
	m_pMat->Specular.b = 1.f;
	m_pMat->Power = 0.f;

	return NOERROR;
}

//--------------------------------------------------------------------------------
// �쐬����
//--------------------------------------------------------------------------------
CSceneSkybox *CSceneSkybox::Create(const D3DXVECTOR3 &pos, const D3DXVECTOR3 &rot, const float &size, const std::string &strFilePass, int pry)
{
	// CSceneSkybox�̐���
	CSceneSkybox *pSceneSkydome;
	pSceneSkydome = new CSceneSkybox(pry);
	pSceneSkydome->Init();

	pSceneSkydome->SetPos(pos);
	pSceneSkydome->SetRot(rot);
	pSceneSkydome->SetScl(D3DXVECTOR3(1.f, 1.f, 1.f));
	pSceneSkydome->m_strFilePass = strFilePass;

	pSceneSkydome->m_size = size;

	// ���_�ݒ菈��
	pSceneSkydome->SetVtx();
	
	// �e�N�X�`���̃Z�b�g
	CManager::GetTexManager()->SetTex(strFilePass);

	return pSceneSkydome;
}