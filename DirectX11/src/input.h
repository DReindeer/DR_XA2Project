// ======== ======== ======== ======== ======== ======== ======== ========
//
// ���͏��� [input.h]
// Author : KENSUKE WATANABE
//
// ======== ======== ======== ======== ======== ======== ======== ========
#ifndef _INPUT_H_
#define _INPUT_H_

#define DIRECTINPUT_VERSION (0x0800)	// dinput�C���N���[�h�ƃZ�b�g�ŏ���
#include <dinput.h>
#pragma comment(lib,"dinput8.lib")	// Dinput�p�̃��C�u����
// -------- -------- -------- -------- -------- -------- -------- --------
// �萔��`
// -------- -------- -------- -------- -------- -------- -------- --------
static const unsigned int NUM_KEY_MAX = 256;		// �L�[�̍ő吔
static const unsigned int NUM_MOUSE_MAX = 8;		// �}�E�X�{�^���̍ő吔
static const unsigned int COUNT_WAIT_REPEAT = 20;	// ���s�[�g�҂�����
static const unsigned int GAMEPAD_MAX = 1;			// �K�v�ȃQ�[���p�b�h�̍ő吔
static const unsigned int JOY_MARGIN = 50;			// �W���C�X�e�B�b�N�̗V��

// -------- -------- -------- -------- -------- -------- -------- --------
// ���̓N���X
// -------- -------- -------- -------- -------- -------- -------- --------
class CInput
{
public:
	CInput() {}
	virtual ~CInput() {}

	virtual HRESULT Init(HINSTANCE hInst);
	virtual void Uninit(void);
	virtual void Update(void) {};

protected:
	static LPDIRECTINPUT8 m_pDInput;	// DirectInput�I�u�W�F�N�g
};

#endif