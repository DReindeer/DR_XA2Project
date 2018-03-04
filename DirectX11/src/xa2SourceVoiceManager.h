//================================================================================
//
// Auter : KENSUKE WATANABE
// Data  : 2018/01/02
//
//================================================================================
#ifndef _XA2SOURCEVOICEMANAGER_H_
#define _XA2SOURCEVOICEMANAGER_H_

#include <unordered_map>
#include <thread>
#include <mutex>

class XA2SourceVoiceInterface;

// ソースボイスマネージャ
//--------------------------------------------------------------------------------
class XA2SourceVoiceManager
{
public:
	XA2SourceVoiceManager();
	~XA2SourceVoiceManager();

	void Update();			// 更新
	void Uninit();			// 終了
	void StopAndUninit();	// 停止して終了

	// 別スレッドポーリング処理
	static void Polling();	

	// コンテナ
	//--------------------------------------------------------------------------------
	// 取得
	XA2SourceVoiceInterface *GetXA2SourceVoice(void *thisPointer);

	// 登録
	void SetXA2SoundObject(XA2SourceVoiceInterface* pXA2SourceVoice, void* thisPointer) {
		if (!pXA2SourceVoice || !thisPointer) return;
		if (m_sourceVoices.count(thisPointer) != 0) return;
		m_sourceVoices[thisPointer] = pXA2SourceVoice; 
	}

private:
	// ソースボイスインターフェイスのコンテナ
	static std::unordered_map<void*, XA2SourceVoiceInterface*> m_sourceVoices;

	// Streaming再生用のスレッド
	static std::thread m_pollingThread;	// スレッド
	static bool m_endPollingThread;		// スレッドの終了判定
};

#endif
