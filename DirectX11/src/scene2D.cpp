// ======== ======== ======== ======== ======== ======== ======== ========
//
//
//	title : 3D�|���S���̃V�[��
//	Auter : KENSUKE WATANABE
//
//
// ======== ======== ======== ======== ======== ======== ======== ========
#include "main.h"
#include "manager.h"
#include "texManager.h"
#include "scene2D.h"
#include "input.h"
#include "renderer.h"
#include "camera.h"
#include "utility.h"

#include <assert.h>
#include <tchar.h>

ID3D11Buffer *CScene2D::m_pConstantBuffer2DCam = nullptr;

// -------- -------- -------- -------- -------- -------- -------- --------
// ������
// -------- -------- -------- -------- -------- -------- -------- --------
void CScene2D::Init(void)
{
	m_pVertexBuffer = nullptr;
	m_pIndexBuffer = nullptr;
	m_pTexSampler = nullptr;

	m_pConstantBufferMat = nullptr;
	m_pConstantBufferModel = nullptr;
	m_pTexSampler = nullptr;
	m_pVertexShader = nullptr;
	m_pPixelTexShader = nullptr;
	m_pVertexLayout = nullptr;

	// �ϐ��̏�����
	SetPos(XMFLOAT3(0.f, 0.f, 0.f));		// ���W
	SetSize(XMFLOAT3(1.f, 1.f, 1.f));		// �T�C�Y
	SetRot(XMFLOAT3(0.f, 0.f, 0.f));		// ��]��
	SetScl(XMFLOAT3(1.f, 1.f, 1.f));		// �g�嗦
	m_posUV = XMFLOAT2(0.f, 0.f);
	m_widthUV = 1.f;
	m_heightUV = 1.f;
}

// -------- -------- -------- -------- -------- -------- -------- --------
// ���_���̍쐬
// -------- -------- -------- -------- -------- -------- -------- --------
void CScene2D::CreateVtxBuffer(void)
{
	// �f�o�C�X�̎擾
	// -------- -------- -------- -------- -------- -------- -------- --------
	ID3D11DeviceContext* pDeviceContext = CRenderer::GetDeviceContext();
	ID3D11Device* pDevice = CManager::GetRenderer()->GetDevice();

	// �V�F�[�_�[�ǂݍ���
	BinFile vscode(L"data/SHADER/vs2D.cso");
	BinFile psTexcode(L"data/SHADER/ps2DTex.cso");
	BinFile pscode(L"data/SHADER/ps.cso");

	// ���_���W�̐ݒ�
	m_pVtxData[0].pos = XMFLOAT3(-0.5f, -0.5f, 0.f);
	m_pVtxData[1].pos = XMFLOAT3(0.5f, -0.5f, 0.f);
	m_pVtxData[2].pos = XMFLOAT3(-0.5f, 0.5f, 0.f);
	m_pVtxData[3].pos = XMFLOAT3(0.5f, 0.5f, 0.f);
	// ���_�F�̐ݒ�
	m_pVtxData[0].col = XMFLOAT4(1.f, 1.f, 1.f, 1.f);
	m_pVtxData[1].col = XMFLOAT4(1.f, 1.f, 1.f, 1.f);
	m_pVtxData[2].col = XMFLOAT4(1.f, 1.f, 1.f, 1.f);
	m_pVtxData[3].col = XMFLOAT4(1.f, 1.f, 1.f, 1.f);
	// UV���W�̐ݒ�
	m_pVtxData[0].tex = XMFLOAT2(0.f, 0.f);
	m_pVtxData[1].tex = XMFLOAT2(1.f, 0.f);
	m_pVtxData[2].tex = XMFLOAT2(0.f, 1.f);
	m_pVtxData[3].tex = XMFLOAT2(1.f, 1.f);

	// ���_�o�b�t�@�쐬
	// -------- -------- -------- -------- -------- -------- -------- --------
	D3D11_BUFFER_DESC bd;
	bd.ByteWidth = sizeof(VERTEX_2D) * NUM_VERTEX;		// �o�b�t�@�T�C�Y
	bd.Usage = D3D11_USAGE_DEFAULT;
	bd.BindFlags = D3D11_BIND_VERTEX_BUFFER;			// ���_�o�b�t�@�[�Ƃ��ăo�C���h
	bd.CPUAccessFlags = 0;
	bd.MiscFlags = 0;
	bd.StructureByteStride = 0;

	D3D11_SUBRESOURCE_DATA InitData;
	InitData.pSysMem = m_pVtxData;						// ���\�[�X�f�[�^�̃|�C���^
	InitData.SysMemPitch = 0;
	InitData.SysMemSlicePitch = 0;
	if (FAILED(pDevice->CreateBuffer(&bd, &InitData, &m_pVertexBuffer)))
	{
		MessageBox(nullptr, "���_�o�b�t�@�쐬���s", "Err", MB_ICONSTOP);
		return;
	}

	// �C���f�b�N�X�o�b�t�@�̍쐬
	// -------- -------- -------- -------- -------- -------- -------- --------
	WORD indices[] =
	{
		0, 1, 2, 3
	};
	bd.Usage = D3D11_USAGE_DEFAULT;
	bd.ByteWidth = sizeof(WORD) * 4;
	bd.BindFlags = D3D11_BIND_INDEX_BUFFER;
	bd.CPUAccessFlags = 0;
	InitData.pSysMem = indices;
	if (FAILED(pDevice->CreateBuffer(&bd, &InitData, &m_pIndexBuffer))) {
		MessageBox(nullptr, "�C���f�b�N�X�o�b�t�@�̍쐬���s", "Err", MB_ICONSTOP);
		return;
	}

	// ���̓��C�A�E�g��`
	D3D11_INPUT_ELEMENT_DESC layout[] = {
		{ "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT	, 0, 0 , D3D11_INPUT_PER_VERTEX_DATA, 0 },
		{ "COLOR"	, 0, DXGI_FORMAT_R32G32B32A32_FLOAT	, 0, 4*3, D3D11_INPUT_PER_VERTEX_DATA, 0 },
		{ "TEXCOORD", 0, DXGI_FORMAT_R32G32_FLOAT		, 0, 4*3+4*4, D3D11_INPUT_PER_VERTEX_DATA, 0 }
	};

	// ���̓��C�A�E�g�쐬
	m_pVertexLayout = nullptr;
	UINT elem_num = ARRAYSIZE(layout);
	if (FAILED(pDevice->CreateInputLayout(layout, elem_num, vscode.get(), vscode.size(), &m_pVertexLayout))) {
		MessageBox(nullptr, "���̓��C�A�E�g�쐬���s", "Err", MB_ICONSTOP);
		return;
	}
	
	// ���_�V�F�[�_�쐬
	if (FAILED(pDevice->CreateVertexShader(vscode.get(), vscode.size(), nullptr, &m_pVertexShader))) {
		MessageBox(nullptr, "���_�V�F�[�_�[�쐬���s", "Err", MB_ICONSTOP);
		return;
	}

	// �s�N�Z���V�F�[�_�쐬
	if (FAILED(pDevice->CreatePixelShader(pscode.get(), pscode.size(), nullptr, &m_pPixelTexShader))) {
		MessageBox(nullptr, "�s�N�Z���V�F�[�_�[�쐬���s", "Err", MB_ICONSTOP);
		return;
	}
	if (FAILED(pDevice->CreatePixelShader(psTexcode.get(), psTexcode.size(), nullptr, &m_pPixelTexShader))) {
		MessageBox(nullptr, "�s�N�Z���V�F�[�_�[(�e�N�X�`���p)�쐬���s", "Err", MB_ICONSTOP);
		return;
	}

	//�e�N�X�`���[�p�T���v���[�쐬
	D3D11_SAMPLER_DESC SamDesc;
	ZeroMemory(&SamDesc, sizeof(D3D11_SAMPLER_DESC));

	SamDesc.Filter = D3D11_FILTER_MIN_MAG_MIP_LINEAR;
	SamDesc.AddressU = D3D11_TEXTURE_ADDRESS_WRAP;
	SamDesc.AddressV = D3D11_TEXTURE_ADDRESS_WRAP;
	SamDesc.AddressW = D3D11_TEXTURE_ADDRESS_WRAP;
	pDevice->CreateSamplerState(&SamDesc, &m_pTexSampler);

	// �萔�o�b�t�@�쐬
	bd.Usage = D3D11_USAGE_DEFAULT;
	bd.ByteWidth = sizeof(CCamera::CBufferModel);
	bd.CPUAccessFlags = 0;
	if (FAILED(pDevice->CreateBuffer(&bd, NULL, &m_pConstantBufferModel))) {
		MessageBox(nullptr, "�萔�o�b�t�@�쐬���s", "Err", MB_ICONSTOP);
		return;
	}

	// �R���X�^���g�o�b�t�@�̍쐬
	if (m_pConstantBuffer2DCam == nullptr)
	{
		D3D11_BUFFER_DESC bd;
		ZeroMemory(&bd, sizeof(bd));
		bd.ByteWidth = sizeof(CCamera::ConstantBuffer);
		bd.Usage = D3D11_USAGE_DEFAULT;
		bd.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
		bd.CPUAccessFlags = 0;
		bd.MiscFlags = 0;
		bd.StructureByteStride = sizeof(float);
		if (FAILED(pDevice->CreateBuffer(&bd, nullptr, &m_pConstantBuffer2DCam)))
		{
			MessageBox(nullptr, "�R���X�^���g�o�b�t�@�̍쐬���s", "�x��", S_OK);
		}
	}
}

// -------- -------- -------- -------- -------- -------- -------- --------
// �I��
// -------- -------- -------- -------- -------- -------- -------- --------
void CScene2D::Uninit(void)
{
	SAFE_RELEASE(m_pVertexBuffer);
	SAFE_RELEASE(m_pIndexBuffer);
	SAFE_RELEASE(m_pTexSampler);

	SAFE_RELEASE(m_pConstantBufferMat);
	SAFE_RELEASE(m_pConstantBufferModel);
	SAFE_RELEASE(m_pTexSampler);
	SAFE_RELEASE(m_pVertexShader);
	SAFE_RELEASE(m_pPixelTexShader);
	SAFE_RELEASE(m_pVertexLayout);
}

// -------- -------- -------- -------- -------- -------- -------- --------
// �X�V
// -------- -------- -------- -------- -------- -------- -------- --------
void CScene2D::Update(void)
{
	ID3D11DeviceContext* pDeviceContext = CRenderer::GetDeviceContext();

	XMFLOAT3 pos = GetPos();
	XMFLOAT3 size = GetSize();
	XMFLOAT3 scl = GetScl();
	XMFLOAT3 rot = GetRot();
	float len = sqrt(size.x * size.x + size.y * size.y);
	float ang = atan2(size.y, size.x);
	m_pVtxData[0].pos = XMFLOAT3(cosf(ang + PI + rot.z) * len * scl.x + pos.x,
		sinf(ang + PI + rot.z) * len * scl.y + pos.y, 0.f);
	m_pVtxData[1].pos = XMFLOAT3(cosf(-ang + rot.z) * len * scl.x + pos.x,
		sinf(-ang + rot.z) * len * scl.y + pos.y, 0.f);
	m_pVtxData[2].pos = XMFLOAT3(cosf(-ang + PI + rot.z) * len * scl.x + pos.x,
		sinf(-ang + PI + rot.z) * len * scl.y + pos.y, 0.f);
	m_pVtxData[3].pos = XMFLOAT3(cosf(ang + rot.z) * len * scl.x + pos.x,
		sinf(ang + rot.z) * len * scl.y + pos.y, 0.f);

	// UV���W�̐ݒ�
	m_pVtxData[0].tex = XMFLOAT2(m_posUV.x, m_posUV.y);
	m_pVtxData[1].tex = XMFLOAT2(m_posUV.x + m_widthUV, m_posUV.y);
	m_pVtxData[2].tex = XMFLOAT2(m_posUV.x, m_posUV.y + m_heightUV);
	m_pVtxData[3].tex = XMFLOAT2(m_posUV.x + m_widthUV, m_posUV.y + m_heightUV);

	pDeviceContext->UpdateSubresource(m_pVertexBuffer, 0, nullptr, &m_pVtxData, 0, 0);
}

// -------- -------- -------- -------- -------- -------- -------- --------
// �`��
// -------- -------- -------- -------- -------- -------- -------- --------
void CScene2D::Draw(void)
{
	// �擾
	ID3D11DeviceContext* pDeviceContext = CRenderer::GetDeviceContext();
	CCamera *cam = CManager::GetCamera();

	// ���[���h�}�g���N�X�쐬
	// -------- -------- -------- --------
	XMFLOAT3 pos = GetPos();
	XMFLOAT3 size = GetSize();
	XMFLOAT3 scl = GetScl();
	XMFLOAT3 rot = GetRot();

	XMMATRIX mtxScl = XMMatrixScalingFromVector(XMLoadFloat3(&scl));				// �g�嗦
	XMMATRIX mtxRot = XMMatrixRotationRollPitchYaw(rot.x, rot.y, rot.z);			// �I�C���[�p�i�s�b�`(rx)�A���[(ry)�A���[��(rz))����s��
	XMMATRIX mtxPos = XMMatrixTranslationFromVector(XMLoadFloat3(&pos));			// ���W
	XMFLOAT4X4 mtxWorld = XM::F4X4::WorldMatrixMultiply(&mtxScl, &mtxRot, &mtxPos);	// �X�P�[���A��]�A�ړ��̍s�񍇐�

	// ���[���h�}�g���N�X���V�F�[�_�[�֓n������
	// -------- -------- -------- --------
	CCamera::CBufferModel mtx;
	mtx.mWorld = XMMatrixTranspose(XMLoadFloat4x4(&mtxWorld));// ���[���h�ϊ��s��
	mtx.mLight = cam->GetLightPos();
	pDeviceContext->UpdateSubresource(m_pConstantBufferModel, 0, nullptr, &mtx, 0, 0);

	// �ϐ��̍Đݒ�
	// -------- -------- -------- --------
	SetPos(pos);
	SetSize(size);
	SetScl(scl);
	SetRot(rot);

	//�����ˉe�ϊ��s��
	CRenderer *pRenderer = CManager::GetRenderer();
	XMFLOAT2 screensize = pRenderer->GetScreenSize();
	XMMATRIX hProjection = XMMATRIX(
		2 / screensize.x, 0.0f, 0.0f, 0.0f,
		0.0f, -2 / screensize.y, 0.0f, 0.0f,
		0.0f, 0.0f, 1.0f, 0.0f,
		-1.0f, 1.0f, 0.0f, 1.0f);
	//
	// �ϐ��̍X�V
	//
	CCamera::ConstantBuffer hConstantBuffer;
	hConstantBuffer.mView = XMMatrixTranspose(XMLoadFloat4x4(&cam->GetView()));
	hConstantBuffer.mProjection = XMMatrixTranspose(hProjection);
	pDeviceContext->UpdateSubresource(m_pConstantBuffer2DCam, 0, nullptr, &hConstantBuffer, 0, 0);
	CCamera::CBufferModel hBufferScene;
	hBufferScene.mWorld = XMMatrixTranspose(XMLoadFloat4x4(&cam->GetWorld()));
	hBufferScene.mLight = XMFLOAT4(0.f, 1.f, 0.f, 1.0f);
	pDeviceContext->UpdateSubresource(m_pConstantBufferModel, 0, nullptr, &hBufferScene, 0, 0);
	//�R���e�L�X�g�ɐݒ�
	pDeviceContext->VSSetConstantBuffers(0, 1, &m_pConstantBuffer2DCam);

	// ���C�A�E�g�̐ݒ�
	// -------- -------- -------- --------
	UINT hStrides = sizeof(VERTEX_2D);
	UINT hOffsets = 0;
	pDeviceContext->IASetInputLayout(m_pVertexLayout);									// ���̓��C�A�E�g���R���e�L�X�g�ɐݒ�
	pDeviceContext->IASetVertexBuffers(0, 1, &m_pVertexBuffer, &hStrides, &hOffsets);	// ���_�o�b�t�@���R���e�L�X�g�ɐݒ�
	pDeviceContext->IASetIndexBuffer(m_pIndexBuffer, DXGI_FORMAT_R16_UINT, 0);			// �C���f�b�N�X�o�b�t�@�̐ݒ�
	pDeviceContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLESTRIP);		// �v���~�e�B�u

	// �e�N�X�`���̎擾
	ID3D11ShaderResourceView *pTex = CManager::GetTexManager()->GetTexInterface(m_strFilePass);

	// �V�F�[�_�[�̐ݒ�
	// -------- -------- -------- --------
	pDeviceContext->VSSetShader(m_pVertexShader, nullptr, 0);				// ���_�V�F�[�_�[���R���e�L�X�g�ɐݒ�
	pDeviceContext->VSSetConstantBuffers(1, 1, &m_pConstantBufferModel);	// ���[���h���W���V�F�[�_�[�ɓn��
	if (pTex)
	{	
		// �e�N�X�`���[���V�F�[�_�[�ɓn��
		pDeviceContext->PSSetSamplers(0, 1, &m_pTexSampler);
		pDeviceContext->PSSetShaderResources(0, 1, &pTex);
		// �e�N�X�`���p�s�N�Z���V�F�[�_�[ 
		pDeviceContext->PSSetShader(m_pPixelTexShader, nullptr, 0);
	}
	else
	{
		// �e�N�X�`�������p�s�N�Z���V�F�[�_�[
		pDeviceContext->PSSetShader(m_pPixelShader, nullptr, 0);
	}

	// �v���~�e�B�u�������_�����O
	pDeviceContext->DrawIndexed(4, 0, 0);

	// ��芸�����߂�
	pDeviceContext->VSSetShader(nullptr, nullptr, 0);	// ���_�V�F�[�_�[���R���e�L�X�g�ɐݒ�
	pDeviceContext->PSSetShader(nullptr, nullptr, 0);	// �s�N�Z���V�F�[�_�[���R���e�L�X�g�ɐݒ�
}

// -------- -------- -------- -------- -------- -------- -------- --------
// �쐬����
// -------- -------- -------- -------- -------- -------- -------- --------
CScene2D *CScene2D::Create(const XMFLOAT2 &pos, const float &rot, const XMFLOAT2 &scl, const std::string &strFilePass, int priority)
{
	// CScene2D�̐���
	CScene2D *pScene2D;
	pScene2D = new CScene2D(priority);
	pScene2D->Init();

	pScene2D->SetPos(XMFLOAT3(pos.x, pos.y, 0.f));
	pScene2D->SetRot(XMFLOAT3(0.f, 0.f, rot));
	pScene2D->SetSize(XMFLOAT3(scl.x, scl.y, 1.f));
	pScene2D->SetScl(XMFLOAT3(1.f, 1.f, 1.f));
	pScene2D->m_strFilePass = strFilePass;

	// ���_, �V�F�[�_�[�̍쐬
	pScene2D->CreateVtxBuffer();
	
	// �e�N�X�`���̃Z�b�g
	CManager::GetTexManager()->SetTex(strFilePass);

	return pScene2D;
}
