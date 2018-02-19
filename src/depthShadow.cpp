//================================================================================
//
//	Auter : KENSUKE WATANABE
//	Data  : 2018/02/10
//
//--------------------------------------------------------------------------------
#include "texManager.h"
#include "inputKeyboard.h"
#include "depthShadow.h"
#include "manager.h"
#include "renderer.h"
#include "camera.h"
#include "lightCamera.h"
#include "scene2D.h"
#include "utility.h"

//--------------------------------------------------------------------------------
// �f�X�g���N�^
//--------------------------------------------------------------------------------
DepthShadow::~DepthShadow()
{
	// �I������
	Uninit();
}

//--------------------------------------------------------------------------------
// ������
//--------------------------------------------------------------------------------
void DepthShadow::Init()
{
	// �ϐ��̏�����
	m_posUV = D3DXVECTOR2(0.f, 0.f);
	m_widthUV = 1.f;
	m_heightUV = 1.f;
}

//--------------------------------------------------------------------------------
// �I��
//--------------------------------------------------------------------------------
void DepthShadow::Uninit()
{
	// �e�N�X�`��
	if (m_depthShadowTexture)
	{
		m_depthShadowTexture->Release();
		m_depthShadowTexture = nullptr;
	}
	// �T�[�t�F�X
	if (m_depthShadowSurface)
	{
		m_depthShadowSurface->Release();
		m_depthShadowSurface = nullptr;
	}
	// �e�N�X�`��
	if (m_renderTargetZBuffer)
	{
		m_renderTargetZBuffer->Release();
		m_renderTargetZBuffer = nullptr;
	}

	// �V�F�[�_�[�̉��
	if (m_pDepthShadow)
	{
		// �s�N�Z���V�F�[�_�[�Œ�e�[�u��
		if (m_pDepthShadow->pPConstantTable)
		{
			m_pDepthShadow->pPConstantTable->Release();
			m_pDepthShadow->pPConstantTable = nullptr;
		}
		// �s�N�Z���V�F�[�_�[
		if (m_pDepthShadow->pPixelShader)
		{
			m_pDepthShadow->pPixelShader->Release();
			m_pDepthShadow->pPixelShader = nullptr;
		}
		// ���_�V�F�[�_�[�Œ�e�[�u��
		if (m_pDepthShadow->pVConstantTable)
		{
			m_pDepthShadow->pVConstantTable->Release();
			m_pDepthShadow->pVConstantTable = nullptr;
		}
		// ���_�V�F�[�_�[
		if (m_pDepthShadow->pVertexShader)
		{
			m_pDepthShadow->pVertexShader->Release();
			m_pDepthShadow->pVertexShader = nullptr;
		}
		// ���_�f�N�����[�V����
		if (m_pDepthShadow->pVertexDeclaration)
		{
			m_pDepthShadow->pVertexDeclaration->Release();
			m_pDepthShadow->pVertexDeclaration = nullptr;
		}

		delete m_pDepthShadow;
		m_pDepthShadow = nullptr;
	}
}

//--------------------------------------------------------------------------------
// �X�V
//--------------------------------------------------------------------------------
void DepthShadow::Update()
{
	// L�����ŕ\���̐؂�ւ�
//	if (CManager::GetKeyboard()->GetTrigger(DIK_L))
//	{
//		SwapDrawFlag();
//	}
	m_pDepthShadowPolygon->SetDrawFlag(false);
}

//--------------------------------------------------------------------------------
// �I�u�W�F�N�g�`�揀��
//--------------------------------------------------------------------------------
void DepthShadow::DrawBigin(D3DXMATRIX mtxWorld)
{
	// �X���b�h�Z�[�t(���b�N)
#ifdef THREAD_ON
	std::unique_lock<std::recursive_mutex> locker = CManager::Locker();
#endif

	// D3Device�̎擾
	LPDIRECT3DDEVICE9 pDevice = CRenderer::GetDevice();

	// ���C�g�̈ʒu�̃J�����̐ݒ�ɕύX
	CLightCamera *pLightCamera = CManager::GetLightCamera();
	pLightCamera->SetOrthoLH();

	// ���_�V�F�[�_�[�̒萔�o�b�t�@�p�ϐ�
	//--------------------------------------------------------------------------------
	// ���C�g�擾
//	CLightCamera *pLightCamera = CManager::GetLightCamera();
	D3DXMATRIX mtxView = pLightCamera->GetMtxView();
	D3DXMATRIX mtxProj = pLightCamera->GetMtxProj();
	D3DXMATRIX mtxLightWV = mtxWorld * mtxView;
	m_pDepthShadow->pVConstantTable->SetMatrix(pDevice, "mtxLightWV", &mtxLightWV);
	D3DXMATRIX mtxLightWVP = mtxWorld * mtxView * mtxProj;
	m_pDepthShadow->pVConstantTable->SetMatrix(pDevice, "mtxLightWVP", &mtxLightWVP);
	m_pDepthShadow->pVConstantTable->SetFloat(pDevice, "lightFar", pLightCamera->GetFar());

	// ���_�V�F�[�_�[�̐ݒ�
	//------------------------------------------------------------------------
	pDevice->SetVertexShader(m_pDepthShadow->pVertexShader);
	// �s�N�Z���V�F�[�_�[�̐ݒ�
	//------------------------------------------------------------------------
	pDevice->SetPixelShader(m_pDepthShadow->pPixelShader);
}

//--------------------------------------------------------------------------------
// �I�u�W�F�N�g�`��I��
//--------------------------------------------------------------------------------
void DepthShadow::DrawEnd()
{
	// �X���b�h�Z�[�t(���b�N)
#ifdef THREAD_ON
	std::unique_lock<std::recursive_mutex> locker = CManager::Locker();
#endif

	// D3Device�̎擾
	LPDIRECT3DDEVICE9 pDevice = CRenderer::GetDevice();

	pDevice->SetPixelShader(nullptr);
	pDevice->SetVertexShader(nullptr);
}

//--------------------------------------------------------------------------------
// �V���h�E�������ݗp�̃����_�[�^�[�Q�b�g�֐؂�ւ�
//--------------------------------------------------------------------------------
void DepthShadow::DrawDeviceBegin()
{
	// �X���b�h�Z�[�t(���b�N)
#ifdef THREAD_ON
	std::unique_lock<std::recursive_mutex> locker = CManager::Locker();
#endif

	// D3Device�̎擾
	LPDIRECT3DDEVICE9 pDevice = CRenderer::GetDevice();

	// Z�o�b�t�@�؂�ւ�
	pDevice->SetDepthStencilSurface(m_renderTargetZBuffer);

	// �T�[�t�F�X�؂�ւ�
	pDevice->SetRenderTarget(0, m_depthShadowSurface);

	// �f�o�C�X�Ƀr���[�|�[�g�̐ݒ�
	D3DVIEWPORT9 viewData = { 0, 0, 4096, 4096, 0.0f, 1.0f };
	pDevice->SetViewport(&viewData);

	// �o�b�N�o�b�t�@���y�o�b�t�@�̃N���A
	pDevice->Clear(0, NULL, (D3DCLEAR_TARGET | D3DCLEAR_ZBUFFER | D3DCLEAR_STENCIL), D3DCOLOR_RGBA(255, 255, 255, 255), 1.f, 0);
}

//--------------------------------------------------------------------------------
// �����_�[�^�[�Q�b�g��߂�
//--------------------------------------------------------------------------------
void DepthShadow::DrawDeviceEnd()
{
	// �X���b�h�Z�[�t(���b�N)
#ifdef THREAD_ON
	std::unique_lock<std::recursive_mutex> locker = CManager::Locker();
#endif

	// D3Device�̎擾
	LPDIRECT3DDEVICE9 pDevice = CRenderer::GetDevice();

	// �T�[�t�F�X��߂�
	pDevice->SetRenderTarget(0, CManager::GetRenderer()->GetRenderTarget()->GetRenderTargetSurface(1));

	// Z�o�b�t�@��߂�
	pDevice->SetDepthStencilSurface(*CManager::GetRenderer()->GetZBuffer());

	// �r���[�|�[�g��߂�
	D3DVIEWPORT9 viewData = CManager::GetViewPort();
	pDevice->SetViewport(&viewData);
}

//--------------------------------------------------------------------------------
// �o�b�N�o�b�t�@���y�o�b�t�@�̃N���A
//--------------------------------------------------------------------------------
void DepthShadow::ClearBackBuffer()
{
	// �X���b�h�Z�[�t(���b�N)
#ifdef THREAD_ON
	std::unique_lock<std::recursive_mutex> locker = CManager::Locker();
#endif

	// D3Device�̎擾
	LPDIRECT3DDEVICE9 pDevice = CRenderer::GetDevice();

	// Z�o�b�t�@�؂�ւ�
	pDevice->SetDepthStencilSurface(m_renderTargetZBuffer);

	// �T�[�t�F�X�؂�ւ�
	pDevice->SetRenderTarget(0, m_depthShadowSurface);

	// �o�b�N�o�b�t�@���y�o�b�t�@�̃N���A
	pDevice->Clear(0, NULL, (D3DCLEAR_TARGET | D3DCLEAR_ZBUFFER | D3DCLEAR_STENCIL), D3DCOLOR_RGBA(255, 255, 255, 255), 1.f, 0);

	// Z�o�b�t�@��߂�
	pDevice->SetDepthStencilSurface(CManager::GetRenderer()->GetRenderTarget()->GetRenderTargetZBuffer());

	// �T�[�t�F�X��߂�
	pDevice->SetRenderTarget(0, CManager::GetRenderer()->GetRenderTarget()->GetRenderTargetSurface(1));
}

//--------------------------------------------------------------------------------
// �쐬����
//--------------------------------------------------------------------------------
DepthShadow *DepthShadow::Create()
{
	// �X���b�h�Z�[�t(���b�N)
#ifdef THREAD_ON
	std::unique_lock<std::recursive_mutex> locker = CManager::Locker();
#endif

	// D3Device�̎擾
	LPDIRECT3DDEVICE9 pDevice = CRenderer::GetDevice();

	// DepthShadow�̐���
	DepthShadow *pDepthShadow;
	pDepthShadow = new DepthShadow();

	pDepthShadow->Init();
	pDepthShadow->m_strFilePass = "DepthShadowTexture";

	// �f�v�X�E�X�e���V���T�[�t�F�X�̐��� / �ݒ�
	pDevice->CreateDepthStencilSurface(4096, 4096, D3DFMT_D24S8, D3DMULTISAMPLE_NONE, 0, FALSE, &pDepthShadow->m_renderTargetZBuffer, NULL);

	// ��̃e�N�X�`���𐶐�
	D3DXCreateTexture(pDevice, 4096, 4096, 1, D3DUSAGE_RENDERTARGET, D3DFMT_R32F, D3DPOOL_DEFAULT, &pDepthShadow->m_depthShadowTexture);

	// �e�N�X�`���}�l�[�W���ւ̐ݒ�
	pDepthShadow->m_pTexManager = new CTexManager;
	pDepthShadow->m_pTexManager->SetTex(pDepthShadow->m_strFilePass, pDepthShadow->m_depthShadowTexture);

	// �T�[�t�F�X�̎擾
	pDepthShadow->m_depthShadowTexture->GetSurfaceLevel(0, &pDepthShadow->m_depthShadowSurface);

	// �V�F�[�_�[�̐ݒ�
	pDepthShadow->CreateShader();

	// CScene2D�̃|���S���쐬
	D3DXVECTOR2 size = CUtility::RatioToScrXY(D3DXVECTOR2(0.1f, 0.1f));	// �T�C�Y
	pDepthShadow->m_pDepthShadowPolygon = CScene2D::Create(D3DXVECTOR3(size.x, size.y, 0.f), D3DXVECTOR3(0.f, 0.f, 0.f),
		D3DXVECTOR3(size.x, size.y, 0.f), pDepthShadow->m_strFilePass, 9, pDepthShadow->m_pTexManager);
	pDepthShadow->m_pDepthShadowPolygon->SetOverall();					// �J�ڂŏ�����Ȃ�
	pDepthShadow->m_pDepthShadowPolygon->SetCol(255, 255, 255, 255);
	pDepthShadow->m_pDepthShadowPolygon->SetAlpha(false);
	pDepthShadow->m_pDepthShadowPolygon->SetDrawFlag(true);

	return pDepthShadow;
}

//--------------------------------------------------------------------------------
// hlsl�ǂݍ���
//--------------------------------------------------------------------------------
void DepthShadow::CreateShader()
{
	// �X���b�h�Z�[�t(���b�N)
#ifdef THREAD_ON
	std::unique_lock<std::recursive_mutex> locker = CManager::Locker();
#endif

	// D3Device�̎擾
	LPDIRECT3DDEVICE9 pDevice = CRenderer::GetDevice();

	// �V�F�[�_�[�p�\����
	m_pDepthShadow = new DEPTHSHADOW_SHADER_SET;

	// �V�F�[�_�[�ǂݍ��ݗp�ϐ�
	LPD3DXBUFFER err = NULL;
	LPD3DXBUFFER code = NULL;

	// ���_�V�F�[�_�[�̃R���p�C��
	//----------------------------------------------------------------
	HRESULT hr = D3DXCompileShaderFromFile("hlsl/depthShadowVS.hlsl", NULL, NULL, "main", "vs_3_0", 0, &code, &err, &m_pDepthShadow->pVConstantTable);
	if (FAILED(hr))
	{
		MessageBox(NULL, (char*)err->GetBufferPointer(), "D3DXCompileShaderFromFile(VS)", MB_OK);
		err->Release();
		return;
	}
	// ���_�V�F�[�_�[�̐���
	hr = pDevice->CreateVertexShader((LPDWORD)code->GetBufferPointer(), &m_pDepthShadow->pVertexShader);
	if (FAILED(hr))
	{
		MessageBox(NULL, "���_�V�F�[�_�[�������s", "CreateVertexShader(VS)", MB_OK);
		err->Release();
		return;
	}

	// �s�N�Z���V�F�[�_�[�̃R���p�C��
	//----------------------------------------------------------------
	hr = D3DXCompileShaderFromFile("hlsl/depthShadowPS.hlsl", NULL, NULL, "main", "ps_3_0", 0, &code, &err, &m_pDepthShadow->pPConstantTable);
	if (FAILED(hr))
	{
		MessageBox(NULL, (char*)err->GetBufferPointer(), "D3DXCompileShaderFromFile(PS)", MB_OK);
		err->Release();
		return;
	}
	// �s�N�Z���V�F�[�_�[�̐���
	hr = pDevice->CreatePixelShader((LPDWORD)code->GetBufferPointer(), &m_pDepthShadow->pPixelShader);
	if (FAILED(hr))
	{
		MessageBox(NULL, "�s�N�Z���V�F�[�_�[�������s", "CreatePixelShader(PS)", MB_OK);
		err->Release();
		return;
	}
	// �����[�X
	if (err)err->Release();
	if (code)code->Release();

	// �f�N�����[�V����
	m_pDepthShadow->pVertexDeclaration = nullptr;
//	D3DVERTEXELEMENT9 decl[] = {
//		// �p�C�v���C��, �I�t�Z�b�g, �^, , �Z�}���e�B�N�X, ,
//		{ 0, 0, D3DDECLTYPE_FLOAT3, D3DDECLMETHOD_DEFAULT, D3DDECLUSAGE_POSITION, 0 },
//		D3DDECL_END()	// �I�[�q�}�N��(�^�[�~�l�[�^)
//	};
//	pDevice->CreateVertexDeclaration(decl, &m_pDepthShadow->pVertexDeclaration);
}
