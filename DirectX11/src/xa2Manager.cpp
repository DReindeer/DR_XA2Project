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

// 静的メンバの初期化
//--------------------------------------------------------------------------------
XA2Core *XA2Manager::m_pXA2Core = nullptr;								// XAudio2のエンジン
XA2MasteringVoice *XA2Manager::m_pMaster = nullptr;						// マスタリングボイス
XA2SourceVoiceManager *XA2Manager::m_pSourceVoiceManager = nullptr;		// サウンドオブジェクトマネージャ
XA2SoundResourceManager *XA2Manager::m_pSoundResourceManager = nullptr;	// サウンドリソースマネージャ
XA2LoadWaveOnAll *XA2Manager::m_pLoadWaveOnAll;							// wave読み込み - CPU全のせ
XA2LoadWaveStreaming *XA2Manager::m_pLoadWaveStreaming;					// wave読み込み - ストリーミング
XA2LoadOggOnAll *XA2Manager::m_pLoadOggOnAll;							// ogg読み込み - CPU全のせ

std::recursive_mutex XA2Manager::m_mutex;								// ミューテクス
X3DAUDIO_HANDLE XA2Manager::m_x3dInstance = { 0 };						// ハンドル

//--------------------------------------------------------------------------------
// コンストラクタ
//--------------------------------------------------------------------------------
XA2Manager::XA2Manager(HWND hWnd)
{
	// XAudio2のエンジン
	//--------------------------------------------------------------------------------
	m_pXA2Core = new XA2Core;
	if (FAILED(m_pXA2Core->Init(hWnd)))
	{
		// 解放
		if (m_pXA2Core)
		{
			delete m_pXA2Core;
			m_pXA2Core = nullptr;
		}
		return;
	}

	// マスタリングボイス
	//--------------------------------------------------------------------------------
	m_pMaster = new XA2MasteringVoice;
	if (FAILED(m_pMaster->Init(hWnd)))
	{
		// 解放
		if (m_pMaster)
		{
			delete m_pMaster;
			m_pMaster = nullptr;
		}
		// XAudio2エンジンの解放
		if (m_pXA2Core)
		{
			m_pXA2Core->Uninit();
			delete m_pXA2Core;
			m_pXA2Core = nullptr;
		}
		return;
	}

	// フォーマット毎の読み込み処理
	//--------------------------------------------------------------------------------
	m_pLoadWaveOnAll = new XA2LoadWaveOnAll;			// wave - CPU全のせ
	m_pLoadWaveStreaming = new XA2LoadWaveStreaming;	// wave - ストリーミング
	m_pLoadOggOnAll = new XA2LoadOggOnAll;			// ogg - CPU全のせ

	// ソースボイスマネージャ
	//--------------------------------------------------------------------------------
	m_pSourceVoiceManager = new XA2SourceVoiceManager;

	// サウンドリソースマネージャ
	//--------------------------------------------------------------------------------
	m_pSoundResourceManager = new XA2SoundResourceManager;

	// 3D環境の初期化
	//--------------------------------------------------------------------------------
	const DWORD channelMask = m_pXA2Core->GetChannelMask();
	X3DAudioInitialize(channelMask, X3DAUDIO_SPEED_OF_SOUND, m_x3dInstance);
}

//--------------------------------------------------------------------------------
// デストラクタ
//--------------------------------------------------------------------------------
XA2Manager::~XA2Manager()
{
	// サウンドオブジェクトマネージャ
	//--------------------------------------------------------------------------------
	if (m_pSourceVoiceManager)
	{
		m_pSourceVoiceManager->Uninit();
		delete m_pSourceVoiceManager;
		m_pSourceVoiceManager = nullptr;
	}

	// サウンドリソースマネージャ
	//--------------------------------------------------------------------------------
	if (m_pSoundResourceManager)
	{
		m_pSoundResourceManager->Uninit();
		delete m_pSoundResourceManager;
		m_pSoundResourceManager = nullptr;
	}

	// wave読み込み - CPU全のせ
	//--------------------------------------------------------------------------------
	if (m_pLoadWaveOnAll)
	{
		delete m_pLoadWaveOnAll;
		m_pLoadWaveOnAll = nullptr;
	}

	// wave読み込み - ストリーミング
	//--------------------------------------------------------------------------------
	if (m_pLoadWaveStreaming)
	{
		delete m_pLoadWaveStreaming;
		m_pLoadWaveStreaming = nullptr;
	}

	// ogg読み込み - CPU全のせ
	//--------------------------------------------------------------------------------
	if (m_pLoadOggOnAll)
	{
		delete m_pLoadOggOnAll;
		m_pLoadOggOnAll = nullptr;
	}

	// マスタリングボイス
	//--------------------------------------------------------------------------------
	if (m_pMaster)
	{
		m_pMaster->Uninit();
		delete m_pMaster;
		m_pMaster = nullptr;
	}

	// XAudio2エンジン
	//--------------------------------------------------------------------------------
	if (m_pXA2Core)
	{
		m_pXA2Core->Uninit();
		delete m_pXA2Core;
		m_pXA2Core = nullptr;
	}
}
