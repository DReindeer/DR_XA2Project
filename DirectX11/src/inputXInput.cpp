// ======== ======== ======== ======== ======== ======== ======== ========
//
//
//	title : Xインプットコントローラ
//	Auter : KENSUKE WATANABE
//
//
// ======== ======== ======== ======== ======== ======== ======== ========
#include "input.h"
#include "inputXInput.h"
#pragma warning(disable : 4995)

HRESULT CInputXinput::Init(HINSTANCE hInst)
{
	// 入力処理の初期化
	CInput::Init(hInst);
	
	m_XInputState.Gamepad.bLeftTrigger = 0;
	m_XInputState.Gamepad.bRightTrigger = 0;
	m_XInputState.Gamepad.sThumbLX = 0;
	m_XInputState.Gamepad.sThumbLY = 0;
	m_XInputState.Gamepad.sThumbRX = 0;
	m_XInputState.Gamepad.sThumbRY = 0;
	m_XInputState.Gamepad.wButtons = 0;

	m_GamepadState.bLeftTrigger = 0;
	m_GamepadState.bRightTrigger = 0;
	m_GamepadState.sThumbLX = 0;
	m_GamepadState.sThumbLY = 0;
	m_GamepadState.sThumbRX = 0;
	m_GamepadState.sThumbRY = 0;
	m_GamepadState.wButtons = 0;

	m_GamepadStateOld.bLeftTrigger = 0;
	m_GamepadStateOld.bRightTrigger = 0;
	m_GamepadStateOld.sThumbLX = 0;
	m_GamepadStateOld.sThumbLY = 0;
	m_GamepadStateOld.sThumbRX = 0;
	m_GamepadStateOld.sThumbRY = 0;
	m_GamepadStateOld.wButtons = 0;

	return E_NOTIMPL;
}

void CInputXinput::Uninit(void)
{
	XInputEnable(false); // bool enable
}

void CInputXinput::Update(void)
{
	// Stickデッドゾーンの設定
	SetDedZone();

	DWORD dwResult;
	ZeroMemory(&m_XInputState, sizeof(XINPUT_STATE));
	dwResult = XInputGetState(0, &m_XInputState);    //Xinputからコントローラの状態を取得
	if (dwResult == ERROR_SUCCESS)
	{
		/* キートリガー情報を取得 */
		m_GamepadStateOld = m_GamepadState;

		//コントローラ接続時
		m_GamepadState = m_XInputState.Gamepad;
	}
	else
	{
		//コントローラ非接続時
		m_GamepadStateOld.wButtons = m_GamepadState.wButtons = 0;
		//m_GamepadState = m_XInputState[i].Gamepad;
	}
}

bool CInputXinput::GetPress(int nKey)
{
	return (m_XInputState.Gamepad.wButtons & nKey) ? true : false;
}

bool CInputXinput::GetTrigger(int nKey)
{
	return ((m_XInputState.Gamepad.wButtons & nKey) && !(m_GamepadStateOld.wButtons & nKey)) ? true : false;
}

bool CInputXinput::GetRelease(int nKey)
{
	return (!(m_XInputState.Gamepad.wButtons & nKey) && (m_GamepadStateOld.wButtons & nKey)) ? true : false;
}

bool CInputXinput::GetRepeat(int nKey)
{
	return false;
}

void CInputXinput::SetDedZone(void)
{
	// Zero value if thumbsticks are within the dead zone 
	if ((m_XInputState.Gamepad.sThumbLX < XINPUT_GAMEPAD_LEFT_THUMB_DEADZONE &&
		m_XInputState.Gamepad.sThumbLX > -XINPUT_GAMEPAD_LEFT_THUMB_DEADZONE) &&
		(m_XInputState.Gamepad.sThumbLY < XINPUT_GAMEPAD_LEFT_THUMB_DEADZONE &&
			m_XInputState.Gamepad.sThumbLY > -XINPUT_GAMEPAD_LEFT_THUMB_DEADZONE))
	{
		m_XInputState.Gamepad.sThumbLX = 0;
		m_XInputState.Gamepad.sThumbLY = 0;
	}

	if ((m_XInputState.Gamepad.sThumbRX < XINPUT_GAMEPAD_RIGHT_THUMB_DEADZONE &&
		m_XInputState.Gamepad.sThumbRX > -XINPUT_GAMEPAD_RIGHT_THUMB_DEADZONE) &&
		(m_XInputState.Gamepad.sThumbRY < XINPUT_GAMEPAD_RIGHT_THUMB_DEADZONE &&
			m_XInputState.Gamepad.sThumbRY > -XINPUT_GAMEPAD_RIGHT_THUMB_DEADZONE))
	{
		m_XInputState.Gamepad.sThumbRX = 0;
		m_XInputState.Gamepad.sThumbRY = 0;
	}

}