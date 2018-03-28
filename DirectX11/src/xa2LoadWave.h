//================================================================================
//
// Auter : KENSUKE WATANABE
// Data  : 2018/01/02
//
//================================================================================
#ifndef _LOADWAVE_H_
#define _LOADWAVE_H_

#include <string>
#include <list>
#include <vector>

// �O���錾
//--------------------------------------------------------------------------------
class XA2SourceVoiceData;
class XA2SourceVoiceInterface;

//--------------------------------------------------------------------------------
// 
// Wave�t�@�C���̃��[�h - ���N���X
// 
//--------------------------------------------------------------------------------
class XA2LoadWave
{
public:
	typedef enum
	{
		LOAD_RESULT_FAILD = 0,	// ���s
		LOAD_RESULT_DISCOVERY,	// ����
		LOAD_RESULT_CREATE		// �V�K����
	}LOAD_RESULT;

	XA2LoadWave() {}
	virtual ~XA2LoadWave() {}

	// ��������
	virtual XA2LoadWave* Create(const std::string& file_path, const int loopCount) = 0;

	// �Đ�
	virtual void Play(XA2SourceVoiceData *pSourceVoiceData, XA2SourceVoiceInterface *pSourceVoiceInterface) = 0;

	// ��~
	void Stop(IXAudio2SourceVoice *pSourceVoice);

	// �I��
	void Uninit();

	// �擾
	const int &GetLoopCount()const { return m_loopCount; }
	const DWORD &GetAudioDataOrigin()const { return m_dataStart; }
	const DWORD &GetAudioSize()const { return m_audioSize; }
	const WAVEFORMATEXTENSIBLE *GetWfx()const { return m_pWfx; }
	const std::string &GetFilePass ()const { return m_filePass; }
	const bool &GetStreamingFlag()const { return m_streaming; }

protected:
	// �T�E���h�ǂݍ���/����
	LOAD_RESULT Load(std::string strFilePath, int loopCount, bool streaming = false);
	// �`�����N�`�F�b�N
	static HRESULT CheckChunk(HANDLE hFile, DWORD format, DWORD *pChunkSize, DWORD *pChunkDataPosition);
	// �`�����N�ǂݍ���
	static DWORD ReadChunkData(HANDLE hFile, void *pBuffer, DWORD dwBuffersize, DWORD dwBufferoffset);

	// �ϐ���`
	bool					m_streaming = false;	// �X�g���[�~���O�t���O
	int						m_loopCount = 0;		// ���[�v�J�E���g
	DWORD					m_audioSize = 0;		// �I�[�f�B�I�f�[�^�T�C�Y
	DWORD					m_dataStart = 0;		// �I�[�f�B�I�f�[�^�����̐擪
	WAVEFORMATEXTENSIBLE*	m_pWfx = nullptr;		// Wave�̃t�H�[�}�b�g
	HANDLE					m_file = nullptr;		// �J�[�\���ʒu
	std::string				m_filePass;				// �t�@�C���p�X
};


//--------------------------------------------------------------------------------
// 
// Wave�t�@�C���̃��[�h - CPU�S�̂�
// 
//--------------------------------------------------------------------------------
class XA2LoadWaveOnAll final : public XA2LoadWave
{
public:
	XA2LoadWaveOnAll() {}
	virtual ~XA2LoadWaveOnAll();

	// ��������
	XA2LoadWaveOnAll *Create(const std::string& file_path, const int loopCount)override;

	// �Đ�
	void Play(XA2SourceVoiceData *pSourceVoiceData, XA2SourceVoiceInterface *pSourceVoiceInterface)override;

	// �擾
	std::vector<BYTE> *GetAudioData() { return &m_pAudioData; }

private:
	// ���[�h����
	bool Load(std::string strFilePath, int loopCount);

	// �I�[�f�B�I�f�[�^
	std::vector<BYTE> m_pAudioData;
};

//--------------------------------------------------------------------------------
// 
// Wave�t�@�C���̃��[�h - �X�g���[�~���O
// 
//--------------------------------------------------------------------------------
class XA2LoadWaveStreaming final : public XA2LoadWave
{ 
public:
	XA2LoadWaveStreaming() {}
	virtual ~XA2LoadWaveStreaming() {}

	// ��������
	XA2LoadWaveStreaming* Create(const std::string& strFilePath, const int loopCount)override;

	// �Đ�
	void Play(XA2SourceVoiceData *pSourceVoiceData, XA2SourceVoiceInterface *pSourceVoiceInterface)override;

	// �j������
	void Release(void);

	// �|�[�����O����
	void Polling(IXAudio2SourceVoice *pSourceVoice);

private:
	// ���[�h����
	bool Load(const std::string& strFilePath, const int loopCount);

	// �v�f��ǉ�����
	void AddNextBuffer(IXAudio2SourceVoice *pSourceVoice);

	// �擪�v�f�����
	void XA2LoadWaveStreaming::ReleaseFlontBuffer();

	// �|�[�����O�p�̍Đ��҂��f�[�^���X�g(�擪�v�f���Đ���)
	std::list<std::vector<BYTE>> m_pAudioDatas;

	// �t�@�C���ǂݍ��݃J�[�\��
	DWORD m_writeCursor = 0;
};

#endif
