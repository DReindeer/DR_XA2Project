
#ifndef INCLUDE_INPUT_XINPUT
#define INCLUDE_INPUT_XINPUT

#include "input.h"

#include <XInput.h>
#pragma comment( lib, "xinput.lib")


////
//// Constants for gamepad buttons
////
#define XI_UP		0x0001
#define XI_DOWN		0x0002
#define XI_LEFT		0x0004
#define XI_RIGHT	0x0008
#define XI_START	0x0010
#define XI_BACK		0x0020
#define XI_LT		0x0040
#define XI_RT		0x0080
#define XI_LB		0x0100
#define XI_RB		0x0200
#define XI_A		0x1000
#define XI_B		0x2000
#define XI_X		0x4000
#define XI_Y		0x8000

class CInputXinput : public CInput
{
public:
	CInputXinput() {}
	~CInputXinput() {}

	HRESULT Init(HINSTANCE hInst);
	void Uninit(void);
	void Update(void)override;

	bool GetPress(int nKey);
	bool GetTrigger(int nKey);
	bool GetRelease(int nKey);
	bool GetRepeat(int nKey);
	const BYTE &GetLTrigger(void) { return m_GamepadState.bLeftTrigger; }
	const BYTE &GetRTrigger(void) { return m_GamepadState.bRightTrigger; }
	const SHORT &GetStickLX(void) { return m_GamepadState.sThumbLX; }
	const SHORT &GetStickLY(void) { return m_GamepadState.sThumbLY; }
	const SHORT &GetStickRX(void) { return m_GamepadState.sThumbRX; }
	const SHORT &GetStickRY(void) { return m_GamepadState.sThumbRY; }

	void SetDedZone(void);

private:
	XINPUT_GAMEPAD m_GamepadState;			//ゲームパッドステート
	XINPUT_GAMEPAD m_GamepadStateOld;		//ゲームパッドステート
	XINPUT_STATE m_XInputState;				//ゲームパッドステート格納用。
};

#endif

