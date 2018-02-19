//================================================================================
//
//	Auter : KENSUKE WATANABE
//	Data  : 2017/04/27
//
//--------------------------------------------------------------------------------
#ifndef _TEXMANAGER_H_
#define _TEXMANAGER_H_

#include "main.h"
#include <algorithm>
#include <unordered_map>

//================================================================================
// クラス
//--------------------------------------------------------------------------------
class CTexManager
{
public:
	CTexManager(){};		// コンストラクタ
	~CTexManager(){};		// デストラクタ
	
	void Init();		// 初期化
	void Uninit();		// 終了

	bool SetTex(const std::string &strFilePass);						// テクスチャの設定
	bool SetTex(const std::string &strFilePass, LPDIRECT3DTEXTURE9 texInterface);
	LPDIRECT3DTEXTURE9 GetTexInterface(const std::string &strFilePass);	// テクスチャバッファの取得
	bool CheckTex(const std::string &strFilePass);						// テクスチャ名の有無を確認
	
	bool Release(const std::string &strFilePass);						// 解放
	void ReleaseAll();													// 全開放
	
private:
	std::unordered_map<std::string, LPDIRECT3DTEXTURE9> m_umapTexInterface;		// インターフェイス
};

#endif