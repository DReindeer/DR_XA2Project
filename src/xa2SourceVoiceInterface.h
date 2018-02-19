//================================================================================
//
// Auter : KENSUKE WATANABE
// Data  : 2018/01/02
//
//================================================================================
#ifndef _XA2SOURCEVOICEINTERFACE_H_
#define _XA2SOURCEVOICEINTERFACE_H_

#include <XAudio2.h>
#include <string>
#include <list>
#include "xa2LoadWave.h"

// �O���錾
//--------------------------------------------------------------------------------
class XA2SourceVoiceInterface;

// �\�[�X�{�C�X�f�[�^ - ���
//--------------------------------------------------------------------------------
class XA2SourceVoiceData
{
public:
	// �I��
	void Uninit()
	{
		m_strSoundFilePath.clear();
		// �\�[�X�{�C�X�̔j��
		m_pSourceVoice->DestroyVoice();
		m_pSourceVoice = nullptr;
	}

	// �擾
	const std::string &GetFilePass() { return m_strSoundFilePath; }			// �t�@�C���p�X
	IXAudio2SourceVoice *GetSourceVoice() { return m_pSourceVoice; }		// �\�[�X�{�C�X
	XA2LoadWave *GetWaveData() { return  m_pWaveData; }						// wave�f�[�^�̃|�C���^

	// �ݒ�
	void SetFilePass(const std::string &filePass) { m_strSoundFilePath = filePass; }				// �t�@�C���p�X
	void SetSourceVoice(IXAudio2SourceVoice *pSourceVoice) { m_pSourceVoice = pSourceVoice; }		// �\�[�X�{�C�X
	void SetWaveData(XA2LoadWave *pWaveData) { m_pWaveData = pWaveData; }							// wave�f�[�^�̃|�C���^

	// �\�[�X�{�C�X
	void FlushSourceBuffers() {if(m_pSourceVoice)m_pSourceVoice->FlushSourceBuffers();}

	// wave
	void Play(XA2SourceVoiceInterface* pSourceVoiceInterface){ 	// �Đ�
		if(m_pWaveData)m_pWaveData->Play(this, pSourceVoiceInterface);
	}
	void Stop() {										// ��~
		if(m_pWaveData)m_pWaveData->Stop(m_pSourceVoice);
	}										

protected:
	std::string			m_strSoundFilePath;				// �t�@�C���p�X
	IXAudio2SourceVoice *m_pSourceVoice	= nullptr;		// �\�[�X�{�C�X
	XA2LoadWave			*m_pWaveData	= nullptr;		// wave�f�[�^�̃|�C���^
};

// �\�[�X�{�C�X�C���^�[�t�F�C�X - ���
//--------------------------------------------------------------------------------
class XA2SourceVoiceInterface
{
public:
	XA2SourceVoiceInterface();
	~XA2SourceVoiceInterface();

	// ����
	virtual IXAudio2SourceVoice *Create(std::string strFilePath, int loopCount, XA2LoadWave *pLoadWave) = 0;
	virtual IXAudio2SourceVoice *CreatePlay(std::string strFilePath, int loopCount, XA2LoadWave *pLoadWave) = 0;

	virtual void Update() = 0;				// �X�V
	void Uninit();							// �I��
	void StopAndUninit();					// ��~���ďI��

	HRESULT Play(std::string strFilePath);	// �J�n
	void Stop(std::string strFilePath);		// ��~
	void StopAll();							// �S��~

	void ReleaseEndSE();					// ��~����SE�̉��
	virtual void Polling() = 0;				// �ʃX���b�h�|�[�����O����

	// �ݒ�
	void SetPitch(std::string filePath, float ratio = 1.f) 
	{
		for (auto it : m_sourceVoices)
		{
			if (it->GetFilePass() == filePath)
			{
				it->GetSourceVoice()->SetFrequencyRatio(ratio);
			}
		}
	}
	void SetVolume(std::string filePath, float volume = 1.f)
	{
		for (auto it : m_sourceVoices)
		{
			if (it->GetFilePass() == filePath)
			{
				it->GetSourceVoice()->SetVolume(volume);
			}
		}
	}

	IXAudio2SubmixVoice *GetSubmixVoice() { return m_pSubmixVoice; }

	// �ǉ�
	void AddSourceVoiceData(XA2SourceVoiceData* pSourceVoiceData) { m_sourceVoices.push_back(pSourceVoiceData); }

protected:
	// �\�[�X�{�C�X�̃��X�g
	std::list<XA2SourceVoiceData*> m_sourceVoices;

	// ���̃N���X(�C���X�^���X)�ɓK�p����T�u�~�b�N�X
	IXAudio2SubmixVoice *m_pSubmixVoice = nullptr;
};

#endif

