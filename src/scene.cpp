//================================================================================
//
//	Auter : KENSUKE WATANABE
//	Data  : 2017/05/11
//
//--------------------------------------------------------------------------------
#include "mode.h"
#include "scene.h"
#include "renderer.h"
#include "manager.h"
#include "camera.h"

//--------------------------------------------------------------------------------
// 静的メンバ変数
//--------------------------------------------------------------------------------
std::list<CScene*> CScene::m_lisScene[MAX_SCENE_PRIORITY];
CScene::CLASSTYPE CScene::m_drawTypeOnly = CLASSTYPE_NONE;	// 唯一描画したいものだけを描画

//--------------------------------------------------------------------------------
// プライオリティ指定コンストラクタ
//--------------------------------------------------------------------------------
CScene::CScene(int priority = 4) : m_pVtxBuffer(nullptr), m_r(255), m_g(255), m_b(255), m_a(255),
									m_priority(priority), m_lightOff(false), m_alpha(false), m_drawFlag(true), m_overall(false)
{
	// スレッドセーフ(ロック)
#ifdef THREAD_ON
	std::unique_lock<std::recursive_mutex> locker = CManager::Locker();
#endif

	// 作成時のモード
	CMode* pMode = CManager::GetMode();
	CMode* pLoadingMode = CManager::GetLoadingMode();
	if (pMode == nullptr && pLoadingMode == nullptr) { m_createModeType = CMode::MODETYPE::NONE; }
	else if (pMode == pLoadingMode || pLoadingMode == nullptr) { m_createModeType = pMode->GetModeType(); }
	else if (pMode != pLoadingMode) { m_createModeType = pLoadingMode->GetModeType(); }

	m_objType = OBJTYPE::OBJTYPE_NONE;		// タイプ
	m_pos = D3DXVECTOR3(0.f, 0.f, 0.f);		// 座標
	m_size = D3DXVECTOR3(0.f, 0.f, 0.f);	// サイズ
	m_rot = D3DXVECTOR3(0.f, 0.f, 0.f);		// 回転角
	m_scl = D3DXVECTOR3(1.f, 1.f, 1.f);		// 拡大率
	m_delete = false;						// 解放フラグ

	//---- コンテナにつめる
	m_lisScene[m_priority].push_back(this);
}

//--------------------------------------------------------------------------------
// デストラクタ
//--------------------------------------------------------------------------------
CScene::~CScene()
{

}

//================================================================================
// CScene::全更新
//--------------------------------------------------------------------------------
void CScene::UpdateAll()
{
	// 現在モードの取得
	CMode::MODETYPE mode = CManager::GetMode()->GetModeType();

	//-------- 全更新
	for (int cntPriority = 0; cntPriority < MAX_SCENE_PRIORITY; cntPriority++)
	{
		// 更新
		for (CScene* pScene : m_lisScene[cntPriority])
		{
			// 現在モードが「ロード」の場合、ロード中のモードの処理は行わない
			if (mode == CMode::LOADING && pScene->m_createModeType != CMode::LOADING) { continue; }

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

//================================================================================
// CScene::プライオリティ指定更新
//--------------------------------------------------------------------------------
void CScene::UpdateAll(int priority)
{
	// 現在モードの取得
	CMode::MODETYPE mode = CManager::GetMode()->GetModeType();

	// 更新
	for (CScene* pScene : m_lisScene[priority])
	{
		// 現在モードが「ロード」の場合、ロード中のモードの処理は行わない
		if (mode == CMode::LOADING && pScene->m_createModeType != CMode::LOADING) { continue; }

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

//================================================================================
// CScene::フラグ付きのsceneを全消去
//--------------------------------------------------------------------------------
void CScene::RemoveAll()
{
	//-------- 全リリース
	for (int cntPriority = 0; cntPriority < MAX_SCENE_PRIORITY; cntPriority++)
	{
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

//================================================================================
// CScene::全描画
//--------------------------------------------------------------------------------
void CScene::DrawAll()
{
	// デバイスの取得
	LPDIRECT3DDEVICE9 pDevice = CRenderer::GetDevice();
	D3DXVECTOR3 camPos = CManager::GetCamera()->GetPos();

	// 現在モードの取得
	CMode::MODETYPE mode = CManager::GetMode()->GetModeType();

	for (int cntPriority = 0; cntPriority < MAX_SCENE_PRIORITY; cntPriority++)
	{
		// カメラからの距離によってソート
		if (mode != CMode::LOADING)
		{
			m_lisScene[cntPriority].sort([camPos](CScene* pSceneA, CScene* pSceneB)
			{
				D3DXVECTOR3 vecA = camPos - pSceneA->GetPos();
				D3DXVECTOR3 vecB = camPos - pSceneB->GetPos();
				return D3DXVec3LengthSq(&vecA) > D3DXVec3LengthSq(&vecB);
			});
		}

		for (CScene* pScene : m_lisScene[cntPriority])
		{
			// 現在モードが「ロード」の場合、ロード中のモードの処理は行わない
			if (mode == CMode::LOADING && pScene->m_createModeType != CMode::LOADING) { continue; }

			// 唯一描画判定
			if (m_drawTypeOnly != CLASSTYPE_NONE && pScene->m_classType != m_drawTypeOnly) { continue; }

			if (!pScene->m_drawFlag) { continue; }		// 描画するか

			if (pScene->m_lightOff) pDevice->SetRenderState(D3DRS_LIGHTING, FALSE);			// ライト表示オフ
			if (pScene->m_alpha) pDevice->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_ONE);	// DESTのRGBの係数を１にする

			// 描画処理
			pScene->Draw();

			if (pScene->m_alpha)
			{
				//レンダーステートの設定を戻す
				pDevice->SetRenderState(D3DRS_BLENDOP, D3DBLENDOP_ADD);
				pDevice->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
				pDevice->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA);
			}
			if (pScene->m_lightOff)pDevice->SetRenderState(D3DRS_LIGHTING, TRUE);	// ライトの表示を有効にする
		}
	}
}

//================================================================================
// CScene::デファードレンダリング用全描画
//--------------------------------------------------------------------------------
void CScene::DrawDefferdRenderingAll()
{
	// デバイスの取得
	LPDIRECT3DDEVICE9 pDevice = CRenderer::GetDevice();
	D3DXVECTOR3 camPos = CManager::GetCamera()->GetPos();

	// 現在モードの取得
	CMode::MODETYPE mode = CManager::GetMode()->GetModeType();

	for (int cntPriority = 0; cntPriority < MAX_SCENE_PRIORITY; cntPriority++)
	{
		// カメラからの距離によってソート
		if (mode != CMode::LOADING)
		{
			m_lisScene[cntPriority].sort([camPos](CScene* pSceneA, CScene* pSceneB)
			{
				D3DXVECTOR3 vecA = camPos - pSceneA->GetPos();
				D3DXVECTOR3 vecB = camPos - pSceneB->GetPos();
				return D3DXVec3LengthSq(&vecA) > D3DXVec3LengthSq(&vecB);
			});
		}

		for (CScene* pScene : m_lisScene[cntPriority])
		{
			// 現在モードが「ロード」の場合、ロード中のモードの処理は行わない
			if (mode == CMode::LOADING && pScene->m_createModeType != CMode::LOADING) { continue; }

			// 唯一描画判定
			if (m_drawTypeOnly != CLASSTYPE_NONE && pScene->m_classType != m_drawTypeOnly) { continue; }

			if (!pScene->m_drawFlag) { continue; }		// 描画するか

			if (pScene->m_lightOff) pDevice->SetRenderState(D3DRS_LIGHTING, FALSE);			// ライト表示オフ
			if (pScene->m_alpha) pDevice->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_ONE);	// DESTのRGBの係数を１にする

			// 描画処理
			pScene->DrawDefferdRendering();

			if (pScene->m_alpha)
			{
				//レンダーステートの設定を戻す
				pDevice->SetRenderState(D3DRS_BLENDOP, D3DBLENDOP_ADD);
				pDevice->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
				pDevice->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA);
			}
			if (pScene->m_lightOff)pDevice->SetRenderState(D3DRS_LIGHTING, TRUE);	// ライトの表示を有効にする
		}
	}
}

//================================================================================
// CScene::プライオリティ指定描画
//--------------------------------------------------------------------------------
void CScene::DrawAll(int priority)
{
	// デバイスの取得
	LPDIRECT3DDEVICE9 pDevice = CRenderer::GetDevice();
	D3DXVECTOR3 camPos = CManager::GetCamera()->GetPos();

	// 現在モードの取得
	CMode::MODETYPE mode = CManager::GetMode()->GetModeType();

	// カメラからの距離によってソート
	if (mode != CMode::LOADING)
	{
		m_lisScene[priority].sort([camPos](CScene* pSceneA, CScene* pSceneB)
		{
			D3DXVECTOR3 vecA = camPos - pSceneA->GetPos();
			D3DXVECTOR3 vecB = camPos - pSceneB->GetPos();
			return D3DXVec3LengthSq(&vecA) > D3DXVec3LengthSq(&vecB);
		});
	}

	for (CScene* pScene : m_lisScene[priority])
	{
		// 現在モードが「ロード」の場合、ロード中のモードの処理は行わない
		if (mode == CMode::LOADING && pScene->m_createModeType != CMode::LOADING) { continue; }

		// 唯一描画判定
		if (m_drawTypeOnly != CLASSTYPE_NONE && pScene->m_classType != m_drawTypeOnly) { continue; }

		if (!pScene->m_drawFlag) { continue; }		// 描画するか

		if (pScene->m_lightOff)pDevice->SetRenderState(D3DRS_LIGHTING, FALSE);			// ライト表示オフ
		if (pScene->m_alpha) pDevice->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_ONE);	// DESTのRGBの係数を１にする

		// 描画処理
		pScene->Draw();

		if (pScene->m_alpha)
		{
			//レンダーステートの設定を戻す
			pDevice->SetRenderState(D3DRS_BLENDOP, D3DBLENDOP_ADD);
			pDevice->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
			pDevice->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA);
		}
		if (pScene->m_lightOff)pDevice->SetRenderState(D3DRS_LIGHTING, TRUE);	// ライトの表示を有効にする
	}
}

//================================================================================
// CScene::全リリース
//--------------------------------------------------------------------------------
void CScene::ReleaseAll()
{
	for (int cntPriority = 0; cntPriority < MAX_SCENE_PRIORITY; cntPriority++)
	{
		// 全sceneの削除
		for (auto pScene : m_lisScene[cntPriority])
		{
			//---- 解放
			pScene->Uninit();
			delete pScene;
			pScene = nullptr;
		}
		m_lisScene[cntPriority].clear();
	}
}

//================================================================================
// CScene::フラグがfalseのシーンを全リリース
//--------------------------------------------------------------------------------
void CScene::FlagReleaseAll()
{
	for (int cntPriority = 0; cntPriority < MAX_SCENE_PRIORITY; cntPriority++)
	{
		// 削除フラグをたてる
		for (auto pScene : m_lisScene[cntPriority])
		{
			//---- trueなので無視
			if (pScene->m_overall == true) {
				continue;
			}

			//---- 解放
			pScene->Release();
		}
	}
	// 削除フラグがたっているsceneの消去
	RemoveAll();
}

//================================================================================
// CScene::指定タイプ全リリース
//--------------------------------------------------------------------------------
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

//================================================================================
// 指定タイプ取得
//--------------------------------------------------------------------------------
CScene *CScene::GetSceneClassType(const CLASSTYPE &selectObjType)
{
	// スレッドセーフ(ロック)
#ifdef THREAD_ON
	std::unique_lock<std::recursive_mutex> locker = CManager::Locker();
#endif

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

//--------------------------------------------------------------------------------
// Deviceを占有ロックしてバッファ書き込み
//--------------------------------------------------------------------------------
void CScene::WriteBuffer(LPDIRECT3DVERTEXBUFFER9 pVtxBuffer, VERTEX_3D* pInVtx, int numVtx
						,LPDIRECT3DINDEXBUFFER9 pIdxBuffer, WORD *pInIdx, int numIdx)
{
	// スレッドセーフ(ロック)
#ifdef THREAD_ON
	std::unique_lock<std::recursive_mutex> locker = CManager::Locker();
#endif

	if (pVtxBuffer)
	{
		VERTEX_3D* pVtx;	// バッファをロックする
		pVtxBuffer->Lock(0, 0, (void**)&pVtx, 0);

		for (int cntVtx = 0; cntVtx < numVtx; cntVtx++)
		{
			pVtx[cntVtx].pos = pInVtx[cntVtx].pos;
			pVtx[cntVtx].nom = pInVtx[cntVtx].nom;
			pVtx[cntVtx].col = pInVtx[cntVtx].col;
			pVtx[cntVtx].tex = pInVtx[cntVtx].tex;
		}

		//ロックの解除
		pVtxBuffer->Unlock();
	}

	if (pIdxBuffer)
	{
		WORD* pIdx;	// バッファをロックする
		pIdxBuffer->Lock(0, 0, (void**)&pIdx, 0);

		for (int cntIdx = 0; cntIdx < numIdx; cntIdx++)
		{
			pIdx[cntIdx] = pInIdx[cntIdx];
		}

		//ロックの解除
		pIdxBuffer->Unlock();
	}
}