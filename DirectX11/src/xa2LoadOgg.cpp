//================================================================================
//
// Auter : KENSUKE WATANABE
//
//================================================================================
#define _CRT_SECURE_NO_WARNINGS

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

	// OggVorbis������
	if (ov_open(fp, &m_vf, NULL, 0) < 0) {
		fprintf(stderr, "Input does not appear to be an Ogg bitstream.\n");
		fclose(fp);
		return false;
	}

	// �ڍ׎��� - �`�����l������A�T���v�����O���[�g��
	vorbis_info *vi;
	vi = ov_info(&m_vf, -1);
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
	m_audioSize = (long)ceil(vi->channels * vi->rate * ov_time_total(&m_vf, -1) * quantizationBits);
	m_pAudioData.resize(m_audioSize + 1);

	// �w�b�_��������
	//--------------------------------------------------------------------------------
	memcpy(m_waveHeader.ckidRIFF, "RIFF", 4);
	m_waveHeader.ckSizeRIFF = whsize + m_audioSize - 8;
	memcpy(m_waveHeader.fccType, "WAVE", 4);
	memcpy(m_waveHeader.ckidFmt, "fmt ", 4);
	m_waveHeader.ckSizeFmt = sizeof(WAVEFORMATEX);

	m_pWfx = new WAVEFORMATEXTENSIBLE;
	m_pWfx->Format.cbSize = m_waveHeader.WaveFmt.cbSize = sizeof(WAVEFORMATEX);
	m_pWfx->Format.wFormatTag = m_waveHeader.WaveFmt.wFormatTag = WAVE_FORMAT_PCM;
	m_pWfx->Format.nChannels = m_waveHeader.WaveFmt.nChannels = vi->channels;
	m_pWfx->Format.nSamplesPerSec = m_waveHeader.WaveFmt.nSamplesPerSec = vi->rate;
	m_pWfx->Format.nAvgBytesPerSec = m_waveHeader.WaveFmt.nAvgBytesPerSec = vi->rate * vi->channels * quantizationBits;
	m_pWfx->Format.nBlockAlign = m_waveHeader.WaveFmt.nBlockAlign = vi->channels * quantizationBits;
	m_pWfx->Format.wBitsPerSample = m_waveHeader.WaveFmt.wBitsPerSample = quantizationBits * 8;

	memcpy(m_waveHeader.ckidData, "data", 4);
	m_waveHeader.ckSizeData = m_audioSize;
	
	// �f�R�[�h - �擾����BYTE�����s��̂悤�Ȃ̂Œ���
	//--------------------------------------------------------------------------------
	long size = 0;
	long ret;
	int currentSection = 0;
	bool eof = false;
	while (!eof) {
		ret = ov_read(&m_vf, (char*)&m_pAudioData[size], m_audioSize - size, 0, quantizationBits, 1, &currentSection);
		if (ret == 0) {
			eof = true;
		}
		else if (ret < 0) {
			ov_clear(&m_vf);
			fclose(fp);
			fprintf(stderr, "Decode Error\n");
			return false;
		}
		else {
			size += ret;
		}
	}

	// ogg�t�@�C���N���[�Y
	//--------------------------------------------------------------------------------
	ov_clear(&m_vf);

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


//--------------------------------------------------------------------------------
// 
// ogg�f�R�[�h��Wave�t�@�C���Ƃ��ď��� - �X�g���[�~���O
// 
//--------------------------------------------------------------------------------
//--------------------------------------------------------------------------------
//  ��������
//--------------------------------------------------------------------------------
XA2LoadOggStreaming* XA2LoadOggStreaming::Create(const std::string& strFilePath, const int loopCount)
{
	XA2LoadOggStreaming* result = new XA2LoadOggStreaming;
	result->m_loopCount = loopCount;

	// ���[�h
	if (!result->Load(strFilePath, loopCount))
	{
		// ���ɑ��݂��邽�߉��
		result->Release();
		delete result;
		return nullptr;
	}

	// �X�g���[�~���O�p�̃f�[�^�̓��ꕨ��p��
	std::vector<BYTE> data = { 0 };
	for (int cntAudioData = 0; cntAudioData < MAX_STREAM_AUDIODATA; ++cntAudioData)
	{
		m_pAudioDatas.push_back(data);
	}

	return result;
}

//--------------------------------------------------------------------------------
//  ���[�h����
//--------------------------------------------------------------------------------
bool XA2LoadOggStreaming::Load(const std::string& strFilePath, const int loopCount)
{
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

	// OggVorbis������
	m_pvf = new OggVorbis_File;
	if (ov_open(fp, m_pvf, NULL, 0) < 0) {
		fprintf(stderr, "Input does not appear to be an Ogg bitstream.\n");
		fclose(fp);
		return false;
	}

	// �ڍ׎��� - �`�����l������A�T���v�����O���[�g��
	vorbis_info *vi;
	vi = ov_info(m_pvf, -1);
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
	m_audioSize = (long)ceil(vi->channels * vi->rate * ov_time_total(m_pvf, -1) * quantizationBits);

	// �w�b�_��������
	//--------------------------------------------------------------------------------
	memcpy(m_waveHeader.ckidRIFF, "RIFF", 4);
	m_waveHeader.ckSizeRIFF = whsize + m_audioSize - 8;
	memcpy(m_waveHeader.fccType, "WAVE", 4);
	memcpy(m_waveHeader.ckidFmt, "fmt ", 4);
	m_waveHeader.ckSizeFmt = sizeof(WAVEFORMATEX);

	m_pWfx = new WAVEFORMATEXTENSIBLE;
	m_pWfx->Format.cbSize = m_waveHeader.WaveFmt.cbSize = sizeof(WAVEFORMATEX);
	m_pWfx->Format.wFormatTag = m_waveHeader.WaveFmt.wFormatTag = WAVE_FORMAT_PCM;
	m_pWfx->Format.nChannels = m_waveHeader.WaveFmt.nChannels = vi->channels;
	m_pWfx->Format.nSamplesPerSec = m_waveHeader.WaveFmt.nSamplesPerSec = vi->rate;
	m_pWfx->Format.nAvgBytesPerSec = m_waveHeader.WaveFmt.nAvgBytesPerSec = vi->rate * vi->channels * quantizationBits;
	m_pWfx->Format.nBlockAlign = m_waveHeader.WaveFmt.nBlockAlign = vi->channels * quantizationBits;
	m_pWfx->Format.wBitsPerSample = m_waveHeader.WaveFmt.wBitsPerSample = quantizationBits * 8;

	memcpy(m_waveHeader.ckidData, "data", 4);
	m_waveHeader.ckSizeData = m_audioSize;

	// �P�b�̃o�b�t�@��v�f�����p��
	for (auto& data : m_pAudioDatas)
	{
		data.resize(m_pWfx->Format.nAvgBytesPerSec);
	}

	// �J�n�ʒu��ݒ�
	m_writeCursor = m_dataStart = 0;

	// Streaming�t���O
	m_streaming = true;

	// ���[�v�J�E���g�̐ݒ�
	m_loopCount = loopCount;

	// �t�@�C���p�X�̐ݒ�
	m_filePass = strFilePath;

	// �T�E���h���\�[�X�̓o�^
	XA2SoundResourceManager *pXA2SoundResourceManager = XA2Manager::GetSoundResourceManager();
	pXA2SoundResourceManager->AddXA2SoundResource(this);

	return true;
}

//--------------------------------------------------------------------------------
//  �j������
//--------------------------------------------------------------------------------
void XA2LoadOggStreaming::Release(void)
{
	// �v�f�����폜
	for (auto& data : m_pAudioDatas)
	{
		data.clear();
	}

	if (m_pvf)
	{
		ov_clear(m_pvf);
		delete m_pvf;
		m_pvf = nullptr;
	}
}

//--------------------------------------------------------------------------------
//  �Đ�
//--------------------------------------------------------------------------------
void XA2LoadOggStreaming::Play(XA2SourceVoiceData *pSourceVoiceData, XA2SourceVoiceInterface *pSourceVoiceInterface)
{
	UNREFERENCED_PARAMETER(pSourceVoiceInterface);
	IXAudio2SourceVoice *pSourceVoice = pSourceVoiceData->GetSourceVoice();

	pSourceVoice->Stop();
	m_writeCursor = m_dataStart;
	AddNextBuffer(pSourceVoice);
	pSourceVoice->Start(0);
}


//--------------------------------------------------------------------------------
//  �|�[�����O����
//--------------------------------------------------------------------------------
void XA2LoadOggStreaming::Polling(IXAudio2SourceVoice *pSourceVoice)
{
	if (pSourceVoice == nullptr)return;

	// �~���[�e�N�X
	std::unique_lock<std::recursive_mutex> locker = XA2Manager::Locker();

	//��Ԃ��擾
	XAUDIO2_VOICE_STATE state = { 0 };
	pSourceVoice->GetState(&state);

	//�Đ��L���[�ɏ�Ƀo�b�t�@�𗭂߂Ă���
	if (state.BuffersQueued < MAX_STREAM_AUDIODATA)
	{
		if (m_writeCursor >= m_audioSize)
		{
			if (m_loopCount == -1)
			{// ���[�v����Ȃ�擪�ɖ߂�
				ov_time_seek(m_pvf, 0.0);
				m_writeCursor = m_dataStart;
			}
			else
			{// ���[�v���Ȃ��Ȃ�Đ��I��
				return;
			}
		}

		//�o�b�t�@�Ƀf�[�^����������ŁA�Đ��L���[�ɒǉ�
		AddNextBuffer(pSourceVoice);
	}

	// �s�v�Ȑ擪�v�f�̉��
	if (m_pAudioDatas.size() > MAX_STREAM_AUDIODATA)
	{
		ReleaseFlontBuffer();
	}
}

//--------------------------------------------------------------------------------
//  �v�f��ǉ�����
//--------------------------------------------------------------------------------
void XA2LoadOggStreaming::AddNextBuffer(IXAudio2SourceVoice *pSourceVoice)
{
	// �f�[�^����������
	std::vector<BYTE> audioDatas = { 0 };
	DWORD dataSize = m_pWfx->Format.nAvgBytesPerSec;	// �f�[�^��
	audioDatas.resize(dataSize);
	long readPosition = 0, ret = 0;
	int currentSection = 0;
	while(readPosition != dataSize)
	{
		ret = ov_read(m_pvf, (char*)&audioDatas[readPosition], dataSize - readPosition, 0, quantizationBits, 1, &currentSection);
		if (ret == 0)
		{
			break;
		}
		else
		{
			// �������݈ʒu��i�߂�
			readPosition += ret;
		}
	}

	// �J�[�\����i�߂�
	m_writeCursor += dataSize;

	// ���X�g�ɒǉ�
	m_pAudioDatas.push_back(audioDatas);

	// SourceVoice�Ƀf�[�^�𑗐M
	XAUDIO2_BUFFER xa2buffer = { 0 };
	xa2buffer.AudioBytes = dataSize;
	xa2buffer.pAudioData = &m_pAudioDatas.back()[0];
	if (m_audioSize <= m_writeCursor)
	{
		xa2buffer.Flags = XAUDIO2_END_OF_STREAM;
	}
	pSourceVoice->SubmitSourceBuffer(&xa2buffer);
}

//--------------------------------------------------------------------------------
//  �擪�v�f���������
//--------------------------------------------------------------------------------
void XA2LoadOggStreaming::ReleaseFlontBuffer()
{
	m_pAudioDatas.pop_front();
}
