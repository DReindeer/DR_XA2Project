//================================================================================
//
//	Auter : KENSUKE WATANABE
//	Data  : 2018/02/10
//
//--------------------------------------------------------------------------------
#include "texManager.h"
#include "fontManager.h"
#include "manager.h"
#include "scene2D.h"
#include "sceneFont.h"
#include "input.h"
#include "renderer.h"

//--------------------------------------------------------------------------------
// ������
//--------------------------------------------------------------------------------
void CSceneFont::Init()
{
	SetPos(D3DXVECTOR3(0.0f, 0.0f, 0.0f));		// ���W
	SetRot(D3DXVECTOR3(0.0f, 0.0f, 0.0f));		// ��]��
	SetSize(D3DXVECTOR3(1.0f, 1.0f, 1.0f));		// �T�C�Y
	SetScl(D3DXVECTOR3(1.0f, 1.0f, 1.0f));		// �g�嗦
}

//--------------------------------------------------------------------------------
// �I��
//--------------------------------------------------------------------------------
void CSceneFont::Uninit()
{
	// vtx�o�b�t�@�̉��
	if (m_pVtxBuffer != nullptr)
	{
		m_pVtxBuffer->Release();
		m_pVtxBuffer = nullptr;
	}
}

//--------------------------------------------------------------------------------
// �X�V
//--------------------------------------------------------------------------------
void CSceneFont::Update()
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
	int r = GetR(), g = GetG(), b = GetB(), a = GetA();

	// rhw�̐ݒ�
	pVtx[0].rhw = 1.0f;
	pVtx[1].rhw = 1.0f;
	pVtx[2].rhw = 1.0f;
	pVtx[3].rhw = 1.0f;

	// ���_���W�̐ݒ�
	pVtx[0].pos = D3DXVECTOR3(pos.x, pos.y - m_fontRelativeVtxPos, 0.0f);
	pVtx[1].pos = D3DXVECTOR3(pos.x, pos.y - m_fontHeight - m_fontRelativeVtxPos, 0.0f);
	pVtx[2].pos = D3DXVECTOR3(m_fontWidth + pos.x, pos.y - m_fontRelativeVtxPos, 0.0f);
	pVtx[3].pos = D3DXVECTOR3(m_fontWidth + pos.x, pos.y - m_fontHeight - m_fontRelativeVtxPos, 0.0f);

	// ���_�F�̐ݒ�
	pVtx[0].col = D3DCOLOR_RGBA(r, g, b, a);
	pVtx[1].col = D3DCOLOR_RGBA(r, g, b, a);
	pVtx[2].col = D3DCOLOR_RGBA(r, g, b, a);
	pVtx[3].col = D3DCOLOR_RGBA(r, g, b, a);

	// UV�̐ݒ�
	pVtx[0].tex = D3DXVECTOR2(0.f, 0.f);
	pVtx[1].tex = D3DXVECTOR2(1.f, 0.f);
	pVtx[2].tex = D3DXVECTOR2(0.f, 1.f);
	pVtx[3].tex = D3DXVECTOR2(1.f, 1.f);

	// �ݒ��o�b�t�@�̃A�����b�N
	m_pVtxBuffer->Unlock();
}

//--------------------------------------------------------------------------------
// �`��
//--------------------------------------------------------------------------------
void CSceneFont::Draw()
{
	// �f�o�C�X�̎擾
	LPDIRECT3DDEVICE9 dev = CManager::GetRenderer()->GetDevice();

	// ���u�����h�ݒ�
	dev->SetRenderState(D3DRS_ALPHABLENDENABLE, TRUE);
	dev->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
	dev->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA);
	dev->SetSamplerState(0, D3DSAMP_ADDRESSU, D3DTADDRESS_CLAMP);	// �e�N�X�`���t�l�̌J��Ԃ��ݒ�
	dev->SetSamplerState(0, D3DSAMP_ADDRESSV, D3DTADDRESS_CLAMP);	// �e�N�X�`���u�l�̌J��Ԃ��ݒ�

	// ���_�o�b�t�@���f�o�C�X�̃f�[�^ �X�g���[���Ƀo�C���h����
	dev->SetStreamSource(0, m_pVtxBuffer, 0, sizeof(VERTEX_2D));

	// ���_�t�H�[�}�b�g�̐ݒ�
	dev->SetFVF(FVF_VERTEX_2D);

	// �e�N�X�`���̐ݒ�
	dev->SetTexture(0, CManager::GetFontManager()->GetFont(m_strFilePass)->pTex);

	// �|���S���̕`�� // �|���S���̌`, ���[�h����ŏ��̒��_�̃C���f�b�N�X, �|���S����
	dev->DrawPrimitive(D3DPT_TRIANGLESTRIP, 0, NUM_POLYGON);

	dev->SetSamplerState(0, D3DSAMP_ADDRESSU, D3DTADDRESS_WRAP);	// �e�N�X�`���t�l�̌J��Ԃ��ݒ�
	dev->SetSamplerState(0, D3DSAMP_ADDRESSV, D3DTADDRESS_WRAP);	// �e�N�X�`���u�l�̌J��Ԃ��ݒ�
	dev->SetRenderState(D3DRS_ALPHATESTENABLE, FALSE);
}

//--------------------------------------------------------------------------------
// �쐬����
//--------------------------------------------------------------------------------
CSceneFont *CSceneFont::Create(const D3DXVECTOR3 &pos, const D3DXVECTOR3 &rot, const float &height, const int &weight, const wchar_t *wcFont)
{
	// CSceneFont�̐���
	CSceneFont *pSceneFont;
	pSceneFont = new CSceneFont;
	pSceneFont->Init();

	// �e�N�X�`������&�Z�b�g
	CFontManager::FONT *pFont = CFontManager::SetFontTex(weight, wcFont);
	if (pFont)
	{
		pSceneFont->m_gm = pFont->gm;
		pSceneFont->m_fontWidth = pFont->fontWidth;
		pSceneFont->m_fontHeight = pFont->fontHeight;
		pSceneFont->m_fontRelativeVtxPos = pFont->relativeVtxPos;
	}

	// �|���S���̒��_�̐ݒ�
	pSceneFont->SetPos(D3DXVECTOR3(pos.x, pos.y + SCREEN_HEIGHT, 0.0f));
	pSceneFont->SetRot(rot);
	pSceneFont->SetSize(D3DXVECTOR3(height, height, 0.0f));

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
		&pSceneFont->m_pVtxBuffer,							// 
		nullptr)))
	{
		// ���s������G���[��Ԃ�
		MessageBox(nullptr, "2D�pVtxBuffer�̍쐬�Ɏ��s���܂���", "�x��", MB_OK);
		return nullptr;
	}

	setlocale(LC_CTYPE, "jpn");
	char ms[256];
	size_t size;
	wcstombs_s(&size, ms, (size_t)256, wcFont , (size_t)256);
	pSceneFont->m_strFilePass = ms;

	return pSceneFont;
}
