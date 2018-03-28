//================================================================================
//
// Auter : KENSUKE WATANABE
// Data  : 2018/01/02
//
//================================================================================
#include "xa2Manager.h"
#include "xa2Core.h"
#include "xa2MasteringVoice.h"
#include "xa2SourceVoiceManager.h"
#include "xa2SoundResourceManager.h"

// �ÓI�����o�̏�����
//--------------------------------------------------------------------------------
XA2Core *XA2Manager::m_pXA2Core = nullptr;								// XAudio2�̃G���W��
XA2MasteringVoice *XA2Manager::m_pMaster = nullptr;						// �}�X�^�����O�{�C�X
XA2SourceVoiceManager *XA2Manager::m_pSourceVoiceManager = nullptr;		// �T�E���h�I�u�W�F�N�g�}�l�[�W��
XA2SoundResourceManager *XA2Manager::m_pSoundResourceManager = nullptr;	// �T�E���h���\�[�X�}�l�[�W��
XA2LoadWaveOnAll *XA2Manager::m_pLoadWaveOnAll;							// wave�ǂݍ��� - CPU�S�̂�
XA2LoadWaveStreaming *XA2Manager::m_pLoadWaveStreaming;					// wave�ǂݍ��� - �X�g���[�~���O
XA2LoadOggOnAll *XA2Manager::m_pLoadOggOnAll;							// ogg�ǂݍ��� - CPU�S�̂�
XA2LoadOggStreaming *XA2Manager::m_pLoadOggStreaming;					// ogg�ǂݍ��� - CPU�X�g���[�~���O

std::recursive_mutex XA2Manager::m_mutex;								// �~���[�e�N�X
X3DAUDIO_HANDLE XA2Manager::m_x3dInstance = { 0 };						// �n���h��

//--------------------------------------------------------------------------------
// �R���X�g���N�^
//--------------------------------------------------------------------------------
XA2Manager::XA2Manager(HWND hWnd)
{
	// XAudio2�̃G���W��
	//--------------------------------------------------------------------------------
	m_pXA2Core = new XA2Core;
	if (FAILED(m_pXA2Core->Init(hWnd)))
	{
		// ���
		if (m_pXA2Core)
		{
			delete m_pXA2Core;
			m_pXA2Core = nullptr;
		}
		return;
	}

	// �}�X�^�����O�{�C�X
	//--------------------------------------------------------------------------------
	m_pMaster = new XA2MasteringVoice;
	if (FAILED(m_pMaster->Init(hWnd)))
	{
		// ���
		if (m_pMaster)
		{
			delete m_pMaster;
			m_pMaster = nullptr;
		}
		// XAudio2�G���W���̉��
		if (m_pXA2Core)
		{
			m_pXA2Core->Uninit();
			delete m_pXA2Core;
			m_pXA2Core = nullptr;
		}
		return;
	}

	// �t�H�[�}�b�g���̓ǂݍ��ݏ���
	//--------------------------------------------------------------------------------
	m_pLoadWaveOnAll = new XA2LoadWaveOnAll;			// wave - CPU�S�̂�
	m_pLoadWaveStreaming = new XA2LoadWaveStreaming;	// wave - �X�g���[�~���O
	m_pLoadOggOnAll = new XA2LoadOggOnAll;				// ogg - CPU�S�̂�
	m_pLoadOggStreaming = new XA2LoadOggStreaming;		// ogg - �X�g���[�~���O

	// �\�[�X�{�C�X�}�l�[�W��
	//--------------------------------------------------------------------------------
	m_pSourceVoiceManager = new XA2SourceVoiceManager;

	// �T�E���h���\�[�X�}�l�[�W��
	//--------------------------------------------------------------------------------
	m_pSoundResourceManager = new XA2SoundResourceManager;

	// 3D���̏�����
	//--------------------------------------------------------------------------------
	const DWORD channelMask = m_pXA2Core->GetChannelMask();
	X3DAudioInitialize(channelMask, X3DAUDIO_SPEED_OF_SOUND, m_x3dInstance);
}

//--------------------------------------------------------------------------------
// �f�X�g���N�^
//--------------------------------------------------------------------------------
XA2Manager::~XA2Manager()
{
	// �T�E���h�I�u�W�F�N�g�}�l�[�W��
	//--------------------------------------------------------------------------------
	if (m_pSourceVoiceManager)
	{
		m_pSourceVoiceManager->Uninit();
		delete m_pSourceVoiceManager;
		m_pSourceVoiceManager = nullptr;
	}

	// �T�E���h���\�[�X�}�l�[�W��
	//--------------------------------------------------------------------------------
	if (m_pSoundResourceManager)
	{
		m_pSoundResourceManager->Uninit();
		delete m_pSoundResourceManager;
		m_pSoundResourceManager = nullptr;
	}

	// wave�ǂݍ��� - CPU�S�̂�
	//--------------------------------------------------------------------------------
	if (m_pLoadWaveOnAll)
	{
		delete m_pLoadWaveOnAll;
		m_pLoadWaveOnAll = nullptr;
	}

	// wave�ǂݍ��� - �X�g���[�~���O
	//--------------------------------------------------------------------------------
	if (m_pLoadWaveStreaming)
	{
		delete m_pLoadWaveStreaming;
		m_pLoadWaveStreaming = nullptr;
	}

	// ogg�ǂݍ��� - CPU�S�̂�
	//--------------------------------------------------------------------------------
	if (m_pLoadOggOnAll)
	{
		delete m_pLoadOggOnAll;
		m_pLoadOggOnAll = nullptr;
	}

	// ogg�ǂݍ��� - �X�g���[�~���O
	//--------------------------------------------------------------------------------
	if (m_pLoadOggStreaming)
	{
		delete m_pLoadOggStreaming;
		m_pLoadOggStreaming = nullptr;
	}

	// �}�X�^�����O�{�C�X
	//--------------------------------------------------------------------------------
	if (m_pMaster)
	{
		m_pMaster->Uninit();
		delete m_pMaster;
		m_pMaster = nullptr;
	}

	// XAudio2�G���W��
	//--------------------------------------------------------------------------------
	if (m_pXA2Core)
	{
		m_pXA2Core->Uninit();
		delete m_pXA2Core;
		m_pXA2Core = nullptr;
	}
}
