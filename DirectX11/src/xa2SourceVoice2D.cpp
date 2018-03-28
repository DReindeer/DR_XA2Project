//================================================================================
//
// Auter : KENSUKE WATANABE
// Data  : 2018/01/31
//
//================================================================================
#include <algorithm>
#include "xa2Manager.h"
#include "xa2Core.h"
#include "xa2MasteringVoice.h"
#include "xa2SourceVoiceManager.h"
#include "xa2SourceVoice2D.h"
#include "xa2SoundResourceManager.h"

//--------------------------------------------------------------------------------
// 生成処理
//--------------------------------------------------------------------------------
IXAudio2SourceVoice *XA2SourceVoice2D::Create(std::string strFilePath, int loopCount, XA2LoadAudio *pLoadWave)
{
	// ミューテックス
#ifdef THREAD_ON
	std::unique_lock<std::recursive_mutex> locker = CManager::Locker();
#endif

	// オーディオデータの生成
	XA2SourceVoiceData2D *pSourceVoiceData = new XA2SourceVoiceData2D;

	XA2Core *pXA2Core = XA2Manager::GetXA2Core();				// XAudio2エンジン
	XA2SoundResourceManager *pXA2SoundResourceManager = XA2Manager::GetSoundResourceManager();	// サウンドリソースマネージャ

	// リソースの取得
	pLoadWave->Create(strFilePath, loopCount);	// wave形式で取得
	XA2LoadAudio *pSoundResource = pXA2SoundResourceManager->GetXA2SoundResource(strFilePath);

	// ソースボイスの生成
	IXAudio2SourceVoice *pSourceVoice = pSourceVoiceData->GetSourceVoice();
	HRESULT hr = pXA2Core->GetXAudio2()->CreateSourceVoice(
		&pSourceVoice,							// ソースボイス
		&(pSoundResource->GetWfx()->Format),	// リソースのフォーマット
		0,										// ソースボイスの動作を指定するフラグ
		XAUDIO2_DEFAULT_FREQ_RATIO,				// 最大許容周波数比
		nullptr,								// コールバック インターフェイスへのポインタ
		nullptr,								// ソースボイスに対する一連のデスティネーションボイス
		nullptr);								// ソースボイスで使用するエフェクトチェーン
	if (FAILED(hr))
	{
		MessageBox(nullptr, "ソースボイスの生成に失敗！", "警告", MB_ICONWARNING);
		return nullptr;
	}

	// ソースボイスの設定
	pSourceVoiceData->SetSourceVoice(pSourceVoice);

	// オーディオデータのポインタの設定
	pSourceVoiceData->SetWaveData(pSoundResource);

	// リストに登録
	pSourceVoiceData->SetFilePass(strFilePath);		// ファイル名
	AddSourceVoiceData(pSourceVoiceData);			// 登録

	return pSourceVoice;
}

//--------------------------------------------------------------------------------
// 生成して再生
//--------------------------------------------------------------------------------
IXAudio2SourceVoice *XA2SourceVoice2D::CreatePlay(std::string strFilePath, int loopCount, XA2LoadAudio *pLoadWave)
{
	// 生成
	IXAudio2SourceVoice *pSourceVoice = Create(strFilePath, loopCount, pLoadWave);

	// リソースの取得
	XA2SoundResourceManager *pXA2SoundResourceManager = XA2Manager::GetSoundResourceManager();		// サウンドリソースマネージャ
	XA2LoadAudio *pSoundResource = pXA2SoundResourceManager->GetXA2SoundResource(strFilePath);

	// バッファの値設定
	XAUDIO2_BUFFER xa2buffer;
	memset(&xa2buffer, 0, sizeof(XAUDIO2_BUFFER));
	XA2LoadWaveOnAll *pLoadWaveOnAll = nullptr;
	if (pSoundResource->GetStreamingFlag() == false)
	{
		pLoadWaveOnAll = (XA2LoadWaveOnAll*)pSoundResource;
		xa2buffer.AudioBytes = pLoadWaveOnAll->GetAudioSize();
		xa2buffer.pAudioData = &pLoadWaveOnAll->GetAudioData()[0][0];
		xa2buffer.Flags = XAUDIO2_END_OF_STREAM;
		xa2buffer.LoopCount = pLoadWaveOnAll->GetLoopCount();

		// オーディオバッファの登録
		pSourceVoice->SubmitSourceBuffer(&xa2buffer);

		// 再生
		pSourceVoice->Start(0);
	}

	return pSourceVoice;
}

//--------------------------------------------------------------------------------
// 更新
//--------------------------------------------------------------------------------
void XA2SourceVoice2D::Update()
{
	// 終了したSEの解放
	XA2SourceVoiceInterface::ReleaseEndSE();
}

//--------------------------------------------------------------------------------
// ポーリング処理 - 別スレッド処理
//--------------------------------------------------------------------------------
void XA2SourceVoice2D::Polling()
{
	if (m_sourceVoices.size() == 0 || m_sourceVoices.size() > MAX_SAUCEVOICE)return;

	// コンテナが空か判定
	if (m_sourceVoices.empty())
	{
		return;
	}
	
	// ロック
	std::unique_lock<std::recursive_mutex> locker = XA2Manager::Locker();

	// ポーリング処理
	XA2LoadAudio *pWaveData = nullptr;
	XA2LoadWaveStreaming *pWaveStreaming = nullptr;
	XA2LoadOggStreaming *pOggStreaming = nullptr;
	for (auto it : m_sourceVoices)
	{
		if (it == nullptr)continue;
		// ストリーミング再生のソースボイスのみ実行
		pWaveData = it->GetWaveData();
		if (pWaveData->GetStreamingFlag())
		{
			switch ((int)pWaveData->GetAudioFormat())
			{
			case XA2LoadAudio::AUDIO_FORMAT_WAVE:
				pWaveStreaming = (XA2LoadWaveStreaming*)pWaveData;
				pWaveStreaming->Polling(it->GetSourceVoice());
				break;
			case XA2LoadAudio::AUDIO_FORMAT_OGG:
				pOggStreaming = (XA2LoadOggStreaming*)pWaveData;
				pOggStreaming->Polling(it->GetSourceVoice());
				break;
			}
		}
	}
}

