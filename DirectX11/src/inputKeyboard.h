// ======== ======== ======== ======== ======== ======== ======== ========
//
// ���͏��� [input.h]
// Author : KENSUKE WATANABE
//
// ======== ======== ======== ======== ======== ======== ======== ========
#pragma once
#ifndef _INPUTKEYBOARD_H_
#define _INPUTKEYBOARD_H_

#include "input.h"

// -------- -------- -------- -------- -------- -------- -------- --------
// �L�[�{�[�h
// -------- -------- -------- -------- -------- -------- -------- --------
class CInputKeyboard : public CInput
{
public:
	CInputKeyboard() :m_pDIDevice(nullptr) {};
	~CInputKeyboard() {};

	HRESULT Init(HINSTANCE hInst, HWND hWnd);
	void Uninit(void);
	void Update(void)override;

	bool GetPress(int nKey) { return (m_aState[nKey] & 0x80) ? true : false; }
	bool GetTrigger(int nKey) { return (m_aStateTrigger[nKey] & 0x80) ? true : false; }
	bool GetRelease(int nKey) { return (m_aStateRelease[nKey] & 0x80) ? true : false; }
	bool GetRepeat(int nKey) { return (m_aStateRepeat[nKey] & 0x80) ? true : false; }
	void FlushTrigger(int nKey) { m_aStateTrigger[nKey] = 0; }

private:
	BYTE	m_aState[NUM_KEY_MAX];			// �L�[�{�[�h�̏�Ԃ��󂯎�郏�[�N
	BYTE	m_aStateTrigger[NUM_KEY_MAX];	// �g���K�[���[�N
	BYTE	m_aStateRelease[NUM_KEY_MAX];	// �����[�X���[�N
	BYTE	m_aStateRepeat[NUM_KEY_MAX];	// ���s�[�g���[�N
	int		m_aStateRepeatCnt[NUM_KEY_MAX];	// ���s�[�g�J�E���^

	LPDIRECTINPUTDEVICE8 m_pDIDevice;		// Device�I�u�W�F�N�g(���͂ɕK�v)
};

#endif