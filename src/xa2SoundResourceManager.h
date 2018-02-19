//================================================================================
//
// Auter : KENSUKE WATANABE
// Data  : 2018/01/02
//
//================================================================================
#ifndef _XA2SOUNDRESOURCEMANAGER_H_
#define _XA2SOUNDRESOURCEMANAGER_H_

#include <XAudio2.h>
#include <string>
#include <unordered_map>
#include "xa2LoadWave.h"

// サウンドリソースマネージャ
//--------------------------------------------------------------------------------
class XA2SoundResourceManager
{
public:
	XA2SoundResourceManager() {}		// コンストラクタ
	~XA2SoundResourceManager();			// デストラクタ

	// 取得
	XA2LoadWave *GetXA2SoundResource(std::string strFilePath);

	// 指定ファイル名の有無を調べる
	int CheckXA2SoundResource(std::string strFilePath);

	// 登録
	void AddXA2SoundResource(XA2LoadWave* soundResource) { m_xa2SoundResources.push_back(soundResource); }

	// 全消去
	void Uninit();
	
private:
	// サウンドリソースのコンテナ
	std::list<XA2LoadWave*> m_xa2SoundResources;
};

#endif
