//================================================================================
//
// Auter : KENSUKE WATANABE
// Data  : 2018/01/02
//
//================================================================================
#ifndef _XA2SOUNDMANAGER_H_
#define _XA2SOUNDMANAGER_H_

#include <mutex>
#include <XAudio2.h>
#include <xaudio2fx.h>
#include <x3daudio.h>

#pragma comment(lib, "x3daudio.lib")

// �O���錾
//--------------------------------------------------------------------------------
class XA2Core;
class XA2MasteringVoice;
class XA2SourceVoiceManager;
class XA2SoundResourceManager;
class XA2LoadWaveOnAll;
class XA2LoadWaveStreaming;


// XAudio2�̏����ɕK�v�ȃN���X���܂Ƃ߂��}�l�[�W��
//--------------------------------------------------------------------------------
class XA2Manager
{
public:
	XA2Manager(HWND hWnd);	// �R���X�g���N�^
	~XA2Manager();			// �f�X�g���N�^

	// �֌W����N���X
	//--------------------------------------------------------------------------------
	// �擾
	static XA2Core *GetXA2Core() { return m_pXA2Core; }												// XAudio2�̃G���W��
	static XA2MasteringVoice *GetMasteringVoice() { return m_pMaster; }								// �}�X�^�����O�{�C�X
	static XA2LoadWaveOnAll *GetLoadWaveOnAll() { return m_pLoadWaveOnAll; }						// wave�ǂݍ��� - CPU�S�̂�
	static XA2LoadWaveStreaming *GetLoadWaveStreaming() { return m_pLoadWaveStreaming; }			// wave�ǂݍ��� - �X�g���[�~���O
	static XA2SourceVoiceManager *GetSourceVoiceManager() { return m_pSourceVoiceManager; }			// �T�E���h�I�u�W�F�N�g�}�l�[�W��
	static XA2SoundResourceManager *GetSoundResourceManager() { return m_pSoundResourceManager; }	// �T�E���h���\�[�X�}�l�[�W��


	// X3DAudio
	//--------------------------------------------------------------------------------
	static const X3DAUDIO_HANDLE &GetX3DInstance() { return m_x3dInstance; }		// X3DAudio�̃n���h��
	

	// �~���[�e�N�X - ���b�N��������^�C�~���O�Ł�������
	// std::unique_lock<std::recursive_mutex> locker = CManager::GetXA2Manager()->Locker();
	//--------------------------------------------------------------------------------
	inline std::unique_lock<std::recursive_mutex> Locker()
	{
		return std::unique_lock<std::recursive_mutex>(m_mutex);
	}
private:
	// �֌W����N���X
	//--------------------------------------------------------------------------------
	static XA2Core *m_pXA2Core;									// XAudio2�̃G���W��
	static XA2MasteringVoice *m_pMaster;						// �}�X�^�����O�{�C�X
	static XA2LoadWaveOnAll *m_pLoadWaveOnAll;					// wave�ǂݍ��� - CPU�S�̂�
	static XA2LoadWaveStreaming *m_pLoadWaveStreaming;			// wave�ǂݍ��� - �X�g���[�~���O
	static XA2SourceVoiceManager *m_pSourceVoiceManager;		// �T�E���h�I�u�W�F�N�g�}�l�[�W��
	static XA2SoundResourceManager *m_pSoundResourceManager;	// �T�E���h���\�[�X�}�l�[�W��


	// X3DAudio
	//--------------------------------------------------------------------------------
	static X3DAUDIO_HANDLE m_x3dInstance;						// �n���h��


	// �~���[�e�N�X
	//--------------------------------------------------------------------------------
	std::recursive_mutex m_mutex;
};

#endif
