//================================================================================
//
//	Auter : KENSUKE WATANABE
//	Data  : 2018/02/10
//
//--------------------------------------------------------------------------------
#include "texManager.h"
#include "inputKeyboard.h"
#include "deferredRendering.h"
#include "manager.h"
#include "renderer.h"
#include "camera.h"
#include "lightCamera.h"
#include "scene2D.h"
#include "scene2DDeferred.h"
#include "scene2DBloom.h"
#include "scene2DPx.h"
#include "utility.h"

//--------------------------------------------------------------------------------
// �f�X�g���N�^
//--------------------------------------------------------------------------------
DefferdRendering::~DefferdRendering()
{
	// �I������
	Uninit();
}

//--------------------------------------------------------------------------------
// ������
//--------------------------------------------------------------------------------
void DefferdRendering::Init()
{
	// �ϐ��̏�����
	m_posUV = D3DXVECTOR2(0.f, 0.f);
	m_widthUV = 1.f;
	m_heightUV = 1.f;
}

//--------------------------------------------------------------------------------
// �I��
//--------------------------------------------------------------------------------
void DefferdRendering::Uninit()
{
	for (int i = 0; i < MAX_RENDERTARGET; i++)
	{
		// �e�N�X�`��
		if (m_renderTexture[i])
		{
			m_renderTexture[i]->Release();
			m_renderTexture[i] = nullptr;
		}
		// �T�[�t�F�X
		if (m_renderSurface[i])
		{
			m_renderSurface[i]->Release();
			m_renderSurface[i] = nullptr;
		}
	}
	// �e�N�X�`��
	if (m_renderTargetZBuffer)
	{
		m_renderTargetZBuffer->Release();
		m_renderTargetZBuffer = nullptr;
	}

	// �V�F�[�_�[�̉��
	if (m_pShaderSet)
	{
		// �s�N�Z���V�F�[�_�[�Œ�e�[�u��
		if (m_pShaderSet->pPConstantTable)
		{
			m_pShaderSet->pPConstantTable->Release();
			m_pShaderSet->pPConstantTable = nullptr;
		}
		// �s�N�Z���V�F�[�_�[
		if (m_pShaderSet->pPixelShader)
		{
			m_pShaderSet->pPixelShader->Release();
			m_pShaderSet->pPixelShader = nullptr;
		}
		// ���_�V�F�[�_�[�Œ�e�[�u��
		if (m_pShaderSet->pVConstantTable)
		{
			m_pShaderSet->pVConstantTable->Release();
			m_pShaderSet->pVConstantTable = nullptr;
		}
		// ���_�V�F�[�_�[
		if (m_pShaderSet->pVertexShader)
		{
			m_pShaderSet->pVertexShader->Release();
			m_pShaderSet->pVertexShader = nullptr;
		}

		delete m_pShaderSet;
		m_pShaderSet = nullptr;
	}
}

//--------------------------------------------------------------------------------
// �X�V
//--------------------------------------------------------------------------------
void DefferdRendering::Update()
{
	// L�����ŕ\���̐؂�ւ�
	if (CManager::GetKeyboard()->GetTrigger(DIK_L))
	{
		SwapDrawFlag();
	}
}

//--------------------------------------------------------------------------------
// VX�V�F�[�_�[�̐ݒ�
//--------------------------------------------------------------------------------
void DefferdRendering::SetVXShader(D3DXMATRIX mtxWorld)
{
	// �X���b�h�Z�[�t(���b�N)
#ifdef THREAD_ON
	std::unique_lock<std::recursive_mutex> locker = CManager::Locker();
#endif

	// D3Device�̎擾
	LPDIRECT3DDEVICE9 pDevice = CRenderer::GetDevice();
	// �J����
	CCamera *pCamera = CManager::GetCamera();

	// �t�s��̓]�u�s��
	D3DXMATRIX mtxWorldInverse;
	D3DXMATRIX mtxWIT;

	m_pShaderSet->pVConstantTable->SetValue(pDevice, "mtxWorld", mtxWorld, sizeof(D3DXMATRIX));

	D3DXMatrixInverse(&mtxWIT, nullptr, &mtxWorld);		// �t�s��
	D3DXMatrixTranspose(&mtxWIT, &mtxWIT);				// �]�u�s��
	m_pShaderSet->pVConstantTable->SetMatrix(pDevice, "mtxWIT", &mtxWIT);

	// �}�g���N�X�̐ݒ�
	D3DXMATRIX mtxWVP = (mtxWorld * pCamera->GetView() * pCamera->GetProj());
	m_pShaderSet->pVConstantTable->SetMatrix(pDevice, "mtxWVP", &mtxWVP);
	m_pShaderSet->pVConstantTable->SetMatrix(pDevice, "mtxW", &mtxWorld);
	m_pShaderSet->pVConstantTable->SetFloat(pDevice, "far", pCamera->GetFar());

	// ���_�V�F�[�_�[�̐ݒ�
	pDevice->SetVertexShader(m_pShaderSet->pVertexShader);
}

//--------------------------------------------------------------------------------
// PX�V�F�[�_�[�̐ݒ�
//--------------------------------------------------------------------------------
void DefferdRendering::SetPXShader(D3DMATERIAL9* pMat)
{
	// �X���b�h�Z�[�t(���b�N)
#ifdef THREAD_ON
	std::unique_lock<std::recursive_mutex> locker = CManager::Locker();
#endif

	// D3Device�̎擾
	LPDIRECT3DDEVICE9 pDevice = CRenderer::GetDevice();

	// �s�N�Z���V�F�[�_�[�̐ݒ�
	//------------------------------------------------------------------------
	float power = pMat->Power + 50.f;
	m_pShaderSet->pPConstantTable->SetFloat(pDevice, "speclarPower", power);

	D3DXVECTOR3 modelEmissive(pMat->Emissive.r, pMat->Emissive.g, pMat->Emissive.b);
	m_pShaderSet->pPConstantTable->SetValue(pDevice, "modelEmissive", modelEmissive, sizeof(D3DXVECTOR3));

	D3DXVECTOR4 modelDiffuse = D3DXVECTOR4(pMat->Diffuse.r, pMat->Diffuse.g, pMat->Diffuse.b, pMat->Diffuse.a);
	m_pShaderSet->pPConstantTable->SetValue(pDevice, "modelDiffuse", modelDiffuse, sizeof(D3DXVECTOR4));

	// �s�N�Z���V�F�[�_�[�̐ݒ�
	pDevice->SetPixelShader(m_pShaderSet->pPixelShader);
}

//--------------------------------------------------------------------------------
// �e�N�X�`������PX�V�F�[�_�[�̐ݒ�
//--------------------------------------------------------------------------------
void DefferdRendering::SetNoTexturePXShader(D3DMATERIAL9* pMat)
{
	// �X���b�h�Z�[�t(���b�N)
#ifdef THREAD_ON
	std::unique_lock<std::recursive_mutex> locker = CManager::Locker();
#endif

	// D3Device�̎擾
	LPDIRECT3DDEVICE9 pDevice = CRenderer::GetDevice();

	// �s�N�Z���V�F�[�_�[�̐ݒ�
	//------------------------------------------------------------------------
	float power = pMat->Power + 50.f;
	m_pShaderSet->pPConstantTableNoTex->SetFloat(pDevice, "speclarPower", power);
	m_pShaderSet->pPConstantTableNoTex->SetValue(pDevice, "resolution", D3DXVECTOR2(SCREEN_WIDTH, SCREEN_HEIGHT), sizeof(D3DXVECTOR2));

	D3DXVECTOR3 modelEmissive(pMat->Emissive.r, pMat->Emissive.g, pMat->Emissive.b);
	m_pShaderSet->pPConstantTableNoTex->SetValue(pDevice, "modelEmissive", modelEmissive, sizeof(D3DXVECTOR3));

	D3DXVECTOR4 modelDiffuse = D3DXVECTOR4(pMat->Diffuse.r, pMat->Diffuse.g, pMat->Diffuse.b, pMat->Diffuse.a);
	m_pShaderSet->pPConstantTableNoTex->SetValue(pDevice, "modelDiffuse", modelDiffuse, sizeof(D3DXVECTOR4));

	// �s�N�Z���V�F�[�_�[�̐ݒ�
	pDevice->SetPixelShader(m_pShaderSet->pPixelShaderNoTex);
}

//--------------------------------------------------------------------------------
// �������ݗp�̃����_�[�^�[�Q�b�g�֐؂�ւ�
//--------------------------------------------------------------------------------
void DefferdRendering::DrawDeviceBegin()
{
	// �X���b�h�Z�[�t(���b�N)
#ifdef THREAD_ON
	std::unique_lock<std::recursive_mutex> locker = CManager::Locker();
#endif

	// D3Device�̎擾
	LPDIRECT3DDEVICE9 pDevice = CRenderer::GetDevice();

	// Z�o�b�t�@�؂�ւ�
	pDevice->SetDepthStencilSurface(m_renderTargetZBuffer);

	for (int i = 0; i < MAX_RENDERTARGET; i++)
	{
		// �����_�[�^�[�Q�b�g������
		pDevice->SetRenderTarget(0, m_renderSurface[i]);
		// �o�b�N�o�b�t�@���y�o�b�t�@�̃N���A
		pDevice->Clear(0, NULL, (D3DCLEAR_TARGET | D3DCLEAR_ZBUFFER | D3DCLEAR_STENCIL), D3DCOLOR_RGBA(255, 255, 255, 255), 1.f, 0);
	}

	// �T�[�t�F�X�؂�ւ�
	for (int i = 0; i < MAX_RENDERTARGET; i++)
	{
		pDevice->SetRenderTarget(i, m_renderSurface[i]);
	}
}

//--------------------------------------------------------------------------------
// �����_�[�^�[�Q�b�g��߂�
//--------------------------------------------------------------------------------
void DefferdRendering::DrawDeviceEnd()
{
	// �X���b�h�Z�[�t(���b�N)
#ifdef THREAD_ON
	std::unique_lock<std::recursive_mutex> locker = CManager::Locker();
#endif

	// D3Device�̎擾
	LPDIRECT3DDEVICE9 pDevice = CRenderer::GetDevice();

	for (int i = 0; i < MAX_RENDERTARGET; i++)
	{
		// �����_�[�^�[�Q�b�g������
		pDevice->SetRenderTarget(i, nullptr);
	}

	// �����_�[�^�[�Q�b�g��߂�
	pDevice->SetRenderTarget(0, CManager::GetRenderer()->GetRenderTarget()->GetRenderTargetSurface(0));

	// Z�o�b�t�@��߂�
	pDevice->SetDepthStencilSurface(*CManager::GetRenderer()->GetZBuffer());
}

//--------------------------------------------------------------------------------
// �o�b�N�o�b�t�@���y�o�b�t�@�̃N���A
//--------------------------------------------------------------------------------
void DefferdRendering::ClearBackBuffer()
{
	// �X���b�h�Z�[�t(���b�N)
#ifdef THREAD_ON
	std::unique_lock<std::recursive_mutex> locker = CManager::Locker();
#endif

	// D3Device�̎擾
	LPDIRECT3DDEVICE9 pDevice = CRenderer::GetDevice();

	// Z�o�b�t�@�؂�ւ�
	pDevice->SetDepthStencilSurface(m_renderTargetZBuffer);

	for (int i = 0; i < MAX_RENDERTARGET; i++)
	{
		// �T�[�t�F�X�؂�ւ�
		pDevice->SetRenderTarget(0, m_renderSurface[i]);

		// �o�b�N�o�b�t�@���y�o�b�t�@�̃N���A
		pDevice->Clear(0, NULL, (D3DCLEAR_TARGET | D3DCLEAR_ZBUFFER | D3DCLEAR_STENCIL), D3DCOLOR_RGBA(255, 255, 255, 255), 1.f, 0);
	}

	// Z�o�b�t�@��߂�
	pDevice->SetDepthStencilSurface(CManager::GetRenderer()->GetRenderTarget()->GetRenderTargetZBuffer());

	// �T�[�t�F�X��߂�
	pDevice->SetRenderTarget(0, CManager::GetRenderer()->GetRenderTarget()->GetRenderTargetSurface(1));
}

//--------------------------------------------------------------------------------
// �e�N�X�`������
//--------------------------------------------------------------------------------
void DefferdRendering::DrawCombineTexture()
{
	// �X���b�h�Z�[�t(���b�N)
#ifdef THREAD_ON
	std::unique_lock<std::recursive_mutex> locker = CManager::Locker();
#endif
	// D3Device�̎擾
	LPDIRECT3DDEVICE9 pDevice = CRenderer::GetDevice();

	pDevice->SetVertexShader(nullptr);

	// Z�o�b�t�@�؂�ւ�
	pDevice->SetDepthStencilSurface(m_renderTargetZBuffer);

	// �T�[�t�F�X�؂�ւ�
	pDevice->SetRenderTarget(0, m_combineSurface);

	// �o�b�N�o�b�t�@���y�o�b�t�@�̃N���A
	pDevice->Clear(0, NULL, (D3DCLEAR_TARGET | D3DCLEAR_ZBUFFER | D3DCLEAR_STENCIL), D3DCOLOR_RGBA(255, 255, 255, 255), 1.f, 0);

	// ���C�g�̈ʒu�̃J�����̐ݒ�ɕύX
	CLightCamera *pLightCamera = CManager::GetLightCamera();
	pLightCamera->SetOrthoLH();

	CCamera *pCamera = CManager::GetCamera();
	// �J�������W - W
	D3DXVECTOR3 eye = pCamera->GetPos();
	m_pPConstantTableCombine->SetValue(pDevice, "posEyeW", eye, sizeof(D3DXVECTOR3));
	// ���C�g���� - W
	D3DXVECTOR3 lightDirW = pLightCamera->GetDir();
	m_pPConstantTableCombine->SetValue(pDevice, "lightDirW", &lightDirW, sizeof(D3DXVECTOR3));

	// �_����
	m_pPConstantTableCombine->SetValue(pDevice, "attenuation012", D3DXVECTOR3(0.f, 1.f, 0.f), sizeof(D3DXVECTOR3));
	D3DXVECTOR4 pointLightPos[4] =
	{
		D3DXVECTOR4(0.f, 3.f, -1.f, 0.f),
		D3DXVECTOR4(3.f, 0.5f, -2.f, 0.f),
		D3DXVECTOR4(-3.f, 0.5f, -2.f, 0.f),
		D3DXVECTOR4(0.f, 0.1f, 0.5f, 0.f)
	};
	m_pPConstantTableCombine->SetVectorArray(pDevice, "posPointLightW", pointLightPos, 4);
	D3DXVECTOR4 pointLightCol[4] =
	{
		D3DXVECTOR4(1.f, 0.f, 0.f, 1.f),
		D3DXVECTOR4(1.f, 1.f, 0.f, 1.f),
		D3DXVECTOR4(0.f, 1.f, 0.f, 1.f),
		D3DXVECTOR4(0.f, 0.f, 1.f, 1.f)
	};
	m_pPConstantTableCombine->SetVectorArray(pDevice, "colPointLight", pointLightCol, 4);

	// G�o�b�t�@�擾
	pDevice->SetTexture(0, m_renderTexture[0]);
	pDevice->SetTexture(1, m_renderTexture[1]);
	pDevice->SetTexture(2, m_renderTexture[2]);
	pDevice->SetTexture(3, m_renderTexture[3]);

	// �s�N�Z���V�F�[�_�[�ݒ�
	pDevice->SetPixelShader(m_pPixelShaderCombine);
}

//--------------------------------------------------------------------------------
// �����_�[�^�[�Q�b�g��߂�
//--------------------------------------------------------------------------------
void DefferdRendering::DrawCombineEnd()
{
	// �X���b�h�Z�[�t(���b�N)
#ifdef THREAD_ON
	std::unique_lock<std::recursive_mutex> locker = CManager::Locker();
#endif

	// �u���[���̐ݒ�
	UpdateBloom();

	// D3Device�̎擾
	LPDIRECT3DDEVICE9 pDevice = CRenderer::GetDevice();

	for (int i = 0; i < MAX_RENDERTARGET; i++)
	{
		// �����_�[�^�[�Q�b�g������
		pDevice->SetRenderTarget(i, nullptr);
	}

	pDevice->SetVertexShader(nullptr);
	pDevice->SetPixelShader(nullptr);

	// �����_�[�^�[�Q�b�g��߂�
	pDevice->SetRenderTarget(0, CManager::GetRenderer()->GetRenderTarget()->GetRenderTargetSurface(1));

	// Z�o�b�t�@��߂�
	pDevice->SetDepthStencilSurface(CManager::GetRenderer()->GetRenderTarget()->GetRenderTargetZBuffer());
}

//--------------------------------------------------------------------------------
// �e�N�X�`������
//--------------------------------------------------------------------------------
void DefferdRendering::DrawCombineBloom()
{
	// �X���b�h�Z�[�t(���b�N)
#ifdef THREAD_ON
	std::unique_lock<std::recursive_mutex> locker = CManager::Locker();
#endif
	// D3Device�̎擾
	LPDIRECT3DDEVICE9 pDevice = CRenderer::GetDevice();

	pDevice->SetVertexShader(nullptr);

	//----------------------------------------------------------------
	// G�o�b�t�@�e�N�X�`���ɍ���
	pDevice->SetDepthStencilSurface(m_renderTargetZBuffer);	// Z�o�b�t�@�؂�ւ�

	// �f�o�C�X�Ƀr���[�|�[�g�̐ݒ�
	D3DXVECTOR2 screenSize = CManager::GetRenderer()->GetScreenSize();
	D3DVIEWPORT9 viewData = { 0, 0, (DWORD)screenSize.x, (DWORD)screenSize.y, 0.0f, 1.0f };
	pDevice->SetViewport(&viewData);

	pDevice->SetRenderTarget(0, m_bloomSurface[3]);			// �����_�[�^�[�Q�b�g�؂�ւ�
	// �o�b�N�o�b�t�@���y�o�b�t�@�̃N���A
	pDevice->Clear(0, NULL, (D3DCLEAR_TARGET | D3DCLEAR_ZBUFFER | D3DCLEAR_STENCIL), D3DCOLOR_RGBA(255, 255, 255, 255), 1.f, 0);

	pDevice->SetTexture(0, m_bloomTexture[0]);				// �T���v���[�ݒ�
	pDevice->SetTexture(1, m_combineTexture);				// �T���v���[�ݒ�
	pDevice->SetPixelShader(m_pPBloomCombine);				// �s�N�Z���V�F�[�_�[�ݒ�
}

//--------------------------------------------------------------------------------
// �����_�[�^�[�Q�b�g��߂�
//--------------------------------------------------------------------------------
void DefferdRendering::DrawCombineBloomEnd()
{
	// �X���b�h�Z�[�t(���b�N)
#ifdef THREAD_ON
	std::unique_lock<std::recursive_mutex> locker = CManager::Locker();
#endif

	// D3Device�̎擾
	LPDIRECT3DDEVICE9 pDevice = CRenderer::GetDevice();

	pDevice->SetVertexShader(nullptr);
	pDevice->SetPixelShader(nullptr);

	// �����_�[�^�[�Q�b�g��߂�
	pDevice->SetRenderTarget(0, CManager::GetRenderer()->GetRenderTarget()->GetRenderTargetSurface(1));

	// Z�o�b�t�@��߂�
	pDevice->SetDepthStencilSurface(CManager::GetRenderer()->GetRenderTarget()->GetRenderTargetZBuffer());
}

//--------------------------------------------------------------------------------
// �u���[���̍X�V
//--------------------------------------------------------------------------------
void DefferdRendering::UpdateBloom()
{
	// �X���b�h�Z�[�t(���b�N)
#ifdef THREAD_ON
	std::unique_lock<std::recursive_mutex> locker = CManager::Locker();
#endif
	// D3Device�̎擾
	LPDIRECT3DDEVICE9 pDevice = CRenderer::GetDevice();
	D3DXVECTOR2 screenSize = CManager::GetRenderer()->GetScreenSize();
	D3DXVECTOR2 textureSize4 = D3DXVECTOR2(screenSize.x / 4.f, screenSize.y / 4.f);
	D3DXVECTOR2 textureSize16 = D3DXVECTOR2(screenSize.x / 16.f, screenSize.y / 16.f);

	// �T���v���[�X�e�[�g�̐ݒ�
	pDevice->SetSamplerState(0, D3DSAMP_ADDRESSU, D3DTADDRESS_MIRROR);	// �e�N�X�`���t�l�̌J��Ԃ��ݒ�
	pDevice->SetSamplerState(0, D3DSAMP_ADDRESSV, D3DTADDRESS_MIRROR);	// �e�N�X�`���u�l�̌J��Ԃ��ݒ�

	//----------------------------------------------------------------
	// 1/4�e�N�X�`���ɏ�������
	pDevice->SetDepthStencilSurface(m_renderTargetZBuffer4);			// Z�o�b�t�@�؂�ւ�

	// �f�o�C�X�Ƀr���[�|�[�g�̐ݒ�
	D3DVIEWPORT9 viewData = { 0, 0, (DWORD)textureSize4.x, (DWORD)textureSize4.y, 0.0f, 1.0f };
	pDevice->SetViewport(&viewData);

	pDevice->SetRenderTarget(0, m_bloomSurface[0]);										// �����_�[�^�[�Q�b�g�؂�ւ�
	// �o�b�N�o�b�t�@���y�o�b�t�@�̃N���A
	pDevice->Clear(0, NULL, (D3DCLEAR_TARGET | D3DCLEAR_ZBUFFER | D3DCLEAR_STENCIL), D3DCOLOR_RGBA(255, 255, 255, 0), 1.f, 0);

	pDevice->SetTexture(0, m_combineTexture);			// �T���v���[�ݒ�
	m_pBloomPolygon[0]->Draw();							// �`��

	//----------------------------------------------------------------
	// 1/16�e�N�X�`���ɏ�������
	pDevice->SetDepthStencilSurface(m_renderTargetZBuffer16);							// Z�o�b�t�@�؂�ւ�

	// �f�o�C�X�Ƀr���[�|�[�g�̐ݒ�
	viewData = { 0, 0, (DWORD)textureSize16.x, (DWORD)textureSize16.y, 0.0f, 1.0f };
	pDevice->SetViewport(&viewData);

	pDevice->SetRenderTarget(0, m_bloomSurface[1]);										// �����_�[�^�[�Q�b�g�؂�ւ�
	// �o�b�N�o�b�t�@���y�o�b�t�@�̃N���A
	pDevice->Clear(0, NULL, (D3DCLEAR_TARGET | D3DCLEAR_ZBUFFER | D3DCLEAR_STENCIL), D3DCOLOR_RGBA(255, 255, 255, 0), 1.f, 0);

	pDevice->SetTexture(0, m_bloomTexture[0]);			// �T���v���[�ݒ�
	m_pBloomPolygon[1]->Draw();							// �`��

	//----------------------------------------------------------------
	// �P�x���o
	pDevice->SetRenderTarget(0, m_bloomSurface[2]);										// �����_�[�^�[�Q�b�g�؂�ւ�
	// �o�b�N�o�b�t�@���y�o�b�t�@�̃N���A
	pDevice->Clear(0, NULL, (D3DCLEAR_TARGET | D3DCLEAR_ZBUFFER | D3DCLEAR_STENCIL), D3DCOLOR_RGBA(255, 255, 255, 0), 1.f, 0);

	pDevice->SetTexture(0, m_bloomTexture[1]);			// �T���v���[�ݒ�
	pDevice->SetPixelShader(m_pPColorBrightPass);		// �s�N�Z���V�F�[�_�[�ݒ�
	m_pBloomPolygon[2]->Draw();							// �`��

	for (int i = 0; i < 2; i++)
	{
		//----------------------------------------------------------------
		// �u���[��H
		pDevice->SetRenderTarget(0, m_bloomSurface[1]);										// �����_�[�^�[�Q�b�g�؂�ւ�
	// �o�b�N�o�b�t�@���y�o�b�t�@�̃N���A
		pDevice->Clear(0, NULL, (D3DCLEAR_TARGET | D3DCLEAR_ZBUFFER | D3DCLEAR_STENCIL), D3DCOLOR_RGBA(255, 255, 255, 0), 1.f, 0);

		pDevice->SetTexture(0, m_bloomTexture[2]);			// �T���v���[�ݒ�
		m_pPCBloomH->SetValue(pDevice, "texSize", textureSize16, sizeof(D3DXVECTOR2));
		pDevice->SetPixelShader(m_pPBloomH);				// �s�N�Z���V�F�[�_�[�ݒ�
		m_pBloomPolygon[1]->Draw();							// �`��

		//----------------------------------------------------------------
		// �u���[��V
		pDevice->SetRenderTarget(0, m_bloomSurface[2]);										// �����_�[�^�[�Q�b�g�؂�ւ�
		// �o�b�N�o�b�t�@���y�o�b�t�@�̃N���A
		pDevice->Clear(0, NULL, (D3DCLEAR_TARGET | D3DCLEAR_ZBUFFER | D3DCLEAR_STENCIL), D3DCOLOR_RGBA(255, 255, 255, 0), 1.f, 0);

		pDevice->SetTexture(0, m_bloomTexture[1]);			// �T���v���[�ݒ�
		m_pPCBloomV->SetValue(pDevice, "texSize", textureSize16, sizeof(D3DXVECTOR2));
		pDevice->SetPixelShader(m_pPBloomV);				// �s�N�Z���V�F�[�_�[�ݒ�
		m_pBloomPolygon[2]->Draw();							// �`��
	}

	//----------------------------------------------------------------
	// 1/4�e�N�X�`���ɏ�������(�u���[������)
	pDevice->SetDepthStencilSurface(m_renderTargetZBuffer4);							// Z�o�b�t�@�؂�ւ�

	// �f�o�C�X�Ƀr���[�|�[�g�̐ݒ�
	viewData = { 0, 0, (DWORD)textureSize4.x, (DWORD)textureSize4.y, 0.0f, 1.0f };
	pDevice->SetViewport(&viewData);

	pDevice->SetRenderTarget(0, m_bloomSurface[0]);										// �����_�[�^�[�Q�b�g�؂�ւ�
		// �o�b�N�o�b�t�@���y�o�b�t�@�̃N���A
	pDevice->Clear(0, NULL, (D3DCLEAR_TARGET | D3DCLEAR_ZBUFFER | D3DCLEAR_STENCIL), D3DCOLOR_RGBA(255, 255, 255, 0), 1.f, 0);

	pDevice->SetTexture(0, m_bloomTexture[2]);				// �T���v���[�ݒ�
	m_pBloomPolygon[0]->Draw();								// �`��

	//----------------------------------------------------------------
	// �I�� - �ݒ��߂�

	// �T���v���[�X�e�[�g�̐ݒ�
	pDevice->SetSamplerState(0, D3DSAMP_ADDRESSU, D3DTADDRESS_WRAP);	// �e�N�X�`���t�l�̌J��Ԃ��ݒ�
	pDevice->SetSamplerState(0, D3DSAMP_ADDRESSV, D3DTADDRESS_WRAP);	// �e�N�X�`���u�l�̌J��Ԃ��ݒ�

	pDevice->SetPixelShader(nullptr);						// �s�N�Z���V�F�[�_�[����芸����������

	// �f�o�C�X�Ƀr���[�|�[�g�̐ݒ�
	pDevice->SetViewport(&CManager::GetViewPort());

	// �����_�[�^�[�Q�b�g��߂�
	pDevice->SetRenderTarget(0, CManager::GetRenderer()->GetRenderTarget()->GetRenderTargetSurface(1));

	// Z�o�b�t�@��߂�
	pDevice->SetDepthStencilSurface(CManager::GetRenderer()->GetRenderTarget()->GetRenderTargetZBuffer());
}

//--------------------------------------------------------------------------------
// �쐬����
//--------------------------------------------------------------------------------
DefferdRendering *DefferdRendering::Create()
{
	// �X���b�h�Z�[�t(���b�N)
#ifdef THREAD_ON
	std::unique_lock<std::recursive_mutex> locker = CManager::Locker();
#endif

	// D3Device�̎擾
	LPDIRECT3DDEVICE9 pDevice = CRenderer::GetDevice();

	// DefferdRendering�̐���
	DefferdRendering *pRendering;
	pRendering = new DefferdRendering();

	pRendering->Init();

	// �f�v�X�E�X�e���V���T�[�t�F�X�̐��� / �ݒ�
	int screenSizeX = static_cast<int>(CManager::GetRenderer()->GetScreenSize().x);
	int screenSizeY = static_cast<int>(CManager::GetRenderer()->GetScreenSize().y);
	pDevice->CreateDepthStencilSurface(screenSizeX, screenSizeY, D3DFMT_D24S8, D3DMULTISAMPLE_NONE, 0, FALSE, &pRendering->m_renderTargetZBuffer, NULL);

	// �f�v�X�E�X�e���V���T�[�t�F�X 1/4
	pDevice->CreateDepthStencilSurface(screenSizeX / 4, screenSizeY / 4, D3DFMT_D24S8, D3DMULTISAMPLE_NONE, 0, FALSE, &pRendering->m_renderTargetZBuffer4, NULL);

	// �f�v�X�E�X�e���V���T�[�t�F�X 1//16
	pDevice->CreateDepthStencilSurface(screenSizeX / 16, screenSizeY / 16, D3DFMT_D24S8, D3DMULTISAMPLE_NONE, 0, FALSE, &pRendering->m_renderTargetZBuffer16, NULL);

	// �O�Ԗڂ̃e�N�X�`������
	//--------------------------------------------------------------------------------
	pRendering->m_strFilePass[0] = "posW";
	// ��̃e�N�X�`���𐶐�
	D3DXCreateTexture(pDevice, screenSizeX, screenSizeY, 1, D3DUSAGE_RENDERTARGET, D3DFMT_A32B32G32R32F, D3DPOOL_DEFAULT, &pRendering->m_renderTexture[0]);
	// �T�[�t�F�X�̎擾
	pRendering->m_renderTexture[0]->GetSurfaceLevel(0, &pRendering->m_renderSurface[0]);

	// �P�Ԗڂ̃e�N�X�`������
	//--------------------------------------------------------------------------------
	pRendering->m_strFilePass[1] = "color";
	// ��̃e�N�X�`���𐶐�
	D3DXCreateTexture(pDevice, screenSizeX, screenSizeY, 1, D3DUSAGE_RENDERTARGET, D3DFMT_A8R8G8B8, D3DPOOL_DEFAULT, &pRendering->m_renderTexture[1]);
	// �T�[�t�F�X�̎擾
	pRendering->m_renderTexture[1]->GetSurfaceLevel(0, &pRendering->m_renderSurface[1]);

	// �Q�Ԗڂ̃e�N�X�`������
	//--------------------------------------------------------------------------------
	pRendering->m_strFilePass[2] = "normal";
	// ��̃e�N�X�`���𐶐�
	D3DXCreateTexture(pDevice, screenSizeX, screenSizeY, 1, D3DUSAGE_RENDERTARGET, D3DFMT_A8R8G8B8, D3DPOOL_DEFAULT, &pRendering->m_renderTexture[2]);
	// �T�[�t�F�X�̎擾
	pRendering->m_renderTexture[2]->GetSurfaceLevel(0, &pRendering->m_renderSurface[2]);

	// �R�Ԗڂ̃e�N�X�`������
	//--------------------------------------------------------------------------------
	pRendering->m_strFilePass[3] = "ambient";
	// ��̃e�N�X�`���𐶐�
	D3DXCreateTexture(pDevice, screenSizeX, screenSizeY, 1, D3DUSAGE_RENDERTARGET, D3DFMT_A8R8G8B8, D3DPOOL_DEFAULT, &pRendering->m_renderTexture[3]);
	// �T�[�t�F�X�̎擾
	pRendering->m_renderTexture[3]->GetSurfaceLevel(0, &pRendering->m_renderSurface[3]);

	// ������̃e�N�X�`��
	//--------------------------------------------------------------------------------
	// ��̃e�N�X�`���𐶐�
	D3DXCreateTexture(pDevice, screenSizeX, screenSizeY, 1, D3DUSAGE_RENDERTARGET, D3DFMT_A8R8G8B8, D3DPOOL_DEFAULT, &pRendering->m_combineTexture);
	// �T�[�t�F�X�̎擾
	pRendering->m_combineTexture->GetSurfaceLevel(0, &pRendering->m_combineSurface);

	// �u���[���p�e�N�X�`��(1/4)
	//--------------------------------------------------------------------------------
	// ��̃e�N�X�`���𐶐�
	D3DXCreateTexture(pDevice, screenSizeX / 4, screenSizeY / 4, 1, D3DUSAGE_RENDERTARGET, D3DFMT_A8R8G8B8, D3DPOOL_DEFAULT, &pRendering->m_bloomTexture[0]);
	// �T�[�t�F�X�̎擾
	pRendering->m_bloomTexture[0]->GetSurfaceLevel(0, &pRendering->m_bloomSurface[0]);

	// �u���[���p�e�N�X�`��(1/16) - 1
	//--------------------------------------------------------------------------------
	// ��̃e�N�X�`���𐶐�
	D3DXCreateTexture(pDevice, screenSizeX / 16, screenSizeY / 16, 1, D3DUSAGE_RENDERTARGET, D3DFMT_A8R8G8B8, D3DPOOL_DEFAULT, &pRendering->m_bloomTexture[1]);
	// �T�[�t�F�X�̎擾
	pRendering->m_bloomTexture[1]->GetSurfaceLevel(0, &pRendering->m_bloomSurface[1]);

	// �u���[���p�e�N�X�`��(1/16) - 2
	//--------------------------------------------------------------------------------
	// ��̃e�N�X�`���𐶐�
	D3DXCreateTexture(pDevice, screenSizeX / 16, screenSizeY / 16, 1, D3DUSAGE_RENDERTARGET, D3DFMT_A8R8G8B8, D3DPOOL_DEFAULT, &pRendering->m_bloomTexture[2]);
	// �T�[�t�F�X�̎擾
	pRendering->m_bloomTexture[2]->GetSurfaceLevel(0, &pRendering->m_bloomSurface[2]);

	// �u���[��������̃e�N�X�`��
	//--------------------------------------------------------------------------------
	// ��̃e�N�X�`���𐶐�
	D3DXCreateTexture(pDevice, screenSizeX, screenSizeY, 1, D3DUSAGE_RENDERTARGET, D3DFMT_A8R8G8B8, D3DPOOL_DEFAULT, &pRendering->m_bloomTexture[3]);
	// �T�[�t�F�X�̎擾
	pRendering->m_bloomTexture[3]->GetSurfaceLevel(0, &pRendering->m_bloomSurface[3]);


	// �V�F�[�_�[�̐ݒ�
	//--------------------------------------------------------------------------------
	pRendering->CreateShader();

	// �e�N�X�`���}�l�[�W������
	pRendering->m_pTexManager = new CTexManager;

	// ��ʃT�C�Y�̎擾
	D3DXVECTOR2 size = CUtility::RatioToScrXY(D3DXVECTOR2(0.1f, 0.1f));

	for (int i = 0; i < MAX_RENDERTARGET; i++)
	{
		// �e�N�X�`���}�l�[�W���ւ̐ݒ�
		pRendering->m_pTexManager->SetTex(pRendering->m_strFilePass[i], pRendering->m_renderTexture[i]);

		// CScene2D�̃|���S���쐬
		pRendering->m_pRenderPolygon[i] = CScene2D::Create(D3DXVECTOR3(size.x - 0.5f, size.y - 0.5f + (size.y * 2 * i), 0.f), D3DXVECTOR3(0.f, 0.f, 0.f),
			D3DXVECTOR3(size.x, size.y, 0.f), pRendering->m_strFilePass[i], 9, pRendering->m_pTexManager);
		pRendering->m_pRenderPolygon[i]->SetOverall();					// �J�ڂŏ�����Ȃ�
		pRendering->m_pRenderPolygon[i]->SetCol(255, 255, 255, 255);
		pRendering->m_pRenderPolygon[i]->SetAlpha(false);
	}

	// ������̃e�N�X�`��
	//--------------------------------------------------------------------------------
	char *texName = "combine";
	// �e�N�X�`���}�l�[�W���ւ̐ݒ�
	pRendering->m_pTexManager->SetTex(texName, pRendering->m_combineTexture);

	// G�o�b�t�@�쐬�p�|���S���쐬
	pRendering->m_pCombinePolygon = pRendering->m_pCombinePolygon->Create(D3DXVECTOR3(screenSizeX * 0.5f - 0.5f, screenSizeY * 0.5f - 0.5f, 0.f), D3DXVECTOR3(0.f, 0.f, 0.f),
		D3DXVECTOR3(screenSizeX * 0.5f, screenSizeY * 0.5f, 0.f), texName, 9, pRendering->m_pTexManager);
	pRendering->m_pCombinePolygon->SetOverall();					// �J�ڂŏ�����Ȃ�
	pRendering->m_pCombinePolygon->SetCol(255, 255, 255, 255);
	pRendering->m_pCombinePolygon->SetAlpha(false);

	// G�o�b�t�@�`��p�̃|���S���쐬
	pRendering->m_pGPolygon = CScene2D::Create(D3DXVECTOR3(size.x - 0.5f, size.y - 0.5f + (size.y * 2 * 3), 0.f), D3DXVECTOR3(0.f, 0.f, 0.f),
		D3DXVECTOR3(size.x, size.y, 0.f), texName, 9, pRendering->m_pTexManager);
	pRendering->m_pGPolygon->SetOverall();				// �J�ڂŏ�����Ȃ�
	pRendering->m_pGPolygon->SetCol(255, 255, 255, 255);
	pRendering->m_pGPolygon->SetAlpha(false);

	// �u���[���p�̃e�N�X�`��
	//--------------------------------------------------------------------------------
	// �e�N�X�`���}�l�[�W���ւ̐ݒ�
	char *texNameBloom[] =
	{
		"bloom1/4",
		"bloom1/16-1",
		"bloom1/16-2",
		"bloomCombine"
	};
	pRendering->m_pTexManager->SetTex(texNameBloom[0], pRendering->m_bloomTexture[0]);
	pRendering->m_pTexManager->SetTex(texNameBloom[1], pRendering->m_bloomTexture[1]);
	pRendering->m_pTexManager->SetTex(texNameBloom[2], pRendering->m_bloomTexture[2]);
	pRendering->m_pTexManager->SetTex(texNameBloom[3], pRendering->m_bloomTexture[3]);

	// �u���[���p�|���S���쐬
	pRendering->m_pBloomPolygon[0] = CScene2DPx::Create(D3DXVECTOR3(screenSizeX * 0.5f / 4 - 0.5f, screenSizeY * 0.5f / 4 - 0.5f, 0.f), D3DXVECTOR3(0.f, 0.f, 0.f),
		D3DXVECTOR3(screenSizeX * 0.5f / 4, screenSizeY * 0.5f / 4, 0.f), texName, 9, pRendering->m_pTexManager);
	pRendering->m_pBloomPolygon[0]->SetOverall();					// �J�ڂŏ�����Ȃ�
	pRendering->m_pBloomPolygon[0]->SetCol(255, 255, 255, 255);
	pRendering->m_pBloomPolygon[0]->SetAlpha(false);
	pRendering->m_pBloomPolygon[0]->SetDrawFlag(false);

	pRendering->m_pBloomPolygon[1] = CScene2DPx::Create(D3DXVECTOR3(screenSizeX * 0.5f / 16 - 0.5f, screenSizeY * 0.5f / 16 - 0.5f, 0.f), D3DXVECTOR3(0.f, 0.f, 0.f),
		D3DXVECTOR3(screenSizeX * 0.5f / 16, screenSizeY * 0.5f / 16, 0.f), texNameBloom[0], 9, pRendering->m_pTexManager);
	pRendering->m_pBloomPolygon[1]->SetOverall();					// �J�ڂŏ�����Ȃ�
	pRendering->m_pBloomPolygon[1]->SetCol(255, 255, 255, 255);
	pRendering->m_pBloomPolygon[1]->SetAlpha(false);
	pRendering->m_pBloomPolygon[1]->SetDrawFlag(false);

	pRendering->m_pBloomPolygon[2] = CScene2DPx::Create(D3DXVECTOR3(screenSizeX * 0.5f / 16 - 0.5f, screenSizeY * 0.5f / 16 - 0.5f, 0.f), D3DXVECTOR3(0.f, 0.f, 0.f),
		D3DXVECTOR3(screenSizeX * 0.5f / 16, screenSizeY * 0.5f / 16, 0.f), texNameBloom[1], 9, pRendering->m_pTexManager);
	pRendering->m_pBloomPolygon[2]->SetOverall();					// �J�ڂŏ�����Ȃ�
	pRendering->m_pBloomPolygon[2]->SetCol(255, 255, 255, 255);
	pRendering->m_pBloomPolygon[2]->SetAlpha(false);
	pRendering->m_pBloomPolygon[2]->SetDrawFlag(false);

	// �u���[��������e�N�X�`���`��p�̃|���S���쐬
	pRendering->m_pBloomPolygon[3] = CScene2DPx::Create(D3DXVECTOR3(screenSizeX * 0.5f - 0.5f, screenSizeY * 0.5f - 0.5f, 0.f), D3DXVECTOR3(0.f, 0.f, 0.f),
		D3DXVECTOR3(screenSizeX * 0.5f, screenSizeY * 0.5f, 0.f), texNameBloom[2], 9, pRendering->m_pTexManager);
	pRendering->m_pBloomPolygon[3]->SetOverall();					// �J�ڂŏ�����Ȃ�
	pRendering->m_pBloomPolygon[3]->SetCol(255, 255, 255, 255);
	pRendering->m_pBloomPolygon[3]->SetAlpha(false);
	pRendering->m_pBloomPolygon[3]->SetDrawFlag(false);

	// �u���[���`��p�|���S���쐬
	pRendering->m_pBloom = CScene2D::Create(D3DXVECTOR3(size.x - 0.5f, size.y - 0.5f + (size.y * 2 * 4), 0.f), D3DXVECTOR3(0.f, 0.f, 0.f),
		D3DXVECTOR3(size.x, size.y, 0.f), texNameBloom[0], 9, pRendering->m_pTexManager);
	pRendering->m_pBloom->SetOverall();					// �J�ڂŏ�����Ȃ�
	pRendering->m_pBloom->SetCol(255, 255, 255, 255);
	pRendering->m_pBloom->SetAlpha(false);

//	pBloom = CScene2D::Create(D3DXVECTOR3(size.x * 3 - 0.5f, size.y * 3 - 0.5f, 0.f), D3DXVECTOR3(0.f, 0.f, 0.f),
//		D3DXVECTOR3(size.x, size.y, 0.f), texNameBloom[1], 9, pRendering->m_pTexManager);
//	pBloom->SetOverall();					// �J�ڂŏ�����Ȃ�
//	pBloom->SetCol(255, 255, 255, 255);
//	pBloom->SetAlpha(false);
//
//	pBloom = CScene2D::Create(D3DXVECTOR3(size.x * 3 - 0.5f, size.y * 5 - 0.5f, 0.f), D3DXVECTOR3(0.f, 0.f, 0.f),
//		D3DXVECTOR3(size.x, size.y, 0.f), texNameBloom[2], 9, pRendering->m_pTexManager);
//	pBloom->SetOverall();					// �J�ڂŏ�����Ȃ�
//	pBloom->SetCol(255, 255, 255, 255);
//	pBloom->SetAlpha(false);

	// �u���[��������e�N�X�`���쐬�p�̃|���S���쐬
	CScene2DBloom *pCombineBloom = CScene2DBloom::Create(D3DXVECTOR3(screenSizeX * 0.5f - 0.5f, screenSizeY * 0.5f - 0.5f, 0.f), D3DXVECTOR3(0.f, 0.f, 0.f),
		D3DXVECTOR3(screenSizeX * 0.5f, screenSizeY * 0.5f, 0.f), texNameBloom[3], 9, pRendering->m_pTexManager);
	pCombineBloom->SetOverall();					// �J�ڂŏ�����Ȃ�
	pCombineBloom->SetCol(255, 255, 255, 255);
	pCombineBloom->SetAlpha(false);

	// �u���[��������e�N�X�`���`��p�̃|���S���쐬
	pRendering->m_combineBloomPolygon = CScene2D::Create(D3DXVECTOR3(screenSizeX * 0.5f - 0.5f, screenSizeY * 0.5f - 0.5f, 0.f), D3DXVECTOR3(0.f, 0.f, 0.f),
		D3DXVECTOR3(screenSizeX * 0.5f, screenSizeY * 0.5f, 0.f), texNameBloom[3], 1, pRendering->m_pTexManager);
	pRendering->m_combineBloomPolygon->SetOverall();					// �J�ڂŏ�����Ȃ�
	pRendering->m_combineBloomPolygon->SetCol(255, 255, 255, 255);
	pRendering->m_combineBloomPolygon->SetAlpha(false);

	return pRendering;
}

//--------------------------------------------------------------------------------
// hlsl�ǂݍ���
//--------------------------------------------------------------------------------
void DefferdRendering::CreateShader()
{
	// �X���b�h�Z�[�t(���b�N)
#ifdef THREAD_ON
	std::unique_lock<std::recursive_mutex> locker = CManager::Locker();
#endif

	// D3Device�̎擾
	LPDIRECT3DDEVICE9 pDevice = CRenderer::GetDevice();

	// �V�F�[�_�[�p�\����
	m_pShaderSet = new SHADER_SET;

	// �V�F�[�_�[�ǂݍ��ݗp�ϐ�
	LPD3DXBUFFER err = NULL;
	LPD3DXBUFFER code = NULL;

	// ���_�V�F�[�_�[�̃R���p�C��
	//----------------------------------------------------------------
	HRESULT hr = D3DXCompileShaderFromFile("hlsl/deferredVS.hlsl", NULL, NULL, "main", "vs_3_0", 0, &code, &err, &m_pShaderSet->pVConstantTable);
	if (FAILED(hr))
	{
		MessageBox(NULL, (char*)err->GetBufferPointer(), "D3DXCompileShaderFromFile(VS)", MB_OK);
		err->Release();
		return;
	}
	// ���_�V�F�[�_�[�̐���
	hr = pDevice->CreateVertexShader((LPDWORD)code->GetBufferPointer(), &m_pShaderSet->pVertexShader);
	if (FAILED(hr))
	{
		MessageBox(NULL, "���_�V�F�[�_�[�������s", "CreateVertexShader(VS)", MB_OK);
		err->Release();
		return;
	}

	// �s�N�Z���V�F�[�_�[�̃R���p�C��
	//----------------------------------------------------------------
	hr = D3DXCompileShaderFromFile("hlsl/deferredPS.hlsl", NULL, NULL, "main", "ps_3_0", 0, &code, &err, &m_pShaderSet->pPConstantTable);
	if (FAILED(hr))
	{
		MessageBox(NULL, (char*)err->GetBufferPointer(), "D3DXCompileShaderFromFile(PS)", MB_OK);
		err->Release();
		return;
	}
	// �s�N�Z���V�F�[�_�[�̐���
	hr = pDevice->CreatePixelShader((LPDWORD)code->GetBufferPointer(), &m_pShaderSet->pPixelShader);
	if (FAILED(hr))
	{
		MessageBox(NULL, "�s�N�Z���V�F�[�_�[�������s", "CreatePixelShader(PS)", MB_OK);
		err->Release();
		return;
	}

	// �s�N�Z���V�F�[�_�[�̃R���p�C��
	//----------------------------------------------------------------
	hr = D3DXCompileShaderFromFile("hlsl/deferredPS.hlsl", NULL, NULL, "NoTexture", "ps_3_0", 0, &code, &err, &m_pShaderSet->pPConstantTableNoTex);
	if (FAILED(hr))
	{
		MessageBox(NULL, (char*)err->GetBufferPointer(), "D3DXCompileShaderFromFile(PS)", MB_OK);
		err->Release();
		return;
	}
	// �s�N�Z���V�F�[�_�[�̐���
	hr = pDevice->CreatePixelShader((LPDWORD)code->GetBufferPointer(), &m_pShaderSet->pPixelShaderNoTex);
	if (FAILED(hr))
	{
		MessageBox(NULL, "�s�N�Z���V�F�[�_�[�������s", "CreatePixelShader(PS)", MB_OK);
		err->Release();
		return;
	}

	// �s�N�Z���V�F�[�_�[�̃R���p�C��
	//----------------------------------------------------------------
	hr = D3DXCompileShaderFromFile("hlsl/deferredCombinePS.hlsl", NULL, NULL, "main", "ps_3_0", 0, &code, &err, &m_pPConstantTableCombine);
	if (FAILED(hr))
	{
		MessageBox(NULL, (char*)err->GetBufferPointer(), "D3DXCompileShaderFromFile(PS)", MB_OK);
		err->Release();
		return;
	}
	// �s�N�Z���V�F�[�_�[�̐���
	hr = pDevice->CreatePixelShader((LPDWORD)code->GetBufferPointer(), &m_pPixelShaderCombine);
	if (FAILED(hr))
	{
		MessageBox(NULL, "�s�N�Z���V�F�[�_�[�������s", "CreatePixelShader(PS)", MB_OK);
		err->Release();
		return;
	}

	// �u���[���O�̏k���p�s�N�Z���V�F�[�_�[�̃R���p�C��
	//----------------------------------------------------------------
	hr = D3DXCompileShaderFromFile("hlsl/ColorBrightPass.hlsl", NULL, NULL, "main", "ps_3_0", 0, &code, &err, &m_pPCColorBrightPass);
	if (FAILED(hr))
	{
		MessageBox(NULL, (char*)err->GetBufferPointer(), "D3DXCompileShaderFromFile(PS)", MB_OK);
		err->Release();
		return;
	}
	// �s�N�Z���V�F�[�_�[�̐���
	hr = pDevice->CreatePixelShader((LPDWORD)code->GetBufferPointer(), &m_pPColorBrightPass);
	if (FAILED(hr))
	{
		MessageBox(NULL, "�s�N�Z���V�F�[�_�[�������s", "CreatePixelShader(PS)", MB_OK);
		err->Release();
		return;
	}

	// �u���[��H
	//----------------------------------------------------------------
	hr = D3DXCompileShaderFromFile("hlsl/bloom.hlsl", NULL, NULL, "bloomH", "ps_3_0", 0, &code, &err, &m_pPCBloomH);
	if (FAILED(hr))
	{
		MessageBox(NULL, (char*)err->GetBufferPointer(), "D3DXCompileShaderFromFile(PS)", MB_OK);
		err->Release();
		return;
	}
	// �s�N�Z���V�F�[�_�[�̐���
	hr = pDevice->CreatePixelShader((LPDWORD)code->GetBufferPointer(), &m_pPBloomH);
	if (FAILED(hr))
	{
		MessageBox(NULL, "�s�N�Z���V�F�[�_�[�������s", "CreatePixelShader(PS)", MB_OK);
		err->Release();
		return;
	}
	// �u���[��W
	//----------------------------------------------------------------
	hr = D3DXCompileShaderFromFile("hlsl/bloom.hlsl", NULL, NULL, "bloomV", "ps_3_0", 0, &code, &err, &m_pPCBloomV);
	if (FAILED(hr))
	{
		MessageBox(NULL, (char*)err->GetBufferPointer(), "D3DXCompileShaderFromFile(PS)", MB_OK);
		err->Release();
		return;
	}
	// �s�N�Z���V�F�[�_�[�̐���
	hr = pDevice->CreatePixelShader((LPDWORD)code->GetBufferPointer(), &m_pPBloomV);
	if (FAILED(hr))
	{
		MessageBox(NULL, "�s�N�Z���V�F�[�_�[�������s", "CreatePixelShader(PS)", MB_OK);
		err->Release();
		return;
	}
	// �u���[�������p
	//----------------------------------------------------------------
	hr = D3DXCompileShaderFromFile("hlsl/bloomCombinePS.hlsl", NULL, NULL, "main", "ps_3_0", 0, &code, &err, &m_pPCBloomCombine);
	if (FAILED(hr))
	{
		MessageBox(NULL, (char*)err->GetBufferPointer(), "D3DXCompileShaderFromFile(PS)", MB_OK);
		err->Release();
		return;
	}
	// �s�N�Z���V�F�[�_�[�̐���
	hr = pDevice->CreatePixelShader((LPDWORD)code->GetBufferPointer(), &m_pPBloomCombine);
	if (FAILED(hr))
	{
		MessageBox(NULL, "�s�N�Z���V�F�[�_�[�������s", "CreatePixelShader(PS)", MB_OK);
		err->Release();
		return;
	}

	// �����[�X
	if (err)err->Release();
	if (code)code->Release();
}
