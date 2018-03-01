// ======== ======== ======== ======== ======== ======== ======== ========
//
//	title : classMake[ texManager.h ]		Auter : KENSUKE WATANABE
//													Data  : 2017/04/27
//
// -------- -------- -------- -------- -------- -------- -------- --------
//■Update : 2016/11/17
//
// ======== ======== ======== ======== ======== ======== ======== ========
#ifndef _TEXMANAGER_H_
#define _TEXMANAGER_H_

#include "main.h"
#include <unordered_map>

#include "DirectXTex.h"
#if _DEBUG
#pragma comment(lib, "DirectXTex-d.lib")
#else
#pragma comment(lib, "DirectXTex.lib")
#endif


// ======== ======== ======== ======== ======== ======== ======== ========
// クラス
// -------- -------- -------- -------- -------- -------- -------- --------
class CTexManager
{
public:

	CTexManager(){};		// コンストラクタ
	~CTexManager(){};		// デストラクタ
	
    void Init(void);		// 初期化
    void Uninit(void);		// 終了

	bool SetTex(const std::string &strFilePass);						// テクスチャの設定
	ID3D11ShaderResourceView *GetTexInterface(const std::string &strFilePass);	// テクスチャバッファの取得
	bool CheckTex(const std::string &strFilePass);						// テクスチャ名の有無を確認
	
	bool Release(const std::string &strFilePass);						// 解放
	void ReleaseAll(void);												// 全開放
	
private:
	std::unordered_map<std::string, ID3D11ShaderResourceView*> m_unmapTexInterface;		// インターフェイス
};

#endif