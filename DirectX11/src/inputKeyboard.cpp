// ======== ======== ======== ======== ======== ======== ======== ========
//
//
//	title : �L�[�{�[�h
//	Auter : KENSUKE WATANABE
//
//
// ======== ======== ======== ======== ======== ======== ======== ========
#include "InputKeyboard.h"

// ======== ======== ======== ======== ======== ======== ======== ========
// �L�[�{�[�h�̏���������
// ======== ======== ======== ======== ======== ======== ======== ========
HRESULT CInputKeyboard::Init(HINSTANCE hInst, HWND hWnd)
{
	// �e�̏�����(�f�o�C�X�̍쐬)
	CInput::Init(hInst);

	HRESULT hr;

	// �f�o�C�X�I�u�W�F�N�g���쐬
	hr = m_pDInput->CreateDevice(GUID_SysKeyboard, &m_pDIDevice, nullptr);
	if (FAILED(hr) || m_pDIDevice == nullptr)
	{
		MessageBox(hWnd, "�L�[�{�[�h���˂��I", "�x���I", MB_ICONWARNING);
		return hr;
	}

	// �f�[�^�t�H�[�}�b�g��ݒ�
	hr = m_pDIDevice->SetDataFormat(&c_dfDIKeyboard);
	if (FAILED(hr))
	{
		MessageBox(hWnd, "�L�[�{�[�h�̃f�[�^�t�H�[�}�b�g��ݒ�ł��܂���ł����B", "�x���I", MB_ICONWARNING);
		return hr;
	}

	// �������[�h��ݒ�i�t�H�A�O���E���h����r�����[�h�j
	hr = m_pDIDevice->SetCooperativeLevel(hWnd, (DISCL_FOREGROUND | DISCL_NONEXCLUSIVE));
	if (FAILED(hr))
	{
		MessageBox(hWnd, "�L�[�{�[�h�̋������[�h��ݒ�ł��܂���ł����B", "�x���I", MB_ICONWARNING);
		return hr;
	}

	// m_pDIDevice�A�N�Z�X�����l��(���͐���J�n)
	m_pDIDevice->Acquire();

	for (int i = 0; i < NUM_KEY_MAX; i++)
	{
		FlushTrigger(i);
	}

	return hr;
}

// ======== ======== ======== ======== ======== ======== ======== ========
// �L�[�{�[�h�I������
// ======== ======== ======== ======== ======== ======== ======== ========
void CInputKeyboard::Uninit(void)
{
	if (m_pDIDevice)
	{// �f�o�C�X�I�u�W�F�N�g�̊J��
		m_pDIDevice->Unacquire();

		m_pDIDevice->Release();
		m_pDIDevice = nullptr;
	}
}

// ======== ======== ======== ======== ======== ======== ======== ========
// �L�[�{�[�h�̍X�V����
// ======== ======== ======== ======== ======== ======== ======== ========
void CInputKeyboard::Update(void)
{
	HRESULT hr;
	BYTE aKeyState[NUM_KEY_MAX] = { 0 };

	if (!m_pDIDevice)
	{
		return;
	}

	// �f�o�C�X����f�[�^���擾
	hr = m_pDIDevice->GetDeviceState(sizeof(aKeyState), aKeyState);
	if (SUCCEEDED(hr))
	{
		for (int nCntKey = 0; nCntKey < NUM_KEY_MAX; nCntKey++)
		{
			m_aStateTrigger[nCntKey] = (m_aState[nCntKey] ^ aKeyState[nCntKey]) & aKeyState[nCntKey];
			m_aStateRelease[nCntKey] = (m_aState[nCntKey] ^ aKeyState[nCntKey]) & ~aKeyState[nCntKey];
			m_aStateRepeat[nCntKey] = m_aStateTrigger[nCntKey];

			if (aKeyState[nCntKey])
			{
				m_aStateRepeatCnt[nCntKey]++;
				if (m_aStateRepeatCnt[nCntKey] >= COUNT_WAIT_REPEAT)
				{
					m_aStateRepeat[nCntKey] = aKeyState[nCntKey];
				}
			}
			else
			{
				m_aStateRepeatCnt[nCntKey] = 0;
				m_aStateRepeat[nCntKey] = 0;
			}

			m_aState[nCntKey] = aKeyState[nCntKey];
		}
	}
	else
	{
		m_pDIDevice->Acquire();
	}
}