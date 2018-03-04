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

// 静的メンバ
//--------------------------------------------------------------------------------
std::unordered_map<void*, XA2SourceVoiceInterface*> XA2SourceVoiceManager::m_sourceVoices;
// thread
std::thread XA2SourceVoiceManager::m_pollingThread;
bool XA2SourceVoiceManager::m_endPollingThread = false;

//--------------------------------------------------------------------------------
// コンストラクタ
//--------------------------------------------------------------------------------
XA2SourceVoiceManager::XA2SourceVoiceManager()
{
	m_pollingThread = std::thread(Polling);
}

//--------------------------------------------------------------------------------
// デストラクタ
//--------------------------------------------------------------------------------
XA2SourceVoiceManager::~XA2SourceVoiceManager()
{
	// スレッド終了
	m_endPollingThread = true;
	m_pollingThread.join();
}


//--------------------------------------------------------------------------------
// 取得
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
// 更新
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
// ポーリング処理 - 別スレッド処理
//--------------------------------------------------------------------------------
void XA2SourceVoiceManager::Polling()
{
	while(1)
	{
		// スレッド終了判定
		if (m_endPollingThread == true)break;

		// コンテナが空か判定
		if (m_sourceVoices.empty() || m_sourceVoices.size() == 0 || m_sourceVoices.size() > MAX_SAUCEVOICE)
		{
			// 10msスレッド停止
			std::this_thread::sleep_for(std::chrono::milliseconds(10));
			continue;
		}

		for (auto xa2SoundObject : m_sourceVoices)
		{
			if (xa2SoundObject.second == nullptr)continue;
			xa2SoundObject.second->Polling();
		}
		// 10msスレッド停止
		std::this_thread::sleep_for(std::chrono::milliseconds(10));
	}
}

//--------------------------------------------------------------------------------
// 停止してから終了
//--------------------------------------------------------------------------------
void XA2SourceVoiceManager::StopAndUninit()
{
	std::unique_lock<std::recursive_mutex> locker = XA2Manager::Locker();

	// 解放
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
// 終了
//--------------------------------------------------------------------------------
void XA2SourceVoiceManager::Uninit()
{
	std::unique_lock<std::recursive_mutex> locker = XA2Manager::Locker();

	// 解放
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
