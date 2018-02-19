//--------------------------------------------------------------------------------
//
//	Auter : KENSUKE WATANABE
//	Data  : 2016/08/30
//
//--------------------------------------------------------------------------------

// �C���N���[�h
//--------------------------------------------------------------------------------
#include <math.h>
#include "main.h"
#include "manager.h"
#include "effect2DPoly.h"
#include "modeGame.h"
#include "renderer.h"
#include "texManager.h"

int CEffect2DPoly::m_maxEffect = 0;
int CEffect2DPoly::m_cnt = 0;
std::vector<CEffect2DPoly*> CEffect2DPoly::m_pvecEffect2DPoly;

//--------------------------------------------------------------------------------
// �����������֐�
//--------------------------------------------------------------------------------
void CEffect2DPoly::Init()
{
	m_bUse = false;
	m_fRadius = 0.0f;					// ���a
	m_fRadiusValue = 0.0f;				// ���a�ω���
	m_colorAlphaValue = 0;				// ���l�̕ω���
	m_nLife = 0;						// ����
	m_move = D3DXVECTOR2(0.0f, 0.0f);
}

//--------------------------------------------------------------------------------
// �I������
//--------------------------------------------------------------------------------
void CEffect2DPoly::Uninit()
{

}

//--------------------------------------------------------------------------------
// �X�V�����֐�
//--------------------------------------------------------------------------------
void CEffect2DPoly::Update()
{
	if (m_bUse)
	{
		// �X���b�h�Z�[�t(���b�N)
#ifdef THREAD_ON
		std::unique_lock<std::recursive_mutex> locker = CManager::Locker();
#endif

		VERTEX_2D* pVtx;	//���_��񏑂����ݐ�̃|�C���^�ϐ�
		//�o�b�t�@�����b�N����
		m_pVtxBuffer->Lock(0, 0, (void**)&pVtx, 0);

		//�g�p���̃G�t�F�N�g�̍X�V
		m_nLife--;	//���������炷
		if (m_nLife <= 0)
		{//������0�ȉ��ɂȂ�����
			m_bUse = false;	//�g�p�t���O�I�t
		}
		else
		{
			//�F�̐ݒ�
			m_a -= m_colorAlphaValue;
			if (m_a < 0)
			{//���l��0.0��菬�����Ȃ�Ȃ��悤��
				m_a = 0;
			}
		}
		//���a�̍X�V�Ɛݒ�
		m_fRadius -= m_fRadiusValue;
		if (m_fRadius < 0.0f)
		{//���a��0.0��菬�����Ȃ�Ȃ��悤��
			m_fRadius = 0.0f;
		}

		// ���W�̉��Z
		m_pos += D3DXVECTOR3(m_move.x, m_move.y, 0.f);

		//���_�|���S���̃|���S�����W�����W�Ɣ��a����Z�o���ď�������
		SetVertex(pVtx);

		//���_�o�b�t�@�A�����b�N
		m_pVtxBuffer->Unlock();
	}
}

//--------------------------------------------------------------------------------
// �`�揈���֐�
//--------------------------------------------------------------------------------
void CEffect2DPoly::Draw()
{
	// �f�o�C�X�̎擾
	LPDIRECT3DDEVICE9 pDevice = CRenderer::GetDevice();

	pDevice->SetRenderState(D3DRS_LIGHTING, FALSE);				// ���C�g�\���I�t
	pDevice->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_ONE);		// DEST��RGB�̌W�����P�ɂ���
	pDevice->SetRenderState(D3DRS_ZENABLE, D3DZB_FALSE);		// Z�o�b�t�@

	//�g���Ă���p�[�e�B�N���̕`��
	if (m_bUse)
	{
		// ���_�o�b�t�@���f�o�C�X�̃f�[�^ �X�g���[���Ƀo�C���h����
		pDevice->SetStreamSource(0, m_pVtxBuffer, 0, sizeof(VERTEX_2D));

		// ���_�t�H�[�}�b�g�̐ݒ�
		pDevice->SetFVF(FVF_VERTEX_2D);

		// �e�N�X�`���̐ݒ�
		pDevice->SetTexture(0, CManager::GetTexManager()->GetTexInterface(m_strFilePass));

		// �|���S���̕`�� // �|���S���̌`, ���[�h����ŏ��̒��_�̃C���f�b�N�X, �|���S����
		pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, 0, NUM_POLYGON);
	}

	pDevice->SetRenderState(D3DRS_ZENABLE, D3DZB_TRUE);			// Z�o�b�t�@
	// ���C�g�̕\����L���ɂ���
	pDevice->SetRenderState(D3DRS_LIGHTING, TRUE);

	//�����_�[�X�e�[�g�̐ݒ��߂�
	pDevice->SetRenderState(D3DRS_BLENDOP, D3DBLENDOP_ADD);
	pDevice->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
	pDevice->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA);
}

//--------------------------------------------------------------------------------
// �쐬
//--------------------------------------------------------------------------------
CEffect2DPoly *CEffect2DPoly::Create(int n, std::string texName)
{
	// �e�N�X�`���̃Z�b�g
	CManager::GetTexManager()->SetTex(texName);

	// �G�t�F�N�g
	CEffect2DPoly *pEffPoly = nullptr;
	m_pvecEffect2DPoly.clear();

	for (int nCnt = 0; nCnt < n; nCnt++)
	{
		pEffPoly = new CEffect2DPoly(5);
		pEffPoly->Init();
		pEffPoly->m_strFilePass = texName;
		pEffPoly->SetAlpha();
		pEffPoly->m_pvecEffect2DPoly.push_back(pEffPoly);
		pEffPoly->CEffect2DPoly::MakeVertex();
	}
	// �L���p�V�e�B�ݒ�
	pEffPoly->m_maxEffect = n;
	pEffPoly->m_pvecEffect2DPoly.shrink_to_fit();

	return pEffPoly;
}

//--------------------------------------------------------------------------------
// �G�t�F�N�g���ݒ�
//--------------------------------------------------------------------------------
void CEffect2DPoly::SetEffect2DPoly(D3DXVECTOR2 pos, float rot, D3DXVECTOR2 move, float radius, int &r, int &g, int &b, int &a, int life, std::string texName)
{
	// �e�N�X�`���̃Z�b�g
	CManager::GetTexManager()->SetTex(texName);

	for (int i = 0; i < m_maxEffect; i++)
	{
		auto it = m_pvecEffect2DPoly.at(i);
		if (it->m_bUse == false)
		{
			// �G�t�F�N�g�Z�b�g
			it->m_strFilePass = texName;
			it->m_pos = D3DXVECTOR3(pos.x, pos.y, 0.f);
			it->m_move = move;
			it->m_rot = D3DXVECTOR3(0.f, 0.f, rot);
			it->m_fRadius = radius;
			it->SetCol(r, g, b, a);
			it->m_nLife = life;

			it->m_bUse = true;
			it->m_fRadiusValue = radius * 0.5f / life;		//�������s����܂łɑ傫���𔼕��ɂȂ�悤�ɒl��ݒ�
			it->m_colorAlphaValue = 255 / life;				//�������s����܂łɓ����ɂȂ�悤�ɒl��ݒ�
			break;
		}
	}
}

//--------------------------------------------------------------------------------
// ���_�쐬
//--------------------------------------------------------------------------------
HRESULT CEffect2DPoly::MakeVertex()
{
	// �X���b�h�Z�[�t(���b�N)
#ifdef THREAD_ON
	std::unique_lock<std::recursive_mutex> locker = CManager::Locker();
#endif

	// D3Device�̎擾
	LPDIRECT3DDEVICE9 pDevice = CManager::GetRenderer()->GetDevice();

	// ���_���̍쐬
	//---------------------
	if (FAILED(pDevice->CreateVertexBuffer(
		sizeof(VERTEX_2D) * NUM_VERTEX,			// �������̃T�C�Y
		D3DUSAGE_WRITEONLY,					// �g�p���@(�������݂̂�)
		FVF_VERTEX_2D,						// �g�p���钸�_�t�H�[�}�b�g(���Ȃ�)
		D3DPOOL_MANAGED,					// �o�b�t�@�̊Ǘ����@(����ɕ������Ă����)
		&m_pVtxBuffer,							// 
		nullptr)))
	{
		// ���s������G���[��Ԃ�
		MessageBox(nullptr, "2D�pVtxBuffer�̍쐬�Ɏ��s���܂���", "�x��", MB_OK);
		return E_FAIL;
	}
	return S_OK;
}

//--------------------------------------------------------------------------------
// ���_���W�ݒ�
//--------------------------------------------------------------------------------
void CEffect2DPoly::SetVertex(VERTEX_2D* pVtx)
{
	//���_���W�̐ݒ�
	pVtx[0].pos = D3DXVECTOR3(-m_fRadius + m_pos.x, -m_fRadius + m_pos.y, 0.0f);
	pVtx[1].pos = D3DXVECTOR3(m_fRadius + m_pos.x, -m_fRadius + m_pos.y, 0.0f);
	pVtx[2].pos = D3DXVECTOR3(-m_fRadius + m_pos.x, m_fRadius + m_pos.y, 0.0f);
	pVtx[3].pos = D3DXVECTOR3(m_fRadius + m_pos.x, m_fRadius + m_pos.y, 0.0f);

	//���_�F�̐ݒ�
	int r = m_r;
	int g = m_g;
	int b = m_b;
	int a = m_a;

	pVtx[0].col = D3DCOLOR_RGBA(r, g, b, a);
	pVtx[1].col = D3DCOLOR_RGBA(r, g, b, a);
	pVtx[2].col = D3DCOLOR_RGBA(r, g, b, a);
	pVtx[3].col = D3DCOLOR_RGBA(r, g, b, a);

	//���W�ƐF�ݒ�
	pVtx[0].tex = D3DXVECTOR2(0.0f, 0.0f);
	pVtx[1].tex = D3DXVECTOR2(1.0f, 0.0f);
	pVtx[2].tex = D3DXVECTOR2(0.0f, 1.0f);
	pVtx[3].tex = D3DXVECTOR2(1.0f, 1.0f);

	// rhw
	for (int i = 0; i < 4; i++)
	{
		pVtx[i].rhw = 1.0f;
	}
}
