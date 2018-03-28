//================================================================================
//
// Auter : KENSUKE WATANABE
//
//================================================================================
#define _CRT_SECURE_NO_WARNINGS

#include "vorbis/codec.h"
#include "vorbis/vorbisfile.h"

#include "xa2Manager.h"
#include "xa2LoadOgg.h"
#include "xa2SoundResourceManager.h"
#include "xa2SourceVoiceInterface.h"

//--------------------------------------------------------------------------------
// 
// ogg�f�R�[�h��Wave�t�@�C���Ƃ��ď��� - CPU�S�̂�
// 
//--------------------------------------------------------------------------------
//--------------------------------------------------------------------------------
//  �f�X�g���N�^
//--------------------------------------------------------------------------------
XA2LoadOggOnAll::~XA2LoadOggOnAll()
{
	m_pAudioData.clear();
}

//--------------------------------------------------------------------------------
// ogg�t�@�C���I�[�v����wave�Ƃ��ēǂݍ���
//--------------------------------------------------------------------------------
XA2LoadOggOnAll *XA2LoadOggOnAll::Create(const std::string& strFilePath, const int loopCount)
{
	XA2LoadOggOnAll* result = new XA2LoadOggOnAll;
	result->m_loopCount = loopCount;

	// ���[�h
	if (!result->Load(strFilePath, loopCount))
	{
		// ���ɑ��݂���̂ŉ��
		delete result;
		return nullptr;
	}

	return result;
}

//--------------------------------------------------------------------------------
//  ���[�h����
//--------------------------------------------------------------------------------
bool XA2LoadOggOnAll::Load(std::string strFilePath, int loopCount)
{
	// �ǂݍ��ݍς݂����ׂ�
	//--------------------------------------------------------------------------------
	XA2SoundResourceManager *pXA2SoundResourceManager = XA2Manager::GetSoundResourceManager();
	if (pXA2SoundResourceManager->CheckXA2SoundResource(strFilePath))
	{// �o�^�ς�
		return false;
	}

	// ogg�t�@�C���I�[�v��
	//--------------------------------------------------------------------------------
	// �ǂݍ���ogg
	FILE *fp;

	// �t�@�C�����J��
	fp = fopen(&strFilePath[0], "rb");
	if (fp == NULL) {
		fprintf(stderr, "Open Error : %s", &strFilePath[0]);
		return false;
	}

	OggVorbis_File vf;

	// OggVorbis������
	if (ov_open(fp, &vf, NULL, 0) < 0) {
		fprintf(stderr, "Input does not appear to be an Ogg bitstream.\n");
		fclose(fp);
		return false;
	}

	// �ڍ׎��� - �`�����l������A�T���v�����O���[�g��
	vorbis_info *vi;
	vi = ov_info(&vf, -1);
	if (vi == NULL) 
	{
		MessageBox(nullptr, "Get Info Error\r\n�ڍ׎擾�G���[\n", "err", MB_OK);
		return false;
	}
	
	//  �w�b�_�T�C�Y�̎���
	long whsize;
	whsize = sizeof(m_waveHeader.ckidRIFF) + sizeof(m_waveHeader.ckSizeRIFF) + sizeof(m_waveHeader.fccType) +
		sizeof(m_waveHeader.ckidFmt) + sizeof(m_waveHeader.ckSizeFmt) + sizeof(m_waveHeader.WaveFmt) +
		sizeof(m_waveHeader.ckidData) + sizeof(m_waveHeader.ckSizeData);


	// �f�R�[�h��̃f�[�^�T�C�Y�����߂�
	WORD quantizationBits = 2;		// �ʎq���r�b�g - 8bit����1�A16bit����2�B
	m_audioSize = (long)ceil(vi->channels * vi->rate * ov_time_total(&vf, -1) * quantizationBits);
	m_pAudioData.resize(m_audioSize + 1);
	
	// �f�R�[�h - �f�[�^�̈搶��
	long size = 0;
	long ret;
	int currentSection = 0;
	bool eof = false;
	while (!eof) {
		ret = ov_read(&vf, (char*)&m_pAudioData[size], m_audioSize - size, 0, quantizationBits, 1, &currentSection);
		if (ret == 0) {
			eof = true;
		}
		else if (ret < 0) {
			ov_clear(&vf);
			fclose(fp);
			fprintf(stderr, "Decode Error\n");
			return false;
		}
		else {
			size += ret;
		}
	}

	// �w�b�_��������
	memcpy(m_waveHeader.ckidRIFF, "RIFF", 4);
	m_waveHeader.ckSizeRIFF = whsize + size - 8;
	memcpy(m_waveHeader.fccType, "WAVE", 4);
	memcpy(m_waveHeader.ckidFmt, "fmt ", 4);
	m_waveHeader.ckSizeFmt = sizeof(WAVEFORMATEX);

	m_pWfx = new WAVEFORMATEXTENSIBLE;
	m_pWfx->Format.cbSize			= m_waveHeader.WaveFmt.cbSize			= sizeof(WAVEFORMATEX);
	m_pWfx->Format.wFormatTag		= m_waveHeader.WaveFmt.wFormatTag		= WAVE_FORMAT_PCM;
	m_pWfx->Format.nChannels		= m_waveHeader.WaveFmt.nChannels		= vi->channels;
	m_pWfx->Format.nSamplesPerSec	= m_waveHeader.WaveFmt.nSamplesPerSec	= vi->rate;
	m_pWfx->Format.nAvgBytesPerSec	= m_waveHeader.WaveFmt.nAvgBytesPerSec	= vi->rate * vi->channels * quantizationBits;
	m_pWfx->Format.nBlockAlign		= m_waveHeader.WaveFmt.nBlockAlign		= vi->channels * quantizationBits;
	m_pWfx->Format.wBitsPerSample	= m_waveHeader.WaveFmt.wBitsPerSample	= quantizationBits * 8;

	memcpy(m_waveHeader.ckidData, "data", 4);
	m_waveHeader.ckSizeData = size;

	// �t�@�C���N���[�Y
	ov_clear(&vf);

	// Streaming�t���O
	m_streaming = false;

	// ���[�v�J�E���g�̐ݒ�
	m_loopCount = loopCount;

	// �t�@�C���p�X�̐ݒ�
	m_filePass = strFilePath;

	// �T�E���h���\�[�X�̓o�^
	pXA2SoundResourceManager->AddXA2SoundResource(this);

	return true;
}

//--------------------------------------------------------------------------------
// �Đ�
//--------------------------------------------------------------------------------
void XA2LoadOggOnAll::Play(XA2SourceVoiceData *pSourceVoiceData, XA2SourceVoiceInterface *pSourceVoiceInterface)
{
	// ��Ԏ擾�p�ϐ�
	XAUDIO2_VOICE_STATE xa2state;

	// �o�b�t�@�̒l�ݒ�
	XAUDIO2_BUFFER xa2buffer;
	memset(&xa2buffer, 0, sizeof(XAUDIO2_BUFFER));
	xa2buffer.AudioBytes = m_audioSize;
	xa2buffer.pAudioData = &m_pAudioData[0];
	xa2buffer.Flags = XAUDIO2_END_OF_STREAM;
	xa2buffer.LoopCount = m_loopCount;

	IXAudio2SourceVoice *pSourceVoice = pSourceVoiceData->GetSourceVoice();
	std::string filePass = pSourceVoiceData->GetFilePass();

	// ��Ԏ擾
	pSourceVoice->GetState(&xa2state);
	if (xa2state.BuffersQueued != 0 && xa2buffer.LoopCount >= 0)
	{
		// �Đ����Ȃ�V�K�Ń\�[�X�{�C�X���쐬���čĐ�
		//--------------------------------------------------------------------------------
		pSourceVoiceInterface->CreatePlay(filePass, xa2buffer.LoopCount, this);

		return;
	}

	// �I�[�f�B�I�o�b�t�@�̓o�^
	pSourceVoice->SubmitSourceBuffer(&xa2buffer);

	// �Đ�
	pSourceVoice->Start(0);
}

