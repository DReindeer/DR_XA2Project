//================================================================================
//
// Auter : KENSUKE WATANABE
// Data  : 2018/01/02
//
//================================================================================
#ifndef _XA2SOUNDRESOURCEMANAGER_H_
#define _XA2SOUNDRESOURCEMANAGER_H_

#include "../Libs/Microsoft_DirectX_SDK_(June 2010)/Include/XAudio2.h"
#include <string>
#include <unordered_map>
#include "xa2LoadWave.h"
#include "xa2LoadOgg.h"

// �T�E���h���\�[�X�}�l�[�W��
//--------------------------------------------------------------------------------
class XA2SoundResourceManager
{
public:
	XA2SoundResourceManager() {}		// �R���X�g���N�^
	~XA2SoundResourceManager();			// �f�X�g���N�^

	// �擾
	XA2LoadAudio *GetXA2SoundResource(std::string strFilePath);

	// �w��t�@�C�����̗L���𒲂ׂ�
	int CheckXA2SoundResource(std::string strFilePath);

	// �o�^
	void AddXA2SoundResource(XA2LoadAudio* soundResource) { m_xa2SoundResources.push_back(soundResource); }

	// �S����
	void Uninit();
	
private:
	// �T�E���h���\�[�X�̃R���e�i
	std::list<XA2LoadAudio*> m_xa2SoundResources;
};

#endif
