//================================================================================
//
//	Auter : KENSUKE WATANABE
//	Data  : 2018/02/10
//
//--------------------------------------------------------------------------------
#include "gravityObj.h"
#include "input.h"
#include "camera.h"
#include "renderer.h"
#include "manager.h"
#include "debugStr.h"
#include "modelManager.h"
#include "sceneModel.h"
#include "texManager.h"
#include "sceneField.h"

//================================================================================
// CGravityObj::初期化処理
//================================================================================
void CGravityObj::Init()
{
	// 親クラスの初期化
	CSceneModel::Init();

	m_move = D3DXVECTOR3(0.f, 0.f, 0.f);		// 移動量
	m_posOldY = 0.f;
	m_jump = true;
}

//================================================================================
// CGravityObj::終了処理
//================================================================================
void CGravityObj::Uninit()
{
	// 親クラスの終了
	CSceneModel::Uninit();
}

//================================================================================
// CGravityObj::更新処理
//================================================================================
void CGravityObj::Update()
{
	// 親クラスの更新
	CSceneModel::Update();

	// 地面との高さが確定した場合は落下判定をしない
	float y = GetPos().y; 
	if (m_posOldY != y)
	{
		m_jump = true;
	}

	if (m_jump == true)
	{
		m_move.y -= 0.1f;		// 落下！！！
		AddPos(m_move);			// 移動量の加算

		// 衝突判定
		for (int n = 0; n < MAX_SCENE_PRIORITY; n++)
		{
			std::list<CScene*> *lisScene = CScene::GetScene(n);
			for (CScene* pScene : *lisScene)
			{
				if (pScene == nullptr)continue;
				// タイプの取得
				CScene::OBJTYPE objType = pScene->GetObjType();

				// フィールドの場合
				if (objType == CScene::OBJTYPE::FIELD)
				{
					// ダウンキャスト
					CSceneField *pField = ((CSceneField*)pScene);
					D3DXVECTOR3 gObjPos = GetPos();
					bool hit;
					float fieldY = pField->GetHeight(gObjPos, &hit);
					if (!hit)
					{
						//					this->Release();
						//					return;
					}
					if (gObjPos.y < fieldY)
					{
						SetPos(D3DXVECTOR3(gObjPos.x, fieldY, gObjPos.z));
						m_move.y = 0.f;
						m_jump = false;
						m_posOldY = fieldY;
						break;
					}
				}
			}
		}
	}
}

//================================================================================
// CGravityObj::描画処理
//================================================================================
void CGravityObj::Draw()
{
	// 親クラスの描画
	CSceneModel::Draw();
}

//--------------------------------------------------------------------------------
// 作成処理
//--------------------------------------------------------------------------------
CGravityObj *CGravityObj::Create(const D3DXVECTOR3 &pos, const D3DXVECTOR3 &rotY, const D3DXVECTOR3 &scl, const std::string &strFilePass)
{
	// CScene3Dの生成
	CGravityObj *pObj = new CGravityObj();
	pObj->Init();

	pObj->SetPos(pos);
	pObj->SetRot(rotY);
	pObj->SetScl(scl);
	pObj->SetFilePass(strFilePass);

	//---------------------
	// モデルのセット
	//---------------------
	CManager::GetModelManager()->SetModel(strFilePass);

	//---------------------
	// テクスチャのセット
	//---------------------
	D3DXMATERIAL* pMat = CManager::GetModelManager()->GetModelMat(strFilePass);
	for (int nCntMat = 0; nCntMat < (int)CManager::GetModelManager()->GetModelNumMat(strFilePass); nCntMat++)
	{
		// 使用しているテクスチャがあれば読み込む
		if (pMat[nCntMat].pTextureFilename != nullptr && lstrlen(pMat[nCntMat].pTextureFilename) > 0)
		{
			CManager::GetTexManager()->SetTex(pMat[nCntMat].pTextureFilename);
		}
		else { pMat[nCntMat].pTextureFilename = nullptr; }
	}

	return pObj;
}