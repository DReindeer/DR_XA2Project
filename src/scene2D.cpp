#include "texManager.h"
#include "manager.h"
#include "modeGame.h"
#include "scene2D.h"
#include "input.h"
#include "renderer.h"

//--------------------------------------------------------------------------------
// �f�X�g���N�^
//--------------------------------------------------------------------------------
CScene2D::~CScene2D()
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
void CScene2D::Init()
{
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
void CScene2D::Uninit()
{

}

//--------------------------------------------------------------------------------
// �X�V
//--------------------------------------------------------------------------------
void CScene2D::Update()
{
	// �X���b�h�Z�[�t(���b�N)
#ifdef THREAD_ON
	std::unique_lock<std::recursive_mutex> locker = CManager::Locker();
#endif

	// ���z�A�h���X���擾���邽�߂̕ϐ�
	VERTEX_2D* pVtx;
	// �o�b�t�@�����b�N�����z�A�h���X���擾����(0�̏ꏊ����0��(�S��)���b�N)
	m_pVtxBuffer->Lock(0, 0, (void**)&pVtx, 0);

	D3DXVECTOR3 pos = GetPos();
	D3DXVECTOR3 size = GetSize();
	D3DXVECTOR3 rot = GetRot();
	float len = sqrt(size.x * size.x + size.y * size.y);
	float ang = atan2(size.y, size.x);
	pVtx[0].pos = D3DXVECTOR3(cosf(ang + D3DX_PI + rot.z) * len + pos.x,
		sinf(ang + D3DX_PI + rot.z) * len + pos.y, 0.f);
	pVtx[1].pos = D3DXVECTOR3(cosf(-ang + rot.z) * len + pos.x,
		sinf(-ang + rot.z) * len + pos.y, 0.f);
	pVtx[2].pos = D3DXVECTOR3(cosf(-ang + D3DX_PI + rot.z) * len + pos.x,
		sinf(-ang + D3DX_PI + rot.z) * len + pos.y, 0.f);
	pVtx[3].pos = D3DXVECTOR3(cosf(ang + rot.z) * len + pos.x,
		sinf(ang + rot.z) * len + pos.y, 0.f);

	// UV���W�̐ݒ�
	pVtx[0].tex = D3DXVECTOR2(m_posUV.x, m_posUV.y);
	pVtx[1].tex = D3DXVECTOR2(m_posUV.x + m_widthUV, m_posUV.y);
	pVtx[2].tex = D3DXVECTOR2(m_posUV.x, m_posUV.y + m_heightUV);
	pVtx[3].tex = D3DXVECTOR2(m_posUV.x + m_widthUV, m_posUV.y + m_heightUV);

	// ���_�F�̐ݒ�
	pVtx[0].col = D3DCOLOR_RGBA(m_r, m_g, m_b, m_a);
	pVtx[1].col = D3DCOLOR_RGBA(m_r, m_g, m_b, m_a);
	pVtx[2].col = D3DCOLOR_RGBA(m_r, m_g, m_b, m_a);
	pVtx[3].col = D3DCOLOR_RGBA(m_r, m_g, m_b, m_a);

	// rhw�̐ݒ�
	pVtx[0].rhw = 1.f;
	pVtx[1].rhw = 1.f;
	pVtx[2].rhw = 1.f;
	pVtx[3].rhw = 1.f;

	// �ݒ��o�b�t�@�̃A�����b�N
	m_pVtxBuffer->Unlock();
}

//--------------------------------------------------------------------------------
// �`��
//--------------------------------------------------------------------------------
void CScene2D::Draw()
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

	// �e�N�X�`���̐ݒ�
	if (m_pUseTexManager)
	{
		pDevice->SetTexture(0, m_pUseTexManager->GetTexInterface(m_strFilePass));
	}
	else
	{
		pDevice->SetTexture(0, CManager::GetTexManager()->GetTexInterface(m_strFilePass));
	}

	// �|���S���̕`��
	pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, 0, NUM_POLYGON);
}

//--------------------------------------------------------------------------------
// �쐬����
//--------------------------------------------------------------------------------
CScene2D *CScene2D::Create(const D3DXVECTOR3 &pos, const D3DXVECTOR3 &rot, const D3DXVECTOR3 &size, const std::string &strFilePass, int priority, CTexManager *pUseTexManager)
{
	// CScene2D�̐���
	CScene2D *pScene2D;
	pScene2D = new CScene2D(priority);

	pScene2D->Init();

	pScene2D->SetPos(pos);
	pScene2D->SetRot(rot);
	pScene2D->SetSize(size);
	pScene2D->m_strFilePass = strFilePass;

	pScene2D->m_pUseTexManager = pUseTexManager;

	// �X���b�h�Z�[�t(���b�N)
#ifdef THREAD_ON
	std::unique_lock<std::recursive_mutex> locker = CManager::Locker();
#endif

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

	// �e�N�X�`���̐ݒ�
	if (pScene2D->m_pUseTexManager)
	{
		pScene2D->m_pUseTexManager->SetTex(strFilePass);
	}
	else
	{
		CManager::GetTexManager()->SetTex(strFilePass);
	}
	// �e�N�X�`���̃Z�b�g

	return pScene2D;
}

void CScene2D::SetTexturePos(D3DXVECTOR2 uv, D3DXVECTOR2 length)
{
	m_posUV = uv;
	m_heightUV = length.y;
	m_widthUV = length.x;
}
