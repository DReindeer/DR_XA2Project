//================================================================================
//
//	Auter : KENSUKE WATANABE
//	Data  : 2018/02/10
//
//--------------------------------------------------------------------------------
#include "texManager.h"
#include "renderer.h"
#include "manager.h"

#include <mutex>
//================================================================================
// ������
//--------------------------------------------------------------------------------
void CTexManager::Init()
{
	m_umapTexInterface.clear();
}

//================================================================================
// �I��
//--------------------------------------------------------------------------------
void CTexManager::Uninit()
{
	ReleaseAll();
}

//================================================================================
// �e�N�X�`���ݒ�
//--------------------------------------------------------------------------------
bool CTexManager::SetTex(const std::string &strFilePass)
{
	// null�`�F�b�N
	if(strFilePass.empty()){return false;}

	// �v�f�̗L�����m���߂�
	if(m_umapTexInterface.count(strFilePass) == 1)
	{// ����܂���
		return true;
	}

	// �X���b�h�Z�[�t(���b�N)
#ifdef THREAD_ON
	std::unique_lock<std::recursive_mutex> locker = CManager::Locker();
#endif

	// �f�o�C�X�̎擾
	LPDIRECT3DDEVICE9 pDevice = CRenderer::GetDevice();

	/*	
	HRESULT D3DXCreateTextureFromFileEx(
	LPDIRECT3DDEVICE9	pDevice,
	LPCTSTR	pSrcFile,
	UINT	Width,
	UINT	Height,
	UINT	MipLevels,
	DWORD	Usage,
	D3DFORMAT	Format,
	D3DPOOL	Pool,
	DWORD	Filter,
	DWORD	MipFilter,
	D3DCOLOR	ColorKey,
	D3DXIMAGE_INFO*	pSrcInfo,
	PALETTEENTRY*	pPalette,
	LPDIRECT3DTEXTURE9*	ppTexture);	
	*/
	// �e�N�X�`���C���^�[�t�F�[�X�Ƀf�[�^����
	LPDIRECT3DTEXTURE9 texInterface = nullptr;
	if(D3DXCreateTextureFromFileEx(pDevice, &strFilePass[0], 0, 0, 5, 0, D3DFMT_A8R8G8B8, D3DPOOL_MANAGED, D3DX_FILTER_POINT, D3DX_DEFAULT, 0, nullptr, nullptr, &texInterface) < 0)
	{
		texInterface = nullptr;
		return false;
	}

	// �v�f�̒ǉ�
	m_umapTexInterface[strFilePass] = texInterface;

	// �ǉ����������܂���
	return true;
}

//================================================================================
// �e�N�X�`���ݒ�
//--------------------------------------------------------------------------------
bool CTexManager::SetTex(const std::string &strFilePass, LPDIRECT3DTEXTURE9 texInterface)
{
	// null�`�F�b�N
	if (strFilePass.empty()) { return false; }
	if (texInterface == nullptr) { return false; }

	// �v�f�̗L�����m���߂�
	if (m_umapTexInterface.count(strFilePass) == 1)
	{// ����܂���
		return true;
	}

	// �v�f�̒ǉ�
	m_umapTexInterface[strFilePass] = texInterface;

	// �ǉ����������܂���
	return true;
}

//================================================================================
// �e�N�X�`���擾
//--------------------------------------------------------------------------------
LPDIRECT3DTEXTURE9 CTexManager::GetTexInterface(const std::string &strFilePass)
{
	// null�`�F�b�N
	if(strFilePass.empty())
	{
		return nullptr;
	}

	// �v�f�̗L�����m���߂�
	if(m_umapTexInterface.count(strFilePass) == 1)
	{// ����܂���
		return m_umapTexInterface[strFilePass];
	}

	// ���̗v�f�͖����ł�
	return nullptr;
}

//================================================================================
// �e�N�X�`���̗L�����m�F
//--------------------------------------------------------------------------------
bool CTexManager::CheckTex(const std::string &strFilePass)
{
	// null�`�F�b�N
	if (strFilePass.empty())
	{
		MessageBox(nullptr, "�e�N�X�`���̃t�@�C���̎w�肪����Ă��܂���", "�x��", MB_OK);
	}

	// �v�f�̗L�����m���߂�
	if (m_umapTexInterface.count(strFilePass) == 1)
	{// ����܂���
		return true;
	}

	// ���̗v�f�͖����ł�
	return false;
}

//================================================================================
// �e�N�X�`�����
//--------------------------------------------------------------------------------
bool CTexManager::Release(const std::string &strFilePass)
{
	// �v�f�̗L�����m���߂�
	if(m_umapTexInterface.count(strFilePass) == 1)
	{// ����܂���
		m_umapTexInterface[strFilePass]->Release();		// �o�b�t�@�̉��
		m_umapTexInterface[strFilePass] = nullptr;
		m_umapTexInterface.erase(strFilePass);			// �v�f�̍폜

		return true;
	}
	return false;
}

//================================================================================
// �e�N�X�`���S���
//--------------------------------------------------------------------------------
void CTexManager::ReleaseAll()
{
	// �R���e�i�𖖔��܂Ń��[�v
	for(auto& unmapTex : m_umapTexInterface)
	{
		if(unmapTex.second)
		{
			unmapTex.second->Release();	// �o�b�t�@�̉��
			unmapTex.second = nullptr;
		}
	}
	m_umapTexInterface.clear();
}