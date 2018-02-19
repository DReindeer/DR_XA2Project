//================================================================================
//
//	Auter : KENSUKE WATANABE
//	Data  : 2018/02/10
//
//--------------------------------------------------------------------------------
#include "main.h"
#include "renderer.h"
#include "scene.h"
#include "manager.h"
#include "camera.h"
#include "inputKeyboard.h"
#include "debugStr.h"
#include "texManager.h"

#include "scene2D.h"
#include "mainController.h"

//================================================================================
// �ÓI�����o�ϐ�
//--------------------------------------------------------------------------------
LPDIRECT3DDEVICE9 CRenderer::m_pD3DDevice = nullptr;

//================================================================================
// CFeedbackBlur::�R���X�g���N�^
//--------------------------------------------------------------------------------
CFeedbackBlur::CFeedbackBlur()
{
	for (int i = 0; i < 2; i++)
	{
		m_renderTargetTexture[i] = nullptr;		// �e�N�X�`��
		m_renderTargetSurface[i] = nullptr;		// �T�[�t�F�X
		m_renderTarget2D[i] = nullptr;
	}
	m_renderTargetZBuffer = nullptr;

	// �e�N�X�`���}�l�[�W���̐���
	m_pTexManager = new CTexManager;
	m_pTexManager->Init();
}

//================================================================================
// CFeedbackBlur::�f�X�g���N�^
//--------------------------------------------------------------------------------
CFeedbackBlur::~CFeedbackBlur()
{
	for (int i = 0; i < 2; i++)
	{
		// �T�[�t�F�X
		if (m_renderTargetSurface[i] != nullptr)
		{
			m_renderTargetSurface[i]->Release();
			m_renderTargetSurface[i] = nullptr;
		}
	}

	// Z�o�b�t�@�i�[�p�T�[�t�F�X
	if (m_renderTargetZBuffer != nullptr)
	{
		m_renderTargetZBuffer->Release();
		m_renderTargetZBuffer = nullptr;
	}

	// �e�N�X�`���}�l�[�W��
	if (m_pTexManager != nullptr)
	{
		m_pTexManager->Uninit();
		delete m_pTexManager;
		m_pTexManager = nullptr;
	}
}

//================================================================================
// CRenderer::�R���X�g���N�^
//--------------------------------------------------------------------------------
CRenderer::CRenderer() : m_screenSize(D3DXVECTOR2(SCREEN_WIDTH, SCREEN_HEIGHT)), m_HWnd(0)
{
	// �l�̃N���A
	m_pD3D = nullptr;			// Direct3D�I�u�W�F�N�g
	m_pD3DDevice = nullptr;		// Device�I�u�W�F�N�g(�`��ɕK�v)

	m_pRenderTarget = nullptr;
	m_BackBufferSurface = nullptr;			// �o�b�N�o�b�t�@
	m_blur = 0.f;
}

//================================================================================
// ����������
//================================================================================
HRESULT CRenderer::Init(HWND hWnd, bool bWindow)
{
	m_HWnd = hWnd;

	// Direct3D�I�u�W�F�N�g�̍쐬
	m_pD3D = Direct3DCreate9(D3D_SDK_VERSION);
	if (m_pD3D == nullptr)
	{
		return E_FAIL;
	}

	// ���݂̃f�B�X�v���C���[�h���擾
	D3DDISPLAYMODE d3ddm;
	if (FAILED(m_pD3D->GetAdapterDisplayMode(D3DADAPTER_DEFAULT, &d3ddm)))
	{
		return E_FAIL;
	}

	// (�E�B���h�E�쐬��)�𑜓x���㏈��
	m_screenSize * 1.f;
	int screenX = static_cast<int>(m_screenSize.x);
	int screenY = static_cast<int>(m_screenSize.y);

	// �f�o�C�X�̃v���[���e�[�V�����p�����[�^�̐ݒ�
	D3DPRESENT_PARAMETERS d3dpp;
	ZeroMemory(&d3dpp, sizeof(d3dpp));								// ���[�N���[���N���A
	d3dpp.BackBufferCount = 1;										// �o�b�N�o�b�t�@�̐�
	d3dpp.BackBufferWidth = screenX;								// �Q�[����ʃT�C�Y(��)
	d3dpp.BackBufferHeight = screenY;								// �Q�[����ʃT�C�Y(����)
	d3dpp.BackBufferFormat = d3ddm.Format;							// �J���[���[�h�̎w��
	d3dpp.SwapEffect = D3DSWAPEFFECT_DISCARD;						// �f���M���ɓ������ăt���b�v����
	d3dpp.EnableAutoDepthStencil = TRUE;							// �f�v�X�o�b�t�@�i�y�o�b�t�@�j�ƃX�e���V���o�b�t�@���쐬
	d3dpp.AutoDepthStencilFormat = D3DFMT_D24S8;					// �f�v�X�o�b�t�@�Ƃ���24bit���g��
	d3dpp.Windowed = bWindow;										// �E�B���h�E���[�h
	d3dpp.FullScreen_RefreshRateInHz = D3DPRESENT_RATE_DEFAULT;		// ���t���b�V�����[�g
	d3dpp.PresentationInterval = D3DPRESENT_INTERVAL_DEFAULT;		// �C���^�[�o��

	// �f�B�X�v���C�A�_�v�^��\�����߂̃f�o�C�X���쐬
	// �`��ƒ��_�������n�[�h�E�F�A�ōs�Ȃ�
	if (FAILED(m_pD3D->CreateDevice(D3DADAPTER_DEFAULT,
		D3DDEVTYPE_HAL,
		hWnd,
		D3DCREATE_HARDWARE_VERTEXPROCESSING | D3DCREATE_MULTITHREADED,
		&d3dpp, &m_pD3DDevice)))
	{
		// ��L�̐ݒ肪���s������
		// �`����n�[�h�E�F�A�ōs���A���_������CPU�ōs�Ȃ�
		if (FAILED(m_pD3D->CreateDevice(D3DADAPTER_DEFAULT,
			D3DDEVTYPE_HAL,
			hWnd,
			D3DCREATE_SOFTWARE_VERTEXPROCESSING | D3DCREATE_MULTITHREADED,
			&d3dpp, &m_pD3DDevice)))
		{
			// ��L�̐ݒ肪���s������
			// �`��ƒ��_������CPU�ōs�Ȃ�
			if (FAILED(m_pD3D->CreateDevice(D3DADAPTER_DEFAULT,
				D3DDEVTYPE_REF, hWnd,
				D3DCREATE_SOFTWARE_VERTEXPROCESSING | D3DCREATE_MULTITHREADED,
				&d3dpp, &m_pD3DDevice)))
			{
				// �������s
				return E_FAIL;
			}
		}
	}

	// �����_�[�X�e�[�g�̐ݒ�
	m_pD3DDevice->SetRenderState(D3DRS_CULLMODE, D3DCULL_CCW);				// �J�����O���s��Ȃ�
	m_pD3DDevice->SetRenderState(D3DRS_ZENABLE, TRUE);						// Z�o�b�t�@���g�p
	m_pD3DDevice->SetRenderState(D3DRS_ALPHABLENDENABLE, TRUE);				// ���u�����h���s��
	m_pD3DDevice->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);		// ���\�[�X�J���[�̎w��
	m_pD3DDevice->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA);	// ���f�X�e�B�l�[�V�����J���[�̎w��

	// �T���v���[�X�e�[�g�̐ݒ�
	m_pD3DDevice->SetSamplerState(0, D3DSAMP_ADDRESSU, D3DTADDRESS_WRAP);	// �e�N�X�`���t�l�̌J��Ԃ��ݒ�
	m_pD3DDevice->SetSamplerState(0, D3DSAMP_ADDRESSV, D3DTADDRESS_WRAP);	// �e�N�X�`���u�l�̌J��Ԃ��ݒ�
	m_pD3DDevice->SetSamplerState(0, D3DSAMP_MINFILTER, D3DTEXF_LINEAR);	// �e�N�X�`���g�厞�̕�Ԑݒ�
	m_pD3DDevice->SetSamplerState(0, D3DSAMP_MAGFILTER, D3DTEXF_LINEAR);	// �e�N�X�`���k�����̕�Ԑݒ�

	// �e�N�X�`���X�e�[�W�X�e�[�g�̐ݒ�
	m_pD3DDevice->SetTextureStageState(0, D3DTSS_ALPHAOP, D3DTOP_MODULATE);	// �A���t�@�u�����f�B���O����(�����l��D3DTOP_SELECTARG1)
	m_pD3DDevice->SetTextureStageState(0, D3DTSS_ALPHAARG1, D3DTA_TEXTURE);	// �ŏ��̃A���t�@����(�����l��D3DTA_TEXTURE�A�e�N�X�`�����Ȃ��ꍇ��D3DTA_DIFFUSE)
	m_pD3DDevice->SetTextureStageState(0, D3DTSS_ALPHAARG2, D3DTA_DIFFUSE);	// �Q�Ԗڂ̃A���t�@����(�����l��D3DTA_DIFFUSE)
	
//	// �A���t�@�e�X�g�̗L����
//	m_pD3DDevice->SetRenderState(D3DRS_ALPHATESTENABLE, TRUE);
//	m_pD3DDevice->SetRenderState(D3DRS_ALPHAFUNC, D3DCMP_GREATEREQUAL);
//	// �s�����ɂ���l�̐ݒ�
//	m_pD3DDevice->SetRenderState(D3DRS_ALPHAREF, (DWORD)0x00000001);

	// �����_�[�^�[�Q�b�g�̐ݒ�
	m_pRenderTarget = new CFeedbackBlur;

	CTexManager *pTexManager = m_pRenderTarget->GetTexManager();
	LPDIRECT3DTEXTURE9 tex0;
	LPDIRECT3DTEXTURE9 tex1;

	// ��̃e�N�X�`���̐���(�o�b�N�o�b�t�@�Ɠ����T�C�Y...�łȂ��ƁAZ�o�b�t�@�����L�o���Ȃ�)
	D3DXCreateTexture(m_pD3DDevice, screenX, screenY, 1, D3DUSAGE_RENDERTARGET, D3DFMT_A8R8G8B8, D3DPOOL_DEFAULT, &tex0);				
	D3DXCreateTexture(m_pD3DDevice, screenX, screenY, 1, D3DUSAGE_RENDERTARGET, D3DFMT_A8R8G8B8, D3DPOOL_DEFAULT, &tex1); 
	m_pRenderTarget->SetRenderTargetTexture(&tex0, 0);
	m_pRenderTarget->SetRenderTargetTexture(&tex1, 1);
	pTexManager->SetTex("TEX_RenderTarget", tex0);
	pTexManager->SetTex("TEX_RenderTarget2", tex1);
	tex0->GetSurfaceLevel(0, &m_pRenderTarget->GetRenderTargetSurface(0));		// �T�[�t�F�X�̎擾
	tex1->GetSurfaceLevel(0, &m_pRenderTarget->GetRenderTargetSurface(1));		// �T�[�t�F�X�̎擾
	m_pD3DDevice->GetRenderTarget(0, &m_BackBufferSurface);						// ���ݐݒ肳��Ă��郌���_�[�^�[�Q�b�g�擾

	// �f�v�X�E�X�e���V���T�[�t�F�X�̐���
	m_pD3DDevice->CreateDepthStencilSurface(screenX, screenY, D3DFMT_D24S8, D3DMULTISAMPLE_NONE, 0, FALSE, &m_pRenderTarget->GetRenderTargetZBuffer(), nullptr);
	m_pD3DDevice->GetDepthStencilSurface(&m_ZBuffer);
	
	CScene2D *pRenderTarget2D = CScene2D::Create(D3DXVECTOR3(screenX * 0.5f - 0.5f, screenY * 0.5f - 0.5f, 0.f), D3DXVECTOR3(0.f, 0.f, 0.f), D3DXVECTOR3(screenX * 0.5f, screenY * 0.5f, 0.f), "TEX_RenderTarget", 4, pTexManager);
	pRenderTarget2D->SetOverall();
	m_pRenderTarget->SetRenderTarget2D(pRenderTarget2D, 0);

	pRenderTarget2D = CScene2D::Create(D3DXVECTOR3(screenX * 0.5f - 0.5f, screenY * 0.5f - 0.5f, 0.f), D3DXVECTOR3(0.f, 0.f, 0.f), D3DXVECTOR3(screenX * 0.5f, screenY * 0.5f, 0.f), "TEX_RenderTarget2", 4, pTexManager);
	pRenderTarget2D->SetOverall();
	m_pRenderTarget->SetRenderTarget2D(pRenderTarget2D, 1);
	return S_OK;
}

//================================================================================
// �I������
//================================================================================
void CRenderer::Uninit()
{
	// �����_�[�^�[�Q�b�g�̔j��
	if (m_pRenderTarget != nullptr)
	{
		delete m_pRenderTarget;
		m_pRenderTarget = nullptr;
	}

	// �o�b�N�o�b�t�@�̔j��
	if (m_BackBufferSurface != nullptr)
	{
		m_BackBufferSurface->Release();
		m_BackBufferSurface = nullptr;
	}

	// Direct3D�I�u�W�F�N�g�̔j��
	if(m_pD3D != nullptr)
	{
		m_pD3D->Release();
		m_pD3D = nullptr;
	}

	// �f�o�C�X�̔j��
	if (m_pD3DDevice != nullptr)
	{
		m_pD3DDevice->Release();
		m_pD3DDevice = nullptr;
	}
}

//================================================================================
// �`��̐擪�ɍs������ 
//================================================================================
bool CRenderer::DrawBigin()
{
	// ��ʃT�C�Y�̍Ď擾
	RECT rect;
	GetClientRect(m_HWnd, &rect);
	m_screenSize = D3DXVECTOR2((FLOAT)rect.right, (FLOAT)rect.bottom);

	m_pD3DDevice->SetDepthStencilSurface(m_BackBufferSurface);
	m_pD3DDevice->SetRenderTarget(0, m_pRenderTarget->GetRenderTargetSurface(1));
	CScene2D *pPolygon2D0 = m_pRenderTarget->GetRenderTarget2D(0);
	CScene2D *pPolygon2D1 = m_pRenderTarget->GetRenderTarget2D(1);
	pPolygon2D0->SetDrawFlag(false);
	pPolygon2D1->SetDrawFlag(false);
	pPolygon2D0->Update();
	pPolygon2D1->Update();

	// �o�b�N�o�b�t�@���y�o�b�t�@�̃N���A
	m_pD3DDevice->Clear(0, NULL, (D3DCLEAR_TARGET | D3DCLEAR_ZBUFFER | D3DCLEAR_STENCIL), D3DCOLOR_RGBA(255, 255, 255, 255), 1.f, 0);
	
	// ���C���[�t���[���̐؂�ւ�(�؂�ւ����Ɏg�p����{�^��)
//	DrawWireframe(DIK_1);

	return true;
}

//================================================================================
// �`��̌�ɍs������ 
//================================================================================
void CRenderer::DrawEnd()
{
	LPDIRECT3DTEXTURE9 tex0 = m_pRenderTarget->GetRenderTargetTexture(0);
	LPDIRECT3DTEXTURE9 tex1 = m_pRenderTarget->GetRenderTargetTexture(1);
	CScene2D *pPolygon2D0 = m_pRenderTarget->GetRenderTarget2D(0);
	CScene2D *pPolygon2D1 = m_pRenderTarget->GetRenderTarget2D(1);
	if (m_blur > 0.f)
	{
		m_pD3DDevice->SetTexture(0, tex0);
		//pPolygon2D0->SetCol(255, 255, 255, 255 * (1 - m_blur));
		//pPolygon2D0->SetScl(1.f + m_blur * 0.05f);
		//pPolygon2D0->SetDrawFlag(true);
		//pPolygon2D0->Draw();
	}
	else
	{
		pPolygon2D0->SetCol(255, 255, 255, 255);
		pPolygon2D0->SetScl(1.f);
	}

	// �����_�[�^�[�Q�b�g�̐؂�ւ�
	m_pD3DDevice->SetRenderTarget(0, m_BackBufferSurface);

	// �o�b�N�o�b�t�@���y�o�b�t�@�̃N���A
	m_pD3DDevice->Clear(0, NULL, (D3DCLEAR_TARGET | D3DCLEAR_ZBUFFER | D3DCLEAR_STENCIL), D3DCOLOR_RGBA(255, 255, 255, 255), 1.f, 0);

	m_pD3DDevice->SetTexture(0, tex1);
	
	pPolygon2D1->SetDrawFlag(true);
	pPolygon2D1->Draw();

	// ����ւ�
	LPDIRECT3DTEXTURE9 &tex = tex0;
	LPDIRECT3DSURFACE9 sfc = m_pRenderTarget->GetRenderTargetSurface(0);
	m_pRenderTarget->SetRenderTargetTexture(&m_pRenderTarget->GetRenderTargetTexture(1), 0);
	m_pRenderTarget->SetRenderTargetSurface(&m_pRenderTarget->GetRenderTargetSurface(1), 0);
	m_pRenderTarget->SetRenderTargetTexture(&tex, 1);
	m_pRenderTarget->SetRenderTargetSurface(&sfc, 1);

	CScene2D *pPolygon2D = m_pRenderTarget->GetRenderTarget2D(0);
	m_pRenderTarget->SetRenderTarget2D(m_pRenderTarget->GetRenderTarget2D(1), 0);
	m_pRenderTarget->SetRenderTarget2D(pPolygon2D, 0);
}

//================================================================================
// D3DDevice�̎擾
//================================================================================
LPDIRECT3DDEVICE9 &CRenderer::GetDevice()
{
	return m_pD3DDevice;
}

//================================================================================
// ���C���[�t���[���̐؂�ւ�
//================================================================================
void CRenderer::DrawWireframe(unsigned int key)
{
	// �L�[�{�[�h�̎擾
	CInputKeyboard *keyboard = CManager::GetKeyboard();
	if (!keyboard)return;
	// �`����@�̐؂�ւ�
	if (keyboard->GetPress(key))
	{
#ifdef _DEBUG
		m_pD3DDevice->SetRenderState(D3DRS_FILLMODE, D3DFILL_WIREFRAME);
#endif
	}
	else {
		m_pD3DDevice->SetRenderState(D3DRS_FILLMODE, D3DFILL_SOLID);
	}
}