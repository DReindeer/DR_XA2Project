//================================================================================
//
// Auter : KENSUKE WATANABE
// Data  : 2018/01/02
//
//================================================================================
#include "xa2SoundResourceManager.h"

//--------------------------------------------------------------------------------
// デストラクタ
//--------------------------------------------------------------------------------
XA2SoundResourceManager::~XA2SoundResourceManager()
{

}

//--------------------------------------------------------------------------------
// コンテナからリソースを取得
//--------------------------------------------------------------------------------
XA2LoadAudio *XA2SoundResourceManager::GetXA2SoundResource(std::string strFilePath)
{
	for (auto it : m_xa2SoundResources)
	{
		if (it->GetFilePass() == strFilePath)
		{// 登録済み
			return it;
		}
	}
	return nullptr;
}

//--------------------------------------------------------------------------------
// 全消去
//--------------------------------------------------------------------------------
void XA2SoundResourceManager::Uninit()
{
	// サウンドリソースのコンテナ
	for (auto xa2Data : m_xa2SoundResources)
	{
		if (xa2Data)
		{
			// リソースの解放
			xa2Data->Uninit();
			delete xa2Data;
			xa2Data = nullptr;
		}
	}
	// コンテナを空にする
	m_xa2SoundResources.clear();
}

//--------------------------------------------------------------------------------
// 指定ファイル名の有無を調べる
//--------------------------------------------------------------------------------
int XA2SoundResourceManager::CheckXA2SoundResource(std::string strFilePath)
{
	int count = 0;
	for (auto it : m_xa2SoundResources)
	{
		if (it->GetFilePass() == strFilePath)
		{// 登録済み
			++count;
		}
	}
	return count;
}
