//================================================================================
//
//	Auter : KENSUKE WATANABE
//	Data  : 2018/02/10
//
//--------------------------------------------------------------------------------
#include "main.h"
#include "manager.h"
#include "texManager.h"
#include "renderer.h"
#include "scene2D.h"
#include "input.h"
#include "number.h"

//--------------------------------------------------------------------------------
// �萔��`
//--------------------------------------------------------------------------------
#define TEXNAME_NUMBER "data/TEXTURE/number000.png"	// �i���o�[�̃e�N�X�`��

//--------------------------------------------------------------------------------
// CNumber::�R���X�g���N�^
//--------------------------------------------------------------------------------
CNumber::CNumber()
{
	m_pVtxBuffer = nullptr;
}

//--------------------------------------------------------------------------------
// CEnemy::�f�X�g���N�^
//--------------------------------------------------------------------------------
CNumber::~CNumber()
{

}

//--------------------------------------------------------------------------------
// CNumber::����������
//--------------------------------------------------------------------------------
HRESULT CNumber::Init(D3DXVECTOR3 pos, float rot, D3DXVECTOR3 size)
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

	m_pos = pos;
	m_size = size;
	m_rot = rot;

	// �X���b�h�Z�[�t(���b�N)
#ifdef THREAD_ON
	std::unique_lock<std::recursive_mutex> locker = CManager::Locker();
#endif

	// ���_����ݒ�
	VERTEX_2D* pVtx;	//���_��񏑂����ݐ�̃|�C���^�ϐ�
	// �o�b�t�@�����b�N����
	m_pVtxBuffer->Lock(0, 0, (void**)&pVtx, 0);
	
	// ���_���W
	pVtx[0].pos = D3DXVECTOR3(m_pos.x, m_pos.y, 0.0f);
	pVtx[1].pos = D3DXVECTOR3(m_pos.x + m_size.x, m_pos.y, 0.0f);
	pVtx[2].pos = D3DXVECTOR3(m_pos.x, m_pos.y + m_size.y, 0.0f);
	pVtx[3].pos = D3DXVECTOR3(m_pos.x + m_size.x, m_pos.y + m_size.y, 0.0f);

	// UV
	pVtx[0].tex = D3DXVECTOR2(0.0f, 0.0f);
	pVtx[1].tex = D3DXVECTOR2(0.1f, 0.0f);
	pVtx[2].tex = D3DXVECTOR2(0.0f, 1.0f);
	pVtx[3].tex = D3DXVECTOR2(0.1f, 1.0f);

	// �F
	pVtx[0].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
	pVtx[1].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
	pVtx[2].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
	pVtx[3].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);

	// �`��D��x
	pVtx[0].rhw = 255;
	pVtx[1].rhw = 255;
	pVtx[2].rhw = 255;
	pVtx[3].rhw = 255;

	//���b�N�̉���
	m_pVtxBuffer->Unlock();
	return S_OK;
}

//--------------------------------------------------------------------------------
// CNumber::�I������
//--------------------------------------------------------------------------------
void CNumber::Uninit()
{
	// ���_�o�b�t�@�̔j��
	if(m_pVtxBuffer != nullptr)
	{
		m_pVtxBuffer->Release();
		m_pVtxBuffer = nullptr;
	}
}

//--------------------------------------------------------------------------------
// CNumber::�X�V����
//--------------------------------------------------------------------------------
void CNumber::Update()
{
	// �X���b�h�Z�[�t(���b�N)
#ifdef THREAD_ON
	std::unique_lock<std::recursive_mutex> locker = CManager::Locker();
#endif

	// ���_����ݒ�
	VERTEX_2D* pVtx;	//���_��񏑂����ݐ�̃|�C���^�ϐ�
	// �o�b�t�@�����b�N����
	m_pVtxBuffer->Lock(0, 0, (void**)&pVtx, 0);

	//���_�F�̐ݒ�
	pVtx[0].col = D3DCOLOR_RGBA(m_r, m_g, m_b, m_a);
	pVtx[1].col = D3DCOLOR_RGBA(m_r, m_g, m_b, m_a);
	pVtx[2].col = D3DCOLOR_RGBA(m_r, m_g, m_b, m_a);
	pVtx[3].col = D3DCOLOR_RGBA(m_r, m_g, m_b, m_a);

	//���b�N�̉���
	m_pVtxBuffer->Unlock();
}

//--------------------------------------------------------------------------------
// CNumber::�`�揈��
//--------------------------------------------------------------------------------
void CNumber::Draw()
{
	// �f�o�C�X�̎擾
	LPDIRECT3DDEVICE9 pDevice = CManager::GetRenderer()->GetDevice();

	// ���_�o�b�t�@���f�[�^�X�g���[���ɐݒ�
	pDevice->SetStreamSource(0, m_pVtxBuffer, 0, sizeof(VERTEX_2D));

	// ���_�t�H�[�}�b�g�̐ݒ�
	pDevice->SetFVF(FVF_VERTEX_2D);	//�f�o�C�X�ɒ��_�̐ݒ��������

	// �e�N�X�`���̐ݒ�
	pDevice->SetTexture(0, CManager::GetTexManager()->GetTexInterface(TEXNAME_NUMBER));

	// �|���S���̕`��
	pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, 0, NUM_POLYGON);
}

//--------------------------------------------------------------------------------
// CNumber::�쐬����
//--------------------------------------------------------------------------------
CNumber *CNumber::Create(D3DXVECTOR3 pos, float rot, D3DXVECTOR3 size)
{
	// ����
	CNumber *pNumber;
	pNumber = new CNumber;
	pNumber->Init(pos, rot, size);		// ������
	pNumber->SetNumber(0);
	pNumber->SetCol(255, 255, 255, 255);

	// �e�N�X�`���̐ݒ�
	CManager::GetTexManager()->SetTex(TEXNAME_NUMBER);

	return pNumber;
}

CNumber *CNumber::Create(D3DXVECTOR3 pos, D3DXVECTOR3 rot, D3DXVECTOR3 size, const int disit)
{
	// ����
	CNumber *pNumber = new CNumber[disit];
	for (int num = 0; num < disit; num++)
	{
		pNumber[num].Init(D3DXVECTOR3(pos.x + num * size.x, pos.y, 0.0f), rot.z, size);		// ������
		pNumber[num].SetNumber(0);
		pNumber[num].SetCol(255, 255, 255, 255);
	}

	// �e�N�X�`���̐ݒ�
	CManager::GetTexManager()->SetTex(TEXNAME_NUMBER);

	return pNumber;
}

//--------------------------------------------------------------------------------
// CNumber::�ݒ菈��
//--------------------------------------------------------------------------------
void CNumber::SetNumber(int nNumber)
{
	// �X���b�h�Z�[�t(���b�N)
#ifdef THREAD_ON
	std::unique_lock<std::recursive_mutex> locker = CManager::Locker();
#endif

	VERTEX_2D* pVtx;	// �o�b�t�@�����b�N����
	m_pVtxBuffer->Lock(0, 0, (void**)&pVtx, 0);

	// �e�N�X�`����uv�l�̐ݒ�
	pVtx[0].tex = D3DXVECTOR2(0.1f * nNumber, 0.0f);
	pVtx[1].tex = D3DXVECTOR2(0.1f * nNumber + 0.1f, 0.0f);
	pVtx[2].tex = D3DXVECTOR2(0.1f * nNumber, 1.0f);
	pVtx[3].tex = D3DXVECTOR2(0.1f * nNumber + 0.1f, 1.0f);
	// �|���S���̍��W�̐ݒ�
	pVtx[0].pos = D3DXVECTOR3(m_pos.x, m_pos.y, 0.0f);
	pVtx[1].pos = D3DXVECTOR3(m_pos.x + m_size.x, m_pos.y, 0.0f);
	pVtx[2].pos = D3DXVECTOR3(m_pos.x, m_pos.y + m_size.y, 0.0f);
	pVtx[3].pos = D3DXVECTOR3(m_pos.x + m_size.x, m_pos.y + m_size.y, 0.0f);

	//���_�o�b�t�@�A�����b�N
	m_pVtxBuffer->Unlock();
}
