//================================================================================
//
// 入力処理 [input.h]
// Author : KENSUKE WATANABE
//
//================================================================================
#pragma once
#ifndef _INPUTJOYSTICK_H_
#define _INPUTJOYSTICK_H_

#include "input.h"

//--------------------------------------------------------------------------------
// ジョイスティック
//--------------------------------------------------------------------------------
class CInputJoyStick : public CInput
{
public:
	typedef enum	// ボタン配置
	{
		BUT_1 = 0,	// 1
		BUT_2,		// 2
		BUT_3,		// 3
		BUT_4,		// 4
		BUT_L1,		// L1
		BUT_R1,		// R1
		BUT_L2,		// L2
		BUT_R2,		// R2
		BUT_9,		// 9
		BUT_10,		// 10 SELECT
		BUT_L3,		// L3 START
		BUT_R3,		// R3
		NUM_BUT_MAX = 32
	}ELECOM_BUTTON;

	CInputJoyStick() :m_bJoystick(false){ m_nCntUseJoy++; }
	~CInputJoyStick() { m_nCntUseJoy--; }

	HRESULT Init(HINSTANCE hInst, HWND hWnd);
	void Uninit();
	void Update()override;

	static BOOL CALLBACK EnumJoysticksCallback(const DIDEVICEINSTANCE *pdidInstance, void* pContext);
	static BOOL CALLBACK EnumAxisCallback(const DIDEVICEOBJECTINSTANCE *pdidoi, void* pCintext);

	bool GetPress(int nKey) { return (m_aState[nKey] & 0x80) ? true : false; }
	bool GetTrigger(int nKey) { return (m_aStateTrigger[nKey] & 0x80) ? true : false; }
	bool GetRelease(int nKey) { return (m_aStateRelease[nKey] & 0x80) ? true : false; }
	bool GetRepeat(int nKey) { return (m_aStateRepeat[nKey] & 0x80) ? true : false; }
	void FlushTrigger(int nKey) { m_aStateTrigger[nKey] = 0; }
	D3DXVECTOR2 GetLeftAxis() { return D3DXVECTOR2((float)m_nLx, (float)m_nLy); }
	D3DXVECTOR2 GetRightAxis() { return D3DXVECTOR2((float)m_nLz, (float)m_nLRz); }
	int GetCntUse() { return m_nCntUseJoy; }

private:
	DIDEVCAPS	m_diDevCaps;					// 
	BYTE		m_aState[NUM_BUT_MAX];			// ジョイスティックの入力情報ワーク キモ押されているか
	BYTE		m_aStateTrigger[NUM_BUT_MAX];	// ジョイスティックのトリガー情報ワーク押した瞬間
	BYTE		m_aStateRelease[NUM_BUT_MAX];	// ジョイスティックのリリース情報ワーク離した瞬間
	BYTE		m_aStateRepeat[NUM_BUT_MAX];	// ジョイスティックのリピート情報ワーク
	int			m_aStateRepeatCnt[NUM_BUT_MAX];	// ジョイスティックのリピートカウンタ
	int			m_nLx;							// 左スティックの軸左右
	int			m_nLy;							// 左スティックの軸上下
	int			m_nLz;							// 右スティックの上下
	int			m_nLRz;							// 右スティックの左
	bool		m_bJoystick;					// ジョイスティックを使用するかどうか
	static int			m_nCntUseJoy;			// 使用ジョイスティックの数

	static LPDIRECTINPUTDEVICE8 m_pDIDevice;	// Deviceオブジェクト(入力に必要)
};

#endif