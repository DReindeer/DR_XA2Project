//================================================================================
//
// ���͏��� [input.h]
// Author : KENSUKE WATANABE
//
//================================================================================
#pragma once
#ifndef _INPUTJOYSTICK_H_
#define _INPUTJOYSTICK_H_

#include "input.h"

//--------------------------------------------------------------------------------
// �W���C�X�e�B�b�N
//--------------------------------------------------------------------------------
class CInputJoyStick : public CInput
{
public:
	typedef enum	// �{�^���z�u
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
	BYTE		m_aState[NUM_BUT_MAX];			// �W���C�X�e�B�b�N�̓��͏�񃏁[�N �L��������Ă��邩
	BYTE		m_aStateTrigger[NUM_BUT_MAX];	// �W���C�X�e�B�b�N�̃g���K�[��񃏁[�N�������u��
	BYTE		m_aStateRelease[NUM_BUT_MAX];	// �W���C�X�e�B�b�N�̃����[�X��񃏁[�N�������u��
	BYTE		m_aStateRepeat[NUM_BUT_MAX];	// �W���C�X�e�B�b�N�̃��s�[�g��񃏁[�N
	int			m_aStateRepeatCnt[NUM_BUT_MAX];	// �W���C�X�e�B�b�N�̃��s�[�g�J�E���^
	int			m_nLx;							// ���X�e�B�b�N�̎����E
	int			m_nLy;							// ���X�e�B�b�N�̎��㉺
	int			m_nLz;							// �E�X�e�B�b�N�̏㉺
	int			m_nLRz;							// �E�X�e�B�b�N�̍�
	bool		m_bJoystick;					// �W���C�X�e�B�b�N���g�p���邩�ǂ���
	static int			m_nCntUseJoy;			// �g�p�W���C�X�e�B�b�N�̐�

	static LPDIRECTINPUTDEVICE8 m_pDIDevice;	// Device�I�u�W�F�N�g(���͂ɕK�v)
};

#endif