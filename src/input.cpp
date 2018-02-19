//================================================================================
//
//	Auter : KENSUKE WATANABE
//	Data  : 2016/08/29
//
//--------------------------------------------------------------------------------
#include "input.h"
#include "InputKeyboard.h"
#include "inputJoyStick.h"
#include "InputMouse.h"

//--------------------------------------------------------------------------------
// 静的変数
//--------------------------------------------------------------------------------
LPDIRECTINPUT8 CInput::m_pDInput = nullptr;	// DirectInputオブジェクト

//================================================================================
// 入力処理の初期化処理
//================================================================================
HRESULT CInput::Init(HINSTANCE hInst)
{
	HRESULT hr = S_FALSE;

	// DirectInputオブジェクトの作成
	if(!m_pDInput)
	{
		hr = DirectInput8Create(hInst, DIRECTINPUT_VERSION,
			IID_IDirectInput8, (void**)&m_pDInput, nullptr);
	}

	return hr;
}

//================================================================================
// 入力処理の終了処理
//================================================================================
void CInput::Uninit()
{
	if(m_pDInput)
	{// DirectInputオブジェクトの開放
		m_pDInput->Release();
		m_pDInput = nullptr;
	}
}