//================================================================================
//
// Auter : KENSUKE WATANABE
// Data  : 2018/02/11
//
//================================================================================
#include <algorithm>
#include "manager.h"
#include "xa2Manager.h"
#include "xa2Core.h"
#include "xa2MasteringVoice.h"
#include "xa2SoundResourceManager.h"
#include "xa2SourceVoiceManager.h"
#include "xa2SourceVoiceInterface.h"

//--------------------------------------------------------------------------------
// コンストラクタ
//--------------------------------------------------------------------------------
XA2SourceVoiceInterface::XA2SourceVoiceInterface()
{
	UINT32 flags = 0;
#ifdef _DEBUG
	flags |= XAUDIO2FX_DEBUG;
#endif
	IUnknown* pReverbEffect;
	HRESULT hr = XAudio2CreateReverb(&pReverbEffect, flags);
	if (FAILED(hr))
	{
		MessageBox(nullptr, "XAudio2CreateReverb()に失敗", "警告", MB_OK);
		return;
	}

	// エンジン
	XA2Core *pCore = XA2Manager::GetXA2Core();

	XAUDIO2_EFFECT_DESCRIPTOR effects[] = { { pReverbEffect, TRUE, 1 } };
	XAUDIO2_EFFECT_CHAIN effectChain = { 1, effects };

	if (FAILED(hr = pCore->GetXAudio2()->CreateSubmixVoice(&m_pSubmixVoice, 1,
		pCore->GetSamplesPerSec(), 0, 0, NULL, &effectChain)))
	{
		pReverbEffect->Release();
		MessageBox(nullptr, "pCore->GetXAudio2()->CreateSubmixVoice()に失敗", "警告", MB_OK);
		return;
	}

	// デフォルト設定
	XAUDIO2FX_REVERB_PARAMETERS native = {0};
	XAUDIO2FX_REVERB_I3DL2_PARAMETERS presetParameter = XAUDIO2FX_I3DL2_PRESET_FOREST;
	ReverbConvertI3DL2ToNative(&presetParameter, &native);
	m_pSubmixVoice->SetEffectParameters(0, &native, sizeof(native));
}

//--------------------------------------------------------------------------------
// デストラクタ
//--------------------------------------------------------------------------------
XA2SourceVoiceInterface::~XA2SourceVoiceInterface()
{
	if (m_pSubmixVoice)
	{
		m_pSubmixVoice->DestroyVoice();
		m_pSubmixVoice = nullptr;
	}
}

//--------------------------------------------------------------------------------
// 終了したSEの解放
//--------------------------------------------------------------------------------
void XA2SourceVoiceInterface::ReleaseEndSE()
{
	// ミューテクス
	std::unique_lock<std::recursive_mutex> locker = CManager::GetXA2Manager()->Locker();

	// 状態取得用変数
	XAUDIO2_VOICE_STATE xa2state;

	// 不要なソースボイスを探査
	XA2LoadWave *pWaveData = nullptr;
	m_sourceVoices.erase(remove_if(m_sourceVoices.begin(), m_sourceVoices.end(), [&](XA2SourceVoiceData *pSourceVoiceData)
	{
		// 状態取得
		pSourceVoiceData->GetSourceVoice()->GetState(&xa2state);
		pWaveData = pSourceVoiceData->GetWaveData();			// waveデータのポインタ
		if (xa2state.BuffersQueued == 0 && pWaveData->GetLoopCount() >= 0)
		{
			// 同じ場所に登録されているソースボイスを探査
			for (XA2SourceVoiceData *otherVoice : m_sourceVoices)
			{
				// 同じリソースを参照しているソースボイスを確認
				if (pSourceVoiceData->GetFilePass() == otherVoice->GetFilePass())
				{
					// 自分自身だったら飛ばす
					if (pSourceVoiceData == otherVoice)continue;

					// ソースボイスの破棄
					pSourceVoiceData->Uninit();

					return true;
				}
			}
		}
		return false;
	}), m_sourceVoices.end());
}

//--------------------------------------------------------------------------------
// 終了処理
//--------------------------------------------------------------------------------
void XA2SourceVoiceInterface::StopAndUninit()
{
	// ミューテクス
	std::unique_lock<std::recursive_mutex> locker = CManager::GetXA2Manager()->Locker();

	for (auto sourceVoiceData : m_sourceVoices)
	{
		if (sourceVoiceData)
		{
			// 停止
			sourceVoiceData->GetSourceVoice()->Stop();

			// ソースボイスの破棄
			sourceVoiceData->Uninit();

			// 自分自身の破棄
			delete sourceVoiceData;
			sourceVoiceData = nullptr;
		}
	}
	m_sourceVoices.clear();
}

//--------------------------------------------------------------------------------
// 終了処理
//--------------------------------------------------------------------------------
void XA2SourceVoiceInterface::Uninit()
{
	// ミューテクス
	std::unique_lock<std::recursive_mutex> locker = CManager::GetXA2Manager()->Locker();

	for (auto sourceVoiceData : m_sourceVoices)
	{
		if (sourceVoiceData)
		{
			// ソースボイスの破棄
			sourceVoiceData->Uninit();

			delete sourceVoiceData;
			sourceVoiceData = nullptr;
		}
	}
	m_sourceVoices.clear();
}

//--------------------------------------------------------------------------------
// セグメント再生 - 再生中なら新規でソースボイスを作成して再生
//--------------------------------------------------------------------------------
HRESULT XA2SourceVoiceInterface::Play(std::string strFilePath)
{
	// ミューテクス
	std::unique_lock<std::recursive_mutex> locker = CManager::GetXA2Manager()->Locker();

	for (auto pSourceVoiceData : m_sourceVoices)
	{
		if (pSourceVoiceData->GetFilePass() == strFilePath)
		{
			// オーディオの再生
			pSourceVoiceData->Play(this);

			return S_OK;
		}
	}

	return E_FAIL;
}

//--------------------------------------------------------------------------------
// セグメント停止
//--------------------------------------------------------------------------------
void XA2SourceVoiceInterface::Stop(std::string strFilePath)
{
	// ミューテクス
	std::unique_lock<std::recursive_mutex> locker = CManager::GetXA2Manager()->Locker();

	// 状態取得
	XAUDIO2_VOICE_STATE xa2state;
	for (auto sourceVoiceData : m_sourceVoices)
	{
		if (sourceVoiceData->GetFilePass() == strFilePath)
		{
			sourceVoiceData->GetSourceVoice()->GetState(&xa2state);
			if (xa2state.BuffersQueued != 0)
			{
				// 一時停止
				sourceVoiceData->Stop();

				// オーディオバッファの削除
				sourceVoiceData->FlushSourceBuffers();
			}
		}
	}
}

//--------------------------------------------------------------------------------
// セグメント停止(全て)
//--------------------------------------------------------------------------------
void XA2SourceVoiceInterface::StopAll()
{
	// ミューテクス
	std::unique_lock<std::recursive_mutex> locker = CManager::GetXA2Manager()->Locker();

	// 全て停止
	XAUDIO2_VOICE_STATE xa2state;
	IXAudio2SourceVoice *pSourceVoice = nullptr;		// ソースボイス

	for (auto sourceVoiceData : m_sourceVoices)
	{
		pSourceVoice = sourceVoiceData->GetSourceVoice();

		if (pSourceVoice)
		{
			pSourceVoice->GetState(&xa2state);
			if (xa2state.BuffersQueued != 0)
			{
				// 一時停止
				sourceVoiceData->Stop();

				// オーディオバッファの削除
				sourceVoiceData->FlushSourceBuffers();
			}
		}
	}
}
