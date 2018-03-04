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

// �ÓI�����o
//--------------------------------------------------------------------------------
int XA2Listener::m_numListener = 0;

// �R���X�g���N�^
//--------------------------------------------------------------------------------
XA2Listener::XA2Listener()
{
	if (m_numListener >= MAX_LISTENER)
		assert(!"Listener�͒�`�ł��܂���B");

	// �R�[���̐���
	m_listener.pCone = new X3DAUDIO_CONE;

	// ���X�i�[���𑝂₷
	++m_numListener;
}

// �f�X�g���N�^
//--------------------------------------------------------------------------------
XA2Listener::~XA2Listener()
{
	// �R�[���̉��
	if (m_listener.pCone)
	{
		delete m_listener.pCone;
		m_listener.pCone = nullptr;
	}

	// ���X�i�[�������炷
	--m_numListener;
}

// �R�[��
//--------------------------------------------------------------------------------
void XA2Listener::SetCone(float innerAngle, float innerVolume, float outerVolume, float lpf, float reverb)
{
	m_listener.pCone->InnerAngle = min(innerAngle, X3DAUDIO_2PI);			// �R�[���̊p�x (���W�A���P��)
	m_listener.pCone->OuterAngle = X3DAUDIO_2PI - m_listener.pCone->InnerAngle;		// 
	m_listener.pCone->InnerVolume = min(innerVolume, 2.0f);					// �{�����[�� �X�P�[��
	m_listener.pCone->OuterVolume = min(outerVolume, 2.0f);					// 
	m_listener.pCone->InnerLPF = lpf;												// �_�C���N�g �p�X�܂��̓��o�[�u �p�X�̌W���X�P�[��
	m_listener.pCone->OuterLPF = lpf;												// 
	m_listener.pCone->InnerReverb = reverb;											// ���o�[�u �Z���h ���x�� �X�P�[��
	m_listener.pCone->OuterReverb = reverb;											// 
}

// �R�[���̊p�x (���W�A���P��)
//--------------------------------------------------------------------------------
void XA2Listener::SetConeAngle(float innerAngle)
{
	m_listener.pCone->InnerAngle = min(innerAngle, X3DAUDIO_2PI);					
	m_listener.pCone->OuterAngle = X3DAUDIO_2PI - m_listener.pCone->InnerAngle;		
}

// �{�����[�� �X�P�[��
//--------------------------------------------------------------------------------
void XA2Listener::SetConeVolume(float innerVolume, float outerVolume)
{
	m_listener.pCone->InnerVolume = min(innerVolume, 2.0f);							
	m_listener.pCone->OuterVolume = min(outerVolume, 2.0f);							
}

// �_�C���N�g �p�X�܂��̓��o�[�u �p�X�̌W���X�P�[��
//--------------------------------------------------------------------------------
void XA2Listener::SetConeLPF(float innerLPF, float outerLPF) 
{
	m_listener.pCone->InnerLPF = min(innerLPF, 1.0f);								
	m_listener.pCone->OuterLPF = min(outerLPF, 1.0f);								
}

// ���o�[�u �Z���h ���x�� �X�P�[��
//--------------------------------------------------------------------------------
void XA2Listener::SetConeReverb(float innerReverb, float outerReverb)
{
	m_listener.pCone->InnerReverb = min(innerReverb, 2.0f);							
	m_listener.pCone->OuterReverb = min(outerReverb, 2.0f);							
}
