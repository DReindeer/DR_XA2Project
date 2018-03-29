//================================================================================
//
// Auter : KENSUKE WATANABE
// Data  : 2018/01/31
//
//================================================================================
#include <algorithm>
#include "xa2Manager.h"
#include "xa2Core.h"
#include "xa2MasteringVoice.h"
#include "xa2SourceVoiceManager.h"
#include "xa2SourceVoice2D.h"
#include "xa2SoundResourceManager.h"

//--------------------------------------------------------------------------------
// ��������
//--------------------------------------------------------------------------------
IXAudio2SourceVoice *XA2SourceVoice2D::Create(std::string strFilePath, int loopCount, XA2LoadAudio *pLoadWave)
{
	// �~���[�e�b�N�X
#ifdef THREAD_ON
	std::unique_lock<std::recursive_mutex> locker = CManager::Locker();
#endif

	// �I�[�f�B�I�f�[�^�̐���
	XA2SourceVoiceData2D *pSourceVoiceData = new XA2SourceVoiceData2D;

	XA2Core *pXA2Core = XA2Manager::GetXA2Core();				// XAudio2�G���W��
	XA2SoundResourceManager *pXA2SoundResourceManager = XA2Manager::GetSoundResourceManager();	// �T�E���h���\�[�X�}�l�[�W��

	// ���\�[�X�̎擾
	pLoadWave->Create(strFilePath, loopCount);	// wave�`���Ŏ擾
	XA2LoadAudio *pSoundResource = pXA2SoundResourceManager->GetXA2SoundResource(strFilePath);

	// �\�[�X�{�C�X�̐���
	IXAudio2SourceVoice *pSourceVoice = pSourceVoiceData->GetSourceVoice();
	HRESULT hr = pXA2Core->GetXAudio2()->CreateSourceVoice(
		&pSourceVoice,							// �\�[�X�{�C�X
		&(pSoundResource->GetWfx()->Format),	// ���\�[�X�̃t�H�[�}�b�g
		0,										// �\�[�X�{�C�X�̓�����w�肷��t���O
		XAUDIO2_DEFAULT_FREQ_RATIO,				// �ő勖�e���g����
		nullptr,								// �R�[���o�b�N �C���^�[�t�F�C�X�ւ̃|�C���^
		nullptr,								// �\�[�X�{�C�X�ɑ΂����A�̃f�X�e�B�l�[�V�����{�C�X
		nullptr);								// �\�[�X�{�C�X�Ŏg�p����G�t�F�N�g�`�F�[��
	if (FAILED(hr))
	{
		MessageBox(nullptr, "�\�[�X�{�C�X�̐����Ɏ��s�I", "�x��", MB_ICONWARNING);
		return nullptr;
	}

	// �\�[�X�{�C�X�̐ݒ�
	pSourceVoiceData->SetSourceVoice(pSourceVoice);

	// �I�[�f�B�I�f�[�^�̃|�C���^�̐ݒ�
	pSourceVoiceData->SetWaveData(pSoundResource);

	// ���X�g�ɓo�^
	pSourceVoiceData->SetFilePass(strFilePath);		// �t�@�C����
	AddSourceVoiceData(pSourceVoiceData);			// �o�^

	return pSourceVoice;
}

//--------------------------------------------------------------------------------
// �������čĐ�
//--------------------------------------------------------------------------------
IXAudio2SourceVoice *XA2SourceVoice2D::CreatePlay(std::string strFilePath, int loopCount, XA2LoadAudio *pLoadWave)
{
	// ����
	IXAudio2SourceVoice *pSourceVoice = Create(strFilePath, loopCount, pLoadWave);

	// ���\�[�X�̎擾
	XA2SoundResourceManager *pXA2SoundResourceManager = XA2Manager::GetSoundResourceManager();		// �T�E���h���\�[�X�}�l�[�W��
	XA2LoadAudio *pSoundResource = pXA2SoundResourceManager->GetXA2SoundResource(strFilePath);

	// �o�b�t�@�̒l�ݒ�
	XAUDIO2_BUFFER xa2buffer;
	memset(&xa2buffer, 0, sizeof(XAUDIO2_BUFFER));
	XA2LoadWaveOnAll *pLoadWaveOnAll = nullptr;
	if (pSoundResource->GetStreamingFlag() == false)
	{
		pLoadWaveOnAll = (XA2LoadWaveOnAll*)pSoundResource;
		xa2buffer.AudioBytes = pLoadWaveOnAll->GetAudioSize();
		xa2buffer.pAudioData = &pLoadWaveOnAll->GetAudioData()[0][0];
		xa2buffer.Flags = XAUDIO2_END_OF_STREAM;
		xa2buffer.LoopCount = pLoadWaveOnAll->GetLoopCount();

		// �I�[�f�B�I�o�b�t�@�̓o�^
		pSourceVoice->SubmitSourceBuffer(&xa2buffer);

		// �Đ�
		pSourceVoice->Start(0);
	}

	return pSourceVoice;
}

//--------------------------------------------------------------------------------
// �X�V
//--------------------------------------------------------------------------------
void XA2SourceVoice2D::Update()
{
	// �I������SE�̉��
	XA2SourceVoiceInterface::ReleaseEndSE();
}

//--------------------------------------------------------------------------------
// �|�[�����O���� - �ʃX���b�h����
//--------------------------------------------------------------------------------
void XA2SourceVoice2D::Polling()
{
	if (m_sourceVoices.size() == 0 || m_sourceVoices.size() > MAX_SAUCEVOICE)return;

	// �R���e�i���󂩔���
	if (m_sourceVoices.empty())
	{
		return;
	}
	
	// ���b�N
	std::unique_lock<std::recursive_mutex> locker = XA2Manager::Locker();

	// �|�[�����O����
	XA2LoadAudio *pWaveData = nullptr;
	XA2LoadWaveStreaming *pWaveStreaming = nullptr;
	XA2LoadOggStreaming *pOggStreaming = nullptr;
	for (auto it : m_sourceVoices)
	{
		if (it == nullptr)continue;
		// �X�g���[�~���O�Đ��̃\�[�X�{�C�X�̂ݎ��s
		pWaveData = it->GetWaveData();
		if (pWaveData->GetStreamingFlag())
		{
			switch ((int)pWaveData->GetAudioFormat())
			{
			case XA2LoadAudio::AUDIO_FORMAT_WAVE:
				pWaveStreaming = (XA2LoadWaveStreaming*)pWaveData;
				pWaveStreaming->Polling(it->GetSourceVoice());
				break;
			case XA2LoadAudio::AUDIO_FORMAT_OGG:
				pOggStreaming = (XA2LoadOggStreaming*)pWaveData;
				pOggStreaming->Polling(it->GetSourceVoice());
				break;
			}
		}
	}
}

