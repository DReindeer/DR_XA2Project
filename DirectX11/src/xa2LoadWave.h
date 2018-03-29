//================================================================================
//
// Auter : KENSUKE WATANABE
// Data  : 2018/01/02
//
//================================================================================
#ifndef _LOADWAVE_H_
#define _LOADWAVE_H_

#include <string>
#include <list>
#include <vector>

// 定数定義
//--------------------------------------------------------------------------------
constexpr int MAX_STREAM_AUDIODATA = 3;

// 前方宣言
//--------------------------------------------------------------------------------
class XA2SourceVoiceData;
class XA2SourceVoiceInterface;

//--------------------------------------------------------------------------------
// 
// オーディオファイルのロード - 基底クラス
// 
//--------------------------------------------------------------------------------
class XA2LoadAudio
{
public:
	typedef enum
	{
		LOAD_RESULT_FAILD = 0,	// 失敗
		LOAD_RESULT_DISCOVERY,	// 発見
		LOAD_RESULT_CREATE		// 新規生成
	}LOAD_RESULT;

	typedef enum
	{
		AUDIO_FORMAT_NONE = 0,		// none
		AUDIO_FORMAT_WAVE,			// wave
		AUDIO_FORMAT_OGG,			// ogg
	}AUDIO_FORMAT;

	XA2LoadAudio() {}
	virtual ~XA2LoadAudio() {}

	// 生成処理
	virtual XA2LoadAudio* Create(const std::string& file_path, const int loopCount) = 0;

	// 再生
	virtual void Play(XA2SourceVoiceData *pSourceVoiceData, XA2SourceVoiceInterface *pSourceVoiceInterface) = 0;

	// 停止
	void Stop(IXAudio2SourceVoice *pSourceVoice);

	// 終了
	void Uninit();

	// 取得
	const int &GetLoopCount()const { return m_loopCount; }
	const DWORD &GetAudioDataOrigin()const { return m_dataStart; }
	const DWORD &GetAudioSize()const { return m_audioSize; }
	const WAVEFORMATEXTENSIBLE *GetWfx()const { return m_pWfx; }
	const std::string &GetFilePass ()const { return m_filePass; }
	const bool &GetStreamingFlag()const { return m_streaming; }
	const AUDIO_FORMAT &GetAudioFormat()const { return m_audioFormat; }

protected:
	// サウンド読み込み/生成
	LOAD_RESULT Load(std::string strFilePath, int loopCount, bool streaming = false);
	// チャンクチェック
	static HRESULT CheckChunk(HANDLE hFile, DWORD format, DWORD *pChunkSize, DWORD *pChunkDataPosition);
	// チャンク読み込み
	static DWORD ReadChunkData(HANDLE hFile, void *pBuffer, DWORD dwBuffersize, DWORD dwBufferoffset);

	// 変数定義
	AUDIO_FORMAT			m_audioFormat;						// オーディオのフォーマット
	bool					m_streaming = false;				// ストリーミングフラグ
	int						m_loopCount = 0;					// ループカウント
	DWORD					m_audioSize = 0;					// オーディオデータサイズ
	DWORD					m_dataStart = 0;					// オーディオデータ部分の先頭
	WAVEFORMATEXTENSIBLE*	m_pWfx = nullptr;					// Waveのフォーマット
	HANDLE					m_file = nullptr;					// カーソル位置
	std::string				m_filePass;							// ファイルパス
};


//--------------------------------------------------------------------------------
// 
// Waveファイルのロード - CPU全のせ
// 
//--------------------------------------------------------------------------------
class XA2LoadWaveOnAll final : public XA2LoadAudio
{
public:
	XA2LoadWaveOnAll() { m_audioFormat = AUDIO_FORMAT_WAVE; }
	virtual ~XA2LoadWaveOnAll();

	// 生成処理
	XA2LoadWaveOnAll *Create(const std::string& file_path, const int loopCount)override;

	// 再生
	void Play(XA2SourceVoiceData *pSourceVoiceData, XA2SourceVoiceInterface *pSourceVoiceInterface)override;

	// 取得
	std::vector<BYTE> *GetAudioData() { return &m_pAudioData; }

private:
	// ロード処理
	bool Load(std::string strFilePath, int loopCount);

	// オーディオデータ
	std::vector<BYTE> m_pAudioData;
};

//--------------------------------------------------------------------------------
// 
// Waveファイルのロード - ストリーミング
// 
//--------------------------------------------------------------------------------
class XA2LoadWaveStreaming final : public XA2LoadAudio
{ 
public:
	XA2LoadWaveStreaming() { m_audioFormat = AUDIO_FORMAT_WAVE; }
	virtual ~XA2LoadWaveStreaming() {}

	// 生成処理
	XA2LoadWaveStreaming* Create(const std::string& strFilePath, const int loopCount)override;

	// 再生
	void Play(XA2SourceVoiceData *pSourceVoiceData, XA2SourceVoiceInterface *pSourceVoiceInterface)override;

	// 破棄処理
	void Release(void);

	// ポーリング処理
	void Polling(IXAudio2SourceVoice *pSourceVoice);

private:
	// ロード処理
	bool Load(const std::string& strFilePath, const int loopCount);

	// 要素を追加する
	void AddNextBuffer(IXAudio2SourceVoice *pSourceVoice);

	// 先頭要素を解放
	void XA2LoadWaveStreaming::ReleaseFlontBuffer();

	// ポーリング用の再生待ちデータリスト(先頭要素が再生中)
	std::list<std::vector<BYTE>> m_pAudioDatas;

	// ファイル読み込みカーソル
	DWORD m_writeCursor = 0;
};

#endif
