//================================================================================
//
// Auter : KENSUKE WATANABE
// Data  : 2018/01/02
//
//================================================================================
#include "manager.h"
#include "xa2Manager.h"
#include "xa2SourceVoiceManager.h"
#include "xa2SourceVoice2D.h"

// �ÓI�����o
//--------------------------------------------------------------------------------
std::unordered_map<void*, XA2SourceVoiceInterface*> XA2SourceVoiceManager::m_sourceVoices;
// thread
std::thread XA2SourceVoiceManager::m_pollingThread;
bool XA2SourceVoiceManager::m_endPollingThread = false;

//--------------------------------------------------------------------------------
// �R���X�g���N�^
//--------------------------------------------------------------------------------
XA2SourceVoiceManager::XA2SourceVoiceManager()
{
	m_pollingThread = std::thread(Polling);
}

//--------------------------------------------------------------------------------
// �f�X�g���N�^
//--------------------------------------------------------------------------------
XA2SourceVoiceManager::~XA2SourceVoiceManager()
{
	// �X���b�h�I��
	m_endPollingThread = true;
	m_pollingThread.join();
}


//--------------------------------------------------------------------------------
// �擾
//--------------------------------------------------------------------------------
XA2SourceVoiceInterface *XA2SourceVoiceManager::GetXA2SourceVoice(void *thisPointer)
{
	if (m_sourceVoices.count(thisPointer) == 0)
	{
		return nullptr;
	}
	
	return m_sourceVoices[thisPointer];
}

//--------------------------------------------------------------------------------
// �X�V
//--------------------------------------------------------------------------------
void XA2SourceVoiceManager::Update()
{
	std::unique_lock<std::recursive_mutex> locker = XA2Manager::Locker();

	for (auto xa2SoundObject : m_sourceVoices)
	{
		xa2SoundObject.second->Update();
	}
}

//--------------------------------------------------------------------------------
// �|�[�����O���� - �ʃX���b�h����
//--------------------------------------------------------------------------------
void XA2SourceVoiceManager::Polling()
{
	while(1)
	{
		// �X���b�h�I������
		if (m_endPollingThread == true)break;

		// �R���e�i���󂩔���
		if (m_sourceVoices.empty() || m_sourceVoices.size() == 0 || m_sourceVoices.size() > MAX_SAUCEVOICE)
		{
			// 10ms�X���b�h��~
			std::this_thread::sleep_for(std::chrono::milliseconds(10));
			continue;
		}

		for (auto xa2SoundObject : m_sourceVoices)
		{
			if (xa2SoundObject.second == nullptr)continue;
			xa2SoundObject.second->Polling();
		}
		// 10ms�X���b�h��~
		std::this_thread::sleep_for(std::chrono::milliseconds(10));
	}
}

//--------------------------------------------------------------------------------
// ��~���Ă���I��
//--------------------------------------------------------------------------------
void XA2SourceVoiceManager::StopAndUninit()
{
	std::unique_lock<std::recursive_mutex> locker = XA2Manager::Locker();

	// ���
	for (auto xa2SoundObject : m_sourceVoices)
	{
		if (xa2SoundObject.second)
		{
			xa2SoundObject.second->StopAndUninit();
			delete xa2SoundObject.second;
			xa2SoundObject.second = nullptr;
		}
	}
	m_sourceVoices.clear();
}

//--------------------------------------------------------------------------------
// �I��
//--------------------------------------------------------------------------------
void XA2SourceVoiceManager::Uninit()
{
	std::unique_lock<std::recursive_mutex> locker = XA2Manager::Locker();

	// ���
	for (auto xa2SoundObject : m_sourceVoices)
	{
		if (xa2SoundObject.second)
		{
			xa2SoundObject.second->Uninit();
			delete xa2SoundObject.second;
			xa2SoundObject.second = nullptr;
		}
	}
	m_sourceVoices.clear();
}
