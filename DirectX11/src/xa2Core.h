//================================================================================
//
// Auter : KENSUKE WATANABE
// Data  : 2018/01/02
//
//================================================================================
#ifndef _XA2CORE_H_
#define _XA2CORE_H_

#include "../Libs/Microsoft_DirectX_SDK_(June 2010)/Include/XAudio2.h"
#include "xa2Manager.h"

#define INPUTCHANNELS (1)			// �\�[�X�`�����l���ԍ�
#define OUTPUTCHANNELS (8)			// �T�|�[�g����ő�`�����l��(�\��)

// XAudio2�G���W��
//--------------------------------------------------------------------------------
class XA2Core
{
public:
	XA2Core() {}
	~XA2Core() {}

	static HRESULT Init(HWND hWnd);
	static void Uninit();

	// �G���W���̎擾
	static IXAudio2 *GetXAudio2() { return m_pXAudio2; }

	static const DWORD &GetChannelMask() { return m_pDetails->OutputFormat.dwChannelMask; }				// �`�����l���ʒu���r�b�g��
	static const UINT32 GetDetailsChannels() { return m_pDetails->OutputFormat.Format.nChannels; }		// Wave�t�H�[���I�[�f�B�I�f�[�^�̃`�����l����
	static const UINT32 GetSamplesPerSec() { return m_pDetails->OutputFormat.Format.nSamplesPerSec; }	// Wave�t�H�[���I�[�f�B�I�f�[�^��1�b������̃T���v����

private:
	// XAudio2�I�u�W�F�N�g�ւ̃C���^�[�t�F�C�X
	static IXAudio2 *m_pXAudio2;

	// �f�o�C�X�֌W�̏��
	static XAUDIO2_DEVICE_DETAILS *m_pDetails;
};

#endif
