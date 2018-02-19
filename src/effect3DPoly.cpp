//================================================================================
//
//	Auter : KENSUKE WATANABE
//	Data  : 2016/08/30
//
//--------------------------------------------------------------------------------
//--------------------------------------------------------------------------------
// �C���N���[�h
//--------------------------------------------------------------------------------
#include <math.h>
#include "main.h"
#include "manager.h"
#include "effect3DPoly.h"
#include "modeGame.h"
#include "renderer.h"
#include "texManager.h"
#include "camera.h"
#include "deferredRendering.h"

int CEffect3DPoly::m_maxEffect = 0;
int CEffect3DPoly::m_cnt = 0;
std::vector<CEffect3DPoly*> CEffect3DPoly::m_pvecEffect3DPoly;

//--------------------------------------------------------------------------------
// �����������֐�
//--------------------------------------------------------------------------------
void CEffect3DPoly::Init()
{
	m_bUse = false;
	m_fRadius = 0.f;							// ���a
	m_fRadiusValue = 0.f;						// ���a�ω���
	m_fColorAlphaValue = 0.f;					// ���l�̕ω���
	m_nLife = 0;								// ����
	m_move = D3DXVECTOR3(0.f, 0.f, 0.f);		// �ړ���
}

//--------------------------------------------------------------------------------
// �I������
//--------------------------------------------------------------------------------
void CEffect3DPoly::Uninit()
{

}

//--------------------------------------------------------------------------------
// �X�V�����֐�
//--------------------------------------------------------------------------------
void CEffect3DPoly::Update()
{
	if (m_bUse)
	{
		// �X���b�h�Z�[�t(���b�N)
#ifdef THREAD_ON
		std::unique_lock<std::recursive_mutex> locker = CManager::Locker();
#endif

		//�g�p���̃G�t�F�N�g�̍X�V
		m_nLife--;	//���������炷
		if (m_nLife <= 0)
		{//������0�ȉ��ɂȂ�����
			m_bUse = false;	//�g�p�t���O�I�t
		}
		else
		{
			//�F�̐ݒ�
			float a = GetA();
		//	if (a > 0.f) AddCol(0, 0, 0, -m_fColorAlphaValue);
		}
		//���a�̍X�V�Ɛݒ�
		m_fRadius -= m_fRadiusValue;
		if (m_fRadius < 0.f)
		{//���a��0.0��菬�����Ȃ�Ȃ��悤��
			m_fRadius = 0.f;
		}

		m_pos += m_move;
	}
}

/*
//--------------------------------------------------------------------------------
// �`�揈���֐�
//--------------------------------------------------------------------------------
void CEffect3DPoly::Draw()
{
	//�g���Ă���p�[�e�B�N���̕`��
	if (m_bUse == false)return;

	// �f�o�C�X�̎擾
	LPDIRECT3DDEVICE9 pDevice = CRenderer::GetDevice();

	pDevice->SetRenderState(D3DRS_LIGHTING, FALSE);				// ���C�g�\���I�t

	// �A���t�@�u�����h
	pDevice->SetRenderState(D3DRS_ALPHABLENDENABLE, TRUE);
	pDevice->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
	pDevice->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA);

	// �A���t�@�e�X�g�̗L����
	pDevice->SetRenderState(D3DRS_ALPHATESTENABLE, TRUE);
	pDevice->SetRenderState(D3DRS_ALPHAFUNC, D3DCMP_GREATEREQUAL);
	// �s�����ɂ���l�̐ݒ�
	pDevice->SetRenderState(D3DRS_ALPHAREF, (DWORD)0x00000001);

	D3DXMATRIX mtxScl;			// �g��s��
	D3DXMATRIX mtxRot;			// ��]�s��
	D3DXMATRIX mtxViewInverse;
	D3DXMATRIX mtxPos;			// ���s�s��

	// ���[���h�}�g���N�X�̍쐬
	D3DXMatrixIdentity(&mtxScl);			//
	D3DXMatrixIdentity(&mtxRot);			//
	D3DXMatrixIdentity(&mtxPos);			//

	D3DXMATRIX mtxView = CManager::GetCamera()->GetView();		// �r���[�s��̎擾
	mtxView._41 = 0.f;						//
	mtxView._42 = 0.f;						//
	mtxView._43 = 0.f;						// ���s�s��̃J�b�g

	D3DXMatrixInverse(&mtxViewInverse, nullptr, &mtxView);	// �t�s��̏o��

	D3DXMatrixIdentity(&m_mtxWorld);		// �s���P�ʍs��ɂ���( �g�嗦1,��0�ŏ����� )

	// ���[���h�ϊ��s���ݒ肵�ĕϊ�
	D3DXVECTOR3 scl = GetScl();
	D3DXVECTOR3 rot = m_rot;
	D3DXVECTOR3 pos = m_pos;
	D3DXMatrixScaling(&mtxScl, scl.x, scl.y, scl.z);					// �g��s��
	D3DXMatrixRotationYawPitchRoll(&mtxRot, rot.y, rot.x, rot.z);		// ��]�s��
	D3DXMatrixTranslation(&mtxPos, pos.x, pos.y, pos.z);				// ���s�s��

	D3DXMatrixMultiply(&m_mtxWorld, &m_mtxWorld, &mtxViewInverse);
	D3DXMatrixMultiply(&m_mtxWorld, &m_mtxWorld, &mtxScl);
	D3DXMatrixMultiply(&m_mtxWorld, &m_mtxWorld, &mtxRot);
	D3DXMatrixMultiply(&m_mtxWorld, &m_mtxWorld, &mtxPos);

	// �f�o�C�X�Ƀ��[���h�ϊ��s���ݒ�
	pDevice->SetTransform(D3DTS_WORLD, &m_mtxWorld);

	// ���_�ݒ�
	SetVertex();

	// ���_�t�H�[�}�b�g�̐ݒ�
	pDevice->SetFVF(FVF_VERTEX_3D);

	// �X�g���[���̐ݒ�
	pDevice->SetStreamSource(0, m_pVtxBuffer, 0, sizeof(VERTEX_3D));

	// �e�N�X�`���̐ݒ�
	pDevice->SetTexture(0, CManager::GetTexManager()->GetTexInterface(m_strFilePass));

	pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, 0, 2);	//�v���~�e�B�u��

	// �A���t�@�e�X�g�𖳌���
	pDevice->SetRenderState(D3DRS_ALPHATESTENABLE, FALSE);

	// ���C�g�̕\����L���ɂ���
	pDevice->SetRenderState(D3DRS_LIGHTING, TRUE);
}
*/

//--------------------------------------------------------------------------------
// �f�t�@�[�h�����_�����O�p
//--------------------------------------------------------------------------------
void CEffect3DPoly::DrawDefferdRendering()
{
	//�g���Ă���p�[�e�B�N���̕`��
	if (!m_bUse)return;

	// �f�o�C�X�̎擾
	LPDIRECT3DDEVICE9 pDevice = CRenderer::GetDevice();

	D3DXMATRIX mtxScl;			// �g��s��
	D3DXMATRIX mtxRot;			// ��]�s��
	D3DXMATRIX mtxViewInverse;	// �r���{�[�h�p
	D3DXMATRIX mtxPos;			// ���s�s��

	// ���[���h�}�g���N�X�̍쐬
	D3DXMatrixIdentity(&mtxScl);			// 
	D3DXMatrixIdentity(&mtxRot);			// 
	D3DXMatrixIdentity(&mtxPos);			//

	D3DXMATRIX mtxView = CManager::GetCamera()->GetView();		// �r���[�s��̎擾
	mtxView._41 = 0.f;						// 
	mtxView._42 = 0.f;						// 
	mtxView._43 = 0.f;						// ���s�s��̃J�b�g

	D3DXMatrixInverse(&mtxViewInverse, nullptr, &mtxView);		// �t�s��̏o��

	D3DXMatrixIdentity(&m_mtxWorld);		// �s���P�ʍs��ɂ���( �g�嗦1,��0�ŏ����� )

	// ���[���h�ϊ��s���ݒ肵�ĕϊ�
	D3DXVECTOR3 scl = GetScl();
	D3DXVECTOR3 rot = m_rot;
	D3DXVECTOR3 pos = m_pos;
	D3DXMatrixScaling(&mtxScl, scl.x, scl.y, scl.z);					// �g��s��
	D3DXMatrixRotationYawPitchRoll(&mtxRot, rot.y, rot.x, rot.z);		// ��]�s��
	D3DXMatrixTranslation(&mtxPos, pos.x, pos.y, pos.z);				// ���s�s��

	D3DXMatrixMultiply(&m_mtxWorld, &m_mtxWorld, &mtxViewInverse);
	D3DXMatrixMultiply(&m_mtxWorld, &m_mtxWorld, &mtxScl);
	D3DXMatrixMultiply(&m_mtxWorld, &m_mtxWorld, &mtxRot);
	D3DXMatrixMultiply(&m_mtxWorld, &m_mtxWorld, &mtxPos);

	// �f�o�C�X�Ƀ��[���h�ϊ��s���ݒ�
	pDevice->SetTransform(D3DTS_WORLD, &m_mtxWorld);

	// ���_�ݒ�
	SetVertex();

	// �f�t�@�[�h�����_�����O�̎擾
	DefferdRendering *pDefferdRendering = CManager::GetDefferdRendering();

	// �A���t�@�e�X�g�̗L����
	pDevice->SetRenderState(D3DRS_ALPHATESTENABLE, TRUE);
	pDevice->SetRenderState(D3DRS_ALPHAFUNC, D3DCMP_GREATEREQUAL);
	// �s�����ɂ���l�̐ݒ�
	pDevice->SetRenderState(D3DRS_ALPHAREF, (DWORD)0x00000001);

	// ���_�V�F�[�_�[�̐ݒ�
	//------------------------------------------------------------------------
	pDevice->SetVertexDeclaration(m_pShaderSet->pVertexDeclaration);
	pDefferdRendering->SetVXShader(m_mtxWorld);

	// �s�N�Z���V�F�[�_�[�̐ݒ�
	//------------------------------------------------------------------------
	pDevice->SetTexture(0, CManager::GetTexManager()->GetTexInterface(m_strFilePass));
	pDefferdRendering->SetPXShader(m_pMat);

	// �X�g���[���̐ݒ�
	pDevice->SetStreamSource(0, m_pVtxBuffer, 0, sizeof(VERTEX_3D));
	// �`��
	pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, 0, 2);	//�v���~�e�B�u��

	pDevice->SetRenderState(D3DRS_ALPHATESTENABLE, FALSE);

	pDevice->SetPixelShader(nullptr);
	pDevice->SetVertexShader(nullptr);
}

//--------------------------------------------------------------------------------
// �쐬
//--------------------------------------------------------------------------------
CEffect3DPoly *CEffect3DPoly::Create(int n, std::string texName)
{
	// �e�N�X�`���̃Z�b�g
	CManager::GetTexManager()->SetTex(texName);

	// �G�t�F�N�g
	CEffect3DPoly *pEffPoly = nullptr;
	m_pvecEffect3DPoly.clear();

	for (int nCnt = 0; nCnt < n; nCnt++)
	{
		pEffPoly = nullptr;
		pEffPoly = new CEffect3DPoly(6);
		pEffPoly->Init();
		pEffPoly->m_strFilePass = texName;
		pEffPoly->SetAlpha();
		pEffPoly->SetOverall();
		pEffPoly->m_pvecEffect3DPoly.push_back(pEffPoly);
		pEffPoly->CEffect3DPoly::MakeVertex();

		// �}�e���A������
		pEffPoly->m_pMat = new D3DMATERIAL9;
	}
	// �L���p�V�e�B�ݒ�
	pEffPoly->m_maxEffect = n;
	pEffPoly->m_pvecEffect3DPoly.shrink_to_fit();

	return pEffPoly;
}

//--------------------------------------------------------------------------------
// �G�t�F�N�g���ݒ�
//--------------------------------------------------------------------------------
void CEffect3DPoly::SetEffect3DPoly(D3DXVECTOR3 pos, D3DXVECTOR3 rot, D3DXVECTOR3 move, float radius, float r, float g, float b, float a, float life, std::string texName)
{
	// �e�N�X�`���̃Z�b�g
	CManager::GetTexManager()->SetTex(texName);

	for (auto it : m_pvecEffect3DPoly)
	{
		if (it->m_bUse == false)
		{
			// �G�t�F�N�g�Z�b�g
			it->m_strFilePass = texName;
			it->m_pos = pos;
			it->m_move = move *= CManager::GetMasterScale();	// �X�P�[�������ꗥ��
			it->m_rot = rot;
			it->m_fRadius = radius;
			it->SetCol(r, g, b, a);
			it->m_nLife = life;

			it->m_bUse = true;
			it->m_fRadiusValue = radius * 0.75f / life * CManager::GetMasterScale();	// �������s����܂łɑ傫���𔼕��ɂȂ�悤�ɒl��ݒ�
			it->m_fColorAlphaValue = 1.f / life;			// �������s����܂łɓ����ɂȂ�悤�ɒl��ݒ�
			break;
		}
	}
}

//--------------------------------------------------------------------------------
// ���_�쐬�֐�
//--------------------------------------------------------------------------------
HRESULT CEffect3DPoly::MakeVertex()
{
	// �X���b�h�Z�[�t(���b�N)
#ifdef THREAD_ON
	std::unique_lock<std::recursive_mutex> locker = CManager::Locker();
#endif

	// D3Device�̎擾
	LPDIRECT3DDEVICE9 pDevice = CManager::GetRenderer()->GetDevice();

	// ���_���̍쐬
	//---------------------
	HRESULT hr = pDevice->CreateVertexBuffer(
		sizeof(VERTEX_3D) * NUM_VERTEX,		// �������̃T�C�Y
		D3DUSAGE_WRITEONLY,					// �g�p���@(�������݂̂�)
		FVF_VERTEX_3D,						// �g�p���钸�_�t�H�[�}�b�g(���Ȃ�)
		D3DPOOL_MANAGED,					// �o�b�t�@�̊Ǘ����@(����ɕ������Ă����)
		&m_pVtxBuffer,						// 
		nullptr);
	if (FAILED(hr))
	{
		// ���s������G���[��Ԃ�
		MessageBox(nullptr, "3D�pVtxBuffer�̍쐬�Ɏ��s���܂���", "�x��", MB_OK);
		return hr;
	}

	return hr;
}

//--------------------------------------------------------------------------------
// ���_���W�ݒ�
//--------------------------------------------------------------------------------
void CEffect3DPoly::SetVertex()
{
	//�o�b�t�@�����b�N����
	VERTEX_3D* pVtx;
	m_pVtxBuffer->Lock(0, 0, (void**)&pVtx, 0);

	//���_���W�̐ݒ�
	pVtx[0].pos = D3DXVECTOR3(-m_fRadius, m_fRadius, 0.f);
	pVtx[1].pos = D3DXVECTOR3(m_fRadius, m_fRadius, 0.f);
	pVtx[2].pos = D3DXVECTOR3(-m_fRadius, -m_fRadius, 0.f);
	pVtx[3].pos = D3DXVECTOR3(m_fRadius, -m_fRadius, 0.f);

	// �@��
	for (int i = 0; i < 4; i++)
	{
		pVtx[i].nom = D3DXVECTOR3(0.0f, 1.f, 0.0f);
	}

	//���_�F�̐ݒ�
	float r = GetR();
	float g = GetG();
	float b = GetB();
	float a = GetA();
	pVtx[0].col = D3DXVECTOR4(1.f, 1.f, 1.f, 1.f);
	pVtx[1].col = D3DXVECTOR4(1.f, 1.f, 1.f, 1.f);
	pVtx[2].col = D3DXVECTOR4(1.f, 1.f, 1.f, 1.f);
	pVtx[3].col = D3DXVECTOR4(1.f, 1.f, 1.f, 1.f);

	// �f�B�t���[�Y�F
	m_pMat->Diffuse.a = 1.f;
	m_pMat->Diffuse.r = r;
	m_pMat->Diffuse.g = g;
	m_pMat->Diffuse.b = b;
	m_pMat->Power = 1.f;

	//���W�ƐF�ݒ�
	pVtx[0].tex = D3DXVECTOR2(0.0f, 0.0f);
	pVtx[1].tex = D3DXVECTOR2(1.0f, 0.0f);
	pVtx[2].tex = D3DXVECTOR2(0.0f, 1.0f);
	pVtx[3].tex = D3DXVECTOR2(1.0f, 1.0f);

	//���_�o�b�t�@�A�����b�N
	m_pVtxBuffer->Unlock();
}

void CEffect3DPoly::FlashAll()
{
	for (auto it : m_pvecEffect3DPoly)
	{
		if (it == nullptr)continue;
		it->m_bUse = false;
	}
}
