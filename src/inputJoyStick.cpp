//================================================================================
//
//	Auter : KENSUKE WATANABE
//	Data  : 2016/08/29
//
//--------------------------------------------------------------------------------
#include "inputJoyStick.h"

//================================================================================
// 静的変数
//================================================================================
LPDIRECTINPUTDEVICE8 CInputJoyStick::m_pDIDevice = nullptr;
int CInputJoyStick::m_nCntUseJoy = false;						// 使用ジョイスティックの数

//================================================================================
// ジョイスティックの初期化
//================================================================================
HRESULT CInputJoyStick::Init(HINSTANCE hInstance, HWND hWnd)
{
	// 親の初期化(デバイスの作成)
	CInput::Init(hInstance);

	HRESULT hr;
	// デバイスの作成
	if (FAILED(m_pDInput->EnumDevices(DI8DEVCLASS_GAMECTRL, EnumJoysticksCallback, nullptr, DIEDFL_ATTACHEDONLY)) || m_pDIDevice == nullptr)
	{
		//	MessageBox(hWnd, "コントローラが見つかりませんでした。", "警告", MB_ICONWARNING);
		m_bJoystick = false;
		return E_FAIL;
	}

	m_bJoystick = true;	//ジョイスティックを使用

						// データフォーマットを設定
	if (FAILED(m_pDIDevice->SetDataFormat(&c_dfDIJoystick)))
	{
		MessageBox(hWnd, "ジョイステックのデータフォーマットを設定できませんでした。", "警告", MB_ICONWARNING);
		return E_FAIL;
	}

	// 協調モードを設定（フォアグラウンド＆非排他モード）
	if (FAILED(m_pDIDevice->SetCooperativeLevel(hWnd, (DISCL_FOREGROUND | DISCL_NONEXCLUSIVE))))
	{
		MessageBox(hWnd, "ジョイスティックの協調モードを設定できませんでした。", "警告", MB_ICONWARNING);
		return E_FAIL;
	}

	//デバイスの能力取得
	m_diDevCaps.dwSize = sizeof(DIDEVCAPS);
	if (FAILED(m_pDIDevice->GetCapabilities(&m_diDevCaps)))
	{
		MessageBox(hWnd, "ジョイスティックの能力取得ができませんでした。", "警告", MB_ICONWARNING);
		return E_FAIL;
	}

	//軸の範囲設定
	if (FAILED(m_pDIDevice->EnumObjects(EnumAxisCallback, (void*)hWnd, DIDFT_AXIS)))
	{
		MessageBox(hWnd, "ジョイスティックの設定ができませんでした。", "警告", MB_ICONWARNING);
		return E_FAIL;
	}

	// ジョイスティックへのアクセス権を獲得(入力制御開始) デバイスによっては何回も取りに行かなくてはいけない
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
// ジョイスティックのデバイス生成
//================================================================================
BOOL CALLBACK CInputJoyStick::EnumJoysticksCallback(const DIDEVICEINSTANCE *pdidInstance, void *pContext)
{
	UNREFERENCED_PARAMETER(pContext);

	static GUID pad_discrimination;					// デバイスの識別子を格納
	DIDEVCAPS diDevCaps;							// ジョイパッドの能力情報

	if (pad_discrimination == pdidInstance->guidInstance)
	{
		return DIENUM_CONTINUE;
	}

	// 列挙されたジョイスティックへのインターフェイスを取得
	if (FAILED(m_pDInput->CreateDevice(pdidInstance->guidInstance, &m_pDIDevice, nullptr)))
	{
		return DIENUM_CONTINUE;
	}

	// ジョイパッドの能力を調べる
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

	// デバイスの識別子を保存
	pad_discrimination = pdidInstance->guidInstance;

	// このデバイスを使う(列挙を終了)
	return DIENUM_STOP;
}

//================================================================================
// 軸の設定
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
// ジョイスティックの終了処理
//================================================================================
void CInputJoyStick::Uninit()
{
	if (m_pDIDevice)
	{// ジョイスティックへのアクセス権を開放(入力制御終了)
		m_pDIDevice->Unacquire();

		m_pDIDevice->Release();
		m_pDIDevice = nullptr;
	}
}

//================================================================================
// ジョイスティックの更新処理
//================================================================================
void CInputJoyStick::Update()
{
	if (m_bJoystick == true)
	{
		DIJOYSTATE joyState;

		m_pDIDevice->Poll();

		// デバイスからデータを取得
		if (SUCCEEDED(m_pDIDevice->GetDeviceState(sizeof(DIJOYSTATE), &joyState)))
		{
			for (int nCnKey = 0; nCnKey < NUM_BUT_MAX; nCnKey++)
			{
				// キープレス情報を保存
				m_aStateTrigger[nCnKey] = (m_aState[nCnKey] ^ joyState.rgbButtons[nCnKey]) & joyState.rgbButtons[nCnKey];	//トリガー情報を保存
				m_aStateRelease[nCnKey] = (m_aState[nCnKey] ^ joyState.rgbButtons[nCnKey]) & ~joyState.rgbButtons[nCnKey];	//リリース情報を保存

																															//キーリピート情報の生成 (不具合がおきたら&0x80)
				if (joyState.rgbButtons[nCnKey])
				{//キーが押されていたら
					if (m_aStateRepeatCnt[nCnKey] < COUNT_WAIT_REPEAT)
					{//リピートカウントが上限に達していなかったら
						m_aStateRepeatCnt[nCnKey]++;
						if (m_aStateRepeatCnt[nCnKey] == 1 || m_aStateRepeatCnt[nCnKey] >= COUNT_WAIT_REPEAT)
						{//押しはじめか、上限に達したとき
							m_aStateRepeat[nCnKey] = joyState.rgbButtons[nCnKey];
						}
						else
						{//上限に達するまでの間
							m_aStateRepeat[nCnKey] = 0;
						}
					}
				}
				else
				{//キーが押されていなかったら
					m_aStateRepeat[nCnKey] = 0;
					m_aStateRepeatCnt[nCnKey] = 0;
				}

				m_aState[nCnKey] = joyState.rgbButtons[nCnKey];	//現在のキー情報を保存
			}
			// アナログスティック情報を保存
			m_nLx = joyState.lX;
			m_nLy = joyState.lY;
			m_nLz = joyState.lZ;
			m_nLRz = joyState.lRz;
		}
		else
		{//失敗してたらアクセス権とりにいく
			m_pDIDevice->Acquire();
		}
	}
}