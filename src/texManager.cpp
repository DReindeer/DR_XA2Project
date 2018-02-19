//================================================================================
//
//	Auter : KENSUKE WATANABE
//	Data  : 2018/02/10
//
//--------------------------------------------------------------------------------
#include "texManager.h"
#include "renderer.h"
#include "manager.h"

#include <mutex>
//================================================================================
// 初期化
//--------------------------------------------------------------------------------
void CTexManager::Init()
{
	m_umapTexInterface.clear();
}

//================================================================================
// 終了
//--------------------------------------------------------------------------------
void CTexManager::Uninit()
{
	ReleaseAll();
}

//================================================================================
// テクスチャ設定
//--------------------------------------------------------------------------------
bool CTexManager::SetTex(const std::string &strFilePass)
{
	// nullチェック
	if(strFilePass.empty()){return false;}

	// 要素の有無を確かめる
	if(m_umapTexInterface.count(strFilePass) == 1)
	{// ありますよ
		return true;
	}

	// スレッドセーフ(ロック)
#ifdef THREAD_ON
	std::unique_lock<std::recursive_mutex> locker = CManager::Locker();
#endif

	// デバイスの取得
	LPDIRECT3DDEVICE9 pDevice = CRenderer::GetDevice();

	/*	
	HRESULT D3DXCreateTextureFromFileEx(
	LPDIRECT3DDEVICE9	pDevice,
	LPCTSTR	pSrcFile,
	UINT	Width,
	UINT	Height,
	UINT	MipLevels,
	DWORD	Usage,
	D3DFORMAT	Format,
	D3DPOOL	Pool,
	DWORD	Filter,
	DWORD	MipFilter,
	D3DCOLOR	ColorKey,
	D3DXIMAGE_INFO*	pSrcInfo,
	PALETTEENTRY*	pPalette,
	LPDIRECT3DTEXTURE9*	ppTexture);	
	*/
	// テクスチャインターフェースにデータを代入
	LPDIRECT3DTEXTURE9 texInterface = nullptr;
	if(D3DXCreateTextureFromFileEx(pDevice, &strFilePass[0], 0, 0, 5, 0, D3DFMT_A8R8G8B8, D3DPOOL_MANAGED, D3DX_FILTER_POINT, D3DX_DEFAULT, 0, nullptr, nullptr, &texInterface) < 0)
	{
		texInterface = nullptr;
		return false;
	}

	// 要素の追加
	m_umapTexInterface[strFilePass] = texInterface;

	// 追加が完了しました
	return true;
}

//================================================================================
// テクスチャ設定
//--------------------------------------------------------------------------------
bool CTexManager::SetTex(const std::string &strFilePass, LPDIRECT3DTEXTURE9 texInterface)
{
	// nullチェック
	if (strFilePass.empty()) { return false; }
	if (texInterface == nullptr) { return false; }

	// 要素の有無を確かめる
	if (m_umapTexInterface.count(strFilePass) == 1)
	{// ありますよ
		return true;
	}

	// 要素の追加
	m_umapTexInterface[strFilePass] = texInterface;

	// 追加が完了しました
	return true;
}

//================================================================================
// テクスチャ取得
//--------------------------------------------------------------------------------
LPDIRECT3DTEXTURE9 CTexManager::GetTexInterface(const std::string &strFilePass)
{
	// nullチェック
	if(strFilePass.empty())
	{
		return nullptr;
	}

	// 要素の有無を確かめる
	if(m_umapTexInterface.count(strFilePass) == 1)
	{// ありますよ
		return m_umapTexInterface[strFilePass];
	}

	// その要素は無いです
	return nullptr;
}

//================================================================================
// テクスチャの有無を確認
//--------------------------------------------------------------------------------
bool CTexManager::CheckTex(const std::string &strFilePass)
{
	// nullチェック
	if (strFilePass.empty())
	{
		MessageBox(nullptr, "テクスチャのファイルの指定がされていません", "警告", MB_OK);
	}

	// 要素の有無を確かめる
	if (m_umapTexInterface.count(strFilePass) == 1)
	{// ありますよ
		return true;
	}

	// その要素は無いです
	return false;
}

//================================================================================
// テクスチャ解放
//--------------------------------------------------------------------------------
bool CTexManager::Release(const std::string &strFilePass)
{
	// 要素の有無を確かめる
	if(m_umapTexInterface.count(strFilePass) == 1)
	{// ありますよ
		m_umapTexInterface[strFilePass]->Release();		// バッファの解放
		m_umapTexInterface[strFilePass] = nullptr;
		m_umapTexInterface.erase(strFilePass);			// 要素の削除

		return true;
	}
	return false;
}

//================================================================================
// テクスチャ全解放
//--------------------------------------------------------------------------------
void CTexManager::ReleaseAll()
{
	// コンテナを末尾までループ
	for(auto& unmapTex : m_umapTexInterface)
	{
		if(unmapTex.second)
		{
			unmapTex.second->Release();	// バッファの解放
			unmapTex.second = nullptr;
		}
	}
	m_umapTexInterface.clear();
}