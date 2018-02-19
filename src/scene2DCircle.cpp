//================================================================================
//
//	Auter : KENSUKE WATANABE
//	Data  : 2018/02/10
//
//--------------------------------------------------------------------------------
#include "texManager.h"
#include "manager.h"
#include "modeGame.h"
#include "scene2DCircle.h"
#include "input.h"
#include "renderer.h"

#include <assert.h>

//--------------------------------------------------------------------------------
// ������
//--------------------------------------------------------------------------------
void CScene2DCircle::Init()
{
	// �ϐ��̏�����
	m_pVtxBuffer = nullptr;
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
}

//--------------------------------------------------------------------------------
// �I��
//--------------------------------------------------------------------------------
void CScene2DCircle::Uninit()
{
	// vtx�o�b�t�@�̉��
	if(m_pVtxBuffer != nullptr)
	{
		m_pVtxBuffer->Release();
		m_pVtxBuffer = nullptr;
	}
}

//--------------------------------------------------------------------------------
// �X�V
//--------------------------------------------------------------------------------
void CScene2DCircle::Update()
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
	float ang = D3DX_PI * 2 / (m_numOutVtx);

	pVtx[0].pos = D3DXVECTOR3(pos.x, pos.y, 0.f);	// ���_���W�̐ݒ�
	pVtx[0].tex = m_posUV;							// UV���W�̐ݒ�
	for (int i = 0; i < m_numOutVtx + 1; i++)
	{
		// ���_���W�̐ݒ�
		pVtx[i + 1].pos = D3DXVECTOR3(cosf((ang * i) + rot.z) * m_len + pos.x,
			sinf((ang * i) + rot.z) * m_len + pos.y, 0.f);
		// UV���W�̐ݒ�
		pVtx[i + 1].tex = D3DXVECTOR2(cosf((ang * i) + m_rotUV) * m_lenUV + m_posUV.x,
			sinf((ang * i) + m_rotUV) * m_lenUV + m_posUV.y);
	}
	int r = GetR(); int g = GetB(); int b = GetB(); int a = GetA();
	for (int i = 0; i < m_numOutVtx + 2; i++)
	{
		// rhw�̐ݒ�
		pVtx[i].rhw = 1.f;
		// ���_�F�̐ݒ�
		pVtx[i].col = D3DCOLOR_RGBA(r, g, b, a);
	}

	// �ݒ��o�b�t�@�̃A�����b�N
	m_pVtxBuffer->Unlock();
}

//--------------------------------------------------------------------------------
// �`��
//--------------------------------------------------------------------------------
void CScene2DCircle::Draw()
{
	// D3Device�̎擾
	LPDIRECT3DDEVICE9 pDevice = CRenderer::GetDevice();
	
	// ���_�o�b�t�@���f�o�C�X�̃f�[�^ �X�g���[���Ƀo�C���h����
	pDevice->SetStreamSource(0, m_pVtxBuffer, 0, sizeof(VERTEX_2D));

	// ���_�t�H�[�}�b�g�̐ݒ�
	pDevice->SetFVF(FVF_VERTEX_2D);

	// �e�N�X�`���̐ݒ�
	pDevice->SetTexture(0, CManager::GetTexManager()->GetTexInterface(m_strFilePass));

	// �|���S���̕`��
	pDevice->DrawPrimitive(D3DPT_TRIANGLEFAN, 0, m_numOutVtx);
}

//--------------------------------------------------------------------------------
// �쐬����
//--------------------------------------------------------------------------------
CScene2DCircle *CScene2DCircle::Create(const D3DXVECTOR3 &pos, const D3DXVECTOR3 &rot, float len, int numOutVtx, D3DXVECTOR2 posUV, float lenUV, const std::string &strFilePass, int priority)
{
	// CScene2DCircle�̐���
	CScene2DCircle *pScene2DCircle;
	pScene2DCircle = new CScene2DCircle(priority);
	pScene2DCircle->Init();

	pScene2DCircle->SetPos(pos);
	pScene2DCircle->SetRot(rot);
	pScene2DCircle->SetSize(D3DXVECTOR3(0.f, 0.f, 0.f));
	pScene2DCircle->m_posUV = posUV;
	pScene2DCircle->m_lenUV = lenUV;
	pScene2DCircle->m_len = len;
	pScene2DCircle->m_numOutVtx = max(16, numOutVtx);	// �Œ�16
	pScene2DCircle->m_strFilePass = strFilePass;

	// �X���b�h�Z�[�t(���b�N)
#ifdef THREAD_ON
	std::unique_lock<std::recursive_mutex> locker = CManager::Locker();
#endif

	// D3Device�̎擾
	LPDIRECT3DDEVICE9 pDevice = CManager::GetRenderer()->GetDevice();

	// ���_���̍쐬
	//---------------------
	if (FAILED(pDevice->CreateVertexBuffer(
		sizeof(VERTEX_2D) * pScene2DCircle->m_numOutVtx + 2,			// �������̃T�C�Y
		D3DUSAGE_WRITEONLY,					// �g�p���@(�������݂̂�)
		FVF_VERTEX_2D,						// �g�p���钸�_�t�H�[�}�b�g(���Ȃ�)
		D3DPOOL_MANAGED,					// �o�b�t�@�̊Ǘ����@(����ɕ������Ă����)
		&pScene2DCircle->m_pVtxBuffer,							// 
		nullptr)))
	{
		// ���s������G���[��Ԃ�
		MessageBox(nullptr, "2D�pVtxBuffer�̍쐬�Ɏ��s���܂���", "�x��", MB_OK);
		return nullptr;
	}
	
	// �e�N�X�`���̃Z�b�g
	CManager::GetTexManager()->SetTex(strFilePass);

	return pScene2DCircle;
}

void CScene2DCircle::SetTexturePos(D3DXVECTOR2 uv, D3DXVECTOR2 length)
{
	m_posUV = uv;
	m_heightUV = length.y;
	m_widthUV = length.x;
}