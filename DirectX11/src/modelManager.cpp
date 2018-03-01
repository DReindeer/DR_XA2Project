// ======== ======== ======== ======== ======== ======== ======== ========
//
//
//	title : モデル管理
//	Auter : KENSUKE WATANABE
//
//
// ======== ======== ======== ======== ======== ======== ======== ========
#include "modelManager.h"
#include "texManager.h"
#include "manager.h"

#include <assert.h>

// ======== ======== ======== ======== ======== ======== ======== ========
// 初期化
// -------- -------- -------- -------- -------- -------- -------- --------
void CXModelManager::Init(void)
{
	m_unmapModel.clear();
}

// ======== ======== ======== ======== ======== ======== ======== ========
// 終了
// -------- -------- -------- -------- -------- -------- -------- --------
void CXModelManager::Uninit(void)
{
	ReleaseAll();
}

// ======== ======== ======== ======== ======== ======== ======== ========
// モデル設定
// -------- -------- -------- -------- -------- -------- -------- --------
bool CXModelManager::SetModel(const std::string &strFilePass)
{
	// nullチェック
	if(strFilePass.empty()){return false;}

	// 同名のパスが既にあるかチェック
	if (m_unmapModel.count(strFilePass) >= 1) { return true; }

	// モデルインスタンスの生成
	std::vector<XModel::MESH*> *vecpModel;

	// Xモデルの生成
	XModel::Loader XLoader;
	vecpModel = XLoader.Load(strFilePass);

	// テクスチャの生成
	for (auto pModel : *vecpModel)
	{
		CManager::GetTexManager()->SetTex(pModel->mat.filePass);
	}

	m_unmapModel[strFilePass] = vecpModel;

	return true;
}

// ======== ======== ======== ======== ======== ======== ======== ========
// モデル取得
// -------- -------- -------- -------- -------- -------- -------- --------
std::vector<XModel::MESH*> *CXModelManager::GetModel(const std::string &strFilePass)
{
	// nullチェック
	if (strFilePass.empty()) { return nullptr; }

	// 同名のパスが既にあるかチェック
	if (m_unmapModel.count(strFilePass) >= 1)
	{
		return m_unmapModel[strFilePass];
	}

	// マテリアル数は未設定です
	return nullptr;
}

// ======== ======== ======== ======== ======== ======== ======== ========
// モデル解放
// -------- -------- -------- -------- -------- -------- -------- --------
bool CXModelManager::Release(const std::string &strFilePass)
{
	// nullチェック
	if (strFilePass.empty()) { return false; }

	// 同名のパスが既にあるかチェック
	if (m_unmapModel.count(strFilePass) >= 1)
	{
		if (m_unmapModel[strFilePass] != nullptr)
		{
			m_unmapModel[strFilePass] = nullptr;
		}
		m_unmapModel.erase(strFilePass);
		return true;
	}
	// 解放できませんでした
	return false;
}

// ======== ======== ======== ======== ======== ======== ======== ========
// モデル全解放
// -------- -------- -------- -------- -------- -------- -------- --------
void CXModelManager::ReleaseAll(void)
{
	// 全解放
	for (auto it : m_unmapModel)
	{
		if (it.second != nullptr)
		{
			it.second = nullptr;
		}
	}
	m_unmapModel.clear();
}