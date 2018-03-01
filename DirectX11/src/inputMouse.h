// ======== ======== ======== ======== ======== ======== ======== ========
//
// 入力処理 [input.h]
// Author : KENSUKE WATANABE
//
// ======== ======== ======== ======== ======== ======== ======== ========
#pragma once
#ifndef _INPUTMOUSE_H_
#define _INPUTMOUSE_H_

#include "input.h"
#include "Wmath.h"

// -------- -------- -------- -------- -------- -------- -------- --------
// マウス
// -------- -------- -------- -------- -------- -------- -------- --------
class CInputMouse : public CInput
{
public:
	typedef enum			// マウスボタン配置
	{
		MOUSE_RIGHT = 0,	// 右
		MOUSE_LEFT = 1		// 左
	}MOUSE_BUTTON;

	CInputMouse() :m_pDIDevice(nullptr) {};
	~CInputMouse() {};

	HRESULT Init(HINSTANCE hInst, HWND hWnd);
	void Uninit(void);
	void Update(HWND hWnd);

	int GetPosX(void) { return (int)m_Pos.x; }
	int GetPosY(void) { return (int)m_Pos.y; }
	bool GetPress(int nKey) { return (m_aState[nKey] & 0x80) ? true : false; }
	bool GetTrigger(int nKey) { return (m_aStateTrigger[nKey] & 0x80) ? true : false; }
	bool GetRelease(int nKey) { return (m_aStateRelease[nKey] & 0x80) ? true : false; }
	bool GetRepeat(int nKey) { return (m_aStateRepeat[nKey] & 0x80) ? true : false; }
	void FlushTrigger(int nKey) { m_aStateTrigger[nKey] = 0; }

private:
	BYTE			m_aState[NUM_MOUSE_MAX];			//
	BYTE			m_aStateTrigger[NUM_MOUSE_MAX];		//
	BYTE			m_aStateRelease[NUM_MOUSE_MAX];		//
	BYTE			m_aStateRepeat[NUM_MOUSE_MAX];		//
	BYTE			m_aStateRepeatCnt[NUM_MOUSE_MAX];	//
	POINT			m_Pos;								// マウスの座標
	POINT			m_PosOld;							// マウスの1f前の座標
	XMFLOAT2		m_PointMove;						// マウスの移動量

	LPDIRECTINPUTDEVICE8 m_pDIDevice;		// Deviceオブジェクト(入力に必要)
};

#endif