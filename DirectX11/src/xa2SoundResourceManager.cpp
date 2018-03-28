//================================================================================
//
// Auter : KENSUKE WATANABE
// Data  : 2018/01/02
//
//================================================================================
#include "xa2SoundResourceManager.h"

//--------------------------------------------------------------------------------
// �f�X�g���N�^
//--------------------------------------------------------------------------------
XA2SoundResourceManager::~XA2SoundResourceManager()
{

}

//--------------------------------------------------------------------------------
// �R���e�i���烊�\�[�X���擾
//--------------------------------------------------------------------------------
XA2LoadAudio *XA2SoundResourceManager::GetXA2SoundResource(std::string strFilePath)
{
	for (auto it : m_xa2SoundResources)
	{
		if (it->GetFilePass() == strFilePath)
		{// �o�^�ς�
			return it;
		}
	}
	return nullptr;
}

//--------------------------------------------------------------------------------
// �S����
//--------------------------------------------------------------------------------
void XA2SoundResourceManager::Uninit()
{
	// �T�E���h���\�[�X�̃R���e�i
	for (auto xa2Data : m_xa2SoundResources)
	{
		if (xa2Data)
		{
			// ���\�[�X�̉��
			xa2Data->Uninit();
			delete xa2Data;
			xa2Data = nullptr;
		}
	}
	// �R���e�i����ɂ���
	m_xa2SoundResources.clear();
}

//--------------------------------------------------------------------------------
// �w��t�@�C�����̗L���𒲂ׂ�
//--------------------------------------------------------------------------------
int XA2SoundResourceManager::CheckXA2SoundResource(std::string strFilePath)
{
	int count = 0;
	for (auto it : m_xa2SoundResources)
	{
		if (it->GetFilePass() == strFilePath)
		{// �o�^�ς�
			++count;
		}
	}
	return count;
}
