//================================================================================
//
// Auter : KENSUKE WATANABE
// Data  : 2018/01/02
//
//================================================================================
#include "xa2Core.h"

// �ÓI�����o�ϐ�
//--------------------------------------------------------------------------------
IXAudio2 *XA2Core::m_pXAudio2 = nullptr;				// XAudio2�I�u�W�F�N�g�ւ̃C���^�[�t�F�C�X
XAUDIO2_DEVICE_DETAILS *XA2Core::m_pDetails = nullptr;	// �f�o�C�X�֌W�̏��

//--------------------------------------------------------------------------------
// ����������
//--------------------------------------------------------------------------------
HRESULT XA2Core::Init(HWND hWnd)
{
	// COM���C�u�����̏�����
	CoInitializeEx(nullptr, COINIT_MULTITHREADED);

	HRESULT hr;

	// XAudio2�I�u�W�F�N�g�̍쐬
	hr = XAudio2Create(&m_pXAudio2, 0);
	if(FAILED(hr))
	{
		MessageBox(hWnd, "XAudio2�I�u�W�F�N�g�̍쐬�Ɏ��s�I", "�x��", MB_ICONWARNING);

		// COM���C�u�����̏I������
		CoUninitialize();

		return hr;
	}

	// �T�|�[�g����Ă���T���v���̃p�����[�^���ɂ��邱�Ƃ��m�F
	m_pDetails = new XAUDIO2_DEVICE_DETAILS;
	hr = m_pXAudio2->GetDeviceDetails(0, m_pDetails);
	if (FAILED(hr))
	{
		Uninit();
		return hr;
	}

	if (m_pDetails->OutputFormat.Format.nChannels > OUTPUTCHANNELS)
	{
		Uninit();
		return E_FAIL;
	}

	if (m_pDetails->OutputFormat.Format.nSamplesPerSec < XAUDIO2FX_REVERB_MIN_FRAMERATE)
	{
		Uninit();
		return E_FAIL;
	}

	return S_OK;
}

//--------------------------------------------------------------------------------
// �I������
//--------------------------------------------------------------------------------
void XA2Core::Uninit()
{
	if(m_pXAudio2)
	{
		// XAudio2�I�u�W�F�N�g�̊J��
		m_pXAudio2->Release();
		m_pXAudio2 = nullptr;
	}

	if (m_pDetails)
	{
		// �f�o�C�X�֌W�̏��
		delete m_pDetails;
		m_pDetails = nullptr;
	}
	
	// COM���C�u�����̏I������
	CoUninitialize();
}
