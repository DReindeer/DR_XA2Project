//================================================================================
//
//	Auter : KENSUKE WATANABE
//	Data  : 2016/08/29
//
//--------------------------------------------------------------------------------
#include "inputJoyStick.h"

//================================================================================
// �ÓI�ϐ�
//================================================================================
LPDIRECTINPUTDEVICE8 CInputJoyStick::m_pDIDevice = nullptr;
int CInputJoyStick::m_nCntUseJoy = false;						// �g�p�W���C�X�e�B�b�N�̐�

//================================================================================
// �W���C�X�e�B�b�N�̏�����
//================================================================================
HRESULT CInputJoyStick::Init(HINSTANCE hInstance, HWND hWnd)
{
	// �e�̏�����(�f�o�C�X�̍쐬)
	CInput::Init(hInstance);

	HRESULT hr;
	// �f�o�C�X�̍쐬
	if (FAILED(m_pDInput->EnumDevices(DI8DEVCLASS_GAMECTRL, EnumJoysticksCallback, nullptr, DIEDFL_ATTACHEDONLY)) || m_pDIDevice == nullptr)
	{
		//	MessageBox(hWnd, "�R���g���[����������܂���ł����B", "�x��", MB_ICONWARNING);
		m_bJoystick = false;
		return E_FAIL;
	}

	m_bJoystick = true;	//�W���C�X�e�B�b�N���g�p

						// �f�[�^�t�H�[�}�b�g��ݒ�
	if (FAILED(m_pDIDevice->SetDataFormat(&c_dfDIJoystick)))
	{
		MessageBox(hWnd, "�W���C�X�e�b�N�̃f�[�^�t�H�[�}�b�g��ݒ�ł��܂���ł����B", "�x��", MB_ICONWARNING);
		return E_FAIL;
	}

	// �������[�h��ݒ�i�t�H�A�O���E���h����r�����[�h�j
	if (FAILED(m_pDIDevice->SetCooperativeLevel(hWnd, (DISCL_FOREGROUND | DISCL_NONEXCLUSIVE))))
	{
		MessageBox(hWnd, "�W���C�X�e�B�b�N�̋������[�h��ݒ�ł��܂���ł����B", "�x��", MB_ICONWARNING);
		return E_FAIL;
	}

	//�f�o�C�X�̔\�͎擾
	m_diDevCaps.dwSize = sizeof(DIDEVCAPS);
	if (FAILED(m_pDIDevice->GetCapabilities(&m_diDevCaps)))
	{
		MessageBox(hWnd, "�W���C�X�e�B�b�N�̔\�͎擾���ł��܂���ł����B", "�x��", MB_ICONWARNING);
		return E_FAIL;
	}

	//���͈̔͐ݒ�
	if (FAILED(m_pDIDevice->EnumObjects(EnumAxisCallback, (void*)hWnd, DIDFT_AXIS)))
	{
		MessageBox(hWnd, "�W���C�X�e�B�b�N�̐ݒ肪�ł��܂���ł����B", "�x��", MB_ICONWARNING);
		return E_FAIL;
	}

	// �W���C�X�e�B�b�N�ւ̃A�N�Z�X�����l��(���͐���J�n) �f�o�C�X�ɂ���Ă͉�������ɍs���Ȃ��Ă͂����Ȃ�
	if (FAILED(m_pDIDevice->Poll()))
	{
		while (m_pDIDevice->Acquire() == DIERR_INPUTLOST)
		{
			hr = m_pDIDevice->Acquire();
		}
	}

	return S_OK;
}

//================================================================================
// �W���C�X�e�B�b�N�̃f�o�C�X����
//================================================================================
BOOL CALLBACK CInputJoyStick::EnumJoysticksCallback(const DIDEVICEINSTANCE *pdidInstance, void *pContext)
{
	UNREFERENCED_PARAMETER(pContext);

	static GUID pad_discrimination;					// �f�o�C�X�̎��ʎq���i�[
	DIDEVCAPS diDevCaps;							// �W���C�p�b�h�̔\�͏��

	if (pad_discrimination == pdidInstance->guidInstance)
	{
		return DIENUM_CONTINUE;
	}

	// �񋓂��ꂽ�W���C�X�e�B�b�N�ւ̃C���^�[�t�F�C�X���擾
	if (FAILED(m_pDInput->CreateDevice(pdidInstance->guidInstance, &m_pDIDevice, nullptr)))
	{
		return DIENUM_CONTINUE;
	}

	// �W���C�p�b�h�̔\�͂𒲂ׂ�
	diDevCaps.dwSize = sizeof(DIDEVCAPS);
	if (FAILED(m_pDIDevice->GetCapabilities(&diDevCaps)))
	{
		if (m_pDIDevice != nullptr)
		{
			m_pDIDevice->Release();
			m_pDIDevice = nullptr;
		}
		return DIENUM_CONTINUE;
	}

	// �f�o�C�X�̎��ʎq��ۑ�
	pad_discrimination = pdidInstance->guidInstance;

	// ���̃f�o�C�X���g��(�񋓂��I��)
	return DIENUM_STOP;
}

//================================================================================
// ���̐ݒ�
//================================================================================
BOOL CALLBACK CInputJoyStick::EnumAxisCallback(const DIDEVICEOBJECTINSTANCE *pdidoi, void* pContext)
{
	UNREFERENCED_PARAMETER(pContext);

	DIPROPRANGE diprg;

	diprg.diph.dwSize = sizeof(DIPROPRANGE);
	diprg.diph.dwHeaderSize = sizeof(DIPROPHEADER);
	diprg.diph.dwHow = DIPH_BYID;
	diprg.diph.dwObj = pdidoi->dwType;
	diprg.lMin = 0 - 1000;
	diprg.lMax = 0 + 1000;
	if (FAILED(m_pDIDevice->SetProperty(DIPROP_RANGE, &diprg.diph)))
	{
		return DIENUM_STOP;
	}

	return DIENUM_CONTINUE;
}

//================================================================================
// �W���C�X�e�B�b�N�̏I������
//================================================================================
void CInputJoyStick::Uninit()
{
	if (m_pDIDevice)
	{// �W���C�X�e�B�b�N�ւ̃A�N�Z�X�����J��(���͐���I��)
		m_pDIDevice->Unacquire();

		m_pDIDevice->Release();
		m_pDIDevice = nullptr;
	}
}

//================================================================================
// �W���C�X�e�B�b�N�̍X�V����
//================================================================================
void CInputJoyStick::Update()
{
	if (m_bJoystick == true)
	{
		DIJOYSTATE joyState;

		m_pDIDevice->Poll();

		// �f�o�C�X����f�[�^���擾
		if (SUCCEEDED(m_pDIDevice->GetDeviceState(sizeof(DIJOYSTATE), &joyState)))
		{
			for (int nCnKey = 0; nCnKey < NUM_BUT_MAX; nCnKey++)
			{
				// �L�[�v���X����ۑ�
				m_aStateTrigger[nCnKey] = (m_aState[nCnKey] ^ joyState.rgbButtons[nCnKey]) & joyState.rgbButtons[nCnKey];	//�g���K�[����ۑ�
				m_aStateRelease[nCnKey] = (m_aState[nCnKey] ^ joyState.rgbButtons[nCnKey]) & ~joyState.rgbButtons[nCnKey];	//�����[�X����ۑ�

																															//�L�[���s�[�g���̐��� (�s�����������&0x80)
				if (joyState.rgbButtons[nCnKey])
				{//�L�[��������Ă�����
					if (m_aStateRepeatCnt[nCnKey] < COUNT_WAIT_REPEAT)
					{//���s�[�g�J�E���g������ɒB���Ă��Ȃ�������
						m_aStateRepeatCnt[nCnKey]++;
						if (m_aStateRepeatCnt[nCnKey] == 1 || m_aStateRepeatCnt[nCnKey] >= COUNT_WAIT_REPEAT)
						{//�����͂��߂��A����ɒB�����Ƃ�
							m_aStateRepeat[nCnKey] = joyState.rgbButtons[nCnKey];
						}
						else
						{//����ɒB����܂ł̊�
							m_aStateRepeat[nCnKey] = 0;
						}
					}
				}
				else
				{//�L�[��������Ă��Ȃ�������
					m_aStateRepeat[nCnKey] = 0;
					m_aStateRepeatCnt[nCnKey] = 0;
				}

				m_aState[nCnKey] = joyState.rgbButtons[nCnKey];	//���݂̃L�[����ۑ�
			}
			// �A�i���O�X�e�B�b�N����ۑ�
			m_nLx = joyState.lX;
			m_nLy = joyState.lY;
			m_nLz = joyState.lZ;
			m_nLRz = joyState.lRz;
		}
		else
		{//���s���Ă���A�N�Z�X���Ƃ�ɂ���
			m_pDIDevice->Acquire();
		}
	}
}