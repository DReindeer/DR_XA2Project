//================================================================================
//
// Auter : KENSUKE WATANABE
// Data  : 2018/01/02
//
//================================================================================
#include <thread>
#include "xa2Manager.h"
#include "xa2Core.h"
#include "xa2LoadWave.h"
#include "xa2SoundResourceManager.h"
#include "xa2SourceVoiceInterface.h"

//--------------------------------------------------------------------------------
// �ǂݍ���
//--------------------------------------------------------------------------------
XA2LoadAudio::LOAD_RESULT XA2LoadAudio::Load(std::string strFilePath, int loopCount, bool streaming)
{
	// �~���[�e�b�N�X
#ifdef THREAD_ON
	std::unique_lock<std::recursive_mutex> locker = CManager::Locker();
#endif

	XA2SoundResourceManager *pXA2SoundResourceManager = XA2Manager::GetSoundResourceManager();

	if (streaming == false && pXA2SoundResourceManager->CheckXA2SoundResource(strFilePath))
	{// �o�^�ς�
		return LOAD_RESULT_DISCOVERY;
	}

	DWORD dwChunkSize = 0;
	DWORD dwChunkPosition = 0;
	DWORD dwFiletype;

	// �T�E���h�f�[�^�t�@�C���̐���
	m_file = CreateFile(&strFilePath[0], GENERIC_READ, FILE_SHARE_READ, nullptr, OPEN_EXISTING, 0, nullptr);
	if (m_file == INVALID_HANDLE_VALUE)
	{
		MessageBox(nullptr, "�T�E���h�f�[�^�t�@�C���̐������s", "�x��", MB_ICONWARNING);
		HRESULT_FROM_WIN32(GetLastError());
		LOAD_RESULT_FAILD;
	}
	// �t�@�C���|�C���^��擪�Ɉړ�
	if (SetFilePointer(m_file, 0, nullptr, FILE_BEGIN) == INVALID_SET_FILE_POINTER)
	{
		MessageBox(nullptr, "�t�@�C���|�C���^�̈ړ��Ɏ��s", "�x��", MB_ICONWARNING);
		HRESULT_FROM_WIN32(GetLastError());
		LOAD_RESULT_FAILD;
	}

	// WAVE�t�@�C���̃`�F�b�N
	HRESULT hr = CheckChunk(m_file, 'FFIR', &dwChunkSize, &dwChunkPosition);
	if (FAILED(hr))
	{
		MessageBox(nullptr, "WAVE�t�@�C���̃`�F�b�N�Ɏ��s(1)", "�x��", MB_ICONWARNING);
		LOAD_RESULT_FAILD;
	}
	hr = ReadChunkData(m_file, &dwFiletype, sizeof(DWORD), dwChunkPosition);
	if (FAILED(hr))
	{
		MessageBox(nullptr, "WAVE�t�@�C���̃`�F�b�N�Ɏ��s(2)", "�x��", MB_ICONWARNING);
		LOAD_RESULT_FAILD;
	}
	if (dwFiletype != 'EVAW')
	{
		MessageBox(nullptr, "WAVE�t�@�C���̃`�F�b�N�Ɏ��s(3)", "�x��", MB_ICONWARNING);
		LOAD_RESULT_FAILD;
	}

	// �t�H�[�}�b�g�`�F�b�N
	hr = CheckChunk(m_file, ' tmf', &dwChunkSize, &dwChunkPosition);
	if (FAILED(hr))
	{
		MessageBox(nullptr, "�t�H�[�}�b�g�`�F�b�N�Ɏ��s(1)", "�x��", MB_ICONWARNING);
		LOAD_RESULT_FAILD;
	}
	// �t�H�[�}�b�g����
	m_pWfx = new WAVEFORMATEXTENSIBLE;
	hr = ReadChunkData(m_file, m_pWfx, dwChunkSize, dwChunkPosition);
	if (FAILED(hr))
	{
		MessageBox(nullptr, "�t�H�[�}�b�g�`�F�b�N�Ɏ��s(2)", "�x��", MB_ICONWARNING);
		delete m_pWfx;
		m_pWfx = nullptr;
		LOAD_RESULT_FAILD;
	}

	// �I�[�f�B�I�f�[�^�ǂݍ���
	hr = CheckChunk(m_file, 'atad', &m_audioSize, &m_dataStart);
	if (FAILED(hr))
	{
		MessageBox(nullptr, "�I�[�f�B�I�f�[�^�ǂݍ��݂Ɏ��s(1)", "�x��", MB_ICONWARNING);
		LOAD_RESULT_FAILD;
	}

	// ���[�v�J�E���g�̐ݒ�
	m_loopCount = loopCount;

	// �t�@�C���p�X�̐ݒ�
	m_filePass = strFilePath;

	// �T�E���h���\�[�X�̓o�^
	pXA2SoundResourceManager->AddXA2SoundResource(this);

	return LOAD_RESULT_CREATE;
}

//--------------------------------------------------------------------------------
// �I��
//--------------------------------------------------------------------------------
void XA2LoadAudio::Uninit()
{
	// wave�t�H�[�}�b�g�̉��
	if (m_pWfx)
	{
		delete m_pWfx;
		m_pWfx = nullptr;
	}

	// ���O�̃N���A
	m_filePass.clear();
}

//--------------------------------------------------------------------------------
//  ��~
//--------------------------------------------------------------------------------
void XA2LoadAudio::Stop(IXAudio2SourceVoice *pSourceVoice)
{
	// ��Ԏ擾
	XAUDIO2_VOICE_STATE state;
	pSourceVoice->GetState(&state);
	if (state.BuffersQueued != 0)
	{// �Đ����Ȃ�ꎞ��~
		pSourceVoice->Stop(0);

		// �I�[�f�B�I�o�b�t�@�̍폜
		pSourceVoice->FlushSourceBuffers();
	}
}

//--------------------------------------------------------------------------------
// Wavw�t�@�C��(�`�����N)�̃`�F�b�N
//--------------------------------------------------------------------------------
HRESULT XA2LoadAudio::CheckChunk(HANDLE m_file, DWORD format, DWORD *pChunkSize, DWORD *pChunkDataPosition)
{
	HRESULT hr = S_OK;
	DWORD dwRead;
	DWORD dwChunkType;
	DWORD dwChunkDataSize;
	DWORD dwRIFFDataSize = 0;
	DWORD dwFileType;
	DWORD dwBytesRead = 0;
	DWORD dwOffset = 0;
	
	if(SetFilePointer(m_file, 0, nullptr, FILE_BEGIN) == INVALID_SET_FILE_POINTER)
	{// �t�@�C���|�C���^��擪�Ɉړ�
		return HRESULT_FROM_WIN32(GetLastError());
	}
	
	while(hr == S_OK)
	{
		if(ReadFile(m_file, &dwChunkType, sizeof(DWORD), &dwRead, nullptr) == 0)
		{// �`�����N�̓ǂݍ���
			hr = HRESULT_FROM_WIN32(GetLastError());
		}

		if(ReadFile(m_file, &dwChunkDataSize, sizeof(DWORD), &dwRead, nullptr) == 0)
		{// �`�����N�f�[�^�̓ǂݍ���
			hr = HRESULT_FROM_WIN32(GetLastError());
		}

		switch(dwChunkType)
		{
		case 'FFIR':
			dwRIFFDataSize  = dwChunkDataSize;
			dwChunkDataSize = 4;
			if(ReadFile(m_file, &dwFileType, sizeof(DWORD), &dwRead, nullptr) == 0)
			{// �t�@�C���^�C�v�̓ǂݍ���
				hr = HRESULT_FROM_WIN32(GetLastError());
			}
			break;

		default:
			if(SetFilePointer(m_file, dwChunkDataSize, nullptr, FILE_CURRENT) == INVALID_SET_FILE_POINTER)
			{// �t�@�C���|�C���^���`�����N�f�[�^���ړ�
				return HRESULT_FROM_WIN32(GetLastError());
			}
		}

		dwOffset += sizeof(DWORD) * 2;
		if(dwChunkType == format)
		{
			*pChunkSize         = dwChunkDataSize;
			*pChunkDataPosition = dwOffset;

			return S_OK;
		}

		dwOffset += dwChunkDataSize;
		if(dwBytesRead >= dwRIFFDataSize)
		{
			return S_FALSE;
		}
	}
	
	return S_OK;
}

//--------------------------------------------------------------------------------
// Wavw�t�@�C��(�`�����N�f�[�^)�̓ǂݍ���
//--------------------------------------------------------------------------------
DWORD XA2LoadAudio::ReadChunkData(HANDLE m_file, void *pBuffer, DWORD dwBuffersize, DWORD dwBufferoffset)
{
	// �~���[�e�N�X
	std::unique_lock<std::recursive_mutex> locker = XA2Manager::Locker();

	DWORD dwRead = 0;
	
	if(SetFilePointer(m_file, dwBufferoffset, nullptr, FILE_BEGIN) == INVALID_SET_FILE_POINTER)
	{// �t�@�C���|�C���^���w��ʒu�܂ňړ�
		return HRESULT_FROM_WIN32(GetLastError());
	}

	if(ReadFile(m_file, pBuffer, dwBuffersize, &dwRead, nullptr) == 0)
	{// �f�[�^�̓ǂݍ���
		return HRESULT_FROM_WIN32(GetLastError());
	}
	
	return dwRead;
}

//--------------------------------------------------------------------------------
// 
// Wave�t�@�C���̃��[�h - CPU�S�̂�
// 
//--------------------------------------------------------------------------------
//--------------------------------------------------------------------------------
//  �f�X�g���N�^
//--------------------------------------------------------------------------------
XA2LoadWaveOnAll::~XA2LoadWaveOnAll()
{
	m_pAudioData.clear();
}

//--------------------------------------------------------------------------------
//  ��������
//--------------------------------------------------------------------------------
XA2LoadWaveOnAll* XA2LoadWaveOnAll::Create(const std::string& strFilePath, const int loopCount)
{
	XA2LoadWaveOnAll* result = new XA2LoadWaveOnAll;
	result->m_loopCount = loopCount;

	if (!result->Load(strFilePath, loopCount))
	{
		delete result;
		return nullptr;
	}

	return result;
}

//--------------------------------------------------------------------------------
//  ���[�h����
//--------------------------------------------------------------------------------
bool XA2LoadWaveOnAll::Load(std::string strFilePath, int loopCount)
{
	// wav�w�b�_�ǂݍ���
	LOAD_RESULT result = XA2LoadAudio::Load(strFilePath, loopCount, false);
	if (result == LOAD_RESULT_FAILD)
	{
		return false;
	}

	// Streaming�t���O
	m_streaming = false;

	if (result == LOAD_RESULT_CREATE)
	{
		// �f�[�^�̈搶��
		m_pAudioData.resize(m_audioSize);
		if (!ReadChunkData(m_file, &m_pAudioData[0], m_audioSize, m_dataStart))
		{
			MessageBox(NULL, strFilePath.c_str(), "ReadChunkData���s�I", MB_ICONWARNING);
			return false;
		}
	}

	return true;
}

//--------------------------------------------------------------------------------
// �Đ�
//--------------------------------------------------------------------------------
void XA2LoadWaveOnAll::Play(XA2SourceVoiceData *pSourceVoiceData, XA2SourceVoiceInterface *pSourceVoiceInterface)
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
// Wave�t�@�C���̃��[�h - �X�g���[�~���O
// 
//--------------------------------------------------------------------------------
//--------------------------------------------------------------------------------
//  ��������
//--------------------------------------------------------------------------------
XA2LoadWaveStreaming* XA2LoadWaveStreaming::Create(const std::string& strFilePath, const int loopCount)
{
	XA2LoadWaveStreaming* result = new XA2LoadWaveStreaming;
	result->m_loopCount = loopCount;

	if (!result->Load(strFilePath, loopCount))
	{
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
bool XA2LoadWaveStreaming::Load(const std::string& strFilePath, const int loopCount)
{
	// �w�b�_��񃍁[�h
	if (!XA2LoadAudio::Load(strFilePath, loopCount, true))
	{
		return false;
	}

	// Streaming�t���O
	m_streaming = true;

	// �P�b�̃o�b�t�@��v�f�����p��
	for (auto& data : m_pAudioDatas)
	{
		data.resize(m_pWfx->Format.nAvgBytesPerSec);
	}

	// �J�n�ʒu��ݒ�
	m_writeCursor = m_dataStart;

	return true;
}

//--------------------------------------------------------------------------------
//  �j������
//--------------------------------------------------------------------------------
void XA2LoadWaveStreaming::Release(void)
{
	// �v�f�����폜
	for (auto& data : m_pAudioDatas)
	{
		data.clear();
	}
}

//--------------------------------------------------------------------------------
//  �Đ�
//--------------------------------------------------------------------------------
void XA2LoadWaveStreaming::Play(XA2SourceVoiceData *pSourceVoiceData, XA2SourceVoiceInterface *pSourceVoiceInterface)
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
void XA2LoadWaveStreaming::Polling(IXAudio2SourceVoice *pSourceVoice)
{
	if (pSourceVoice == nullptr)return;

	// �~���[�e�N�X
	std::unique_lock<std::recursive_mutex> locker = XA2Manager::Locker();

	//��Ԃ��擾
	XAUDIO2_VOICE_STATE state = {0};
	pSourceVoice->GetState(&state);

	//�Đ��L���[�ɏ�Ƀo�b�t�@�𗭂߂Ă���
	if (state.BuffersQueued < MAX_STREAM_AUDIODATA)
	{
		if (m_writeCursor >= m_audioSize)
		{
			if (m_loopCount == -1)
			{// ���[�v����Ȃ�擪�ɖ߂�
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
void XA2LoadWaveStreaming::AddNextBuffer(IXAudio2SourceVoice *pSourceVoice)
{
	// �f�[�^����������
	std::vector<BYTE> audioDatas = { 0 };
	audioDatas.resize(m_pWfx->Format.nAvgBytesPerSec);
	DWORD read = ReadChunkData(m_file, &audioDatas[0], audioDatas.size(), m_writeCursor);
	if (read == 0)
	{
		MessageBox(NULL, "AddNextBuffer", "���s�I", MB_ICONWARNING);
		return;
	}

	// �J�[�\����i�߂�
	m_writeCursor += read;

	// ���X�g�ɒǉ�
	m_pAudioDatas.push_back(audioDatas);

	// SourceVoice�Ƀf�[�^�𑗐M
	XAUDIO2_BUFFER xa2buffer = { 0 };
	xa2buffer.AudioBytes = read;
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
void XA2LoadWaveStreaming::ReleaseFlontBuffer()
{
	m_pAudioDatas.pop_front();
}
