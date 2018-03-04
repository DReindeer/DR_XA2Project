//================================================================================
//
// Auter : KENSUKE WATANABE
// Data  : 2018/01/02
//
//================================================================================
#include "xa2Core.h"
#include "xa2MasteringVoice.h"

//--------------------------------------------------------------------------------
// 初期化処理
//--------------------------------------------------------------------------------
HRESULT XA2MasteringVoice::Init(HWND hWnd)
{
	HRESULT hr;

	// マスターボイスの生成
	hr = XA2Core::GetXAudio2()->CreateMasteringVoice(&m_pMasteringVoice);
	if(FAILED(hr))
	{
		MessageBox(hWnd, "マスターボイスの生成に失敗！", "警告", MB_ICONWARNING);

		return hr;
	}

	return hr;
}

//--------------------------------------------------------------------------------
// 終了処理
//--------------------------------------------------------------------------------
void XA2MasteringVoice::Uninit()
{
	// マスターボイスの破棄
	if (m_pMasteringVoice)
	{
		m_pMasteringVoice->DestroyVoice();
		m_pMasteringVoice = nullptr;
	}
}
