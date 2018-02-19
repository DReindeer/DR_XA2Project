//================================================================================
//
// Auter : KENSUKE WATANABE
// Data  : 2018/02/01
//
//================================================================================
#include <algorithm>
#include "manager.h"
#include "camera.h"
#include "xa2Listener.h"
#include "xa2Manager.h"
#include "xa2Core.h"
#include "xa2MasteringVoice.h"
#include "xa2SourceVoiceManager.h"
#include "xa2SourceVoice3D.h"
#include "xa2SoundResourceManager.h"

static const X3DAUDIO_DISTANCE_CURVE_POINT Emitter_LFE_CurvePoints[3] = { 0.0f, 1.0f, 0.25f, 0.0f, 1.0f, 0.0f };
static const X3DAUDIO_DISTANCE_CURVE       Emitter_LFE_Curve = { (X3DAUDIO_DISTANCE_CURVE_POINT*)&Emitter_LFE_CurvePoints[0], 3 };

static const X3DAUDIO_DISTANCE_CURVE_POINT Emitter_Reverb_CurvePoints[3] = { 0.0f, 0.5f, 0.75f, 1.0f, 1.0f, 0.0f };
static const X3DAUDIO_DISTANCE_CURVE       Emitter_Reverb_Curve = { (X3DAUDIO_DISTANCE_CURVE_POINT*)&Emitter_Reverb_CurvePoints[0], 3 };

//--------------------------------------------------------------------------------
// 生成処理
//--------------------------------------------------------------------------------
IXAudio2SourceVoice *XA2SourceVoice3D::Create(std::string strFilePath, int loopCount, XA2LoadWave *pLoadWave)
{
	// ミューテックス
#ifdef THREAD_ON
	std::unique_lock<std::recursive_mutex> locker = CManager::Locker();
#endif

	// オーディオデータの生成
	XA2SourceVoiceData3D *pSourceVoiceData = new XA2SourceVoiceData3D;

	XA2Manager *pXA2Manager = CManager::GetXA2Manager();		// Manager
	XA2Core *pXA2Core = pXA2Manager->GetXA2Core();				// XAudio2エンジン
	XA2SoundResourceManager *pXA2SoundResourceManager = pXA2Manager->GetSoundResourceManager();	// サウンドリソースマネージャ

	// リソースの取得
	pLoadWave->Create(strFilePath, loopCount);	// wave形式で取得
	XA2LoadWave *pSoundResource = pXA2SoundResourceManager->GetXA2SoundResource(strFilePath);

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

	// ファイル名
	pSourceVoiceData->SetFilePass(strFilePath);			

	// エミッターの設定
	m_emitter.Position = D3DXVECTOR3(0.f, 0.f, 0.f);	// 座標
	m_emitter.OrientFront = D3DXVECTOR3(0.f, 0.f, 1.f);	// 前方向
	m_emitter.OrientTop = D3DXVECTOR3(0.f, 1.f, 0.f);	// 上方向
	m_emitter.Velocity = D3DXVECTOR3(0.f, 0.f, 0.f);	// 移動量
	// コーン
	m_emitter.pCone = new X3DAUDIO_CONE;				
	SetCone(D3DX_PI * 0.75f, 1.1f, 0.8f);				// コーン設定
	// 音源の周囲の領域
	SetInnerRadius(3.f);								// 内部半径
	SetInnerRadiusAngle(X3DAUDIO_PI / 8.f);				// 内部角度
	// マルチチャンネル
	m_emitter.ChannelCount = 1;							// チャンネル数
	m_emitter.ChannelRadius = 0.0f;						// マルチチャンネル時の半径
	m_emitter.pChannelAzimuths = new FLOAT32[m_emitter.ChannelCount];	// ラジアン単位の方位角で表したチャンネル位置のテーブル
	*m_emitter.pChannelAzimuths = atan2(m_emitter.OrientFront.z, m_emitter.OrientFront.x) - D3DX_PI * 0.5f;

	m_emitter.pVolumeCurve = (X3DAUDIO_DISTANCE_CURVE*)&X3DAudioDefault_LinearCurve;	// ボリュームカーブ
	m_emitter.pLFECurve = (X3DAUDIO_DISTANCE_CURVE*)&Emitter_LFE_Curve;					// LFE(低音増強)カーブ
	m_emitter.pLPFDirectCurve = NULL;													// ローパスフィルタのダイレクトパス係数距離カーブ
	m_emitter.pLPFReverbCurve = NULL;													// ローパスフィルタのリバーブパス係数距離カーブ
	m_emitter.pReverbCurve = (X3DAUDIO_DISTANCE_CURVE*)&Emitter_Reverb_Curve;			// リバーブセンドレベル距離カーブ
	m_emitter.CurveDistanceScaler = 10.f;												// ユーザー定義のワールド単位にするスケーラー
	m_emitter.DopplerScaler = 3.f;														// ドップラー偏移スケーラー

	// リストに登録
	AddSourceVoiceData(pSourceVoiceData);				// 登録

	return pSourceVoice;
}

IXAudio2SourceVoice *XA2SourceVoice3D::CreatePlay(std::string strFilePath, int loopCount, XA2LoadWave *pLoadWave)
{
	// ミューテックス
#ifdef THREAD_ON
	std::unique_lock<std::recursive_mutex> locker = CManager::Locker();
#endif

	// オーディオデータの生成
	XA2SourceVoiceData3D *pSourceVoiceData = new XA2SourceVoiceData3D;

	XA2Manager *pXA2Manager = CManager::GetXA2Manager();		// Manager
	XA2Core *pXA2Core = pXA2Manager->GetXA2Core();				// XAudio2エンジン
	XA2SoundResourceManager *pXA2SoundResourceManager = pXA2Manager->GetSoundResourceManager();	// サウンドリソースマネージャ

	// リソースの取得
	pLoadWave->Create(strFilePath, loopCount);	// wave形式で取得
	XA2LoadWave *pSoundResource = pXA2SoundResourceManager->GetXA2SoundResource(strFilePath);

	// マスターボイス
	IXAudio2MasteringVoice *pMaster = XA2Manager::GetMasteringVoice()->GetMasteringVoice();

	// デステネーションボイスの設定
	XAUDIO2_SEND_DESCRIPTOR sendDescriptors[2];
	sendDescriptors[0].Flags = XAUDIO2_SEND_USEFILTER; // LPF direct-path
	sendDescriptors[0].pOutputVoice = m_pSubmixVoice;
	sendDescriptors[1].Flags = XAUDIO2_SEND_USEFILTER; // LPF direct-path
	sendDescriptors[1].pOutputVoice = pMaster;
	const XAUDIO2_VOICE_SENDS sendList = { 2, sendDescriptors };

	// ソースボイスの生成
	IXAudio2SourceVoice *pSourceVoice = pSourceVoiceData->GetSourceVoice();
	HRESULT hr = pXA2Core->GetXAudio2()->CreateSourceVoice(
		&pSourceVoice,							// ソースボイス
		&(pSoundResource->GetWfx()->Format),	// リソースのフォーマット
		0,										// ソースボイスの動作を指定するフラグ
		XAUDIO2_DEFAULT_FREQ_RATIO,				// 最大許容周波数比
		nullptr,								// コールバック インターフェイスへのポインタ
		&sendList,								// ソースボイスに対する一連のデスティネーションボイス
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

	// エミッターの設定
	m_emitter.Position = D3DXVECTOR3(0.f, 0.f, 0.f);	// 座標
	m_emitter.OrientFront = D3DXVECTOR3(0.f, 0.f, 1.f);	// 前方向
	m_emitter.OrientTop = D3DXVECTOR3(0.f, 1.f, 0.f);	// 上方向
	m_emitter.Velocity = D3DXVECTOR3(0.f, 0.f, 0.f);	// 移動量
														// コーン
	m_emitter.pCone = new X3DAUDIO_CONE;
	SetCone(D3DX_PI * 0.75f, 1.1f, 0.8f);				// コーン設定

	// 音源の周囲の領域
	SetInnerRadius(3.f);								// 内部半径
	SetInnerRadiusAngle(X3DAUDIO_PI / 8.f);				// 内部角度
	
	// マルチチャンネル
	m_emitter.ChannelCount = 1;							// チャンネル数
	m_emitter.ChannelRadius = 0.0f;						// マルチチャンネル時の半径
	m_emitter.pChannelAzimuths = new FLOAT32[m_emitter.ChannelCount];	// ラジアン単位の方位角で表したチャンネル位置のテーブル
	*m_emitter.pChannelAzimuths = atan2(m_emitter.OrientFront.z, m_emitter.OrientFront.x) - D3DX_PI * 0.5f;


	m_emitter.pVolumeCurve = (X3DAUDIO_DISTANCE_CURVE*)&X3DAudioDefault_LinearCurve;	// ボリュームカーブ
	m_emitter.pLFECurve = (X3DAUDIO_DISTANCE_CURVE*)&Emitter_LFE_Curve;					// LFE(低音増強)カーブ
	m_emitter.pLPFDirectCurve = NULL;													// ローパスフィルタのダイレクトパス係数距離カーブ
	m_emitter.pLPFReverbCurve = NULL;													// ローパスフィルタのリバーブパス係数距離カーブ
	m_emitter.pReverbCurve = (X3DAUDIO_DISTANCE_CURVE*)&Emitter_Reverb_Curve;			// リバーブセンドレベル距離カーブ
	m_emitter.CurveDistanceScaler = 10.f;												// ユーザー定義のワールド単位にするスケーラー
	m_emitter.DopplerScaler = 3.f;														// ドップラー偏移スケーラー

//	Update();

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
	else
	{
		//pLoadWaveStreaming = (XA2LoadWaveStreaming*)pSoundResource;
		//xa2buffer.AudioBytes = pLoadWaveStreaming->GetAudioSize();
		//xa2buffer.pAudioData = &pLoadWaveStreaming->GetAudioData()[0][0];
		//xa2buffer.Flags = XAUDIO2_END_OF_STREAM;
		//xa2buffer.LoopCount = pLoadWaveStreaming->GetLoopCount();
	}

	// リストに登録
	pSourceVoiceData->SetFilePass(strFilePath);		// ファイル名
	AddSourceVoiceData(pSourceVoiceData);			// 登録

	return pSourceVoice;
}

//--------------------------------------------------------------------------------
// 終了
//--------------------------------------------------------------------------------
void XA2SourceVoice3D::Uninit()
{
	// エミッターの解放
	//--------------------------------------------------------------------------------
	if (m_emitter.pChannelAzimuths)
	{
		delete m_emitter.pChannelAzimuths;
		m_emitter.pChannelAzimuths = nullptr;
	}
	if (m_emitter.pCone)
	{
		delete m_emitter.pCone;
		m_emitter.pCone = nullptr;
	}
	if (m_emitter.pLFECurve)
	{
		delete m_emitter.pLFECurve;
		m_emitter.pLFECurve = nullptr;
	}
	if (m_emitter.pLPFDirectCurve)
	{
		delete m_emitter.pLPFDirectCurve;
		m_emitter.pLPFDirectCurve = nullptr;
	}
	if (m_emitter.pLPFReverbCurve)
	{
		delete m_emitter.pLPFReverbCurve;
		m_emitter.pLPFReverbCurve = nullptr;
	}
	if (m_emitter.pReverbCurve)
	{
		delete m_emitter.pReverbCurve;
		m_emitter.pReverbCurve = nullptr;
	}
	if (m_emitter.pVolumeCurve)
	{
		delete m_emitter.pVolumeCurve;
		m_emitter.pVolumeCurve = nullptr;
	}
}

//--------------------------------------------------------------------------------
// 更新
//--------------------------------------------------------------------------------
void XA2SourceVoice3D::Update()
{
	XA2Core *pXA2Core = XA2Manager::GetXA2Core();	// XAudio2エンジン

	// 終了したSEの解放
	//--------------------------------------------------------------------------------
	XA2SourceVoiceInterface::ReleaseEndSE();

	static int i;
	if (++i % 2 != 0)return;

	// 3D演算
	//--------------------------------------------------------------------------------
	X3DAUDIO_LISTENER *pListener = CManager::GetCamera()->GetListener()->GetListener();	// リスナー

	// 計算結果の格納用
	X3DAUDIO_DSP_SETTINGS DSPSettings = { 0 };
	DSPSettings.SrcChannelCount = m_emitter.ChannelCount;
	DSPSettings.DstChannelCount = pXA2Core->GetChannels();
	FLOAT32 matrix[6];
	DSPSettings.pMatrixCoefficients = matrix;

	// 計算処理フラグ
	DWORD dwCalcFlags = X3DAUDIO_CALCULATE_MATRIX | X3DAUDIO_CALCULATE_DOPPLER | X3DAUDIO_CALCULATE_LPF_DIRECT | X3DAUDIO_CALCULATE_REVERB;

	// 計算処理
	X3DAudioCalculate(
		XA2Manager::GetX3DInstance(),	// ハンドル
		pListener,						// リスナー
		&m_emitter,						// エミッター
		dwCalcFlags,					// 計算処理内容
		&DSPSettings);					// 計算結果

	// マスターボイス
	IXAudio2MasteringVoice *pMaster = XA2Manager::GetMasteringVoice()->GetMasteringVoice();

	// デバイスの入力チャンネル数
	UINT32 channels = pXA2Core->GetChannels();

	// 各ソースボイスに演算結果を適用
	for (auto pData : m_sourceVoices)
	{
		// ソースボイスのボリュームおよびピッチの値を適用
		IXAudio2SourceVoice *pSourceVoice = pData->GetSourceVoice();
//		pSourceVoice->SetOutputMatrix(pMaster, 1, channels, DSPSettings.pMatrixCoefficients);
		pSourceVoice->SetFrequencyRatio(DSPSettings.DopplerFactor);

		// リバーブレベルをサブミックスボイスに適用
//		pSourceVoice->SetOutputMatrix(m_pSubmixVoice, 1, 1, &DSPSettings.ReverbLevel);

		// ローパスフィルターダイレクト係数をソースボイスに適用
//		XAUDIO2_FILTER_PARAMETERS FilterParameters = { LowPassFilter, 2.0f * sinf(X3DAUDIO_PI / 6.0f * DSPSettings.LPFDirectCoefficient), 1.0f };
//		pSourceVoice->SetFilterParameters(&FilterParameters);
	}
}

//--------------------------------------------------------------------------------
// ポーリング処理 - 別スレッド処理
//--------------------------------------------------------------------------------
void XA2SourceVoice3D::Polling()
{
	// ポーリング処理
	XA2LoadWave *pWaveData = nullptr;
	for (auto it : m_sourceVoices)
	{
		if (it == nullptr)continue;
		// ストリーミング再生のソースボイスのみ実行
		pWaveData = it->GetWaveData();
		if (pWaveData->GetStreamingFlag())
		{
			XA2LoadWaveStreaming* pLoadWaveStreaming = (XA2LoadWaveStreaming*)pWaveData;
			pLoadWaveStreaming->Polling(it->GetSourceVoice());
		}
	}
}

// コーン
//--------------------------------------------------------------------------------
void XA2SourceVoice3D::SetCone(float innerAngle, float innerVolume, float outerVolume, float lpf, float reverb)
{
	m_emitter.pCone->InnerAngle = RETURN_MIN(innerAngle, X3DAUDIO_2PI);				// コーンの角度 (ラジアン単位)
	m_emitter.pCone->OuterAngle = X3DAUDIO_2PI - m_emitter.pCone->InnerAngle;		// 
	m_emitter.pCone->InnerVolume = RETURN_MIN(innerVolume, 2.0f);					// ボリューム スケーラ
	m_emitter.pCone->OuterVolume = RETURN_MIN(outerVolume, 2.0f);					// 
	m_emitter.pCone->InnerLPF = lpf;												// ダイレクト パスまたはリバーブ パスの係数スケーラ
	m_emitter.pCone->OuterLPF = lpf;												// 
	m_emitter.pCone->InnerReverb = reverb;											// リバーブ センド レベル スケーラ
	m_emitter.pCone->OuterReverb = reverb;											// 
}

// コーンの角度 (ラジアン単位)
//--------------------------------------------------------------------------------
void XA2SourceVoice3D::SetConeAngle(float innerAngle)
{
	m_emitter.pCone->InnerAngle = RETURN_MIN(innerAngle, X3DAUDIO_2PI);
	m_emitter.pCone->OuterAngle = X3DAUDIO_2PI - m_emitter.pCone->InnerAngle;
}

// ボリューム スケーラ
//--------------------------------------------------------------------------------
void XA2SourceVoice3D::SetConeVolume(float innerVolume, float outerVolume)
{
	m_emitter.pCone->InnerVolume = RETURN_MIN(innerVolume, 2.0f);
	m_emitter.pCone->OuterVolume = RETURN_MIN(outerVolume, 2.0f);
}

// ダイレクト パスまたはリバーブ パスの係数スケーラ
//--------------------------------------------------------------------------------
void XA2SourceVoice3D::SetConeLPF(float innerLPF, float outerLPF)
{
	m_emitter.pCone->InnerLPF = RETURN_MIN(innerLPF, 1.0f);
	m_emitter.pCone->OuterLPF = RETURN_MIN(outerLPF, 1.0f);
}

// リバーブ センド レベル スケーラ
//--------------------------------------------------------------------------------
void XA2SourceVoice3D::SetConeReverb(float innerReverb, float outerReverb)
{
	m_emitter.pCone->InnerReverb = RETURN_MIN(innerReverb, 2.0f);
	m_emitter.pCone->OuterReverb = RETURN_MIN(outerReverb, 2.0f);
}
