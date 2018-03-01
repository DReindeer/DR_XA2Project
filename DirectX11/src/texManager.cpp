// ======== ======== ======== ======== ======== ======== ======== ========
//
//
//	title : テクスチャ管理
//	Auter : KENSUKE WATANABE
//
//
// ======== ======== ======== ======== ======== ======== ======== ========
#include "texManager.h"
#include "renderer.h"
#include "manager.h"
#include "utility.h"
#include <assert.h>
#include <algorithm>

// ======== ======== ======== ======== ======== ======== ======== ========
// 初期化
// -------- -------- -------- -------- -------- -------- -------- --------
void CTexManager::Init()
{
	m_unmapTexInterface.clear();
}

// ======== ======== ======== ======== ======== ======== ======== ========
// 終了
// -------- -------- -------- -------- -------- -------- -------- --------
void CTexManager::Uninit()
{
	ReleaseAll();
}

// ======== ======== ======== ======== ======== ======== ======== ========
// テクスチャ設定
// -------- -------- -------- -------- -------- -------- -------- --------
bool CTexManager::SetTex(const std::string &strFilePass)
{
	// nullチェック
	if(strFilePass.empty()){return false;}

	// 要素の有無を確かめる
	if(m_unmapTexInterface.count(strFilePass) == 1)
	{// ありますよ
		return true;
	}

	// デバイスの取得
	ID3D11DeviceContext *pDeviceContext = CRenderer::GetDeviceContext();
	ID3D11Device *pDevice = CManager::GetRenderer()->GetDevice();

	ID3D11ShaderResourceView *pTexture = nullptr;

	// テクスチャ読み込み
	TexMetadata metadata;
	DirectX::ScratchImage image;
	wchar_t *filePass = Utility::Cast::CastWChae_t(&strFilePass[0]);
	if (FAILED(LoadFromWICFile(filePass, 0, &metadata, image))) {
		MessageBox(nullptr, "テクスチャ読み込み失敗", "警告", S_OK);
		if(filePass)delete filePass;
		return false;
	}
	if (filePass)delete filePass;

	// 画像からシェーダリソースView DirectXTex
	if (FAILED(CreateShaderResourceView(pDevice, image.GetImages(), image.GetImageCount(), metadata, &pTexture))) {
		MessageBox(nullptr, "シェーダリソースView生成失敗", "警告", S_OK);
		return false;
	}

	// 要素の追加
	m_unmapTexInterface.insert(std::make_pair(strFilePass, pTexture));

	// 追加が完了しました
	return true;
}

// ======== ======== ======== ======== ======== ======== ======== ========
// テクスチャ取得
// -------- -------- -------- -------- -------- -------- -------- --------
ID3D11ShaderResourceView *CTexManager::GetTexInterface(const std::string &strFilePass)
{
	// nullチェック
	if(strFilePass.empty())
	{
		return nullptr;
	}

	// 要素の有無を確かめる
	if(m_unmapTexInterface.count(strFilePass) == 1)
	{// ありますよ
		return m_unmapTexInterface.at(strFilePass);
	}

	// その要素は無いです
	return nullptr;
}

// ======== ======== ======== ======== ======== ======== ======== ========
// テクスチャの有無を確認
// -------- -------- -------- -------- -------- -------- -------- --------
bool CTexManager::CheckTex(const std::string &strFilePass)
{
	// nullチェック
	if (strFilePass.empty())
	{
		assert(!"テクスチャのファイルの指定がされていません");
	}

	// 要素の有無を確かめる
	if (m_unmapTexInterface.count(strFilePass) == 1)
	{// ありますよ
		return true;
	}

	// その要素は無いです
	return false;
}

// ======== ======== ======== ======== ======== ======== ======== ========
// テクスチャ解放
// -------- -------- -------- -------- -------- -------- -------- --------
bool CTexManager::Release(const std::string &strFilePass)
{
	// 空か確かめる
	if(strFilePass.empty()){return false;}
	
	// 要素の有無を確かめる
	if(m_unmapTexInterface.count(strFilePass) == 1)
	{// ありますよ
		ID3D11ShaderResourceView *pTex = m_unmapTexInterface.at(strFilePass);
		if (pTex)pTex->Release();						// バッファの解放
		m_unmapTexInterface.erase(strFilePass);			// 要素の削除

		return true;
	}
	return false;
}

// ======== ======== ======== ======== ======== ======== ======== ========
// テクスチャ全解放
// -------- -------- -------- -------- -------- -------- -------- --------
void CTexManager::ReleaseAll(void)
{
	ID3D11ShaderResourceView *pTex = nullptr;

	// コンテナを末尾までループ
	for(auto it : m_unmapTexInterface)
	{
		if(it.second != nullptr) 
		{
			pTex = it.second;
			if(pTex)pTex->Release(); 	// バッファの解放
		}
	}
	m_unmapTexInterface.clear();				// コンテナのクリア
}