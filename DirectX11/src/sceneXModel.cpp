// ======== ======== ======== ======== ======== ======== ======== ========
//
//
//	title : X���f���̃V�[��
//	Auter : KENSUKE WATANABE
//
//
// ======== ======== ======== ======== ======== ======== ======== ========
#include "manager.h"
#include "texManager.h"
#include "modelManager.h"
#include "XModelLoader.h"
#include "sceneXModel.h"
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
void CSceneXModel::Init(void)
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
void CSceneXModel::CreateVtxBuffer(void)
{
	// �f�o�C�X�̎擾
	// -------- -------- -------- -------- -------- -------- -------- --------
	ID3D11DeviceContext* pDeviceContext = CRenderer::GetDeviceContext();
	ID3D11Device* pDevice = CManager::GetRenderer()->GetDevice();

	// �V�F�[�_�[�ǂݍ���
	BinFile vscode(L"data/SHADER/vs3D.cso");
	BinFile psTexcode(L"data/SHADER/psTex.cso");
	BinFile pscode(L"data/SHADER/ps.cso");

	// ���̓��C�A�E�g��`
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
	if (FAILED(pDevice->CreatePixelShader(pscode.get(), pscode.size(), nullptr, &m_pPixelShader))) {
		MessageBox(nullptr, "�s�N�Z���V�F�[�_�[�쐬���s", "Err", MB_ICONSTOP);
		return;
	}
	if (FAILED(pDevice->CreatePixelShader(psTexcode.get(), psTexcode.size(), nullptr, &m_pPixelTexShader))) {
		MessageBox(nullptr, "�s�N�Z���V�F�[�_�[(�e�N�X�`���p)�쐬���s", "Err", MB_ICONSTOP);
		return;
	}

	// ���b�V���������ꂽ���f���̎擾
	// -------- -------- -------- -------- -------- -------- -------- --------
	std::vector<XModel::MESH*> *vecpModel = CManager::GetXModelManager()->GetModel(m_strFilePass);
	m_vecModelBuffer.resize(vecpModel->size());
	int cntMesh = 0;
	for (auto pModel : *vecpModel)
	{
		// ���b�V���̒��_�f�[�^
		m_vecModelBuffer.at(cntMesh).vecVtxData.resize(pModel->face.size() * 3);
		int cnt = 0;
		for (int i = 0; i < static_cast<int>(pModel->face.size()); i++)
		{
			for (int idx = 0; idx < 3; idx++)
			{
				m_vecModelBuffer.at(cntMesh).vecVtxData[cnt].pos = pModel->face.at(i).vtx[idx].pos;
				m_vecModelBuffer.at(cntMesh).vecVtxData[cnt].nom = pModel->face.at(i).vtx[idx].nom;
				m_vecModelBuffer.at(cntMesh).vecVtxData[cnt].col = XM::F4::Mul(pModel->face.at(i).vtx[idx].col, pModel->mat.matCol);
				m_vecModelBuffer.at(cntMesh).vecVtxData[cnt].tex = pModel->face.at(i).vtx[idx].tex;
				cnt++;
			}
		}

		// ���_�o�b�t�@�쐬
		// -------- -------- -------- -------- -------- -------- -------- --------
		D3D11_BUFFER_DESC bd;
		bd.ByteWidth = sizeof(VERTEX_3D) * static_cast<int>(m_vecModelBuffer.at(cntMesh).vecVtxData.size());		// �o�b�t�@�T�C�Y
		bd.Usage = D3D11_USAGE_DEFAULT;
		bd.BindFlags = D3D11_BIND_VERTEX_BUFFER;					// ���_�o�b�t�@�[�Ƃ��ăo�C���h
		bd.CPUAccessFlags = 0;
		bd.MiscFlags = 0;
		bd.StructureByteStride = 0;

		D3D11_SUBRESOURCE_DATA InitData;
		InitData.pSysMem = &m_vecModelBuffer.at(cntMesh).vecVtxData[0];						// ���\�[�X�f�[�^�̃|�C���^
		InitData.SysMemPitch = 0;
		InitData.SysMemSlicePitch = 0;
		if (FAILED(pDevice->CreateBuffer(&bd, &InitData, &m_vecModelBuffer.at(cntMesh).pVertexBuffer)))
		{
			MessageBox(nullptr, "���_�o�b�t�@�쐬���s", "Err", MB_ICONSTOP);
			return;
		}

		// �C���f�b�N�X�o�b�t�@�̍쐬
		// -------- -------- -------- -------- -------- -------- -------- --------
		std::vector<WORD> m_vecIdxData;
		for (int i = 0; i < static_cast<int>(pModel->face.size()); i++)
		{
			m_vecIdxData.push_back(static_cast<WORD>(i * 3 + 0));
			m_vecIdxData.push_back(static_cast<WORD>(i * 3 + 1));
			m_vecIdxData.push_back(static_cast<WORD>(i * 3 + 2));
		}
		m_vecModelBuffer.at(cntMesh).numIdx = static_cast<int>(m_vecIdxData.size());

		bd.Usage = D3D11_USAGE_DEFAULT;
		bd.ByteWidth = sizeof(WORD) * m_vecModelBuffer.at(cntMesh).numIdx;
		bd.BindFlags = D3D11_BIND_INDEX_BUFFER;
		bd.CPUAccessFlags = 0;
		InitData.pSysMem = &m_vecIdxData[0];
		if (FAILED(pDevice->CreateBuffer(&bd, &InitData, &m_vecModelBuffer.at(cntMesh).pIndexBuffer))) {
			MessageBox(nullptr, "�C���f�b�N�X�o�b�t�@�̍쐬���s", "Err", MB_ICONSTOP);
			return;
		}

		//�e�N�X�`���[�p�T���v���[�쐬
		D3D11_SAMPLER_DESC SamDesc;
		ZeroMemory(&SamDesc, sizeof(D3D11_SAMPLER_DESC));

		SamDesc.Filter = D3D11_FILTER_MIN_MAG_MIP_LINEAR;
		SamDesc.AddressU = D3D11_TEXTURE_ADDRESS_WRAP;
		SamDesc.AddressV = D3D11_TEXTURE_ADDRESS_WRAP;
		SamDesc.AddressW = D3D11_TEXTURE_ADDRESS_WRAP;
		pDevice->CreateSamplerState(&SamDesc, &m_vecModelBuffer.at(cntMesh).pTexSampler);

		// �萔�o�b�t�@�쐬
		bd.Usage = D3D11_USAGE_DEFAULT;
		bd.ByteWidth = sizeof(CCamera::CBufferModel);
		bd.CPUAccessFlags = 0;
		if (FAILED(pDevice->CreateBuffer(&bd, NULL, &m_pConstantBufferModel))) {
			MessageBox(nullptr, "�萔�o�b�t�@�쐬���s", "Err", MB_ICONSTOP);
			return;
		}

		cntMesh++;
	}
}

// -------- -------- -------- -------- -------- -------- -------- --------
// �I��
// -------- -------- -------- -------- -------- -------- -------- --------
void CSceneXModel::Uninit(void)
{
	for (auto modelBuffer : m_vecModelBuffer)
	{
		SAFE_RELEASE(modelBuffer.pVertexBuffer);
		SAFE_RELEASE(modelBuffer.pIndexBuffer);
		SAFE_RELEASE(modelBuffer.pTexSampler);
	}
}

// -------- -------- -------- -------- -------- -------- -------- --------
// �X�V
// -------- -------- -------- -------- -------- -------- -------- --------
void CSceneXModel::Update(void)
{

}

// -------- -------- -------- -------- -------- -------- -------- --------
// �`��
// -------- -------- -------- -------- -------- -------- -------- --------
void CSceneXModel::Draw(void)
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

	// ���b�V���������ꂽ���f���̎擾
	std::vector<XModel::MESH*> *vecpModel = CManager::GetXModelManager()->GetModel(m_strFilePass);

	// ���b�V�����ɕ`��
	// -------- -------- -------- -------- -------- -------- -------- --------	
	int cntMesh = 0;
	for (auto pModel : *vecpModel)
	{
		// �v���~�e�B�u
		// -------- -------- -------- -------- -------- -------- -------- --------	
		UINT hStrides = sizeof(VERTEX_3D);
		UINT hOffsets = 0;
		pDeviceContext->IASetInputLayout(m_pVertexLayout);																// ���̓��C�A�E�g���R���e�L�X�g�ɐݒ�
		pDeviceContext->IASetVertexBuffers(0, 1, &m_vecModelBuffer.at(cntMesh).pVertexBuffer, &hStrides, &hOffsets);	// ���_�o�b�t�@���R���e�L�X�g�ɐݒ�
		pDeviceContext->IASetIndexBuffer(m_vecModelBuffer.at(cntMesh).pIndexBuffer, DXGI_FORMAT_R16_UINT, 0);			// �C���f�b�N�X�o�b�t�@�̐ݒ�
		pDeviceContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);									// �v���~�e�B�u

		// �e�N�X�`���̎擾
		ID3D11ShaderResourceView *pTex = CManager::GetTexManager()->GetTexInterface(pModel->mat.filePass);

		// �V�F�[�_�[�̐ݒ�
		// -------- -------- -------- -------- -------- -------- -------- --------
		// ���_�V�F�[�_�[���R���e�L�X�g�ɐݒ�
		pDeviceContext->VSSetShader(m_pVertexShader, nullptr, 0);
		// ���[���h���W���V�F�[�_�[�ɓn��
		pDeviceContext->VSSetConstantBuffers(1, 1, &m_pConstantBufferModel);

		// �e�N�X�`���̗L���ɂ���ăs�N�Z���V�F�[�_�[�𕪂���
		// -------- -------- -------- -------- -------- -------- -------- --------	
		if (pTex)
		{
			// �e�N�X�`���p�s�N�Z���V�F�[�_�[ 
			pDeviceContext->PSSetShader(m_pPixelTexShader, nullptr, 0);
			// �e�N�X�`���[���V�F�[�_�[�ɓn��
			pDeviceContext->PSSetSamplers(0, 1, &m_vecModelBuffer.at(cntMesh).pTexSampler);
			pDeviceContext->PSSetShaderResources(0, 1, &pTex);;
		}
		else
		{
			// �e�N�X�`�������p�s�N�Z���V�F�[�_�[ 
			pDeviceContext->PSSetShader(m_pPixelShader, nullptr, 0);
		}

		// �v���~�e�B�u�������_�����O
		pDeviceContext->DrawIndexed(m_vecModelBuffer.at(cntMesh).numIdx, 0, 0);

		// ��芸�����߂�
		pDeviceContext->VSSetShader(nullptr, nullptr, 0);	// ���_�V�F�[�_�[���R���e�L�X�g�ɐݒ�
		pDeviceContext->PSSetShader(nullptr, nullptr, 0);	// �s�N�Z���V�F�[�_�[���R���e�L�X�g�ɐݒ�

		cntMesh++;
	}
}

// -------- -------- -------- -------- -------- -------- -------- --------
// �쐬����
// -------- -------- -------- -------- -------- -------- -------- --------
CSceneXModel *CSceneXModel::Create(const XMFLOAT3 &pos, const XMFLOAT3 &rot, const XMFLOAT3 &scl, const std::string &strFilePass, int priority)
{
	// CSceneXModel�̐���
	CSceneXModel *pScene3D;
	pScene3D = new CSceneXModel(priority);
	pScene3D->Init();

	pScene3D->SetPos(pos);
	pScene3D->SetRot(rot);
	pScene3D->SetScl(scl);
	pScene3D->m_strFilePass = strFilePass;

	// ���f���̍쐬
	CManager::GetXModelManager()->SetModel(strFilePass);

	// ���_, �V�F�[�_�[�̍쐬
	pScene3D->CreateVtxBuffer();

	return pScene3D;
}
