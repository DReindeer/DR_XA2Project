//================================================================================
//
// Auter : KENSUKE WATANABE
// Data  : 2018/01/02
//
//================================================================================
#ifndef _XA2CORE_H_
#define _XA2CORE_H_

#include <XAudio2.h>

#define INPUTCHANNELS (1)			// ソースチャンネル番号
#define OUTPUTCHANNELS (8)			// サポートする最大チャンネル(予定)

// XAudio2エンジン
//--------------------------------------------------------------------------------
class XA2Core
{
public:
	XA2Core() {}
	~XA2Core() {}

	static HRESULT Init(HWND hWnd);
	static void Uninit();

	// エンジンの取得
	static IXAudio2 *GetXAudio2() { return m_pXAudio2; }

	static const DWORD &GetChannelMask() { return m_pDetails->OutputFormat.dwChannelMask; }				// チャンネル位置情報ビット列
	static const UINT32 GetChannels() { return m_pDetails->OutputFormat.Format.nChannels; }				// Waveフォームオーディオデータのチャンネル数
	static const UINT32 GetSamplesPerSec() { return m_pDetails->OutputFormat.Format.nSamplesPerSec; }	// Waveフォームオーディオデータの1秒あたりのサンプル数

private:
	// XAudio2オブジェクトへのインターフェイス
	static IXAudio2 *m_pXAudio2;

	// デバイス関係の情報
	static XAUDIO2_DEVICE_DETAILS *m_pDetails;
};

#endif
