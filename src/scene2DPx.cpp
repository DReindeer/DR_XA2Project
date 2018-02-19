//================================================================================
//
//	Auter : KENSUKE WATANABE
//	Data  : 2018/02/10
//
//--------------------------------------------------------------------------------
#include "texManager.h"
#include "manager.h"
#include "modeGame.h"
#include "scene2DPx.h"
#include "deferredRendering.h"
#include "renderer.h"

//--------------------------------------------------------------------------------
// �I��
//--------------------------------------------------------------------------------
CScene2DPx::~CScene2DPx()
{
	// �X���b�h�Z�[�t(���b�N)
#ifdef THREAD_ON
	std::unique_lock<std::recursive_mutex> locker = CManager::Locker();
#endif
	// vtx�o�b�t�@�̉��
	if (m_pVtxBuffer != nullptr)
	{
		m_pVtxBuffer->Release();
		m_pVtxBuffer = nullptr;
	}
}

//--------------------------------------------------------------------------------
// ������
//--------------------------------------------------------------------------------
void CScene2DPx::Init()
{
	CScene2D::Init();
	// �ϐ��̏�����
	SetPos(D3DXVECTOR3(0.f, 0.f, 0.f));		// ���W
	SetSize(D3DXVECTOR3(1.f, 1.f, 1.f));	// �T�C�Y
	SetRot(D3DXVECTOR3(0.f, 0.f, 0.f));		// ��]��
	SetScl(D3DXVECTOR3(1.f, 1.f, 1.f));		// �g�嗦
	m_posUV = D3DXVECTOR2(0.f, 0.f);
	m_widthUV = 1.f;
	m_heightUV = 1.f;
}

//--------------------------------------------------------------------------------
// �I��
//--------------------------------------------------------------------------------
void CScene2DPx::Uninit()
{
	CScene2D::Uninit();
}

//--------------------------------------------------------------------------------
// �X�V
//--------------------------------------------------------------------------------
void CScene2DPx::Update()
{
	CScene2D::Update();
}

//--------------------------------------------------------------------------------
// �`��
//--------------------------------------------------------------------------------
void CScene2DPx::Draw()
{
	// �X���b�h�Z�[�t(���b�N)
#ifdef THREAD_ON
	std::unique_lock<std::recursive_mutex> locker = CManager::Locker();
#endif

	// D3Device�̎擾
	LPDIRECT3DDEVICE9 pDevice = CRenderer::GetDevice();

	// ���_�t�H�[�}�b�g�̐ݒ�
	pDevice->SetFVF(FVF_VERTEX_2D);

	// ���_�o�b�t�@���f�o�C�X�̃f�[�^ �X�g���[���Ƀo�C���h����
	pDevice->SetStreamSource(0, m_pVtxBuffer, 0, sizeof(VERTEX_2D));

	// �|���S���̕`��
	pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, 0, NUM_POLYGON);
}

//--------------------------------------------------------------------------------
// �쐬����
//--------------------------------------------------------------------------------
CScene2DPx *CScene2DPx::Create(const D3DXVECTOR3 &pos, const D3DXVECTOR3 &rot, const D3DXVECTOR3 &size, const std::string &strFilePass, int priority, CTexManager *pUseTexManager)
{
	// �X���b�h�Z�[�t(���b�N)
#ifdef THREAD_ON
	std::unique_lock<std::recursive_mutex> locker = CManager::Locker();
#endif

	// CScene2DPx�̐���
	CScene2DPx *pScene2D;
	pScene2D = new CScene2DPx(priority);

	pScene2D->Init();

	pScene2D->SetPos(pos);
	pScene2D->SetRot(rot);
	pScene2D->SetSize(size);
	pScene2D->m_strFilePass = strFilePass; 
	pScene2D->SetUseTexManager(pUseTexManager);

	// D3Device�̎擾
	LPDIRECT3DDEVICE9 pDevice = CRenderer::GetDevice();

	// ���_���̍쐬
	//---------------------
	if (FAILED(pDevice->CreateVertexBuffer(
		sizeof(VERTEX_2D) * NUM_VERTEX,		// �������̃T�C�Y
		D3DUSAGE_WRITEONLY,					// �g�p���@(�������݂̂�)
		FVF_VERTEX_2D,						// �g�p���钸�_�t�H�[�}�b�g(���Ȃ�)
		D3DPOOL_MANAGED,					// �o�b�t�@�̊Ǘ����@(����ɕ������Ă����)
		&pScene2D->m_pVtxBuffer,			// 
		nullptr)))
	{
		// ���s������G���[��Ԃ�
		MessageBox(nullptr, "2D�pVtxBuffer�̍쐬�Ɏ��s���܂���", "�x��", MB_OK);
		return nullptr;
	}

	return pScene2D;
}
