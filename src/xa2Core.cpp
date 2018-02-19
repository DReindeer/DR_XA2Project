//================================================================================
//
// Auter : KENSUKE WATANABE
// Data  : 2018/01/02
//
//================================================================================
#include "manager.h"
#include "xa2Core.h"

// 静的メンバ変数
//--------------------------------------------------------------------------------
IXAudio2 *XA2Core::m_pXAudio2 = nullptr;				// XAudio2オブジェクトへのインターフェイス
XAUDIO2_DEVICE_DETAILS *XA2Core::m_pDetails = nullptr;	// デバイス関係の情報

//--------------------------------------------------------------------------------
// 初期化処理
//--------------------------------------------------------------------------------
HRESULT XA2Core::Init(HWND hWnd)
{
	// COMライブラリの初期化
	CoInitializeEx(nullptr, COINIT_MULTITHREADED);

	HRESULT hr;

	// XAudio2オブジェクトの作成
	if(FAILED(hr = XAudio2Create(&m_pXAudio2, 0)))
	{
		MessageBox(hWnd, "XAudio2オブジェクトの作成に失敗！", "警告", MB_ICONWARNING);

		// COMライブラリの終了処理
		CoUninitialize();

		return hr;
	}

	// サポートされているサンプルのパラメータ内にあることを確認
	m_pDetails = new XAUDIO2_DEVICE_DETAILS;
	if (FAILED(hr = m_pXAudio2->GetDeviceDetails(0, m_pDetails)))
	{
		Uninit();
		return hr;
	}

	if (m_pDetails->OutputFormat.Format.nChannels > OUTPUTCHANNELS)
	{
		Uninit();
		return E_FAIL;
	}

	return S_OK;
}

//--------------------------------------------------------------------------------
// 終了処理
//--------------------------------------------------------------------------------
void XA2Core::Uninit()
{
	if(m_pXAudio2)
	{
		// XAudio2オブジェクトの開放
		m_pXAudio2->Release();
		m_pXAudio2 = nullptr;
	}

	if (m_pDetails)
	{
		// デバイス関係の情報
		delete m_pDetails;
		m_pDetails = nullptr;
	}
	
	// COMライブラリの終了処理
	CoUninitialize();
}
