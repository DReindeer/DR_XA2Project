// ======== ======== ======== ======== ======== ======== ======== ========
//
// ���͏��� [input.h]
// Author : KENSUKE WATANABE
//
// ======== ======== ======== ======== ======== ======== ======== ========
#pragma once
#ifndef _INPUTMOUSE_H_
#define _INPUTMOUSE_H_

#include "input.h"
#include "Wmath.h"

// -------- -------- -------- -------- -------- -------- -------- --------
// �}�E�X
// -------- -------- -------- -------- -------- -------- -------- --------
class CInputMouse : public CInput
{
public:
	typedef enum			// �}�E�X�{�^���z�u
	{
		MOUSE_RIGHT = 0,	// �E
		MOUSE_LEFT = 1		// ��
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
	POINT			m_Pos;								// �}�E�X�̍��W
	POINT			m_PosOld;							// �}�E�X��1f�O�̍��W
	XMFLOAT2		m_PointMove;						// �}�E�X�̈ړ���

	LPDIRECTINPUTDEVICE8 m_pDIDevice;		// Device�I�u�W�F�N�g(���͂ɕK�v)
};

#endif