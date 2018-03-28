//================================================================================
//
// Auter : KENSUKE WATANABE
// Data  : 2018/02/11
//
//================================================================================
#include <algorithm>
#include "xa2Manager.h"
#include "xa2Core.h"
#include "xa2MasteringVoice.h"
#include "xa2SoundResourceManager.h"
#include "xa2SourceVoiceManager.h"
#include "xa2SourceVoiceInterface.h"

//--------------------------------------------------------------------------------
// �R���X�g���N�^
//--------------------------------------------------------------------------------
XA2SourceVoiceInterface::XA2SourceVoiceInterface()
{
	UINT32 flags = 0;
#ifdef _DEBUG
	flags |= XAUDIO2FX_DEBUG;
#endif
	IUnknown* pReverbEffect;
	HRESULT hr = XAudio2CreateReverb(&pReverbEffect, flags);
	if (FAILED(hr))
	{
		MessageBox(nullptr, "XAudio2CreateReverb()�Ɏ��s", "�x��", MB_OK);
		return;
	}

	// �G���W��
	XA2Core *pCore = XA2Manager::GetXA2Core();

	XAUDIO2_EFFECT_DESCRIPTOR effects[] = { { pReverbEffect, TRUE, 1 } };
	XAUDIO2_EFFECT_CHAIN effectChain = { 1, effects };

	const UINT32 samplesPerSec = pCore->GetSamplesPerSec();
	hr = pCore->GetXAudio2()->CreateSubmixVoice(&m_pSubmixVoice, 1,
		samplesPerSec, 0, 0, NULL, &effectChain);
	if (FAILED(hr))
	{
		pReverbEffect->Release();
		MessageBox(nullptr, "pCore->GetXAudio2()->CreateSubmixVoice()�Ɏ��s", "�x��", MB_OK);
		return;
	}
	
	// �f�t�H���g�ݒ�
	XAUDIO2FX_REVERB_PARAMETERS native = {0};
	XAUDIO2FX_REVERB_I3DL2_PARAMETERS presetParameter = XAUDIO2FX_I3DL2_PRESET_FOREST;
	ReverbConvertI3DL2ToNative(&presetParameter, &native);
	m_pSubmixVoice->SetEffectParameters(0, &native, sizeof(native));
}

//--------------------------------------------------------------------------------
// �f�X�g���N�^
//--------------------------------------------------------------------------------
XA2SourceVoiceInterface::~XA2SourceVoiceInterface()
{
	if (m_pSubmixVoice)
	{
		m_pSubmixVoice->DestroyVoice();
		m_pSubmixVoice = nullptr;
	}
}

//--------------------------------------------------------------------------------
// �I������SE�̉�� - �K�x�[�W�R���N�V����
//--------------------------------------------------------------------------------
void XA2SourceVoiceInterface::ReleaseEndSE()
{
	// �~���[�e�N�X
	std::unique_lock<std::recursive_mutex> locker = XA2Manager::Locker();

	// ��Ԏ擾�p�ϐ�
	XAUDIO2_VOICE_STATE xa2state;

	// �s�v�ȃ\�[�X�{�C�X��T��
	XA2LoadAudio *pWaveData = nullptr;
	m_sourceVoices.erase(remove_if(m_sourceVoices.begin(), m_sourceVoices.end(), [&](XA2SourceVoiceData *pSourceVoiceData)
	{
		// ��Ԏ擾
		pSourceVoiceData->GetSourceVoice()->GetState(&xa2state);
		pWaveData = pSourceVoiceData->GetWaveData();			// wave�f�[�^�̃|�C���^
		if (xa2state.BuffersQueued == 0 && pWaveData->GetLoopCount() >= 0)
		{
			// �����ꏊ�ɓo�^����Ă���\�[�X�{�C�X��T��
			for (XA2SourceVoiceData *otherVoice : m_sourceVoices)
			{
				// �������\�[�X���Q�Ƃ��Ă���\�[�X�{�C�X���m�F
				if (pSourceVoiceData->GetFilePass() == otherVoice->GetFilePass())
				{
					// �������g���������΂�
					if (pSourceVoiceData == otherVoice)continue;

					// �\�[�X�{�C�X�̔j��
					pSourceVoiceData->Uninit();

					return true;
				}
			}
		}
		return false;
	}), m_sourceVoices.end());
}

//--------------------------------------------------------------------------------
// �I������
//--------------------------------------------------------------------------------
void XA2SourceVoiceInterface::StopAndUninit()
{
	// �~���[�e�N�X
	std::unique_lock<std::recursive_mutex> locker = XA2Manager::Locker();

	for (auto sourceVoiceData : m_sourceVoices)
	{
		if (sourceVoiceData)
		{
			// ��~
			sourceVoiceData->GetSourceVoice()->Stop();

			// �\�[�X�{�C�X�̔j��
			sourceVoiceData->Uninit();

			// �������g�̔j��
			delete sourceVoiceData;
			sourceVoiceData = nullptr;
		}
	}
	m_sourceVoices.clear();
}

//--------------------------------------------------------------------------------
// �I������
//--------------------------------------------------------------------------------
void XA2SourceVoiceInterface::Uninit()
{
	// �~���[�e�N�X
	std::unique_lock<std::recursive_mutex> locker = XA2Manager::Locker();

	for (auto sourceVoiceData : m_sourceVoices)
	{
		if (sourceVoiceData)
		{
			// �\�[�X�{�C�X�̔j��
			sourceVoiceData->Uninit();

			delete sourceVoiceData;
			sourceVoiceData = nullptr;
		}
	}
	m_sourceVoices.clear();
}

//--------------------------------------------------------------------------------
// �Z�O�����g�Đ� - �Đ����Ȃ�V�K�Ń\�[�X�{�C�X���쐬���čĐ�
//--------------------------------------------------------------------------------
HRESULT XA2SourceVoiceInterface::Play(std::string strFilePath)
{
	// �~���[�e�N�X
	std::unique_lock<std::recursive_mutex> locker = XA2Manager::Locker();

	for (auto pSourceVoiceData : m_sourceVoices)
	{
		if (pSourceVoiceData->GetFilePass() == strFilePath)
		{
			// �I�[�f�B�I�̍Đ�
			pSourceVoiceData->Play(this);

			return S_OK;
		}
	}

	return E_FAIL;
}

//--------------------------------------------------------------------------------
// �Z�O�����g��~
//--------------------------------------------------------------------------------
void XA2SourceVoiceInterface::Stop(std::string strFilePath)
{
	// �~���[�e�N�X
	std::unique_lock<std::recursive_mutex> locker = XA2Manager::Locker();

	// ��Ԏ擾
	XAUDIO2_VOICE_STATE xa2state;
	for (auto sourceVoiceData : m_sourceVoices)
	{
		if (sourceVoiceData->GetFilePass() == strFilePath)
		{
			sourceVoiceData->GetSourceVoice()->GetState(&xa2state);
			if (xa2state.BuffersQueued != 0)
			{
				// �ꎞ��~
				sourceVoiceData->Stop();

				// �I�[�f�B�I�o�b�t�@�̍폜
				sourceVoiceData->FlushSourceBuffers();
			}
		}
	}
}

//--------------------------------------------------------------------------------
// �Z�O�����g��~(�S��)
//--------------------------------------------------------------------------------
void XA2SourceVoiceInterface::StopAll()
{
	// �~���[�e�N�X
	std::unique_lock<std::recursive_mutex> locker = XA2Manager::Locker();

	// �S�Ē�~
	XAUDIO2_VOICE_STATE xa2state;
	IXAudio2SourceVoice *pSourceVoice = nullptr;		// �\�[�X�{�C�X

	for (auto sourceVoiceData : m_sourceVoices)
	{
		pSourceVoice = sourceVoiceData->GetSourceVoice();

		if (pSourceVoice)
		{
			pSourceVoice->GetState(&xa2state);
			if (xa2state.BuffersQueued != 0)
			{
				// �ꎞ��~
				sourceVoiceData->Stop();

				// �I�[�f�B�I�o�b�t�@�̍폜
				sourceVoiceData->FlushSourceBuffers();
			}
		}
	}
}

//--------------------------------------------------------------------------------
// �s�b�`�ύX
//--------------------------------------------------------------------------------
void XA2SourceVoiceInterface::SetPitch(std::string filePath, float ratio)
{
	for (auto it : m_sourceVoices)
	{
		if (it->GetFilePass() == filePath)
		{
			it->GetSourceVoice()->SetFrequencyRatio(ratio);
		}
	}
}

//--------------------------------------------------------------------------------
// �{�����[���ύX
//--------------------------------------------------------------------------------
void XA2SourceVoiceInterface::SetVolume(std::string filePath, float volume)
{
	for (auto it : m_sourceVoices)
	{
		if (it->GetFilePass() == filePath)
		{
			it->GetSourceVoice()->SetVolume(volume);
		}
	}
}

