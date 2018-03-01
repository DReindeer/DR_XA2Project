// ======== ======== ======== ======== ======== ======== ======== ========
//
//
//	title : BOX�̃V�[��(�g��Ȃ�����)
//	Auter : KENSUKE WATANABE
//
//
// ======== ======== ======== ======== ======== ======== ======== ========
#include "manager.h"
#include "texManager.h"
#include "sceneBox.h"
#include "input.h"
#include "renderer.h"
#include "camera.h"
#include "inputKeyboard.h"
#include "utility.h"

#include <assert.h>
#include <tchar.h>

// -------- -------- -------- -------- -------- -------- -------- --------
// ������
// -------- -------- -------- -------- -------- -------- -------- --------
void CSceneBox::Init(void)
{
	// �ϐ��̏�����
	SetPos(XMFLOAT3(0.f, 0.f, 0.f));		// ���W
	SetSize(XMFLOAT3(1.f, 1.f, 1.f));		// �T�C�Y
	SetRot(XMFLOAT3(0.f, 0.f, 0.f));		// ��]��
	SetScl(XMFLOAT3(1.f, 1.f, 1.f));		// �g�嗦
	m_move = XMFLOAT3(0.f, 0.f, 0.f);
	m_posUV = XMFLOAT2(0.f, 0.f);
	m_widthUV = 1.f;
	m_heightUV = 1.f;
}

// -------- -------- -------- -------- -------- -------- -------- --------
// ���_���̍쐬
// -------- -------- -------- -------- -------- -------- -------- --------
void CSceneBox::CreateVtxBuffer(void)
{
	// �f�o�C�X�̎擾
	// -------- -------- -------- -------- -------- -------- -------- --------
	ID3D11DeviceContext* pDeviceContext = CRenderer::GetDeviceContext();
	ID3D11Device* pDevice = CManager::GetRenderer()->GetDevice();

	// �V�F�[�_�[�ǂݍ���
	BinFile vscode(L"data/SHADER/vs3D.cso");
	BinFile psTexcode(L"data/SHADER/psTex.cso");
	BinFile pscode(L"data/SHADER/ps.cso");

	// ���_���W�̐ݒ�
	m_pVtxData[0].pos = XMFLOAT3(-0.5f, -0.5f, -0.5f);
	m_pVtxData[1].pos = XMFLOAT3(-0.5f, 0.5f, -0.5f);
	m_pVtxData[2].pos = XMFLOAT3(0.5f, -0.5f, -0.5f);
	m_pVtxData[3].pos = XMFLOAT3(0.5f, 0.5f, -0.5f);
	m_pVtxData[4].pos = XMFLOAT3(0.5f, -0.5f, 0.5f);
	m_pVtxData[5].pos = XMFLOAT3(0.5f, 0.5f, 0.5f);
	m_pVtxData[6].pos = XMFLOAT3(-0.5f, -0.5f, 0.5f);
	m_pVtxData[7].pos = XMFLOAT3(-0.5f, 0.5f, 0.5f);
	m_pVtxData[8].pos = XMFLOAT3(-0.5f, -0.5f, -0.5f);
	m_pVtxData[9].pos = XMFLOAT3(-0.5f, 0.5f, -0.5f);

	m_pVtxData[10].pos = XMFLOAT3(-0.5f, 0.5f, -0.5f);
	m_pVtxData[11].pos = XMFLOAT3(-0.5f, 0.5f, 0.5f);
	m_pVtxData[12].pos = XMFLOAT3(0.5f, 0.5f, -0.5f);
	m_pVtxData[13].pos = XMFLOAT3(0.5f, 0.5f, 0.5f);

	m_pVtxData[14].pos = XMFLOAT3(-0.5f, -0.5f, 0.5f);
	m_pVtxData[15].pos = XMFLOAT3(-0.5f, -0.5f, -0.5f);
	m_pVtxData[16].pos = XMFLOAT3(0.5f, -0.5f, 0.5f);
	m_pVtxData[17].pos = XMFLOAT3(0.5f, -0.5f, -0.5f);

	// �@���̐ݒ�
	m_pVtxData[0].nom = XM::F3::Normalize(XMFLOAT3(-1.f, -1.f, -1.f));
	m_pVtxData[1].nom = XM::F3::Normalize(XMFLOAT3(-1.f, 1.f, -1.f));
	m_pVtxData[2].nom = XM::F3::Normalize(XMFLOAT3(1.f, -1.f, -1.f));
	m_pVtxData[3].nom = XM::F3::Normalize(XMFLOAT3(1.f, 1.f, -1.f));
	m_pVtxData[4].nom = XM::F3::Normalize(XMFLOAT3(1.f, -1.f, 1.f));
	m_pVtxData[5].nom = XM::F3::Normalize(XMFLOAT3(1.f, 1.f, 1.f));
	m_pVtxData[6].nom = XM::F3::Normalize(XMFLOAT3(-1.f, -1.f, 1.f));
	m_pVtxData[7].nom = XM::F3::Normalize(XMFLOAT3(-1.f, 1.f, 1.f));
	m_pVtxData[8].nom = XM::F3::Normalize(XMFLOAT3(-1.f, -1.f, -1.f));
	m_pVtxData[9].nom = XM::F3::Normalize(XMFLOAT3(-1.f, 1.f, -1.f));

	m_pVtxData[10].nom = XM::F3::Normalize(XMFLOAT3(-1.f, 1.f, -1.f));
	m_pVtxData[11].nom = XM::F3::Normalize(XMFLOAT3(-1.f, 1.f, 1.f));
	m_pVtxData[12].nom = XM::F3::Normalize(XMFLOAT3(1.f, 1.f, -1.f));
	m_pVtxData[13].nom = XM::F3::Normalize(XMFLOAT3(1.f, 1.f, 1.f));

	m_pVtxData[14].nom = XM::F3::Normalize(XMFLOAT3(-1.f, -1.f, 1.f));
	m_pVtxData[15].nom = XM::F3::Normalize(XMFLOAT3(-1.f, -1.f, -1.f));
	m_pVtxData[16].nom = XM::F3::Normalize(XMFLOAT3(1.f, -1.f, 1.f));
	m_pVtxData[17].nom = XM::F3::Normalize(XMFLOAT3(1.f, -1.f, -1.f));
	
	// UV���W�̐ݒ�
	m_pVtxData[0].tex = XMFLOAT2(0.f, 1.f);
	m_pVtxData[1].tex = XMFLOAT2(0.f, 0.f);
	m_pVtxData[2].tex = XMFLOAT2(1.f, 1.f);
	m_pVtxData[3].tex = XMFLOAT2(1.f, 0.f);
	m_pVtxData[4].tex = XMFLOAT2(2.f, 1.f);
	m_pVtxData[5].tex = XMFLOAT2(2.f, 0.f);
	m_pVtxData[6].tex = XMFLOAT2(3.f, 1.f);
	m_pVtxData[7].tex = XMFLOAT2(3.f, 0.f);
	m_pVtxData[8].tex = XMFLOAT2(4.f, 1.f);
	m_pVtxData[9].tex = XMFLOAT2(4.f, 0.f);

	m_pVtxData[10].tex = XMFLOAT2(0.f, 1.f);
	m_pVtxData[11].tex = XMFLOAT2(0.f, 0.f);
	m_pVtxData[12].tex = XMFLOAT2(1.f, 1.f);
	m_pVtxData[13].tex = XMFLOAT2(1.f, 0.f);

	m_pVtxData[14].tex = XMFLOAT2(0.f, 0.f);
	m_pVtxData[15].tex = XMFLOAT2(0.f, 1.f);
	m_pVtxData[16].tex = XMFLOAT2(1.f, 0.f);
	m_pVtxData[17].tex = XMFLOAT2(1.f, 1.f);

	for (int i = 0; i < 18; i++)
	{
		// ���_�F�̐ݒ�
		m_pVtxData[i].col = XMFLOAT4(1.f, 1.f, 1.f, 1.f);
	}

	// ���_�o�b�t�@�쐬
	// -------- -------- -------- -------- -------- -------- -------- --------
	D3D11_BUFFER_DESC bd;
	bd.ByteWidth = sizeof(VERTEX_3D) * NUM_BOX_VERTEX;		// �o�b�t�@�T�C�Y
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
		0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 9, 
		10, 10, 11, 12, 13, 13, 
		14, 14, 15, 16, 17
	};
	bd.Usage = D3D11_USAGE_DEFAULT;
	bd.ByteWidth = sizeof(WORD) * 22;
	bd.BindFlags = D3D11_BIND_INDEX_BUFFER;
	bd.CPUAccessFlags = 0;
	InitData.pSysMem = indices;
	if (FAILED(pDevice->CreateBuffer(&bd, &InitData, &m_pIndexBuffer))){
		MessageBox(nullptr, "�C���f�b�N�X�o�b�t�@�̍쐬���s", "Err", MB_ICONSTOP);
		return;
	}

	// ���̓��C�A�E�g��`
	// �T�Ԗڂ̃p�����[�^�͐擪����̃o�C�g���Ȃ̂ŁCCOLOR�ɂ�POSITION��float�^4�o�C�g�~3���L�q
	D3D11_INPUT_ELEMENT_DESC layout[] = {
		{ "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT	, 0, 0 , D3D11_INPUT_PER_VERTEX_DATA, 0 },
		{ "NORMAL", 0, DXGI_FORMAT_R32G32B32_FLOAT		, 0, 4 * 3, D3D11_INPUT_PER_VERTEX_DATA, 0 },
		{ "COLOR"	, 0, DXGI_FORMAT_R32G32B32A32_FLOAT	, 0, 4 * 3 * 2, D3D11_INPUT_PER_VERTEX_DATA, 0 },
		{ "TEXCOORD", 0, DXGI_FORMAT_R32G32_FLOAT		, 0, 4 * 3 * 2 + 4 * 4, D3D11_INPUT_PER_VERTEX_DATA, 0 }
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
}

// -------- -------- -------- -------- -------- -------- -------- --------
// �I��
// -------- -------- -------- -------- -------- -------- -------- --------
void CSceneBox::Uninit(void)
{
	SAFE_RELEASE(m_pVertexBuffer);
	SAFE_RELEASE(m_pIndexBuffer);
	SAFE_RELEASE(m_pTexSampler);
}

// -------- -------- -------- -------- -------- -------- -------- --------
// �X�V
// -------- -------- -------- -------- -------- -------- -------- --------
void CSceneBox::Update(void)
{

}

// -------- -------- -------- -------- -------- -------- -------- --------
// �`��
// -------- -------- -------- -------- -------- -------- -------- --------
void CSceneBox::Draw(void)
{
	// D3Device�̎擾
	ID3D11DeviceContext* pDeviceContext = CRenderer::GetDeviceContext();
	CCamera *cam = CManager::GetCamera();

	XMFLOAT3 pos = GetPos();
	XMFLOAT3 size = GetSize();
	XMFLOAT3 scl = GetScl();
	XMFLOAT3 rot = GetRot();
	XMFLOAT3 move = GetMove();

	// ���[���h�}�g���N�X�쐬
	// -------- -------- -------- --------
	XMMATRIX mtxScl = XMMatrixScalingFromVector(XMLoadFloat3(&scl));							// �g��
	XMMATRIX mtxRot;																			// 
	XMFLOAT3 up = XMFLOAT3(0.f, 1.f, 0.f);														// 
	XMVECTOR lookAt = XMLoadFloat3(&pos) + (XMLoadFloat3(&XM::F3::LookAt(rot.y + PI * 0.5f)));	// 
	XM::CalcLookAtMatrixAxisFix(&mtxRot, &XMLoadFloat3(&pos), &lookAt, &XMLoadFloat3(&up));		// ��]
	XMMATRIX mtxPos = XMMatrixTranslationFromVector(XMLoadFloat3(&pos));						// ���W
	XMFLOAT4X4 mtxWorld = XM::F4X4::WorldMatrixMultiply(&mtxScl, &mtxRot, &mtxPos);				// �X�P�[���A��]�A�ړ��̍s�񍇐�

	// ���[���h�}�g���N�X���V�F�[�_�[�֓n������
	// -------- -------- -------- --------
	CCamera::CBufferModel mtx;
	mtx.mWorld = XMMatrixTranspose(XMLoadFloat4x4(&mtxWorld));	// ���[���h�ϊ��s��
	mtx.mLight = cam->GetLightPos();
	pDeviceContext->UpdateSubresource(m_pConstantBufferModel, 0, nullptr, &mtx, 0, 0);

	// ���C�A�E�g�̐ݒ�
	// -------- -------- -------- --------
	UINT hStrides = sizeof(VERTEX_3D);
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
		// �e�N�X�`���p�s�N�Z���V�F�[�_�[ 
		pDeviceContext->PSSetShader(m_pPixelTexShader, nullptr, 0);
		// �e�N�X�`���[���V�F�[�_�[�ɓn��
		pDeviceContext->PSSetSamplers(0, 1, &m_pTexSampler);
		pDeviceContext->PSSetShaderResources(0, 1, &pTex);
	}
	else
	{
		// �e�N�X�`�������p�s�N�Z���V�F�[�_�[ 
		pDeviceContext->PSSetShader(m_pPixelShader, nullptr, 0);
	}

	// �v���~�e�B�u�������_�����O
	pDeviceContext->DrawIndexed(22, 0, 0);

	// ��芸�����߂�
	pDeviceContext->VSSetShader(nullptr, nullptr, 0);	// ���_�V�F�[�_�[���R���e�L�X�g�ɐݒ�
	pDeviceContext->PSSetShader(nullptr, nullptr, 0);	// �s�N�Z���V�F�[�_�[���R���e�L�X�g�ɐݒ�
}

// -------- -------- -------- -------- -------- -------- -------- --------
// �쐬����
// -------- -------- -------- -------- -------- -------- -------- --------
CSceneBox *CSceneBox::Create(const XMFLOAT3 &pos, const XMFLOAT3 &rot, const XMFLOAT3 &size, const std::string &strFilePass, int priority)
{
	// CSceneBox�̐���
	CSceneBox *pScene3D;
	pScene3D = new CSceneBox(priority);
	pScene3D->Init();

	pScene3D->SetPos(pos);
	pScene3D->SetRot(rot);
	pScene3D->SetSize(size);
	pScene3D->m_strFilePass = strFilePass;

	// ���_, �V�F�[�_�[�̍쐬
	pScene3D->CreateVtxBuffer();

	// �e�N�X�`���̃Z�b�g
	CManager::GetTexManager()->SetTex(strFilePass);

	return pScene3D;
}
