//================================================================================
//
// Auter : KENSUKE WATANABE
// Data  : 2018/01/02
//
//================================================================================
#include <thread>
#include "xa2Manager.h"
#include "xa2Core.h"
#include "xa2LoadWave.h"
#include "xa2SoundResourceManager.h"
#include "xa2SourceVoiceInterface.h"

// 定数定義
//--------------------------------------------------------------------------------
constexpr int MAX_STREAM_AUDIODATA = 5;

//--------------------------------------------------------------------------------
// 読み込み
//--------------------------------------------------------------------------------
XA2LoadWave::LOAD_RESULT XA2LoadWave::Load(std::string strFilePath, int loopCount, bool streaming)
{
	// ミューテックス
#ifdef THREAD_ON
	std::unique_lock<std::recursive_mutex> locker = CManager::Locker();
#endif

	XA2SoundResourceManager *pXA2SoundResourceManager = XA2Manager::GetSoundResourceManager();

	if (streaming == false && pXA2SoundResourceManager->CheckXA2SoundResource(strFilePath))
	{// 登録済み
		return LOAD_RESULT_DISCOVERY;
	}

	DWORD dwChunkSize = 0;
	DWORD dwChunkPosition = 0;
	DWORD dwFiletype;

	// サウンドデータファイルの生成
	m_file = CreateFile(&strFilePath[0], GENERIC_READ, FILE_SHARE_READ, nullptr, OPEN_EXISTING, 0, nullptr);
	if (m_file == INVALID_HANDLE_VALUE)
	{
		MessageBox(nullptr, "サウンドデータファイルの生成失敗", "警告", MB_ICONWARNING);
		HRESULT_FROM_WIN32(GetLastError());
		LOAD_RESULT_FAILD;
	}
	// ファイルポインタを先頭に移動
	if (SetFilePointer(m_file, 0, nullptr, FILE_BEGIN) == INVALID_SET_FILE_POINTER)
	{
		MessageBox(nullptr, "ファイルポインタの移動に失敗", "警告", MB_ICONWARNING);
		HRESULT_FROM_WIN32(GetLastError());
		LOAD_RESULT_FAILD;
	}

	// WAVEファイルのチェック
	HRESULT hr = CheckChunk(m_file, 'FFIR', &dwChunkSize, &dwChunkPosition);
	if (FAILED(hr))
	{
		MessageBox(nullptr, "WAVEファイルのチェックに失敗(1)", "警告", MB_ICONWARNING);
		LOAD_RESULT_FAILD;
	}
	hr = ReadChunkData(m_file, &dwFiletype, sizeof(DWORD), dwChunkPosition);
	if (FAILED(hr))
	{
		MessageBox(nullptr, "WAVEファイルのチェックに失敗(2)", "警告", MB_ICONWARNING);
		LOAD_RESULT_FAILD;
	}
	if (dwFiletype != 'EVAW')
	{
		MessageBox(nullptr, "WAVEファイルのチェックに失敗(3)", "警告", MB_ICONWARNING);
		LOAD_RESULT_FAILD;
	}

	// フォーマットチェック
	hr = CheckChunk(m_file, ' tmf', &dwChunkSize, &dwChunkPosition);
	if (FAILED(hr))
	{
		MessageBox(nullptr, "フォーマットチェックに失敗(1)", "警告", MB_ICONWARNING);
		LOAD_RESULT_FAILD;
	}
	// フォーマット生成
	m_pWfx = new WAVEFORMATEXTENSIBLE;
	hr = ReadChunkData(m_file, m_pWfx, dwChunkSize, dwChunkPosition);
	if (FAILED(hr))
	{
		MessageBox(nullptr, "フォーマットチェックに失敗(2)", "警告", MB_ICONWARNING);
		delete m_pWfx;
		m_pWfx = nullptr;
		LOAD_RESULT_FAILD;
	}

	// オーディオデータ読み込み
	hr = CheckChunk(m_file, 'atad', &m_audioSize, &m_dataStart);
	if (FAILED(hr))
	{
		MessageBox(nullptr, "オーディオデータ読み込みに失敗(1)", "警告", MB_ICONWARNING);
		LOAD_RESULT_FAILD;
	}

	// ループカウントの設定
	m_loopCount = loopCount;

	// ファイルパスの設定
	m_filePass = strFilePath;

	// サウンドリソースの登録
	pXA2SoundResourceManager->AddXA2SoundResource(this);

	return LOAD_RESULT_CREATE;
}

//--------------------------------------------------------------------------------
// 終了
//--------------------------------------------------------------------------------
void XA2LoadWave::Uninit()
{
	// waveフォーマットの解放
	if (m_pWfx)
	{
		delete m_pWfx;
		m_pWfx = nullptr;
	}

	// 名前のクリア
	m_filePass.clear();
}

//--------------------------------------------------------------------------------
//  停止
//--------------------------------------------------------------------------------
void XA2LoadWave::Stop(IXAudio2SourceVoice *pSourceVoice)
{
	// 状態取得
	XAUDIO2_VOICE_STATE state;
	pSourceVoice->GetState(&state);
	if (state.BuffersQueued != 0)
	{// 再生中なら一時停止
		pSourceVoice->Stop(0);

		// オーディオバッファの削除
		pSourceVoice->FlushSourceBuffers();
	}
}

//--------------------------------------------------------------------------------
// Wavwファイル(チャンク)のチェック
//--------------------------------------------------------------------------------
HRESULT XA2LoadWave::CheckChunk(HANDLE m_file, DWORD format, DWORD *pChunkSize, DWORD *pChunkDataPosition)
{
	HRESULT hr = S_OK;
	DWORD dwRead;
	DWORD dwChunkType;
	DWORD dwChunkDataSize;
	DWORD dwRIFFDataSize = 0;
	DWORD dwFileType;
	DWORD dwBytesRead = 0;
	DWORD dwOffset = 0;
	
	if(SetFilePointer(m_file, 0, nullptr, FILE_BEGIN) == INVALID_SET_FILE_POINTER)
	{// ファイルポインタを先頭に移動
		return HRESULT_FROM_WIN32(GetLastError());
	}
	
	while(hr == S_OK)
	{
		if(ReadFile(m_file, &dwChunkType, sizeof(DWORD), &dwRead, nullptr) == 0)
		{// チャンクの読み込み
			hr = HRESULT_FROM_WIN32(GetLastError());
		}

		if(ReadFile(m_file, &dwChunkDataSize, sizeof(DWORD), &dwRead, nullptr) == 0)
		{// チャンクデータの読み込み
			hr = HRESULT_FROM_WIN32(GetLastError());
		}

		switch(dwChunkType)
		{
		case 'FFIR':
			dwRIFFDataSize  = dwChunkDataSize;
			dwChunkDataSize = 4;
			if(ReadFile(m_file, &dwFileType, sizeof(DWORD), &dwRead, nullptr) == 0)
			{// ファイルタイプの読み込み
				hr = HRESULT_FROM_WIN32(GetLastError());
			}
			break;

		default:
			if(SetFilePointer(m_file, dwChunkDataSize, nullptr, FILE_CURRENT) == INVALID_SET_FILE_POINTER)
			{// ファイルポインタをチャンクデータ分移動
				return HRESULT_FROM_WIN32(GetLastError());
			}
		}

		dwOffset += sizeof(DWORD) * 2;
		if(dwChunkType == format)
		{
			*pChunkSize         = dwChunkDataSize;
			*pChunkDataPosition = dwOffset;

			return S_OK;
		}

		dwOffset += dwChunkDataSize;
		if(dwBytesRead >= dwRIFFDataSize)
		{
			return S_FALSE;
		}
	}
	
	return S_OK;
}

//--------------------------------------------------------------------------------
// Wavwファイル(チャンクデータ)の読み込み
//--------------------------------------------------------------------------------
DWORD XA2LoadWave::ReadChunkData(HANDLE m_file, void *pBuffer, DWORD dwBuffersize, DWORD dwBufferoffset)
{
	// ミューテクス
	std::unique_lock<std::recursive_mutex> locker = XA2Manager::Locker();

	DWORD dwRead = 0;
	
	if(SetFilePointer(m_file, dwBufferoffset, nullptr, FILE_BEGIN) == INVALID_SET_FILE_POINTER)
	{// ファイルポインタを指定位置まで移動
		return HRESULT_FROM_WIN32(GetLastError());
	}

	if(ReadFile(m_file, pBuffer, dwBuffersize, &dwRead, nullptr) == 0)
	{// データの読み込み
		return HRESULT_FROM_WIN32(GetLastError());
	}
	
	return dwRead;
}

//--------------------------------------------------------------------------------
// 
// Waveファイルのロード - CPU全のせ
// 
//--------------------------------------------------------------------------------
//--------------------------------------------------------------------------------
//  デストラクタ
//--------------------------------------------------------------------------------
XA2LoadWaveOnAll::~XA2LoadWaveOnAll()
{
	m_pAudioData.clear();
}

//--------------------------------------------------------------------------------
//  生成処理
//--------------------------------------------------------------------------------
XA2LoadWaveOnAll* XA2LoadWaveOnAll::Create(const std::string& strFilePath, const int loopCount)
{
	XA2LoadWaveOnAll* result = new XA2LoadWaveOnAll;
	result->m_loopCount = loopCount;

	if (!result->Load(strFilePath, loopCount))
	{
		delete result;
		return nullptr;
	}

	return result;
}

//--------------------------------------------------------------------------------
//  ロード処理
//--------------------------------------------------------------------------------
bool XA2LoadWaveOnAll::Load(std::string strFilePath, int loopCount)
{
	// wavヘッダ読み込み
	LOAD_RESULT result = XA2LoadWave::Load(strFilePath, loopCount, false);
	if (result == LOAD_RESULT_FAILD)
	{
		return false;
	}

	// Streamingフラグ
	m_streaming = false;

	if (result == LOAD_RESULT_CREATE)
	{
		// データ領域生成
		m_pAudioData.resize(m_audioSize);
		if (!ReadChunkData(m_file, &m_pAudioData[0], m_audioSize, m_dataStart))
		{
			MessageBox(NULL, strFilePath.c_str(), "ReadChunkData失敗！", MB_ICONWARNING);
			return false;
		}
	}

	return true;
}

//--------------------------------------------------------------------------------
// 再生
//--------------------------------------------------------------------------------
void XA2LoadWaveOnAll::Play(XA2SourceVoiceData *pSourceVoiceData, XA2SourceVoiceInterface *pSourceVoiceInterface)
{
	// 状態取得用変数
	XAUDIO2_VOICE_STATE xa2state;

	// バッファの値設定
	XAUDIO2_BUFFER xa2buffer;
	memset(&xa2buffer, 0, sizeof(XAUDIO2_BUFFER));
	xa2buffer.AudioBytes = m_audioSize;
	xa2buffer.pAudioData = &m_pAudioData[0];
	xa2buffer.Flags = XAUDIO2_END_OF_STREAM;
	xa2buffer.LoopCount = m_loopCount;

	IXAudio2SourceVoice *pSourceVoice = pSourceVoiceData->GetSourceVoice();
	std::string filePass = pSourceVoiceData->GetFilePass();

	// 状態取得
	pSourceVoice->GetState(&xa2state);
	if (xa2state.BuffersQueued != 0 && xa2buffer.LoopCount >= 0)
	{
		// 再生中なら新規でソースボイスを作成して再生
		//--------------------------------------------------------------------------------
		pSourceVoiceInterface->CreatePlay(filePass, xa2buffer.LoopCount, this);

		return;
	}

	// オーディオバッファの登録
	pSourceVoice->SubmitSourceBuffer(&xa2buffer);

	// 再生
	pSourceVoice->Start(0);
}

//--------------------------------------------------------------------------------
// 
// Waveファイルのロード - ストリーミング
// 
//--------------------------------------------------------------------------------
//--------------------------------------------------------------------------------
//  生成処理
//--------------------------------------------------------------------------------
XA2LoadWaveStreaming* XA2LoadWaveStreaming::Create(const std::string& strFilePath, const int loopCount)
{
	XA2LoadWaveStreaming* result = new XA2LoadWaveStreaming;
	result->m_loopCount = loopCount;

	if (!result->Load(strFilePath, loopCount))
	{
		result->Release();
		delete result;
		return nullptr;
	}

	// ストリーミング用のデータの入れ物を用意
	std::vector<BYTE> data = { 0 };
	for (int cntAudioData = 0; cntAudioData < MAX_STREAM_AUDIODATA; ++cntAudioData)
	{
		m_pAudioDatas.push_back(data);
	}

	return result;
}

//--------------------------------------------------------------------------------
//  ロード処理
//--------------------------------------------------------------------------------
bool XA2LoadWaveStreaming::Load(const std::string& strFilePath, const int loopCount)
{
	// ヘッダ情報ロード
	if (!XA2LoadWave::Load(strFilePath, loopCount, true))
	{
		return false;
	}

	// Streamingフラグ
	m_streaming = true;

	// １秒のバッファを要素数分用意
	for (auto& data : m_pAudioDatas)
	{
		data.resize(m_pWfx->Format.nAvgBytesPerSec);
	}

	// 開始位置を設定
	m_writeCursor = m_dataStart;

	return true;
}

//--------------------------------------------------------------------------------
//  破棄処理
//--------------------------------------------------------------------------------
void XA2LoadWaveStreaming::Release(void)
{
	// 要素数分削除
	for (auto& data : m_pAudioDatas)
	{
		data.clear();
	}
}

//--------------------------------------------------------------------------------
//  再生
//--------------------------------------------------------------------------------
void XA2LoadWaveStreaming::Play(XA2SourceVoiceData *pSourceVoiceData, XA2SourceVoiceInterface *pSourceVoiceInterface)
{
	UNREFERENCED_PARAMETER(pSourceVoiceInterface);
	IXAudio2SourceVoice *pSourceVoice = pSourceVoiceData->GetSourceVoice();

	pSourceVoice->Stop();
	m_writeCursor = m_dataStart;
	AddNextBuffer(pSourceVoice);
	pSourceVoice->Start(0);
}

//--------------------------------------------------------------------------------
//  ポーリング処理
//--------------------------------------------------------------------------------
void XA2LoadWaveStreaming::Polling(IXAudio2SourceVoice *pSourceVoice)
{
	if (pSourceVoice == nullptr)return;

	// ミューテクス
	std::unique_lock<std::recursive_mutex> locker = XA2Manager::Locker();

	//状態を取得
	XAUDIO2_VOICE_STATE state = {0};
	pSourceVoice->GetState(&state);

	//再生キューに常にバッファを溜めておく
	if (state.BuffersQueued < MAX_STREAM_AUDIODATA)
	{
		if (m_writeCursor >= m_audioSize)
		{
			if (m_loopCount == -1)
			{// ループするなら先頭に戻す
				m_writeCursor = m_dataStart;
			}
			else
			{// ループしないなら再生終了
				return;
			}
		}

		//バッファにデータを書き込んで、再生キューに追加
		AddNextBuffer(pSourceVoice);
	}

	// 不要な先頭要素の解放
	if (m_pAudioDatas.size() > MAX_STREAM_AUDIODATA)
	{
		ReleaseFlontBuffer();
	}
}

//--------------------------------------------------------------------------------
//  要素を追加する
//--------------------------------------------------------------------------------
void XA2LoadWaveStreaming::AddNextBuffer(IXAudio2SourceVoice *pSourceVoice)
{
	// データを書き込む
	std::vector<BYTE> audioDatas = { 0 };
	audioDatas.resize(m_pWfx->Format.nAvgBytesPerSec);
	DWORD read = ReadChunkData(m_file, &audioDatas[0], audioDatas.size(), m_writeCursor);
	if (read == 0)
	{
		MessageBox(NULL, "AddNextBuffer", "失敗！", MB_ICONWARNING);
		return;
	}

	// カーソルを進める
	m_writeCursor += read;

	// リストに追加
	m_pAudioDatas.push_back(audioDatas);

	// SourceVoiceにデータを送信
	XAUDIO2_BUFFER xa2buffer = { 0 };
	xa2buffer.AudioBytes = read;
	xa2buffer.pAudioData = &m_pAudioDatas.back()[0];
	if (m_audioSize <= m_writeCursor)
	{
		xa2buffer.Flags = XAUDIO2_END_OF_STREAM;
	}
	pSourceVoice->SubmitSourceBuffer(&xa2buffer);

}

//--------------------------------------------------------------------------------
//  先頭要素を解放する
//--------------------------------------------------------------------------------
void XA2LoadWaveStreaming::ReleaseFlontBuffer()
{
	// 空の要素(先頭要素)を解放
//	if (m_pAudioDatas.front())
//	{
//		delete m_pAudioDatas.front();
//		m_pAudioDatas.front() = nullptr;
//	}
	m_pAudioDatas.pop_front();
}
