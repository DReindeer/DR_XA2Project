//================================================================================
//
//	Auter : KENSUKE WATANABE
//	Data  : 2018/02/10
//
//--------------------------------------------------------------------------------
#include "modelManager.h"
#include "renderer.h"
#include "manager.h"

#include <mutex>

//================================================================================
// 初期化
//--------------------------------------------------------------------------------
void CModelManager::Init()
{
	m_umapModel.clear();
}

//================================================================================
// 終了
//--------------------------------------------------------------------------------
void CModelManager::Uninit()
{
	ReleaseAll();
}

//================================================================================
// モデル設定
//--------------------------------------------------------------------------------
bool CModelManager::SetModel(const std::string &strFilePass)
{
	// nullチェック
	if(strFilePass.empty()){return false;}

	// 同名のパスが既にあるかチェック
	if (m_umapModel.count(strFilePass) >= 1) { return true; }

	// スレッドセーフ(ロック)
#ifdef THREAD_ON
	std::unique_lock<std::recursive_mutex> locker = CManager::Locker();
#endif

	// デバイスの取得
	LPDIRECT3DDEVICE9 pDevice = CRenderer::GetDevice();

	// モデルインスタンスの生成
	MODEL *pModel = new MODEL;
	pModel->numMat = 0;
	pModel->pBuffMat = nullptr;
	pModel->pMat = nullptr;
	pModel->pMesh = nullptr;
	pModel->pMeshClone = nullptr;

	LPD3DXBUFFER pAdjacency;	// 隣接性情報

	// モデル読み込み
	HRESULT hr = D3DXLoadMeshFromX(&strFilePass[0], D3DXMESH_MANAGED, pDevice, &pAdjacency, &pModel->pBuffMat, nullptr, &pModel->numMat, &pModel->pMesh);
	if (hr)
	{
		MessageBox(nullptr, "モデル生成失敗", "警告", MB_OK);
		return false;
	}

	hr = pModel->pMesh->OptimizeInplace( D3DXMESHOPT_COMPACT | D3DXMESHOPT_ATTRSORT | D3DXMESHOPT_VERTEXCACHE, (DWORD*)pAdjacency->GetBufferPointer(), NULL, NULL, NULL);
	if (hr)
	{
		MessageBox(nullptr, "なんでか失敗", "警告", MB_OK);
		return false;
	}
	// 隣接性情報の解放
	if (pAdjacency) { pAdjacency->Release(); }

	// 新しいメッシュの頂点要素
	//----------------------------------------------------------------
	// デクラレーション
	D3DVERTEXELEMENT9 decl[] = {
		// パイプライン, オフセット, 型, , セマンティクス, ,
		{ 0, 0, D3DDECLTYPE_FLOAT3, D3DDECLMETHOD_DEFAULT, D3DDECLUSAGE_POSITION, 0 },
		{ 0, 12, D3DDECLTYPE_FLOAT3, D3DDECLMETHOD_DEFAULT, D3DDECLUSAGE_NORMAL, 0 },
		{ 0, 24, D3DDECLTYPE_FLOAT4, D3DDECLMETHOD_DEFAULT, D3DDECLUSAGE_COLOR, 0 },
		{ 0, 40, D3DDECLTYPE_FLOAT2, D3DDECLMETHOD_DEFAULT, D3DDECLUSAGE_TEXCOORD, 0 },
		D3DDECL_END()	// 終端子マクロ(ターミネータ)
	};

	// マテリアル情報のポインタとしてバッファのアドレスを取得
	pModel->pMat = (D3DXMATERIAL*)pModel->pBuffMat->GetBufferPointer();

	// 新しいメッシュの作成
	hr = pModel->pMesh->CloneMesh(D3DXMESH_MANAGED | D3DXMESH_WRITEONLY, decl, pDevice, &pModel->pMeshClone);
	if (hr)
	{
		MessageBox(nullptr, "メッシュのクローン失敗", "警告", MB_OK);
		return false;
	}

	m_umapModel[strFilePass] = pModel;

	return true;
}

//================================================================================
// モデル取得
//--------------------------------------------------------------------------------
CModelManager::MODEL *CModelManager::GetModel(const std::string &strFilePass)
{
	// nullチェック
	if (strFilePass.empty()) { return nullptr; }

	// 同名のパスが既にあるかチェック
	if (m_umapModel.count(strFilePass) >= 1)
	{
		return m_umapModel[strFilePass];
	}

	// そのメッシュインターフェイスは無いです
	return nullptr;
}

//================================================================================
// メッシュインターフェイス取得
//--------------------------------------------------------------------------------
LPD3DXMESH CModelManager::GetModelMesh(const std::string &strFilePass)
{
	// nullチェック
	if(strFilePass.empty()){return nullptr;}

	// 同名のパスが既にあるかチェック
	if (m_umapModel.count(strFilePass) >= 1)
	{ 
		return m_umapModel[strFilePass]->pMesh;
	}

	// そのメッシュインターフェイスは無いです
	return nullptr;
}

//================================================================================
// マテリアル情報取得
//--------------------------------------------------------------------------------
LPD3DXBUFFER CModelManager::GetModelBuffMat(const std::string &strFilePass)
{
	// nullチェック
	if (strFilePass.empty()) { return nullptr; }

	// 同名のパスが既にあるかチェック
	if (m_umapModel.count(strFilePass) >= 1)
	{
		return m_umapModel[strFilePass]->pBuffMat;
	}

	// そのマテリアル情報は無いです
	return nullptr;
}

//================================================================================
// マテリアル数取得
//--------------------------------------------------------------------------------
DWORD CModelManager::GetModelNumMat(const std::string &strFilePass)
{
	// nullチェック
	if (strFilePass.empty()) { return (DWORD)-1; }

	// 同名のパスが既にあるかチェック
	if (m_umapModel.count(strFilePass) >= 1)
	{
		return m_umapModel[strFilePass]->numMat;
	}

	// マテリアル数は未設定です
	return (DWORD)-1;
}

//================================================================================
// マテリアル取得
//--------------------------------------------------------------------------------
D3DXMATERIAL *CModelManager::GetModelMat(const std::string &strFilePass)
{
	// nullチェック
	if (strFilePass.empty()) { return nullptr; }

	// 同名のパスが既にあるかチェック
	if (m_umapModel.count(strFilePass) >= 1)
	{
		return m_umapModel[strFilePass]->pMat;
	}

	// そのマテリアルは無いです
	return nullptr;
}

//================================================================================
// モデル解放
//--------------------------------------------------------------------------------
bool CModelManager::Release(const std::string &strFilePass)
{
	// nullチェック
	if (strFilePass.empty()) { return false; }

	// 同名のパスが既にあるかチェック
	if (m_umapModel.count(strFilePass) >= 1)
	{
		if (m_umapModel[strFilePass]->pMesh != nullptr)
		{
			m_umapModel[strFilePass]->pMesh->Release();		// メッシュ解放
			m_umapModel[strFilePass]->pMesh = nullptr;
		}
		if (m_umapModel[strFilePass]->pBuffMat != nullptr)
		{
			m_umapModel[strFilePass]->pBuffMat->Release();	// マテリアルバッファ解放
			m_umapModel[strFilePass]->pBuffMat = nullptr;
		}

		m_umapModel.erase(strFilePass);
		return true;
	}

	// 解放できませんでした
	return false;
}

//================================================================================
// モデル全解放
//--------------------------------------------------------------------------------
void CModelManager::ReleaseAll()
{
	// 全解放
	for (auto it : m_umapModel)
	{
		if (it.second->pMesh != nullptr)
		{
			it.second->pMesh->Release();		// メッシュ解放
			it.second->pMesh = nullptr;
		}
		if (it.second->pBuffMat != nullptr)
		{
			it.second->pBuffMat->Release();		// マテリアルバッファ解放
			it.second->pBuffMat = nullptr;
		}
	}
	m_umapModel.clear();
}
