//================================================================================
//
// Auter : KENSUKE WATANABE
// Data  : 2018/02/12
//
//================================================================================
#ifndef _XA2LISTENER_H_
#define _XA2LISTENER_H_

#include <assert.h>
#include "main.h"
#include "math.h"
#include "Wmath.h"
// XAudio2
#include "xa2Manager.h"
#include "xa2SourceVoiceInterface.h"

// 定数定義
//--------------------------------------------------------------------------------
static const float DEFAULT_LISTENER_LPF = 0.5f;
static const float DEFAULT_LISTENER_REVERB = 0.5f;
static const int MAX_LISTENER = 1;

// 3Dリスナー
//--------------------------------------------------------------------------------
class XA2Listener
{
public:
	XA2Listener();
	~XA2Listener();

	// 取得
	//--------------------------------------------------------------------------------
	X3DAUDIO_LISTENER *GetListener() { return &m_listener; }

	// 設定
	//--------------------------------------------------------------------------------
	// 座標
	void SetPos(const XMFLOAT3 &pos) { m_listener.Position = pos; }

	// 速度
	void SetVelocity(const XMFLOAT3 &move) { m_listener.Velocity = move; }

	// 上方向
	void SetTop(XMFLOAT3 top) { XM::F3::Normalize(top); m_listener.OrientTop = top; }

	// 前方向
	void SetFront(XMFLOAT3 front) { XM::F3::Normalize(front); m_listener.OrientFront = front; }

	// コーン
	void SetCone(float innerAngle, float innerVolume, float outerVolume, float lpf = DEFAULT_LISTENER_LPF, float reverb = DEFAULT_LISTENER_REVERB);
	void SetConeAngle(float innerAngle);
	void SetConeVolume(float innerVolume, float outerVolume);
	void SetConeLPF(float innerLPF, float outerLPF);
	void SetConeReverb(float innerReverb, float outerReverb);

protected:
	X3DAUDIO_LISTENER m_listener = {};	// リスナー
	static int m_numListener;
};

#endif
