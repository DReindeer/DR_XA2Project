//================================================================================
//
// Auter : KENSUKE WATANABE
//
//================================================================================
#ifndef _XA2OGGDECODER_H_
#define _XA2OGGDECODER_H_

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
class XA2LoadOggOnAll final : public XA2LoadWave
{
public:
	XA2LoadOggOnAll() {}
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

	// オーディオデータ
	WAVEFILEHEADER m_waveHeader = {};
	std::vector<BYTE> m_pAudioData;
};

#endif
