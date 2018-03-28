//================================================================================
//
// Auter : KENSUKE WATANABE
//
//================================================================================
#ifndef _XA2LOADOGG_H_
#define _XA2LOADOGG_H_

#include "vorbis/codec.h"
#include "vorbis/vorbisfile.h"

#include "xa2Manager.h"
#include "xa2LoadWave.h"

#pragma comment ( lib, "libogg_static.lib" )
#pragma comment ( lib, "libvorbis_static.lib" )
#pragma comment ( lib, "libvorbisfile_static.lib" )

//--------------------------------------------------------------------------------
// 
// WAVE�t�@�C���̃w�b�_
// 
//--------------------------------------------------------------------------------
typedef struct
{
	char ckidRIFF[4];
	int ckSizeRIFF;
	char fccType[4];
	char ckidFmt[4];
	int ckSizeFmt;
	WAVEFORMATEX WaveFmt;
	char ckidData[4];
	int ckSizeData;
} WAVEFILEHEADER;

//--------------------------------------------------------------------------------
// 
// ogg�f�R�[�h��Wave�t�@�C���Ƃ��ď��� - CPU�S�̂�
// 
//--------------------------------------------------------------------------------
class XA2LoadOggOnAll final : public XA2LoadAudio
{
public:
	XA2LoadOggOnAll() { m_audioFormat = AUDIO_FORMAT_OGG; }
	virtual ~XA2LoadOggOnAll();

	// ��������
	XA2LoadOggOnAll *Create(const std::string& file_path, const int loopCount)override;

	// �Đ�
	void Play(XA2SourceVoiceData *pSourceVoiceData, XA2SourceVoiceInterface *pSourceVoiceInterface)override;

	// �擾
	std::vector<BYTE> *GetAudioData() { return &m_pAudioData; }

private:
	// ���[�h����
	bool Load(std::string strFilePath, int loopCount);

	OggVorbis_File m_vf;
	WAVEFILEHEADER m_waveHeader = {};

	// �I�[�f�B�I�f�[�^
	std::vector<BYTE> m_pAudioData;
};

//--------------------------------------------------------------------------------
// 
// ogg�f�R�[�h��Wave�t�@�C���Ƃ��ď��� - �X�g���[�~���O
// 
//--------------------------------------------------------------------------------
class XA2LoadOggStreaming final : public XA2LoadAudio
{
public:
	XA2LoadOggStreaming() { m_audioFormat = AUDIO_FORMAT_OGG; }
	virtual ~XA2LoadOggStreaming() {}

	// ��������
	XA2LoadOggStreaming* Create(const std::string& strFilePath, const int loopCount)override;

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
	void XA2LoadOggStreaming::ReleaseFlontBuffer();

	OggVorbis_File *m_pvf = nullptr;
	WORD quantizationBits = 2;		// �ʎq���r�b�g - 8bit����1�A16bit����2�B
	WAVEFILEHEADER m_waveHeader = {};

	// �|�[�����O�p�̍Đ��҂��f�[�^���X�g(�擪�v�f���Đ���)
	std::list<std::vector<BYTE>> m_pAudioDatas;

	// �t�@�C���ǂݍ��݃J�[�\��
	DWORD m_writeCursor = 0;
};


#endif
