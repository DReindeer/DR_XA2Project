//================================================================================
//
// Auter : KENSUKE WATANABE
//
//================================================================================
#define _CRT_SECURE_NO_WARNINGS

#include "xa2Manager.h"
#include "xa2LoadOgg.h"
#include "xa2SoundResourceManager.h"
#include "xa2SourceVoiceInterface.h"

//--------------------------------------------------------------------------------
// 
// oggデコード→Waveファイルとして処理 - CPU全のせ
// 
//--------------------------------------------------------------------------------
//--------------------------------------------------------------------------------
//  デストラクタ
//--------------------------------------------------------------------------------
XA2LoadOggOnAll::~XA2LoadOggOnAll()
{
	m_pAudioData.clear();
}

//--------------------------------------------------------------------------------
// oggファイルオープン→waveとして読み込み
//--------------------------------------------------------------------------------
XA2LoadOggOnAll *XA2LoadOggOnAll::Create(const std::string& strFilePath, const int loopCount)
{
	XA2LoadOggOnAll* result = new XA2LoadOggOnAll;
	result->m_loopCount = loopCount;

	// ロード
	if (!result->Load(strFilePath, loopCount))
	{
		// 既に存在するので解放
		delete result;
		return nullptr;
	}

	return result;
}

//--------------------------------------------------------------------------------
//  ロード処理
//--------------------------------------------------------------------------------
bool XA2LoadOggOnAll::Load(std::string strFilePath, int loopCount)
{
	// 読み込み済みか調べる
	//--------------------------------------------------------------------------------
	XA2SoundResourceManager *pXA2SoundResourceManager = XA2Manager::GetSoundResourceManager();
	if (pXA2SoundResourceManager->CheckXA2SoundResource(strFilePath))
	{// 登録済み
		return false;
	}

	// oggファイルオープン
	//--------------------------------------------------------------------------------
	// 読み込んだogg
	FILE *fp;

	// ファイルを開く
	fp = fopen(&strFilePath[0], "rb");
	if (fp == NULL) {
		fprintf(stderr, "Open Error : %s", &strFilePath[0]);
		return false;
	}

	// OggVorbis初期化
	if (ov_open(fp, &m_vf, NULL, 0) < 0) {
		fprintf(stderr, "Input does not appear to be an Ogg bitstream.\n");
		fclose(fp);
		return false;
	}

	// 詳細収得 - チャンネル数や、サンプリングレート等
	vorbis_info *vi;
	vi = ov_info(&m_vf, -1);
	if (vi == NULL) 
	{
		MessageBox(nullptr, "Get Info Error\r\n詳細取得エラー\n", "err", MB_OK);
		return false;
	}
	
	//  ヘッダサイズの収得
	long whsize;
	whsize = sizeof(m_waveHeader.ckidRIFF) + sizeof(m_waveHeader.ckSizeRIFF) + sizeof(m_waveHeader.fccType) +
		sizeof(m_waveHeader.ckidFmt) + sizeof(m_waveHeader.ckSizeFmt) + sizeof(m_waveHeader.WaveFmt) +
		sizeof(m_waveHeader.ckidData) + sizeof(m_waveHeader.ckSizeData);


	// デコード後のデータサイズを求める
	WORD quantizationBits = 2;		// 量子化ビット - 8bitだと1、16bitだと2。
	m_audioSize = (long)ceil(vi->channels * vi->rate * ov_time_total(&m_vf, -1) * quantizationBits);
	m_pAudioData.resize(m_audioSize + 1);

	// ヘッダ書き込み
	//--------------------------------------------------------------------------------
	memcpy(m_waveHeader.ckidRIFF, "RIFF", 4);
	m_waveHeader.ckSizeRIFF = whsize + m_audioSize - 8;
	memcpy(m_waveHeader.fccType, "WAVE", 4);
	memcpy(m_waveHeader.ckidFmt, "fmt ", 4);
	m_waveHeader.ckSizeFmt = sizeof(WAVEFORMATEX);

	m_pWfx = new WAVEFORMATEXTENSIBLE;
	m_pWfx->Format.cbSize = m_waveHeader.WaveFmt.cbSize = sizeof(WAVEFORMATEX);
	m_pWfx->Format.wFormatTag = m_waveHeader.WaveFmt.wFormatTag = WAVE_FORMAT_PCM;
	m_pWfx->Format.nChannels = m_waveHeader.WaveFmt.nChannels = vi->channels;
	m_pWfx->Format.nSamplesPerSec = m_waveHeader.WaveFmt.nSamplesPerSec = vi->rate;
	m_pWfx->Format.nAvgBytesPerSec = m_waveHeader.WaveFmt.nAvgBytesPerSec = vi->rate * vi->channels * quantizationBits;
	m_pWfx->Format.nBlockAlign = m_waveHeader.WaveFmt.nBlockAlign = vi->channels * quantizationBits;
	m_pWfx->Format.wBitsPerSample = m_waveHeader.WaveFmt.wBitsPerSample = quantizationBits * 8;

	memcpy(m_waveHeader.ckidData, "data", 4);
	m_waveHeader.ckSizeData = m_audioSize;
	
	// デコード - 取得するBYTE数が不定のようなので注意
	//--------------------------------------------------------------------------------
	long size = 0;
	long ret;
	int currentSection = 0;
	bool eof = false;
	while (!eof) {
		ret = ov_read(&m_vf, (char*)&m_pAudioData[size], m_audioSize - size, 0, quantizationBits, 1, &currentSection);
		if (ret == 0) {
			eof = true;
		}
		else if (ret < 0) {
			ov_clear(&m_vf);
			fclose(fp);
			fprintf(stderr, "Decode Error\n");
			return false;
		}
		else {
			size += ret;
		}
	}

	// oggファイルクローズ
	//--------------------------------------------------------------------------------
	ov_clear(&m_vf);

	// Streamingフラグ
	m_streaming = false;

	// ループカウントの設定
	m_loopCount = loopCount;

	// ファイルパスの設定
	m_filePass = strFilePath;

	// サウンドリソースの登録
	pXA2SoundResourceManager->AddXA2SoundResource(this);

	return true;
}

//--------------------------------------------------------------------------------
// 再生
//--------------------------------------------------------------------------------
void XA2LoadOggOnAll::Play(XA2SourceVoiceData *pSourceVoiceData, XA2SourceVoiceInterface *pSourceVoiceInterface)
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
// oggデコード→Waveファイルとして処理 - ストリーミング
// 
//--------------------------------------------------------------------------------
//--------------------------------------------------------------------------------
//  生成処理
//--------------------------------------------------------------------------------
XA2LoadOggStreaming* XA2LoadOggStreaming::Create(const std::string& strFilePath, const int loopCount)
{
	XA2LoadOggStreaming* result = new XA2LoadOggStreaming;
	result->m_loopCount = loopCount;

	// ロード
	if (!result->Load(strFilePath, loopCount))
	{
		// 既に存在するため解放
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
bool XA2LoadOggStreaming::Load(const std::string& strFilePath, const int loopCount)
{
	// oggファイルオープン
	//--------------------------------------------------------------------------------
	// 読み込んだogg
	FILE *fp;

	// ファイルを開く
	fp = fopen(&strFilePath[0], "rb");
	if (fp == NULL) {
		fprintf(stderr, "Open Error : %s", &strFilePath[0]);
		return false;
	}

	// OggVorbis初期化
	m_pvf = new OggVorbis_File;
	if (ov_open(fp, m_pvf, NULL, 0) < 0) {
		fprintf(stderr, "Input does not appear to be an Ogg bitstream.\n");
		fclose(fp);
		return false;
	}

	// 詳細収得 - チャンネル数や、サンプリングレート等
	vorbis_info *vi;
	vi = ov_info(m_pvf, -1);
	if (vi == NULL)
	{
		MessageBox(nullptr, "Get Info Error\r\n詳細取得エラー\n", "err", MB_OK);
		return false;
	}

	//  ヘッダサイズの収得
	long whsize;
	whsize = sizeof(m_waveHeader.ckidRIFF) + sizeof(m_waveHeader.ckSizeRIFF) + sizeof(m_waveHeader.fccType) +
		sizeof(m_waveHeader.ckidFmt) + sizeof(m_waveHeader.ckSizeFmt) + sizeof(m_waveHeader.WaveFmt) +
		sizeof(m_waveHeader.ckidData) + sizeof(m_waveHeader.ckSizeData);


	// デコード後のデータサイズを求める
	m_audioSize = (long)ceil(vi->channels * vi->rate * ov_time_total(m_pvf, -1) * quantizationBits);

	// ヘッダ書き込み
	//--------------------------------------------------------------------------------
	memcpy(m_waveHeader.ckidRIFF, "RIFF", 4);
	m_waveHeader.ckSizeRIFF = whsize + m_audioSize - 8;
	memcpy(m_waveHeader.fccType, "WAVE", 4);
	memcpy(m_waveHeader.ckidFmt, "fmt ", 4);
	m_waveHeader.ckSizeFmt = sizeof(WAVEFORMATEX);

	m_pWfx = new WAVEFORMATEXTENSIBLE;
	m_pWfx->Format.cbSize = m_waveHeader.WaveFmt.cbSize = sizeof(WAVEFORMATEX);
	m_pWfx->Format.wFormatTag = m_waveHeader.WaveFmt.wFormatTag = WAVE_FORMAT_PCM;
	m_pWfx->Format.nChannels = m_waveHeader.WaveFmt.nChannels = vi->channels;
	m_pWfx->Format.nSamplesPerSec = m_waveHeader.WaveFmt.nSamplesPerSec = vi->rate;
	m_pWfx->Format.nAvgBytesPerSec = m_waveHeader.WaveFmt.nAvgBytesPerSec = vi->rate * vi->channels * quantizationBits;
	m_pWfx->Format.nBlockAlign = m_waveHeader.WaveFmt.nBlockAlign = vi->channels * quantizationBits;
	m_pWfx->Format.wBitsPerSample = m_waveHeader.WaveFmt.wBitsPerSample = quantizationBits * 8;

	memcpy(m_waveHeader.ckidData, "data", 4);
	m_waveHeader.ckSizeData = m_audioSize;

	// １秒のバッファを要素数分用意
	for (auto& data : m_pAudioDatas)
	{
		data.resize(m_pWfx->Format.nAvgBytesPerSec);
	}

	// 開始位置を設定
	m_writeCursor = m_dataStart = 0;

	// Streamingフラグ
	m_streaming = true;

	// ループカウントの設定
	m_loopCount = loopCount;

	// ファイルパスの設定
	m_filePass = strFilePath;

	// サウンドリソースの登録
	XA2SoundResourceManager *pXA2SoundResourceManager = XA2Manager::GetSoundResourceManager();
	pXA2SoundResourceManager->AddXA2SoundResource(this);

	return true;
}

//--------------------------------------------------------------------------------
//  破棄処理
//--------------------------------------------------------------------------------
void XA2LoadOggStreaming::Release(void)
{
	// 要素数分削除
	for (auto& data : m_pAudioDatas)
	{
		data.clear();
	}

	if (m_pvf)
	{
		ov_clear(m_pvf);
		delete m_pvf;
		m_pvf = nullptr;
	}
}

//--------------------------------------------------------------------------------
//  再生
//--------------------------------------------------------------------------------
void XA2LoadOggStreaming::Play(XA2SourceVoiceData *pSourceVoiceData, XA2SourceVoiceInterface *pSourceVoiceInterface)
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
void XA2LoadOggStreaming::Polling(IXAudio2SourceVoice *pSourceVoice)
{
	if (pSourceVoice == nullptr)return;

	// ミューテクス
	std::unique_lock<std::recursive_mutex> locker = XA2Manager::Locker();

	//状態を取得
	XAUDIO2_VOICE_STATE state = { 0 };
	pSourceVoice->GetState(&state);

	//再生キューに常にバッファを溜めておく
	if (state.BuffersQueued < MAX_STREAM_AUDIODATA)
	{
		if (m_writeCursor >= m_audioSize)
		{
			if (m_loopCount == -1)
			{// ループするなら先頭に戻す
				ov_time_seek(m_pvf, 0.0);
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
void XA2LoadOggStreaming::AddNextBuffer(IXAudio2SourceVoice *pSourceVoice)
{
	// データを書き込む
	std::vector<BYTE> audioDatas = { 0 };
	DWORD dataSize = m_pWfx->Format.nAvgBytesPerSec;	// データ長
	audioDatas.resize(dataSize);
	long readPosition = 0, ret = 0;
	int currentSection = 0;
	while(readPosition != dataSize)
	{
		ret = ov_read(m_pvf, (char*)&audioDatas[readPosition], dataSize - readPosition, 0, quantizationBits, 1, &currentSection);
		if (ret == 0)
		{
			break;
		}
		else
		{
			// 書き込み位置を進める
			readPosition += ret;
		}
	}

	// カーソルを進める
	m_writeCursor += dataSize;

	// リストに追加
	m_pAudioDatas.push_back(audioDatas);

	// SourceVoiceにデータを送信
	XAUDIO2_BUFFER xa2buffer = { 0 };
	xa2buffer.AudioBytes = dataSize;
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
void XA2LoadOggStreaming::ReleaseFlontBuffer()
{
	m_pAudioDatas.pop_front();
}
