//================================================================================
//
//	Auter : KENSUKE WATANABE
//	Data  : 2018/02/10
//
//--------------------------------------------------------------------------------
#include "texManager.h"
#include "manager.h"
#include "modeGame.h"
#include "stencilShadow.h"
#include "input.h"
#include "renderer.h"

#include <assert.h>

//--------------------------------------------------------------------------------
// ������
//--------------------------------------------------------------------------------
void CStencilShadow::Init()
{
	CScene2D::Init();
	SetCol(1, 1, 1, 192);
}

//--------------------------------------------------------------------------------
// �I��
//--------------------------------------------------------------------------------
void CStencilShadow::Uninit()
{
	CScene2D::Uninit();
}

//--------------------------------------------------------------------------------
// �X�V
//--------------------------------------------------------------------------------
void CStencilShadow::Update()
{
	CScene2D::Update();
}

//--------------------------------------------------------------------------------
// �`��
//--------------------------------------------------------------------------------
void CStencilShadow::Draw()
{
	// D3Device�̎擾
	LPDIRECT3DDEVICE9 pDevice = CRenderer::GetDevice();

	// ���_�o�b�t�@���f�o�C�X�̃f�[�^ �X�g���[���Ƀo�C���h����
	pDevice->SetStreamSource(0, m_pVtxBuffer, 0, sizeof(VERTEX_2D));

	// ���_�t�H�[�}�b�g�̐ݒ�
	pDevice->SetFVF(FVF_VERTEX_2D);

	// �e�N�X�`���̐ݒ�
	pDevice->SetTexture(0, CManager::GetTexManager()->GetTexInterface(m_strFilePass));

	pDevice->SetRenderState(D3DRS_COLORWRITEENABLE, 0xf);
	pDevice->SetRenderState(D3DRS_CULLMODE, D3DCULL_CCW);
	pDevice->SetRenderState(D3DRS_STENCILFUNC, D3DCMP_EQUAL);
	pDevice->SetRenderState(D3DRS_STENCILENABLE, TRUE);
	pDevice->SetRenderState(D3DRS_STENCILREF, 1);
	pDevice->SetRenderState(D3DRS_ZWRITEENABLE, FALSE);

	// �|���S���̕`�� // �|���S���̌`, ���[�h����ŏ��̒��_�̃C���f�b�N�X, �|���S����
	pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, 0, NUM_POLYGON);

	pDevice->SetRenderState(D3DRS_ZWRITEENABLE, TRUE);
	pDevice->SetRenderState(D3DRS_STENCILENABLE, FALSE);
}

//--------------------------------------------------------------------------------
// �쐬����
//--------------------------------------------------------------------------------
CStencilShadow *CStencilShadow::Create(const D3DXVECTOR3 &pos, const D3DXVECTOR3 &rot, const D3DXVECTOR3 &size, const std::string &strFilePass, int priority)
{
	// CStencilShadow�̐���
	CStencilShadow *pShadow2D;
	pShadow2D = new CStencilShadow(priority);
	pShadow2D->Init();

	pShadow2D->SetPos(pos);
	pShadow2D->SetRot(rot);
	pShadow2D->SetSize(size);
	pShadow2D->m_strFilePass = strFilePass;
	pShadow2D->m_pVtxBuffer = nullptr;

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
		&pShadow2D->m_pVtxBuffer,							// 
		nullptr)))
	{
		// ���s������G���[��Ԃ�
		MessageBox(nullptr, "2D�pVtxBuffer�̍쐬�Ɏ��s���܂���", "�x��", MB_OK);
		return nullptr;
	}

	// �e�N�X�`���̃Z�b�g
	CManager::GetTexManager()->SetTex(strFilePass);

	return pShadow2D;
}
