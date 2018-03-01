// ======== ======== ======== ======== ======== ======== ======== ========
//
//
//	title : ���f���Ǘ�
//	Auter : KENSUKE WATANABE
//
//
// ======== ======== ======== ======== ======== ======== ======== ========
#include "modelManager.h"
#include "texManager.h"
#include "manager.h"

#include <assert.h>

// ======== ======== ======== ======== ======== ======== ======== ========
// ������
// -------- -------- -------- -------- -------- -------- -------- --------
void CXModelManager::Init(void)
{
	m_unmapModel.clear();
}

// ======== ======== ======== ======== ======== ======== ======== ========
// �I��
// -------- -------- -------- -------- -------- -------- -------- --------
void CXModelManager::Uninit(void)
{
	ReleaseAll();
}

// ======== ======== ======== ======== ======== ======== ======== ========
// ���f���ݒ�
// -------- -------- -------- -------- -------- -------- -------- --------
bool CXModelManager::SetModel(const std::string &strFilePass)
{
	// null�`�F�b�N
	if(strFilePass.empty()){return false;}

	// �����̃p�X�����ɂ��邩�`�F�b�N
	if (m_unmapModel.count(strFilePass) >= 1) { return true; }

	// ���f���C���X�^���X�̐���
	std::vector<XModel::MESH*> *vecpModel;

	// X���f���̐���
	XModel::Loader XLoader;
	vecpModel = XLoader.Load(strFilePass);

	// �e�N�X�`���̐���
	for (auto pModel : *vecpModel)
	{
		CManager::GetTexManager()->SetTex(pModel->mat.filePass);
	}

	m_unmapModel[strFilePass] = vecpModel;

	return true;
}

// ======== ======== ======== ======== ======== ======== ======== ========
// ���f���擾
// -------- -------- -------- -------- -------- -------- -------- --------
std::vector<XModel::MESH*> *CXModelManager::GetModel(const std::string &strFilePass)
{
	// null�`�F�b�N
	if (strFilePass.empty()) { return nullptr; }

	// �����̃p�X�����ɂ��邩�`�F�b�N
	if (m_unmapModel.count(strFilePass) >= 1)
	{
		return m_unmapModel[strFilePass];
	}

	// �}�e���A�����͖��ݒ�ł�
	return nullptr;
}

// ======== ======== ======== ======== ======== ======== ======== ========
// ���f�����
// -------- -------- -------- -------- -------- -------- -------- --------
bool CXModelManager::Release(const std::string &strFilePass)
{
	// null�`�F�b�N
	if (strFilePass.empty()) { return false; }

	// �����̃p�X�����ɂ��邩�`�F�b�N
	if (m_unmapModel.count(strFilePass) >= 1)
	{
		if (m_unmapModel[strFilePass] != nullptr)
		{
			m_unmapModel[strFilePass] = nullptr;
		}
		m_unmapModel.erase(strFilePass);
		return true;
	}
	// ����ł��܂���ł���
	return false;
}

// ======== ======== ======== ======== ======== ======== ======== ========
// ���f���S���
// -------- -------- -------- -------- -------- -------- -------- --------
void CXModelManager::ReleaseAll(void)
{
	// �S���
	for (auto it : m_unmapModel)
	{
		if (it.second != nullptr)
		{
			it.second = nullptr;
		}
	}
	m_unmapModel.clear();
}