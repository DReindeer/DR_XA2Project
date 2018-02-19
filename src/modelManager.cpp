//================================================================================
//
//	Auter : KENSUKE WATANABE
//	Data  : 2018/02/10
//
//--------------------------------------------------------------------------------
#include "modelManager.h"
#include "renderer.h"
#include "manager.h"

#include <mutex>

//================================================================================
// ������
//--------------------------------------------------------------------------------
void CModelManager::Init()
{
	m_umapModel.clear();
}

//================================================================================
// �I��
//--------------------------------------------------------------------------------
void CModelManager::Uninit()
{
	ReleaseAll();
}

//================================================================================
// ���f���ݒ�
//--------------------------------------------------------------------------------
bool CModelManager::SetModel(const std::string &strFilePass)
{
	// null�`�F�b�N
	if(strFilePass.empty()){return false;}

	// �����̃p�X�����ɂ��邩�`�F�b�N
	if (m_umapModel.count(strFilePass) >= 1) { return true; }

	// �X���b�h�Z�[�t(���b�N)
#ifdef THREAD_ON
	std::unique_lock<std::recursive_mutex> locker = CManager::Locker();
#endif

	// �f�o�C�X�̎擾
	LPDIRECT3DDEVICE9 pDevice = CRenderer::GetDevice();

	// ���f���C���X�^���X�̐���
	MODEL *pModel = new MODEL;
	pModel->numMat = 0;
	pModel->pBuffMat = nullptr;
	pModel->pMat = nullptr;
	pModel->pMesh = nullptr;
	pModel->pMeshClone = nullptr;

	LPD3DXBUFFER pAdjacency;	// �אڐ����

	// ���f���ǂݍ���
	HRESULT hr = D3DXLoadMeshFromX(&strFilePass[0], D3DXMESH_MANAGED, pDevice, &pAdjacency, &pModel->pBuffMat, nullptr, &pModel->numMat, &pModel->pMesh);
	if (hr)
	{
		MessageBox(nullptr, "���f���������s", "�x��", MB_OK);
		return false;
	}

	hr = pModel->pMesh->OptimizeInplace( D3DXMESHOPT_COMPACT | D3DXMESHOPT_ATTRSORT | D3DXMESHOPT_VERTEXCACHE, (DWORD*)pAdjacency->GetBufferPointer(), NULL, NULL, NULL);
	if (hr)
	{
		MessageBox(nullptr, "�Ȃ�ł����s", "�x��", MB_OK);
		return false;
	}
	// �אڐ����̉��
	if (pAdjacency) { pAdjacency->Release(); }

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

	// �}�e���A�����̃|�C���^�Ƃ��ăo�b�t�@�̃A�h���X���擾
	pModel->pMat = (D3DXMATERIAL*)pModel->pBuffMat->GetBufferPointer();

	// �V�������b�V���̍쐬
	hr = pModel->pMesh->CloneMesh(D3DXMESH_MANAGED | D3DXMESH_WRITEONLY, decl, pDevice, &pModel->pMeshClone);
	if (hr)
	{
		MessageBox(nullptr, "���b�V���̃N���[�����s", "�x��", MB_OK);
		return false;
	}

	m_umapModel[strFilePass] = pModel;

	return true;
}

//================================================================================
// ���f���擾
//--------------------------------------------------------------------------------
CModelManager::MODEL *CModelManager::GetModel(const std::string &strFilePass)
{
	// null�`�F�b�N
	if (strFilePass.empty()) { return nullptr; }

	// �����̃p�X�����ɂ��邩�`�F�b�N
	if (m_umapModel.count(strFilePass) >= 1)
	{
		return m_umapModel[strFilePass];
	}

	// ���̃��b�V���C���^�[�t�F�C�X�͖����ł�
	return nullptr;
}

//================================================================================
// ���b�V���C���^�[�t�F�C�X�擾
//--------------------------------------------------------------------------------
LPD3DXMESH CModelManager::GetModelMesh(const std::string &strFilePass)
{
	// null�`�F�b�N
	if(strFilePass.empty()){return nullptr;}

	// �����̃p�X�����ɂ��邩�`�F�b�N
	if (m_umapModel.count(strFilePass) >= 1)
	{ 
		return m_umapModel[strFilePass]->pMesh;
	}

	// ���̃��b�V���C���^�[�t�F�C�X�͖����ł�
	return nullptr;
}

//================================================================================
// �}�e���A�����擾
//--------------------------------------------------------------------------------
LPD3DXBUFFER CModelManager::GetModelBuffMat(const std::string &strFilePass)
{
	// null�`�F�b�N
	if (strFilePass.empty()) { return nullptr; }

	// �����̃p�X�����ɂ��邩�`�F�b�N
	if (m_umapModel.count(strFilePass) >= 1)
	{
		return m_umapModel[strFilePass]->pBuffMat;
	}

	// ���̃}�e���A�����͖����ł�
	return nullptr;
}

//================================================================================
// �}�e���A�����擾
//--------------------------------------------------------------------------------
DWORD CModelManager::GetModelNumMat(const std::string &strFilePass)
{
	// null�`�F�b�N
	if (strFilePass.empty()) { return (DWORD)-1; }

	// �����̃p�X�����ɂ��邩�`�F�b�N
	if (m_umapModel.count(strFilePass) >= 1)
	{
		return m_umapModel[strFilePass]->numMat;
	}

	// �}�e���A�����͖��ݒ�ł�
	return (DWORD)-1;
}

//================================================================================
// �}�e���A���擾
//--------------------------------------------------------------------------------
D3DXMATERIAL *CModelManager::GetModelMat(const std::string &strFilePass)
{
	// null�`�F�b�N
	if (strFilePass.empty()) { return nullptr; }

	// �����̃p�X�����ɂ��邩�`�F�b�N
	if (m_umapModel.count(strFilePass) >= 1)
	{
		return m_umapModel[strFilePass]->pMat;
	}

	// ���̃}�e���A���͖����ł�
	return nullptr;
}

//================================================================================
// ���f�����
//--------------------------------------------------------------------------------
bool CModelManager::Release(const std::string &strFilePass)
{
	// null�`�F�b�N
	if (strFilePass.empty()) { return false; }

	// �����̃p�X�����ɂ��邩�`�F�b�N
	if (m_umapModel.count(strFilePass) >= 1)
	{
		if (m_umapModel[strFilePass]->pMesh != nullptr)
		{
			m_umapModel[strFilePass]->pMesh->Release();		// ���b�V�����
			m_umapModel[strFilePass]->pMesh = nullptr;
		}
		if (m_umapModel[strFilePass]->pBuffMat != nullptr)
		{
			m_umapModel[strFilePass]->pBuffMat->Release();	// �}�e���A���o�b�t�@���
			m_umapModel[strFilePass]->pBuffMat = nullptr;
		}

		m_umapModel.erase(strFilePass);
		return true;
	}

	// ����ł��܂���ł���
	return false;
}

//================================================================================
// ���f���S���
//--------------------------------------------------------------------------------
void CModelManager::ReleaseAll()
{
	// �S���
	for (auto it : m_umapModel)
	{
		if (it.second->pMesh != nullptr)
		{
			it.second->pMesh->Release();		// ���b�V�����
			it.second->pMesh = nullptr;
		}
		if (it.second->pBuffMat != nullptr)
		{
			it.second->pBuffMat->Release();		// �}�e���A���o�b�t�@���
			it.second->pBuffMat = nullptr;
		}
	}
	m_umapModel.clear();
}
