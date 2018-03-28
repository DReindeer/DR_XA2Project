//================================================================================
//
// Auter : KENSUKE WATANABE
// Data  : 2018/01/02
//
//================================================================================
#ifndef _XA2SOUNDMANAGER_H_
#define _XA2SOUNDMANAGER_H_

#include <mutex>
#include "../Libs/Microsoft_DirectX_SDK_(June 2010)/Include/XAudio2.h"
#include "../Libs/Microsoft_DirectX_SDK_(June 2010)/Include/xaudio2fx.h"
#include <xaudio2fx.h>
#include <x3daudio.h>

#pragma comment(lib, "Xaudio2.lib")
//#pragma comment(lib, "../Libs/Microsoft_DirectX_SDK_(June 2010)/Lib/x86/Xaudio2fx.lib")
#pragma comment(lib, "x3daudio.lib")

// 前方宣言
//--------------------------------------------------------------------------------
class XA2Core;
class XA2MasteringVoice;
class XA2SourceVoiceManager;
class XA2SoundResourceManager;
class XA2LoadWaveOnAll;
class XA2LoadWaveStreaming;
class XA2LoadOggOnAll;


// XAudio2の処理に必要なクラスをまとめたマネージャ
//--------------------------------------------------------------------------------
class XA2Manager
{
public:
	XA2Manager(HWND hWnd);	// コンストラクタ
	~XA2Manager();			// デストラクタ

	// XAudio2
	//--------------------------------------------------------------------------------
	// 取得
	static XA2Core *GetXA2Core() { return m_pXA2Core; }												// XAudio2のエンジン
	static XA2MasteringVoice *GetMasteringVoice() { return m_pMaster; }								// マスタリングボイス
	static XA2SourceVoiceManager *GetSourceVoiceManager() { return m_pSourceVoiceManager; }			// サウンドオブジェクトマネージャ
	static XA2SoundResourceManager *GetSoundResourceManager() { return m_pSoundResourceManager; }	// サウンドリソースマネージャ
	static XA2LoadWaveOnAll *GetLoadWaveOnAll() { return m_pLoadWaveOnAll; }						// wave読み込み - CPU全のせ
	static XA2LoadWaveStreaming *GetLoadWaveStreaming() { return m_pLoadWaveStreaming; }			// wave読み込み - ストリーミング
	static XA2LoadOggOnAll *GetLoadOggOnAll() { return m_pLoadOggOnAll; }							// ogg読み込み - CPU全のせ


	// X3DAudio
	//--------------------------------------------------------------------------------
	static const X3DAUDIO_HANDLE &GetX3DInstance() { return m_x3dInstance; }		// X3DAudioのハンドル
	

	// ミューテクス - ロックをかけるタイミングで↓を書く
	// std::unique_lock<std::recursive_mutex> locker = XA2Manager::Locker();
	//--------------------------------------------------------------------------------
	static std::unique_lock<std::recursive_mutex> Locker()
	{
		return std::unique_lock<std::recursive_mutex>(m_mutex);
	}

private:
	// XAudio2
	//--------------------------------------------------------------------------------
	static XA2Core *m_pXA2Core;									// XAudio2のエンジン
	static XA2MasteringVoice *m_pMaster;						// マスタリングボイス
	static XA2SourceVoiceManager *m_pSourceVoiceManager;		// サウンドオブジェクトマネージャ
	static XA2SoundResourceManager *m_pSoundResourceManager;	// サウンドリソースマネージャ
	static XA2LoadWaveOnAll *m_pLoadWaveOnAll;					// wave読み込み - CPU全のせ
	static XA2LoadWaveStreaming *m_pLoadWaveStreaming;			// wave読み込み - ストリーミング
	static XA2LoadOggOnAll *m_pLoadOggOnAll;					// ogg読み込み - CPU全のせ


	// X3DAudio
	//--------------------------------------------------------------------------------
	static X3DAUDIO_HANDLE m_x3dInstance;						// ハンドル


	// ミューテクス
	//--------------------------------------------------------------------------------
	static std::recursive_mutex m_mutex;
};

#endif
