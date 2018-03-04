//================================================================================
//
// Auter : KENSUKE WATANABE
// Data  : 2018/01/02
//
//================================================================================
#ifndef _LOADWAVE_H_
#define _LOADWAVE_H_

#include <string>
#include <vector>

// 前方宣言
//--------------------------------------------------------------------------------
class XA2SourceVoiceData;
class XA2SourceVoiceInterface;

//--------------------------------------------------------------------------------
// 
// Waveファイルのロード - 基底クラス
// 
//--------------------------------------------------------------------------------
class XA2LoadWave
{
public:
	typedef enum
	{
		LOAD_RESULT_FAILD = 0,	// 失敗
		LOAD_RESULT_DISCOVERY,	// 発見
		LOAD_RESULT_CREATE		// 新規生成
	}LOAD_RESULT;

	XA2LoadWave() {}
	virtual ~XA2LoadWave() {}

	// 生成処理
	virtual XA2LoadWave* Create(const std::string& file_path, const int loopCount) = 0;

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

protected:
	// サウンド読み込み/生成
	LOAD_RESULT Load(std::string strFilePath, int loopCount, bool streaming = false);
	// チャンクチェック
	static HRESULT CheckChunk(HANDLE hFile, DWORD format, DWORD *pChunkSize, DWORD *pChunkDataPosition);
	// チャンク読み込み
	static DWORD ReadChunkData(HANDLE hFile, void *pBuffer, DWORD dwBuffersize, DWORD dwBufferoffset);

	// 変数定義
	bool					m_streaming = false;	// ストリーミングフラグ
	int						m_loopCount = 0;		// ループカウント
	DWORD					m_audioSize = 0;		// オーディオデータサイズ
	DWORD					m_dataStart = 0;		// 元のオーディオデータ
	WAVEFORMATEXTENSIBLE*	m_pWfx = nullptr;		// Waveのフォーマット
	HANDLE					m_file = nullptr;		// カーソル位置
	std::string				m_filePass;				// ファイルパス
};


//--------------------------------------------------------------------------------
// 
// Waveファイルのロード - CPU全のせ
// 
//--------------------------------------------------------------------------------
class XA2LoadWaveOnAll final : public XA2LoadWave
{
public:
	XA2LoadWaveOnAll() {}
	virtual ~XA2LoadWaveOnAll();

	// 生成処理
	XA2LoadWaveOnAll* Create(const std::string& file_path, const int loopCount)override;

	// 再生
	void Play(XA2SourceVoiceData *pSourceVoiceData, XA2SourceVoiceInterface *pSourceVoiceInterface)override;

	// 取得
	std::vector<BYTE> *GetAudioData() { return &m_pAudioData; }

private:
	// ロード処理
	bool Load(std::string strFilePath, int loopCount);

	// 変数定義
	std::vector<BYTE> m_pAudioData;
};

//--------------------------------------------------------------------------------
// 
// Waveファイルのロード - ストリーミング
// 
//--------------------------------------------------------------------------------
class XA2LoadWaveStreaming final : public XA2LoadWave
{
public:
	XA2LoadWaveStreaming() {}
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

	// プライマリとセカンダリのフリップ
	void AddNextBuffer(IXAudio2SourceVoice *pSourceVoice);

	// プライマリとセカンダリのフリップ
	void Flip(void);

	// 変数定義
	std::vector<BYTE> m_pAudioDatas[2];
	int			m_primary = 0;		// 現在再生中のバッファ
	int			m_secondary = 1;	// 新しいデータの書き込み可能なバッファ
	DWORD		m_writeCursor = 0;	// 書き込みカーソル
};


#endif
