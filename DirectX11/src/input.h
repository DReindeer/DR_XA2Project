// ======== ======== ======== ======== ======== ======== ======== ========
//
// 入力処理 [input.h]
// Author : KENSUKE WATANABE
//
// ======== ======== ======== ======== ======== ======== ======== ========
#ifndef _INPUT_H_
#define _INPUT_H_

#define DIRECTINPUT_VERSION (0x0800)	// dinputインクルードとセットで書く
#include <dinput.h>
#pragma comment(lib,"dinput8.lib")	// Dinput用のライブラリ
// -------- -------- -------- -------- -------- -------- -------- --------
// 定数定義
// -------- -------- -------- -------- -------- -------- -------- --------
static const unsigned int NUM_KEY_MAX = 256;		// キーの最大数
static const unsigned int NUM_MOUSE_MAX = 8;		// マウスボタンの最大数
static const unsigned int COUNT_WAIT_REPEAT = 20;	// リピート待ち時間
static const unsigned int GAMEPAD_MAX = 1;			// 必要なゲームパッドの最大数
static const unsigned int JOY_MARGIN = 50;			// ジョイスティックの遊び

// -------- -------- -------- -------- -------- -------- -------- --------
// 入力クラス
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
	static LPDIRECTINPUT8 m_pDInput;	// DirectInputオブジェクト
};

#endif