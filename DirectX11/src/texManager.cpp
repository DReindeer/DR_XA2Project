// ======== ======== ======== ======== ======== ======== ======== ========
//
//
//	title : �e�N�X�`���Ǘ�
//	Auter : KENSUKE WATANABE
//
//
// ======== ======== ======== ======== ======== ======== ======== ========
#include "texManager.h"
#include "renderer.h"
#include "manager.h"
#include "utility.h"
#include <assert.h>
#include <algorithm>

// ======== ======== ======== ======== ======== ======== ======== ========
// ������
// -------- -------- -------- -------- -------- -------- -------- --------
void CTexManager::Init()
{
	m_unmapTexInterface.clear();
}

// ======== ======== ======== ======== ======== ======== ======== ========
// �I��
// -------- -------- -------- -------- -------- -------- -------- --------
void CTexManager::Uninit()
{
	ReleaseAll();
}

// ======== ======== ======== ======== ======== ======== ======== ========
// �e�N�X�`���ݒ�
// -------- -------- -------- -------- -------- -------- -------- --------
bool CTexManager::SetTex(const std::string &strFilePass)
{
	// null�`�F�b�N
	if(strFilePass.empty()){return false;}

	// �v�f�̗L�����m���߂�
	if(m_unmapTexInterface.count(strFilePass) == 1)
	{// ����܂���
		return true;
	}

	// �f�o�C�X�̎擾
	ID3D11DeviceContext *pDeviceContext = CRenderer::GetDeviceContext();
	ID3D11Device *pDevice = CManager::GetRenderer()->GetDevice();

	ID3D11ShaderResourceView *pTexture = nullptr;

	// �e�N�X�`���ǂݍ���
	TexMetadata metadata;
	DirectX::ScratchImage image;
	wchar_t *filePass = Utility::Cast::CastWChae_t(&strFilePass[0]);
	if (FAILED(LoadFromWICFile(filePass, 0, &metadata, image))) {
		MessageBox(nullptr, "�e�N�X�`���ǂݍ��ݎ��s", "�x��", S_OK);
		if(filePass)delete filePass;
		return false;
	}
	if (filePass)delete filePass;

	// �摜����V�F�[�_���\�[�XView DirectXTex
	if (FAILED(CreateShaderResourceView(pDevice, image.GetImages(), image.GetImageCount(), metadata, &pTexture))) {
		MessageBox(nullptr, "�V�F�[�_���\�[�XView�������s", "�x��", S_OK);
		return false;
	}

	// �v�f�̒ǉ�
	m_unmapTexInterface.insert(std::make_pair(strFilePass, pTexture));

	// �ǉ����������܂���
	return true;
}

// ======== ======== ======== ======== ======== ======== ======== ========
// �e�N�X�`���擾
// -------- -------- -------- -------- -------- -------- -------- --------
ID3D11ShaderResourceView *CTexManager::GetTexInterface(const std::string &strFilePass)
{
	// null�`�F�b�N
	if(strFilePass.empty())
	{
		return nullptr;
	}

	// �v�f�̗L�����m���߂�
	if(m_unmapTexInterface.count(strFilePass) == 1)
	{// ����܂���
		return m_unmapTexInterface.at(strFilePass);
	}

	// ���̗v�f�͖����ł�
	return nullptr;
}

// ======== ======== ======== ======== ======== ======== ======== ========
// �e�N�X�`���̗L�����m�F
// -------- -------- -------- -------- -------- -------- -------- --------
bool CTexManager::CheckTex(const std::string &strFilePass)
{
	// null�`�F�b�N
	if (strFilePass.empty())
	{
		assert(!"�e�N�X�`���̃t�@�C���̎w�肪����Ă��܂���");
	}

	// �v�f�̗L�����m���߂�
	if (m_unmapTexInterface.count(strFilePass) == 1)
	{// ����܂���
		return true;
	}

	// ���̗v�f�͖����ł�
	return false;
}

// ======== ======== ======== ======== ======== ======== ======== ========
// �e�N�X�`�����
// -------- -------- -------- -------- -------- -------- -------- --------
bool CTexManager::Release(const std::string &strFilePass)
{
	// �󂩊m���߂�
	if(strFilePass.empty()){return false;}
	
	// �v�f�̗L�����m���߂�
	if(m_unmapTexInterface.count(strFilePass) == 1)
	{// ����܂���
		ID3D11ShaderResourceView *pTex = m_unmapTexInterface.at(strFilePass);
		if (pTex)pTex->Release();						// �o�b�t�@�̉��
		m_unmapTexInterface.erase(strFilePass);			// �v�f�̍폜

		return true;
	}
	return false;
}

// ======== ======== ======== ======== ======== ======== ======== ========
// �e�N�X�`���S���
// -------- -------- -------- -------- -------- -------- -------- --------
void CTexManager::ReleaseAll(void)
{
	ID3D11ShaderResourceView *pTex = nullptr;

	// �R���e�i�𖖔��܂Ń��[�v
	for(auto it : m_unmapTexInterface)
	{
		if(it.second != nullptr) 
		{
			pTex = it.second;
			if(pTex)pTex->Release(); 	// �o�b�t�@�̉��
		}
	}
	m_unmapTexInterface.clear();				// �R���e�i�̃N���A
}