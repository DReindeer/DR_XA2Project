//================================================================================
//
// Auter : KENSUKE WATANABE
// Data  : 2018/02/01
//
//================================================================================
#ifndef _XA2SOURCEVOICE3D_H_
#define _XA2SOURCEVOICE3D_H_

// XAudio2
#include "xa2Manager.h"
#include "xa2SourceVoiceInterface.h"
#include "xa2Core.h"
// STL
#include <list>

// �萔��`
//--------------------------------------------------------------------------------
static const float DEFAULT_LPF = 0.5f;
static const float DEFAULT_REVERB = 0.5f;


// 3D�\�[�X�{�C�X�f�[�^ : [�p��]�\�[�X�{�C�X�f�[�^
//--------------------------------------------------------------------------------
class XA2SourceVoiceData3D : public XA2SourceVoiceData
{
public:

private:
};

// 3D�\�[�X�{�C�X : [�p��]�\�[�X�{�C�X�C���^�[�t�F�C�X
//--------------------------------------------------------------------------------
class XA2SourceVoice3D : public XA2SourceVoiceInterface
{
public:
	XA2SourceVoice3D() {}
	~XA2SourceVoice3D() { Uninit(); }

	IXAudio2SourceVoice *Create(std::string strFilePath, int loopCount, XA2LoadAudio *pLoadWave)override;
	IXAudio2SourceVoice *CreatePlay(std::string strFilePath, int loopCount, XA2LoadAudio *pLoadWave)override;

	void Update()override;
	void Uninit();
	void Polling()override;

	// �擾
	//--------------------------------------------------------------------------------
	// �G�~�b�^�[
	X3DAUDIO_EMITTER *GetEmitter() { return &m_emitter; }

	// �ݒ�
	//--------------------------------------------------------------------------------
	// �R�[��
	void SetCone(float innerAngle, float innerVolume, float outerVolume, float lpf = DEFAULT_LPF, float reverb = DEFAULT_REVERB);
	void SetConeAngle(float innerAngle);
	void SetConeVolume(float innerVolume, float outerVolume);
	void SetConeLPF(float innerLPF, float outerLPF);
	void SetConeReverb(float innerReverb, float outerReverb);

	// ���W
	void SetPos(const XMFLOAT3 &pos) { m_emitter.Position = pos; }

	// ���x
	void SetVelocity(const XMFLOAT3 &move) { m_emitter.Velocity = move; }

	// �����
	void SetTop(XMFLOAT3 top) { XM::F3::Normalize(top); m_emitter.OrientTop = top; }

	// �O����
	void SetFront(XMFLOAT3 front) { XM::F3::Normalize(front); m_emitter.OrientFront = front; }

	// �������a(�������̉��̗̈�)
	void SetInnerRadius(const float &innerRadius) { m_emitter.InnerRadius = max(innerRadius, 0.f); }

	// �����p�x(�㉺�։����L���鎞�̊p�x)
	void SetInnerRadiusAngle(const float &innerRadiusAngle) { m_emitter.InnerRadiusAngle = (FLOAT32)min(innerRadiusAngle, X3DAUDIO_PI / 4.0); }

protected:
	// �G�~�b�^�[
	X3DAUDIO_EMITTER m_emitter = { 0 };
	X3DAUDIO_DSP_SETTINGS m_dspSettings = { 0 };
	FLOAT32 m_azimuths[INPUTCHANNELS] = { 0 };
	FLOAT32 m_matrixCoefficients[INPUTCHANNELS * OUTPUTCHANNELS] = { 0 };

	static FLOAT32 DefaultMatrixCoefficients[12];
};

#endif
