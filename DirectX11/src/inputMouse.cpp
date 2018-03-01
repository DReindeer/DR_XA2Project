// ======== ======== ======== ======== ======== ======== ======== ========
//
//
//	title : マウス
//	Auter : KENSUKE WATANABE
//
//
// ======== ======== ======== ======== ======== ======== ======== ========
#include "InputMouse.h"

// ======== ======== ======== ======== ======== ======== ======== ========
// マウスの初期化
// ======== ======== ======== ======== ======== ======== ======== ========
HRESULT CInputMouse::Init(HINSTANCE hInstance, HWND hWnd)
{
	// 親の初期化(デバイスの作成)
	CInput::Init(hInstance);

	// デバイスの作成
	if (FAILED(m_pDInput->CreateDevice(GUID_SysMouse, &m_pDIDevice, nullptr)))
	{
		MessageBox(hWnd, "マウスがねぇ！", "警告！", MB_ICONWARNING);
		return E_FAIL;
	}

	// データフォーマットを設定
	if (FAILED(m_pDIDevice->SetDataFormat(&c_dfDIMouse2)))
	{
		MessageBox(hWnd, "マウスのデータフォーマットを設定できませんでした。", "警告！", MB_ICONWARNING);
		return E_FAIL;
	}

	// 協調モードを設定（フォアグラウンド＆非排他モード）
	if (FAILED(m_pDIDevice->SetCooperativeLevel(hWnd, (DISCL_FOREGROUND | DISCL_NONEXCLUSIVE))))
	{
		MessageBox(hWnd, "マウスの協調モードを設定できませんでした。", "警告！", MB_ICONWARNING);
		return E_FAIL;
	}

	// マウスへのアクセス権を獲得(入力制御開始) デバイスによっては何回も取りに行かなくてはいけない
	m_pDIDevice->Acquire();

	return S_OK;
}

// ======== ======== ======== ======== ======== ======== ======== ========
// マウスの終了処理
// ======== ======== ======== ======== ======== ======== ======== ========
void CInputMouse::Uninit(void)
{
	if (m_pDIDevice)
	{
		// マウスへのアクセス権を開放(入力制御終了)
		m_pDIDevice->Unacquire();

		m_pDIDevice->Release();
		m_pDIDevice = nullptr;
	}
}

// ======== ======== ======== ======== ======== ======== ======== ========
// マウスの更新処理
// ======== ======== ======== ======== ======== ======== ======== ========
void CInputMouse::Update(HWND hWnd)
{
	// マウス位置更新
	m_PosOld.x = m_Pos.x;
	m_PosOld.y = m_Pos.y;
	GetCursorPos(&m_Pos);

	ScreenToClient(hWnd, &m_Pos);

	DIMOUSESTATE2 MouseState;

	// デバイスからデータを取得
	if (SUCCEEDED(m_pDIDevice->GetDeviceState(sizeof(DIMOUSESTATE2), &MouseState)))
	{
		m_PointMove = XMFLOAT2(static_cast<float>(MouseState.lX), static_cast<float>(MouseState.lY));
		for (int nCnt = 0; nCnt < 8; nCnt++)
		{
			// キープレス情報を保存
			m_aStateTrigger[nCnt] = (m_aState[nCnt] ^ MouseState.rgbButtons[nCnt]) & MouseState.rgbButtons[nCnt];	//トリガー情報を保存
			m_aStateRelease[nCnt] = (m_aState[nCnt] ^ MouseState.rgbButtons[nCnt]) & ~MouseState.rgbButtons[nCnt];	//リリース情報を保存

																													//キーリピート情報の生成 (不具合がおきたら&0x80)
			if (MouseState.rgbButtons[nCnt] & 0x80)
			{//キーが押されていたら
				if (m_aStateRepeatCnt[nCnt] < COUNT_WAIT_REPEAT)
				{//リピートカウントが上限に達していなかったら
					m_aStateRepeatCnt[nCnt]++;
					if (m_aStateRepeatCnt[nCnt] == 1 || m_aStateRepeatCnt[nCnt] >= COUNT_WAIT_REPEAT)
					{//押しはじめか、上限に達したとき
						m_aStateRepeat[nCnt] = MouseState.rgbButtons[nCnt];
					}
					else
					{//上限に達するまでの間
						m_aStateRepeat[nCnt] = 0;
					}
				}
			}
			else
			{//キーが押されていなかったら
				m_aStateRepeat[nCnt] = 0;
				m_aStateRepeatCnt[nCnt] = 0;
			}
			m_aState[nCnt] = MouseState.rgbButtons[nCnt];	//現在のキー情報を保存
		}
	}
	else
	{//失敗してたらアクセス権とりにいく
	 // マウスへのアクセス権を取得 
		m_pDIDevice->Acquire();
	}
}
