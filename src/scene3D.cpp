//================================================================================
//
//	Auter : KENSUKE WATANABE
//	Data  : 2018/02/10
//
//--------------------------------------------------------------------------------
#include "scene3D.h"
#include "renderer.h"

CScene3D::SHADER_SET *CScene3D::m_pShaderSet = nullptr;	// �V�F�[�_�[

//--------------------------------------------------------------------------------
// �R���X�g���N�^
//--------------------------------------------------------------------------------
CScene3D::CScene3D(int priority): CScene(priority), m_r(255), m_g(255), m_b(255), m_a(255), m_pMat(nullptr)
{
	m_classType = CLASSTYPE::SCENE_3DPOLY;
	D3DXMatrixIdentity(&m_mtxWorld);			// ���[���h���W
	m_pVtxBuffer = nullptr;						// Vtx�o�b�t�@
	m_numVtx = 0;
	m_numIndex = 0;

	// �f�o�C�X�̎擾
	LPDIRECT3DDEVICE9 pDevice = CRenderer::GetDevice();

	if (m_pShaderSet == nullptr)
	{
		// �V�F�[�_�[�̐ݒ�
		CreateShader();

		// �V�������b�V���̒��_�v�f
		//----------------------------------------------------------------
		// �f�N�����[�V����
		D3DVERTEXELEMENT9 decl[] = {
			// �p�C�v���C��, �I�t�Z�b�g, �^, , �Z�}���e�B�N�X, ,
			{ 0, 0, D3DDECLTYPE_FLOAT3, D3DDECLMETHOD_DEFAULT, D3DDECLUSAGE_POSITION, 0 },
			{ 0, 12, D3DDECLTYPE_FLOAT3, D3DDECLMETHOD_DEFAULT, D3DDECLUSAGE_NORMAL, 0 },
			{ 0, 24, D3DDECLTYPE_FLOAT4, D3DDECLMETHOD_DEFAULT, D3DDECLUSAGE_COLOR, 0 },
			{ 0, 40, D3DDECLTYPE_FLOAT2, D3DDECLMETHOD_DEFAULT, D3DDECLUSAGE_TEXCOORD, 0 },
			D3DDECL_END()	// �I�[�q�}�N��(�^�[�~�l�[�^)
		};
		pDevice->CreateVertexDeclaration(decl, &m_pShaderSet->pVertexDeclaration);
	}
}

//--------------------------------------------------------------------------------
// �f�X�g���N�^
//--------------------------------------------------------------------------------
CScene3D::~CScene3D()
{
	// �X���b�h�Z�[�t(���b�N)
#ifdef THREAD_ON
	std::unique_lock<std::recursive_mutex> locker = CManager::Locker();
#endif

	if (m_pMat)
	{
		delete m_pMat;
		m_pMat = nullptr;
	}

	// Vtx�o�b�t�@�̉��
	if (m_pVtxBuffer != nullptr)
	{
		m_pVtxBuffer->Release();
		m_pVtxBuffer = nullptr;
	}
}

void CScene3D::CreateShader()
{
	// ����
	m_pShaderSet = new SHADER_SET;
	m_pShaderSet->pVertexDeclaration = nullptr;
	m_pShaderSet->pPConstantTable = nullptr;
	m_pShaderSet->pPConstantTableNoTex = nullptr;
	m_pShaderSet->pPixelShader = nullptr;
	m_pShaderSet->pPixelShaderNoTex = nullptr;
	m_pShaderSet->pVConstantTable = nullptr;
	m_pShaderSet->pVertexShader = nullptr;

	// �X���b�h�Z�[�t(���b�N)
#ifdef THREAD_ON
	std::unique_lock<std::recursive_mutex> locker = CManager::Locker();
#endif

	// �f�o�C�X�̎擾
	LPDIRECT3DDEVICE9 pDevice = CRenderer::GetDevice();

	// �V�F�[�_�[�ǂݍ��ݗp�ϐ�
	LPD3DXBUFFER err = NULL;
	LPD3DXBUFFER code = NULL;

	// ���_�V�F�[�_�[�̃R���p�C��
	//----------------------------------------------------------------
	HRESULT hr = D3DXCompileShaderFromFile("hlsl/SpeclarVS.hlsl", NULL, NULL, "main", "vs_3_0", 0, &code, &err, &m_pShaderSet->pVConstantTable);
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
	hr = D3DXCompileShaderFromFile("hlsl/SpeclarPS.hlsl", NULL, NULL, "main", "ps_3_0", 0, &code, &err, &m_pShaderSet->pPConstantTable);
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

	// �s�N�Z���V�F�[�_�[�̃R���p�C��(�e�N�X�`������)
	//----------------------------------------------------------------
	hr = D3DXCompileShaderFromFile("hlsl/SpeclarPS.hlsl", NULL, NULL, "NoTexture", "ps_3_0", 0, &code, &err, &m_pShaderSet->pPConstantTableNoTex);
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

	// �����[�X
	if (err)err->Release();
	if (code)code->Release();
}
