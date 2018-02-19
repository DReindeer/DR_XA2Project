//================================================================================
//
//	Auter : KENSUKE WATANABE
//	Data  : 2018/01/15
//
//--------------------------------------------------------------------------------
#include "renderer.h"
#include "manager.h"
#include "modeGame.h"
#include "modeTitle.h"
#include "modelManager.h"
#include "texManager.h"
#include "sceneCheckPoint.h"
#include "light.h"
#include "debugStr.h"

//--------------------------------------------------------------------------------
// 初期化
//--------------------------------------------------------------------------------
void SceneCheckPoint::Init()
{
	CSceneModel::Init();
}

//--------------------------------------------------------------------------------
// 終了
//--------------------------------------------------------------------------------
void SceneCheckPoint::Uninit()
{
	CSceneModel::Uninit();
}

//--------------------------------------------------------------------------------
// 更新
//--------------------------------------------------------------------------------
void SceneCheckPoint::Update()
{
	CSceneModel::Update();
}

//--------------------------------------------------------------------------------
// 描画
//--------------------------------------------------------------------------------
void SceneCheckPoint::Draw()
{

}

//--------------------------------------------------------------------------------
// 作成処理
//--------------------------------------------------------------------------------
SceneCheckPoint *SceneCheckPoint::Create(const D3DXVECTOR3 &pos, const D3DXVECTOR3 &rot, const D3DXVECTOR3 &scl, const std::string &strFilePass, int pry)
{
	// 生成
	SceneCheckPoint *pCheckPoint;
	pCheckPoint = new SceneCheckPoint(pry);
	pCheckPoint->Init();

	pCheckPoint->SetPos(pos);
	pCheckPoint->SetRot(rot);
	pCheckPoint->SetBasePos(D3DXVECTOR3(0.f, 0.f, 0.f));
	pCheckPoint->SetBaseRot(D3DXVECTOR3(0.f, 0.f, 0.f));
	pCheckPoint->SetScl(scl);
	pCheckPoint->m_strFilePass = strFilePass;
	pCheckPoint->m_strPartsName = strFilePass;

	// モデルの生成
	pCheckPoint->CreateModel(pCheckPoint->m_strFilePass);

	D3DXVECTOR3 vec01 = pCheckPoint->m_point[1] - pCheckPoint->m_point[0];
	pCheckPoint->m_startRot = atan2(vec01.z, vec01.x) * D3DX_PI * 0.5f;

	return pCheckPoint;
}

//================================================================================
// モデル生成
//--------------------------------------------------------------------------------
void SceneCheckPoint::CreateModel(std::string strFilePass)
{
	if (m_pUseModelManager == nullptr)
	{
		m_pUseModelManager = CManager::GetModelManager();
	}

	// モデルマネージャに生成
	if (m_pUseModelManager->SetModel(strFilePass))
	{
		// テクスチャのセット
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
		//---------------------------------------------
		// モデルの頂点の最大値、最小値の取得
		//---------------------------------------------
		LPD3DXMESH pMesh = nullptr;
		pMesh = m_pUseModelManager->GetModelMesh(strFilePass);
		D3DXVECTOR3 Vertex;
		D3DXVECTOR3 min = D3DXVECTOR3(0.f, 0.f, 0.f);
		D3DXVECTOR3 max = D3DXVECTOR3(0.f, 0.f, 0.f);
		m_centerPos = D3DXVECTOR3(0.f, 0.f, 0.f);

		//バッファをロックする
		BYTE *pVtx = nullptr;							// 頂点バッファ
		pMesh->LockVertexBuffer(D3DLOCK_READONLY, (void**)&pVtx);
		DWORD FVF = pMesh->GetFVF();					// 頂点フォーマットを取得する。
		DWORD VertexSize = D3DXGetFVFVertexSize(FVF);	// 頂点のサイズを取得する。

		// 頂点の取得
		for (int nCntMesh = 0; nCntMesh < 2; nCntMesh++)
		{
			Vertex = *(D3DXVECTOR3*)pVtx * CManager::GetMasterScale();				// 頂点座標を取得する。
			m_point[nCntMesh] = Vertex;					// 頂点を入れ込む
			m_centerPos += Vertex;						// 全頂点の座標を足しこむ
			min.x = RETURN_MIN(min.x, Vertex.x);		// X最小値
			max.x = RETURN_MAX(max.x, Vertex.x);		// X最大値
			min.y = RETURN_MIN(min.y, Vertex.y);		// Y最小値
			max.y = RETURN_MAX(max.y, Vertex.y);		// Y最大値
			min.z = RETURN_MIN(min.z, Vertex.z);		// Z最小値
			max.z = RETURN_MAX(max.z, Vertex.z);		// Z最大値
			pVtx += VertexSize;							// 進める
		}
		// バッファのアンロック
		pMesh->UnlockVertexBuffer();

		// 幅,高さ,奥行の取得
		SetSize(D3DXVECTOR3(max.x - min.x, max.y - min.y, max.z - min.z));

		// 2頂点の中心座標
		m_centerPos /= 2;
	}
}

