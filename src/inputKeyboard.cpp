//================================================================================
//
//	Auter : KENSUKE WATANABE
//	Data  : 2016/08/29
//
//--------------------------------------------------------------------------------
#include "InputKeyboard.h"

//================================================================================
// キーボードの初期化処理
//================================================================================
HRESULT CInputKeyboard::Init(HINSTANCE hInst, HWND hWnd)
{
	// 親の初期化(デバイスの作成)
	CInput::Init(hInst);

	HRESULT hr;

	// デバイスオブジェクトを作成
	hr = m_pDInput->CreateDevice(GUID_SysKeyboard, &m_pDIDevice, nullptr);
	if (FAILED(hr) || m_pDIDevice == nullptr)
	{
		MessageBox(hWnd, "キーボードがねぇ！", "警告", MB_ICONWARNING);
		return hr;
	}

	// データフォーマットを設定
	hr = m_pDIDevice->SetDataFormat(&c_dfDIKeyboard);
	if (FAILED(hr))
	{
		MessageBox(hWnd, "キーボードのデータフォーマットを設定できませんでした。", "警告", MB_ICONWARNING);
		return hr;
	}

	// 協調モードを設定（フォアグラウンド＆非排他モード）
	hr = m_pDIDevice->SetCooperativeLevel(hWnd, (DISCL_FOREGROUND | DISCL_NONEXCLUSIVE));
	if (FAILED(hr))
	{
		MessageBox(hWnd, "キーボードの協調モードを設定できませんでした。", "警告", MB_ICONWARNING);
		return hr;
	}

	// m_pDIDeviceアクセス権を獲得(入力制御開始)
	m_pDIDevice->Acquire();

	for (int i = 0; i < NUM_KEY_MAX; i++)
	{
		FlushTrigger(i);
	}

	return hr;
}

//================================================================================
// キーボード終了処理
//================================================================================
void CInputKeyboard::Uninit()
{
	if (m_pDIDevice)
	{// デバイスオブジェクトの開放
		m_pDIDevice->Unacquire();

		m_pDIDevice->Release();
		m_pDIDevice = nullptr;
	}
}

//================================================================================
// キーボードの更新処理
//================================================================================
void CInputKeyboard::Update()
{
	HRESULT hr;
	BYTE aKeyState[NUM_KEY_MAX] = { 0 };

	if (!m_pDIDevice)
	{
		return;
	}

	// デバイスからデータを取得
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