//================================================================================
//
// Auter : KENSUKE WATANABE
// Data  : 2018/01/02
//
//================================================================================
#include "xa2Core.h"
#include "xa2MasteringVoice.h"

//--------------------------------------------------------------------------------
// ����������
//--------------------------------------------------------------------------------
HRESULT XA2MasteringVoice::Init(HWND hWnd)
{
	HRESULT hr;

	// �}�X�^�[�{�C�X�̐���
	hr = XA2Core::GetXAudio2()->CreateMasteringVoice(&m_pMasteringVoice);
	if(FAILED(hr))
	{
		MessageBox(hWnd, "�}�X�^�[�{�C�X�̐����Ɏ��s�I", "�x��", MB_ICONWARNING);

		return hr;
	}

	return hr;
}

//--------------------------------------------------------------------------------
// �I������
//--------------------------------------------------------------------------------
void XA2MasteringVoice::Uninit()
{
	// �}�X�^�[�{�C�X�̔j��
	if (m_pMasteringVoice)
	{
		m_pMasteringVoice->DestroyVoice();
		m_pMasteringVoice = nullptr;
	}
}
