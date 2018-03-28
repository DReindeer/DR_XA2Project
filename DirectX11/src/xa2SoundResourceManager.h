//================================================================================
//
// Auter : KENSUKE WATANABE
// Data  : 2018/01/02
//
//================================================================================
#ifndef _XA2SOUNDRESOURCEMANAGER_H_
#define _XA2SOUNDRESOURCEMANAGER_H_

#include "../Libs/Microsoft_DirectX_SDK_(June 2010)/Include/XAudio2.h"
#include <string>
#include <unordered_map>
#include "xa2LoadWave.h"
#include "xa2LoadOgg.h"

// サウンドリソースマネージャ
//--------------------------------------------------------------------------------
class XA2SoundResourceManager
{
public:
	XA2SoundResourceManager() {}		// コンストラクタ
	~XA2SoundResourceManager();			// デストラクタ

	// 取得
	XA2LoadAudio *GetXA2SoundResource(std::string strFilePath);

	// 指定ファイル名の有無を調べる
	int CheckXA2SoundResource(std::string strFilePath);

	// 登録
	void AddXA2SoundResource(XA2LoadAudio* soundResource) { m_xa2SoundResources.push_back(soundResource); }

	// 全消去
	void Uninit();
	
private:
	// サウンドリソースのコンテナ
	std::list<XA2LoadAudio*> m_xa2SoundResources;
};

#endif
