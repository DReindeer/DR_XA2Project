// ======== ======== ======== ======== ======== ======== ======== ========
//
//
//	title : ����
//	Auter : KENSUKE WATANABE
//
//
// ======== ======== ======== ======== ======== ======== ======== ========
#include "input.h"

// ======== ======== ======== ======== ======== ======== ======== ========
// �ÓI�ϐ�
// ======== ======== ======== ======== ======== ======== ======== ========
LPDIRECTINPUT8 CInput::m_pDInput = nullptr;	// DirectInput�I�u�W�F�N�g

// ======== ======== ======== ======== ======== ======== ======== ========
// ���͏����̏���������
// ======== ======== ======== ======== ======== ======== ======== ========
HRESULT CInput::Init(HINSTANCE hInst)
{
	HRESULT hr = S_FALSE;

	// DirectInput�I�u�W�F�N�g�̍쐬
	if(!m_pDInput)
	{
		hr = DirectInput8Create(hInst, DIRECTINPUT_VERSION,
			IID_IDirectInput8, (void**)&m_pDInput, nullptr);
	}

	return hr;
}

// ======== ======== ======== ======== ======== ======== ======== ========
// ���͏����̏I������
// ======== ======== ======== ======== ======== ======== ======== ========
void CInput::Uninit(void)
{
	if(m_pDInput)
	{// DirectInput�I�u�W�F�N�g�̊J��
		m_pDInput->Release();
		m_pDInput = nullptr;
	}
}