//================================================================================
//
// Auter : KENSUKE WATANABE
// Data  : 2018/02/12
//
//================================================================================
#include <algorithm>
#include "xa2Manager.h"
#include "xa2Core.h"
#include "xa2Listener.h"

// 静的メンバ
//--------------------------------------------------------------------------------
int XA2Listener::m_numListener = 0;

// コンストラクタ
//--------------------------------------------------------------------------------
XA2Listener::XA2Listener()
{
	if (m_numListener >= MAX_LISTENER)
		assert(!"Listenerは定義できません。");

	// コーンの生成
	m_listener.pCone = new X3DAUDIO_CONE;

	// リスナー数を増やす
	++m_numListener;
}

// デストラクタ
//--------------------------------------------------------------------------------
XA2Listener::~XA2Listener()
{
	// コーンの解放
	if (m_listener.pCone)
	{
		delete m_listener.pCone;
		m_listener.pCone = nullptr;
	}

	// リスナー数を減らす
	--m_numListener;
}

// コーン
//--------------------------------------------------------------------------------
void XA2Listener::SetCone(float innerAngle, float innerVolume, float outerVolume, float lpf, float reverb)
{
	m_listener.pCone->InnerAngle = min(innerAngle, X3DAUDIO_2PI);			// コーンの角度 (ラジアン単位)
	m_listener.pCone->OuterAngle = X3DAUDIO_2PI - m_listener.pCone->InnerAngle;		// 
	m_listener.pCone->InnerVolume = min(innerVolume, 2.0f);					// ボリューム スケーラ
	m_listener.pCone->OuterVolume = min(outerVolume, 2.0f);					// 
	m_listener.pCone->InnerLPF = lpf;												// ダイレクト パスまたはリバーブ パスの係数スケーラ
	m_listener.pCone->OuterLPF = lpf;												// 
	m_listener.pCone->InnerReverb = reverb;											// リバーブ センド レベル スケーラ
	m_listener.pCone->OuterReverb = reverb;											// 
}

// コーンの角度 (ラジアン単位)
//--------------------------------------------------------------------------------
void XA2Listener::SetConeAngle(float innerAngle)
{
	m_listener.pCone->InnerAngle = min(innerAngle, X3DAUDIO_2PI);					
	m_listener.pCone->OuterAngle = X3DAUDIO_2PI - m_listener.pCone->InnerAngle;		
}

// ボリューム スケーラ
//--------------------------------------------------------------------------------
void XA2Listener::SetConeVolume(float innerVolume, float outerVolume)
{
	m_listener.pCone->InnerVolume = min(innerVolume, 2.0f);							
	m_listener.pCone->OuterVolume = min(outerVolume, 2.0f);							
}

// ダイレクト パスまたはリバーブ パスの係数スケーラ
//--------------------------------------------------------------------------------
void XA2Listener::SetConeLPF(float innerLPF, float outerLPF) 
{
	m_listener.pCone->InnerLPF = min(innerLPF, 1.0f);								
	m_listener.pCone->OuterLPF = min(outerLPF, 1.0f);								
}

// リバーブ センド レベル スケーラ
//--------------------------------------------------------------------------------
void XA2Listener::SetConeReverb(float innerReverb, float outerReverb)
{
	m_listener.pCone->InnerReverb = min(innerReverb, 2.0f);							
	m_listener.pCone->OuterReverb = min(outerReverb, 2.0f);							
}
