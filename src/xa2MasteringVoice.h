//================================================================================
//
// Auter : KENSUKE WATANABE
// Data  : 2018/01/02
//
//================================================================================
#ifndef _XA2MASTERINGVOICE_H_
#define _XA2MASTERINGVOICE_H_

#include <XAudio2.h>

// マスタリングボイス
//--------------------------------------------------------------------------------
class XA2MasteringVoice
{
public:
	XA2MasteringVoice() {}
	~XA2MasteringVoice() {}

	// メンバ関数
	HRESULT Init(HWND hWnd);
	void Uninit();

	// 取得
	IXAudio2MasteringVoice *GetMasteringVoice() { return m_pMasteringVoice; }

private:
	// メンバ変数
	IXAudio2MasteringVoice *m_pMasteringVoice;		// マスターボイス
};

#endif
