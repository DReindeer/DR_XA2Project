//================================================================================
//
// Auter : KENSUKE WATANABE
// Data  : 2018/01/02
//
//================================================================================
#ifndef _XA2MASTERINGVOICE_H_
#define _XA2MASTERINGVOICE_H_

#include "../Libs/Microsoft_DirectX_SDK_(June 2010)/Include/XAudio2.h"

// �}�X�^�����O�{�C�X
//--------------------------------------------------------------------------------
class XA2MasteringVoice
{
public:
	XA2MasteringVoice() {}
	~XA2MasteringVoice() {}

	// �����o�֐�
	HRESULT Init(HWND hWnd);
	void Uninit();

	// �擾
	IXAudio2MasteringVoice *GetMasteringVoice() { return m_pMasteringVoice; }

private:
	// �����o�ϐ�
	IXAudio2MasteringVoice *m_pMasteringVoice;		// �}�X�^�[�{�C�X
};

#endif
