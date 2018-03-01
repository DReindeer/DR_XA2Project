// ======== ======== ======== ======== ======== ======== ======== ========
//
//
//	title : �}�E�X
//	Auter : KENSUKE WATANABE
//
//
// ======== ======== ======== ======== ======== ======== ======== ========
#include "InputMouse.h"

// ======== ======== ======== ======== ======== ======== ======== ========
// �}�E�X�̏�����
// ======== ======== ======== ======== ======== ======== ======== ========
HRESULT CInputMouse::Init(HINSTANCE hInstance, HWND hWnd)
{
	// �e�̏�����(�f�o�C�X�̍쐬)
	CInput::Init(hInstance);

	// �f�o�C�X�̍쐬
	if (FAILED(m_pDInput->CreateDevice(GUID_SysMouse, &m_pDIDevice, nullptr)))
	{
		MessageBox(hWnd, "�}�E�X���˂��I", "�x���I", MB_ICONWARNING);
		return E_FAIL;
	}

	// �f�[�^�t�H�[�}�b�g��ݒ�
	if (FAILED(m_pDIDevice->SetDataFormat(&c_dfDIMouse2)))
	{
		MessageBox(hWnd, "�}�E�X�̃f�[�^�t�H�[�}�b�g��ݒ�ł��܂���ł����B", "�x���I", MB_ICONWARNING);
		return E_FAIL;
	}

	// �������[�h��ݒ�i�t�H�A�O���E���h����r�����[�h�j
	if (FAILED(m_pDIDevice->SetCooperativeLevel(hWnd, (DISCL_FOREGROUND | DISCL_NONEXCLUSIVE))))
	{
		MessageBox(hWnd, "�}�E�X�̋������[�h��ݒ�ł��܂���ł����B", "�x���I", MB_ICONWARNING);
		return E_FAIL;
	}

	// �}�E�X�ւ̃A�N�Z�X�����l��(���͐���J�n) �f�o�C�X�ɂ���Ă͉�������ɍs���Ȃ��Ă͂����Ȃ�
	m_pDIDevice->Acquire();

	return S_OK;
}

// ======== ======== ======== ======== ======== ======== ======== ========
// �}�E�X�̏I������
// ======== ======== ======== ======== ======== ======== ======== ========
void CInputMouse::Uninit(void)
{
	if (m_pDIDevice)
	{
		// �}�E�X�ւ̃A�N�Z�X�����J��(���͐���I��)
		m_pDIDevice->Unacquire();

		m_pDIDevice->Release();
		m_pDIDevice = nullptr;
	}
}

// ======== ======== ======== ======== ======== ======== ======== ========
// �}�E�X�̍X�V����
// ======== ======== ======== ======== ======== ======== ======== ========
void CInputMouse::Update(HWND hWnd)
{
	// �}�E�X�ʒu�X�V
	m_PosOld.x = m_Pos.x;
	m_PosOld.y = m_Pos.y;
	GetCursorPos(&m_Pos);

	ScreenToClient(hWnd, &m_Pos);

	DIMOUSESTATE2 MouseState;

	// �f�o�C�X����f�[�^���擾
	if (SUCCEEDED(m_pDIDevice->GetDeviceState(sizeof(DIMOUSESTATE2), &MouseState)))
	{
		m_PointMove = XMFLOAT2(static_cast<float>(MouseState.lX), static_cast<float>(MouseState.lY));
		for (int nCnt = 0; nCnt < 8; nCnt++)
		{
			// �L�[�v���X����ۑ�
			m_aStateTrigger[nCnt] = (m_aState[nCnt] ^ MouseState.rgbButtons[nCnt]) & MouseState.rgbButtons[nCnt];	//�g���K�[����ۑ�
			m_aStateRelease[nCnt] = (m_aState[nCnt] ^ MouseState.rgbButtons[nCnt]) & ~MouseState.rgbButtons[nCnt];	//�����[�X����ۑ�

																													//�L�[���s�[�g���̐��� (�s�����������&0x80)
			if (MouseState.rgbButtons[nCnt] & 0x80)
			{//�L�[��������Ă�����
				if (m_aStateRepeatCnt[nCnt] < COUNT_WAIT_REPEAT)
				{//���s�[�g�J�E���g������ɒB���Ă��Ȃ�������
					m_aStateRepeatCnt[nCnt]++;
					if (m_aStateRepeatCnt[nCnt] == 1 || m_aStateRepeatCnt[nCnt] >= COUNT_WAIT_REPEAT)
					{//�����͂��߂��A����ɒB�����Ƃ�
						m_aStateRepeat[nCnt] = MouseState.rgbButtons[nCnt];
					}
					else
					{//����ɒB����܂ł̊�
						m_aStateRepeat[nCnt] = 0;
					}
				}
			}
			else
			{//�L�[��������Ă��Ȃ�������
				m_aStateRepeat[nCnt] = 0;
				m_aStateRepeatCnt[nCnt] = 0;
			}
			m_aState[nCnt] = MouseState.rgbButtons[nCnt];	//���݂̃L�[����ۑ�
		}
	}
	else
	{//���s���Ă���A�N�Z�X���Ƃ�ɂ���
	 // �}�E�X�ւ̃A�N�Z�X�����擾 
		m_pDIDevice->Acquire();
	}
}
