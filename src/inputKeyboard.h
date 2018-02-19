//================================================================================
//
// 入力処理 [input.h]
// Author : KENSUKE WATANABE
//
//================================================================================
#pragma once
#ifndef _INPUTKEYBOARD_H_
#define _INPUTKEYBOARD_H_

#include "input.h"

//--------------------------------------------------------------------------------
// キーボード
//--------------------------------------------------------------------------------
class CInputKeyboard : public CInput
{
public:
	CInputKeyboard() :m_pDIDevice(nullptr) {};
	~CInputKeyboard() {};

	HRESULT Init(HINSTANCE hInst, HWND hWnd);
	void Uninit();
	void Update()override;

	bool GetPress(int nKey) { return (m_aState[nKey] & 0x80) ? true : false; }
	bool GetTrigger(int nKey) { return (m_aStateTrigger[nKey] & 0x80) ? true : false; }
	bool GetRelease(int nKey) { return (m_aStateRelease[nKey] & 0x80) ? true : false; }
	bool GetRepeat(int nKey) { return (m_aStateRepeat[nKey] & 0x80) ? true : false; }
	void FlushTrigger(int nKey) { m_aStateTrigger[nKey] = 0; }

private:
	BYTE	m_aState[NUM_KEY_MAX];			// キーボードの状態を受け取るワーク
	BYTE	m_aStateTrigger[NUM_KEY_MAX];	// トリガーワーク
	BYTE	m_aStateRelease[NUM_KEY_MAX];	// リリースワーク
	BYTE	m_aStateRepeat[NUM_KEY_MAX];	// リピートワーク
	int		m_aStateRepeatCnt[NUM_KEY_MAX];	// リピートカウンタ

	LPDIRECTINPUTDEVICE8 m_pDIDevice;		// Deviceオブジェクト(入力に必要)
};

#endif