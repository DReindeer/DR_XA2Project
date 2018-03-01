// ======== ======== ======== ======== ======== ======== ======== ========
//
//
//	title : シーン管理
//	Auter : KENSUKE WATANABE
//
//
// ======== ======== ======== ======== ======== ======== ======== ========
#include "scene.h"
#include "renderer.h"
#include "manager.h"
#include "camera.h"
#include "utility.h"

// -------- -------- -------- -------- -------- -------- -------- --------
// 静的メンバ変数
// -------- -------- -------- -------- -------- -------- -------- --------
std::list<CScene*> CScene::m_lisScene[MAX_SCENE_PRIORITY];
int CScene::m_numScene[MAX_SCENE_PRIORITY] = { 0 };			// シーンの総数

// -------- -------- -------- -------- -------- -------- -------- --------
// プライオリティ指定コンストラクタ
// -------- -------- -------- -------- -------- -------- -------- --------
CScene::CScene(int priority = 4) : m_r(255), m_g(255), m_b(255), m_a(255),
									m_priority(priority)
{
	//---- コンテナにつめる
	m_lisScene[m_priority].push_back(this);

	//-------- 初期化
	m_objType = OBJTYPE::OBJTYPE_NONE;
	m_pos = XMFLOAT3(0.f, 0.f, 0.f);		// 座標
	m_size = XMFLOAT3(0.f, 0.f, 0.f);		// サイズ
	m_rot = XMFLOAT3(0.f, 0.f, 0.f);		// 回転角
	m_scl = XMFLOAT3(1.f, 1.f, 1.f);		// 拡大率
	m_delete = false;						// 解放フラグ
	m_numScene[m_priority]++;				// 総数のインクリメント
}
// -------- -------- -------- -------- -------- -------- -------- --------
// デストラクタ
// -------- -------- -------- -------- -------- -------- -------- --------
CScene::~CScene()
{
	m_numScene[m_priority]--;		// 総数のデクリメント
}

// ======== ======== ======== ======== ======== ======== ======== ========
// CScene::全更新
// -------- -------- -------- -------- -------- -------- -------- --------
void CScene::UpdateAll(void)
{
	//-------- 全更新
	for (int cntPriority = 0; cntPriority < MAX_SCENE_PRIORITY; cntPriority++)
	{
		// 更新
		for (CScene* pScene : m_lisScene[cntPriority])
		{
			pScene->Update();
		}

		// 削除フラグの建っているSceneの削除
		m_lisScene[cntPriority].remove_if([](CScene* pScene)
		{
			bool bErase = false;
			if (pScene->m_delete)
			{
				//---- 解放
				pScene->Uninit();
				delete pScene;
				pScene = nullptr; 
				bErase = true;
			}
			return bErase;
		});
	}
}

// ======== ======== ======== ======== ======== ======== ======== ========
// CScene::プライオリティ指定更新
// -------- -------- -------- -------- -------- -------- -------- --------
void CScene::UpdateAll(int priority)
{
	// 更新
	for (CScene* pScene : m_lisScene[priority])
	{
		pScene->Update();
	}

	// 削除フラグの建っているSceneの削除
	m_lisScene[priority].remove_if([](CScene* pScene)
	{
		bool bErase = false;
		if (pScene->m_delete)
		{
			//---- 解放
			pScene->Uninit();
			delete pScene;
			pScene = nullptr;
			bErase = true;
		}
		return bErase;
	});
}

// ======== ======== ======== ======== ======== ======== ======== ========
// CScene::全描画
// -------- -------- -------- -------- -------- -------- -------- --------
void CScene::DrawAll(void)
{
	// デバイスの取得
	ID3D11DeviceContext* pDevice = CRenderer::GetDeviceContext();
	XMFLOAT3 camPos = CManager::GetCamera()->GetPos();

	for (int cntPriority = 0; cntPriority < MAX_SCENE_PRIORITY; cntPriority++)
	{
		// カメラからの距離によってソート
		m_lisScene[cntPriority].sort([camPos](CScene* pSceneA, CScene* pSceneB)
		{
			float vecA = XM::F3::Distance(pSceneA->GetPos(), camPos);
			float vecB = XM::F3::Distance(pSceneB->GetPos(), camPos);
			return vecA > vecB;
		});

		for (CScene* pScene : m_lisScene[cntPriority])
		{
			// 描画処理
			pScene->Draw();
		}
	}
}

// ======== ======== ======== ======== ======== ======== ======== ========
// CScene::プライオリティ指定描画
// -------- -------- -------- -------- -------- -------- -------- --------
void CScene::DrawAll(int priority)
{
	// デバイスの取得
	ID3D11DeviceContext* pDevice = CRenderer::GetDeviceContext();
	XMFLOAT3 camPos = CManager::GetCamera()->GetPos();

	// カメラからの距離によってソート
	m_lisScene[priority].sort([camPos](CScene* pSceneA, CScene* pSceneB)
	{
		float vecA = XM::F3::Distance(pSceneA->GetPos(), camPos);
		float vecB = XM::F3::Distance(pSceneB->GetPos(), camPos);
		return vecA > vecB;
	});

	for (CScene* pScene : m_lisScene[priority])
	{
		pScene->Draw();
	}
}

// ======== ======== ======== ======== ======== ======== ======== ========
// CScene::全リリース
// -------- -------- -------- -------- -------- -------- -------- --------
void CScene::ReleaseAll(void)
{
	for (int cntPriority = 0; cntPriority < MAX_SCENE_PRIORITY; cntPriority++)
	{
		// Sceneの削除
		for(CScene* it : m_lisScene[cntPriority])
		{
			//---- 解放
			it->Uninit();
			it = nullptr;
		};
		m_lisScene[cntPriority].clear();
	}
}

// ======== ======== ======== ======== ======== ======== ======== ========
// CScene::指定タイプ全リリース
// -------- -------- -------- -------- -------- -------- -------- --------
void CScene::TypeRelease(const OBJTYPE &selectObjType)
{
	for (int cntPriority = 0; cntPriority < MAX_SCENE_PRIORITY; cntPriority++)
	{
		// 指定タイプのSceneの削除
		for(CScene* pScene : m_lisScene[cntPriority])
		{
			if (pScene->GetObjType() == selectObjType)
			{
				pScene->Release();
			}
		}
	}
}

// ======== ======== ======== ======== ======== ======== ======== ========
// 指定タイプ取得
// -------- -------- -------- -------- -------- -------- -------- --------
CScene *CScene::GetSceneClassType(const CLASSTYPE &selectObjType)
{
	for (int cntPriority = 0; cntPriority < MAX_SCENE_PRIORITY; cntPriority++)
	{
		// 指定タイプのSceneの取得
		for (CScene* pScene : m_lisScene[cntPriority])
		{
			if (pScene->GetClassType() == selectObjType)
			{
				return pScene;
			}
		}
	}
	return nullptr;
}