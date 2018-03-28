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
// WAVEファイルのヘッダ
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
// oggデコード→Waveファイルとして処理 - CPU全のせ
// 
//--------------------------------------------------------------------------------
class XA2LoadOggOnAll final : public XA2LoadAudio
{
public:
	XA2LoadOggOnAll() { m_audioFormat = AUDIO_FORMAT_OGG; }
	virtual ~XA2LoadOggOnAll();

	// 生成処理
	XA2LoadOggOnAll *Create(const std::string& file_path, const int loopCount)override;

	// 再生
	void Play(XA2SourceVoiceData *pSourceVoiceData, XA2SourceVoiceInterface *pSourceVoiceInterface)override;

	// 取得
	std::vector<BYTE> *GetAudioData() { return &m_pAudioData; }

private:
	// ロード処理
	bool Load(std::string strFilePath, int loopCount);

	OggVorbis_File m_vf;
	WAVEFILEHEADER m_waveHeader = {};

	// オーディオデータ
	std::vector<BYTE> m_pAudioData;
};

//--------------------------------------------------------------------------------
// 
// oggデコード→Waveファイルとして処理 - ストリーミング
// 
//--------------------------------------------------------------------------------
class XA2LoadOggStreaming final : public XA2LoadAudio
{
public:
	XA2LoadOggStreaming() { m_audioFormat = AUDIO_FORMAT_OGG; }
	virtual ~XA2LoadOggStreaming() {}

	// 生成処理
	XA2LoadOggStreaming* Create(const std::string& strFilePath, const int loopCount)override;

	// 再生
	void Play(XA2SourceVoiceData *pSourceVoiceData, XA2SourceVoiceInterface *pSourceVoiceInterface)override;

	// 破棄処理
	void Release(void);

	// ポーリング処理
	void Polling(IXAudio2SourceVoice *pSourceVoice);

private:
	// ロード処理
	bool Load(const std::string& strFilePath, const int loopCount);

	// 要素を追加する
	void AddNextBuffer(IXAudio2SourceVoice *pSourceVoice);

	// 先頭要素を解放
	void XA2LoadOggStreaming::ReleaseFlontBuffer();

	OggVorbis_File *m_pvf = nullptr;
	WORD quantizationBits = 2;		// 量子化ビット - 8bitだと1、16bitだと2。
	WAVEFILEHEADER m_waveHeader = {};

	// ポーリング用の再生待ちデータリスト(先頭要素が再生中)
	std::list<std::vector<BYTE>> m_pAudioDatas;

	// ファイル読み込みカーソル
	DWORD m_writeCursor = 0;
};


#endif
