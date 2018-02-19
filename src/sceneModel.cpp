//================================================================================
//
//	Auter : KENSUKE WATANABE
//	Data  : 2018/02/10
//
//--------------------------------------------------------------------------------
#include "renderer.h"
#include "manager.h"
#include "modeGame.h"
#include "modeTitle.h"
#include "modelManager.h"
#include "texManager.h"
#include "sceneModel.h"
#include "camera.h"
#include "light.h"
#include "lightcamera.h"
#include "depthShadow.h"
#include "deferredRendering.h"

//--------------------------------------------------------------------------------
// 初期化
//--------------------------------------------------------------------------------
void CSceneModel::Init()
{
	m_pUseModelManager = nullptr;
	m_centerPos = D3DXVECTOR3(0.f, 0.f, 0.f);

	SetPos(D3DXVECTOR3(0.f, 0.f, 0.f));		// 座標
	SetRot(D3DXVECTOR3(0.f, 0.f, 0.f));		// 回転量
	SetScl(D3DXVECTOR3(1.f, 1.f, 1.f));		// 拡大率
}

//--------------------------------------------------------------------------------
// 終了
//--------------------------------------------------------------------------------
void CSceneModel::Uninit()
{

}

//--------------------------------------------------------------------------------
// 更新
//--------------------------------------------------------------------------------
void CSceneModel::Update()
{

}

/*
//--------------------------------------------------------------------------------
// 描画
//--------------------------------------------------------------------------------
void CSceneModel::Draw()
{
	// デバイスの取得
	LPDIRECT3DDEVICE9 pDevice = CRenderer::GetDevice();

	D3DXMatrixIdentity(&m_mtxWorld);//
	D3DXMatrixIdentity(&m_mtxScl);	//
	D3DXMatrixIdentity(&m_mtxRot);	//
	D3DXMatrixIdentity(&m_mtxPos);	// 行列を単位行列にする( 拡大率1,他0で初期化 )

	D3DXVECTOR3 scl = GetScl();

	// ワールド変換行列を設定して変換
	D3DXMatrixScaling(&m_mtxScl, scl.x, scl.y, scl.z);	// 拡大行列
	m_mtxRot = GetMatrixRot(this);
	m_mtxPos = GetMatrixPos(this);

	D3DXMatrixMultiply(&m_mtxWorld, &m_mtxWorld, &m_mtxScl);
	D3DXMatrixMultiply(&m_mtxWorld, &m_mtxWorld, &m_mtxRot);
	D3DXMatrixMultiply(&m_mtxWorld, &m_mtxWorld, &m_mtxPos);

	// デバイスにワールド変換行列を設定
	pDevice->SetTransform(D3DTS_WORLD, &m_mtxWorld);

	// モデルの取得
	CModelManager::MODEL* pModel = m_pUseModelManager->GetModel(m_strFilePass);

	// 現在デバイスに設定されているマテリアル情報の取得
	D3DXMATERIAL* pMat = pModel->pMat;
	D3DMATERIAL9 matDef;
	pDevice->GetMaterial(&matDef);

	// シェーダーの設定
	//--------------------------------------------------------------------------------
	// カメラ
	CCamera *pCamera = CManager::GetCamera();

	// ライト
	D3DXVECTOR3 lightDirL;
	D3DXVECTOR3 lightDirW = CManager::GetLightCamera()->GetDir();
	D3DXMATRIX mtxWorldInverse;
	D3DXMATRIX mtxWIT;

	D3DXVECTOR3 eye = pCamera->GetPos();
	m_pShaderSet->pPConstantTable->SetValue(pDevice, "posEyeW", eye, sizeof(D3DXVECTOR3));
	m_pShaderSet->pPConstantTable->SetValue(pDevice, "lightDirW", &lightDirW, sizeof(D3DXVECTOR3));
	// ピクセルシェーダーの設定(テクスチャ無し)
	m_pShaderSet->pPConstantTableNoTex->SetValue(pDevice, "posEyeW", eye, sizeof(D3DXVECTOR3));
	m_pShaderSet->pPConstantTableNoTex->SetValue(pDevice, "lightDirW", &lightDirW, sizeof(D3DXVECTOR3));

	D3DXMatrixInverse(&mtxWorldInverse, nullptr, &m_mtxWorld);
	D3DXVec3TransformNormal(&lightDirL, &lightDirW, &mtxWorldInverse);
	m_pShaderSet->pVConstantTable->SetValue(pDevice, "lightDirL", &lightDirL, sizeof(D3DXVECTOR3));

	m_pShaderSet->pVConstantTable->SetValue(pDevice, "mtxWorld", m_mtxWorld, sizeof(D3DXMATRIX));

//	D3DXVECTOR4 lightAmb = CManager::GetLight()->GetAmbient(0);
//	m_pShaderSet->pVConstantTable->SetValue(pDevice, "ambent", lightAmb, sizeof(D3DXVECTOR4));

	D3DXMatrixInverse(&mtxWIT, nullptr, &m_mtxWorld);	// 逆行列
	D3DXMatrixTranspose(&mtxWIT, &mtxWIT);				// 転置行列
	m_pShaderSet->pVConstantTable->SetMatrix(pDevice, "mtxWIT", &mtxWIT);

	// マトリクスの設定
	D3DXMATRIX mtxWVP = (m_mtxWorld * pCamera->GetView() * pCamera->GetProj());
	m_pShaderSet->pVConstantTable->SetMatrix(pDevice, "mtxWVP", &mtxWVP);
	D3DXMATRIX mtxWV = (m_mtxWorld * pCamera->GetView());
	m_pShaderSet->pVConstantTable->SetMatrix(pDevice, "mtxWV", &mtxWV);
	m_pShaderSet->pVConstantTable->SetFloat(pDevice, "far", pCamera->GetFar());

	// ライトのマトリクスの設定
	CLightCamera *pLightCamera = CManager::GetLightCamera();
	D3DXMATRIX mtxView = pLightCamera->GetMtxView();
	D3DXMATRIX mtxProj = pLightCamera->GetMtxProj();
	D3DXMATRIX mtxLightWV = m_mtxWorld * mtxView;
	m_pShaderSet->pVConstantTable->SetMatrix(pDevice, "mtxLightWV", &mtxLightWV);
	D3DXMATRIX mtxLightWVP = m_mtxWorld * mtxView * mtxProj;
	m_pShaderSet->pVConstantTable->SetMatrix(pDevice, "mtxLightWVP", &mtxLightWVP);
	m_pShaderSet->pVConstantTable->SetFloat(pDevice, "lightFar", pLightCamera->GetFar());

	// 現在デバイスに設定されているマテリアル情報の取得
	pDevice->GetMaterial(&matDef);

	// 頂点シェーダーの設定
	//------------------------------------------------------------------------
	pDevice->SetVertexDeclaration(m_pShaderSet->pVertexDeclaration);
	pDevice->SetVertexShader(m_pShaderSet->pVertexShader);

	// モデルのマテリアルごとに設定
	for (int nCntMat = 0; nCntMat < (int)m_pUseModelManager->GetModelNumMat(m_strFilePass); nCntMat++)
	{
		// デバイスにマテリアルを設定
		pDevice->SetMaterial(&pMat->MatD3D);

		// ピクセルシェーダーの設定
		//------------------------------------------------------------------------
		float power = pMat->MatD3D.Power + 50.f;
		m_pShaderSet->pPConstantTable->SetFloat(pDevice, "speclarPower", power);
		m_pShaderSet->pPConstantTableNoTex->SetFloat(pDevice, "speclarPower", power);
		m_pShaderSet->pPConstantTable->SetValue(pDevice, "resolution", D3DXVECTOR2(SCREEN_WIDTH, SCREEN_HEIGHT), sizeof(D3DXVECTOR2));
		m_pShaderSet->pPConstantTableNoTex->SetValue(pDevice, "resolution", D3DXVECTOR2(SCREEN_WIDTH, SCREEN_HEIGHT), sizeof(D3DXVECTOR2));

		D3DXVECTOR3 modelEmissive(pMat[nCntMat].MatD3D.Emissive.r, pMat[nCntMat].MatD3D.Emissive.g, pMat[nCntMat].MatD3D.Emissive.b);
		m_pShaderSet->pPConstantTable->SetValue(pDevice, "modelEmissive", modelEmissive, sizeof(D3DXVECTOR3));
		m_pShaderSet->pPConstantTableNoTex->SetValue(pDevice, "modelEmissive", modelEmissive, sizeof(D3DXVECTOR3));

		D3DXVECTOR4 modelDiffuse = D3DXVECTOR4(pMat[nCntMat].MatD3D.Diffuse.r, pMat[nCntMat].MatD3D.Diffuse.g, pMat[nCntMat].MatD3D.Diffuse.b, pMat[nCntMat].MatD3D.Diffuse.a);
		m_pShaderSet->pPConstantTable->SetValue(pDevice, "modelDiffuse", modelDiffuse, sizeof(D3DXVECTOR4));
		m_pShaderSet->pPConstantTableNoTex->SetValue(pDevice, "modelDiffuse", modelDiffuse, sizeof(D3DXVECTOR4));

		m_pShaderSet->pPConstantTable->SetFloat(pDevice, "lightFar", pLightCamera->GetFar());
		// 深度マップの取得
		DepthShadow *pDepthShadow = CManager::GetDepthShadow();
		UINT sampleIndex = m_pShaderSet->pPConstantTable->GetSamplerIndex("depthSampler");
		if (pDepthShadow)
		{
			LPDIRECT3DTEXTURE9 depthMap = pDepthShadow->GetTexture();
			pDevice->SetTexture(sampleIndex, depthMap);
		}
		else
		{
			pDevice->SetTexture(sampleIndex, nullptr);
		}
		if (pMat[nCntMat].pTextureFilename != nullptr)
		{
			pDevice->SetTexture(0, CManager::GetTexManager()->GetTexInterface(pMat[nCntMat].pTextureFilename));
			// ピクセルシェーダーの設定
			//------------------------------------------------------------------------
			pDevice->SetPixelShader(m_pShaderSet->pPixelShader);
		}
		else
		{// テクスチャがなければnull
			pDevice->SetTexture(0, nullptr);
			// ピクセルシェーダーの設定
			//------------------------------------------------------------------------
			pDevice->SetPixelShader(m_pShaderSet->pPixelShaderNoTex);
		}
		// 描画
		pModel->pMeshClone->DrawSubset(nCntMat);
	}

	pDevice->SetMaterial(&matDef);	// 元に戻す

	pDevice->SetPixelShader(nullptr);
	pDevice->SetVertexShader(nullptr);
}
*/

//--------------------------------------------------------------------------------
// ディファードレンダリング用
//--------------------------------------------------------------------------------
void CSceneModel::DrawDefferdRendering()
{
	// デバイスの取得
	LPDIRECT3DDEVICE9 pDevice = CRenderer::GetDevice();

	D3DXMatrixIdentity(&m_mtxWorld);//
	D3DXMatrixIdentity(&m_mtxScl);	//
	D3DXMatrixIdentity(&m_mtxRot);	//
	D3DXMatrixIdentity(&m_mtxPos);	// 行列を単位行列にする( 拡大率1,他0で初期化 )

	D3DXVECTOR3 scl = GetScl();

	// ワールド変換行列を設定して変換
	D3DXMatrixScaling(&m_mtxScl, scl.x, scl.y, scl.z);	// 拡大行列
	m_mtxRot = GetMatrixRot(this);
	m_mtxPos = GetMatrixPos(this);

	D3DXMatrixMultiply(&m_mtxWorld, &m_mtxWorld, &m_mtxScl);
	D3DXMatrixMultiply(&m_mtxWorld, &m_mtxWorld, &m_mtxRot);
	D3DXMatrixMultiply(&m_mtxWorld, &m_mtxWorld, &m_mtxPos);

	// デバイスにワールド変換行列を設定
	pDevice->SetTransform(D3DTS_WORLD, &m_mtxWorld);

	// モデルの取得
	CModelManager::MODEL* pModel = m_pUseModelManager->GetModel(m_strFilePass);

	// 現在デバイスに設定されているマテリアル情報の取得
	D3DXMATERIAL* pMat = pModel->pMat;
	D3DMATERIAL9 matDef;
	pDevice->GetMaterial(&matDef);

	// デファードレンダリングの取得
	DefferdRendering *pDefferdRendering = CManager::GetDefferdRendering();

	// 頂点シェーダーの設定
	//------------------------------------------------------------------------
	pDevice->SetVertexDeclaration(m_pShaderSet->pVertexDeclaration);
	pDefferdRendering->SetVXShader(m_mtxWorld);

	// ミップマップ
	pDevice->SetSamplerState(0, D3DSAMP_MIPFILTER, D3DTEXF_LINEAR);

	// モデルのマテリアルごとに設定
	for (int nCntMat = 0; nCntMat < (int)m_pUseModelManager->GetModelNumMat(m_strFilePass); nCntMat++)
	{
		// デバイスにマテリアルを設定
		pDevice->SetMaterial(&pMat->MatD3D);

		if (pMat[nCntMat].pTextureFilename != nullptr)
		{
			pDevice->SetTexture(0, CManager::GetTexManager()->GetTexInterface(pMat[nCntMat].pTextureFilename));
			// ピクセルシェーダーの設定
			//------------------------------------------------------------------------
			pDefferdRendering->SetPXShader(&pMat[nCntMat].MatD3D);
		}
		else
		{// テクスチャがなければnull
			pDevice->SetTexture(0, nullptr);
			// ピクセルシェーダーの設定
			//------------------------------------------------------------------------
			pDefferdRendering->SetNoTexturePXShader(&pMat[nCntMat].MatD3D);
		}
		// 描画
		pModel->pMeshClone->DrawSubset(nCntMat);
	}

	// ミップマップ解除
	pDevice->SetSamplerState(0, D3DSAMP_MIPFILTER, D3DTEXF_NONE);

	pDevice->SetMaterial(&matDef);	// 元に戻す

	pDevice->SetPixelShader(nullptr);
	pDevice->SetVertexShader(nullptr);
}

//================================================================================
// 回転行列
//--------------------------------------------------------------------------------
D3DXMATRIX CSceneModel::GetMatrixRot(CSceneModel *pParts)
{
	D3DXMATRIX mtxrot;
	D3DXMatrixIdentity(&mtxrot);	// 単位行列化
	if (pParts->m_pParent == nullptr)
	{
		D3DXMatrixRotationYawPitchRoll(&mtxrot, m_rot.y, m_rot.x, m_rot.z);
		return mtxrot;
	}

	D3DXMatrixRotationYawPitchRoll(&mtxrot, pParts->m_baseRot.y, pParts->m_baseRot.x, pParts->m_baseRot.z);
	return mtxrot * GetMatrixRot(pParts->m_pParent);
}

//================================================================================
// 並行行列
//--------------------------------------------------------------------------------
D3DXMATRIX CSceneModel::GetMatrixPos(CSceneModel *pParts)
{
	D3DXMATRIX mtxpos;
	D3DXMatrixIdentity(&mtxpos);	// 単位行列化
	if (pParts->m_pParent == nullptr)
	{
		D3DXMatrixTranslation(&mtxpos, m_basePos.x + m_pos.x, m_basePos.y + m_pos.y, m_basePos.z + m_pos.z);
		return mtxpos;
	}

	D3DXMatrixTranslation(&mtxpos, pParts->m_basePos.x + pParts->m_pos.x, pParts->m_basePos.y + pParts->m_pos.y, pParts->m_basePos.z + pParts->m_pos.z);
	return mtxpos * GetMatrixWorld(pParts->m_pParent);
}

//================================================================================
// ワールド行列
//--------------------------------------------------------------------------------
D3DXMATRIX CSceneModel::GetMatrixWorld(CSceneModel *pParts)
{
	D3DXMATRIX mtxWorld, mtxScl, mtxRot, mtxPos;

	D3DXMatrixIdentity(&mtxWorld);	//
	D3DXMatrixIdentity(&mtxScl);	//
	D3DXMatrixIdentity(&mtxRot);	//
	D3DXMatrixIdentity(&mtxPos);	// 行列を単位行列にする( 拡大率1,他0で初期化 )

	// ワールド変換行列を設定して変換
	D3DXVECTOR3 scl = pParts->GetScl();
	D3DXVECTOR3 rot = pParts->GetRot() + pParts->GetBaseRot();
	D3DXVECTOR3 pos = pParts->GetPos() + pParts->GetBasePos();
	D3DXMatrixScaling(&mtxScl, scl.x, scl.y, scl.z);	// 拡大行列
	D3DXMatrixRotationYawPitchRoll(&mtxRot, rot.y, rot.x, rot.z);
	D3DXMatrixTranslation(&mtxPos, pos.x, pos.y, pos.z);

	D3DXMatrixMultiply(&mtxWorld, &mtxWorld, &mtxScl);
	D3DXMatrixMultiply(&mtxWorld, &mtxWorld, &mtxRot);
	D3DXMatrixMultiply(&mtxWorld, &mtxWorld, &mtxPos);

	if (pParts->m_pParent == nullptr)
	{
		return mtxWorld;
	}
	return mtxWorld * GetMatrixWorld(pParts->m_pParent);
}

//--------------------------------------------------------------------------------
// 作成処理
//--------------------------------------------------------------------------------
CSceneModel *CSceneModel::Create(const D3DXVECTOR3 &pos, const D3DXVECTOR3 &rot, const D3DXVECTOR3 &scl, const std::string &strFilePass, int pry, CModelManager *pUseModelManager)
{
	// CScene3Dの生成
	CSceneModel *pSceneModel;
	pSceneModel = new CSceneModel(pry);
	pSceneModel->Init();

	pSceneModel->SetPos(pos);
	pSceneModel->SetRot(rot);
	pSceneModel->SetBasePos(D3DXVECTOR3(0.f, 0.f, 0.f));
	pSceneModel->SetBaseRot(D3DXVECTOR3(0.f, 0.f, 0.f));
	pSceneModel->SetScl(scl);
	pSceneModel->m_strFilePass = strFilePass;
	pSceneModel->m_strPartsName = strFilePass;

	// モデルの生成
	if (pUseModelManager)
	{
		pSceneModel->m_pUseModelManager = pUseModelManager;
	}
	else
	{
		pSceneModel->m_pUseModelManager = CManager::GetModelManager();
	}

	pSceneModel->CreateModel(pSceneModel->m_strFilePass);

	return pSceneModel;
}

//================================================================================
// Parts名指定
//--------------------------------------------------------------------------------
CSceneModel *CSceneModel::Create(const D3DXVECTOR3 &pos, const D3DXVECTOR3 &rot, const D3DXVECTOR3 &scl, const std::string &strFilePass, std::string strPartsName, int pry, CModelManager *pUseModelManager)
{
	// CScene3Dの生成
	CSceneModel *pSceneModel;
	pSceneModel = new CSceneModel(pry);
	pSceneModel->Init();

	pSceneModel->SetPos(pos);
	pSceneModel->SetRot(rot);
	pSceneModel->SetBasePos(D3DXVECTOR3(0.f, 0.f, 0.f));
	pSceneModel->SetBaseRot(D3DXVECTOR3(0.f, 0.f, 0.f));
	pSceneModel->SetScl(scl);
	pSceneModel->m_strFilePass = strFilePass;
	pSceneModel->m_strPartsName = strPartsName;

	// モデルの生成
	if (pUseModelManager)
	{
		pSceneModel->m_pUseModelManager = pUseModelManager;
	}
	else
	{
		pSceneModel->m_pUseModelManager = CManager::GetModelManager();
	}

	pSceneModel->CreateModel(pSceneModel->m_strFilePass);

	return pSceneModel;
}

//================================================================================
// モデル生成
//--------------------------------------------------------------------------------
void CSceneModel::CreateModel(std::string strFilePass)
{
	if (m_pUseModelManager == nullptr)
	{
		m_pUseModelManager = CManager::GetModelManager();
	}

	m_pUseModelManager->SetModel(strFilePass);

	// スレッドセーフ(ロック)
#ifdef THREAD_ON
	std::unique_lock<std::recursive_mutex> locker = CManager::Locker();
#endif

	// テクスチャのセット
	D3DXMATERIAL* pMat = m_pUseModelManager->GetModelMat(strFilePass);
	for (int nCntMat = 0; nCntMat < (int)m_pUseModelManager->GetModelNumMat(strFilePass); nCntMat++)
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
	for (int nCntMesh = 0; nCntMesh < static_cast<int>(pMesh->GetNumVertices()); nCntMesh++)
	{
		Vertex = *(D3DXVECTOR3*)pVtx * CManager::GetMasterScale();				// 頂点座標を取得する。
		m_centerPos += Vertex;						// 全頂点の座標を足しこむ
		min.x = RETURN_MIN(min.x, Vertex.x);		// X最小値
		max.x = RETURN_MAX(max.x, Vertex.x);		// X最大値
		min.y = RETURN_MIN(min.y, Vertex.y);		// Y最小値
		max.y = RETURN_MAX(max.y, Vertex.y);		// Y最大値
		min.z = RETURN_MIN(min.z, Vertex.z);		// Z最小値
		max.z = RETURN_MAX(max.z, Vertex.z);		// Z最大値
		pVtx += VertexSize;							// 進める
	}
	//バッファのアンロック
	pMesh->UnlockVertexBuffer();

	// 幅,高さ,奥行の取得
	SetSize(D3DXVECTOR3(max.x - min.x, max.y - min.y, max.z - min.z));

	// 全頂点の中心座標
	m_centerPos /= static_cast<FLOAT>(pMesh->GetNumVertices());
}

//================================================================================
// Parts取得
//--------------------------------------------------------------------------------
CSceneModel *CSceneModel::GetParts(std::string partsName)
{
	for (int n = 0; n < MAX_SCENE_PRIORITY; n++)
	{
		std::list<CScene*> *lisScene = CScene::GetScene(n);
		for (CScene* pScene : *lisScene)
		{
			if (pScene == nullptr)continue;
			// タイプの取得
			CScene::CLASSTYPE type = pScene->GetClassType();

			if (type == CScene::CLASSTYPE::SCENE_MODEL)
			{
				// ダウンキャスト
				CSceneModel *pModel = ((CSceneModel*)pScene);

				// 要求されたパーツを返却
				if (pModel->GetPartsName() == partsName) { return pModel; }
			}
		}
	}

	// パーツはありませんでした
	return nullptr;
}

//================================================================================
// 親Parts取得
//--------------------------------------------------------------------------------
CSceneModel *CSceneModel::GetParent(CSceneModel *pParts)
{
	if (pParts == nullptr) { return nullptr; }
	return pParts->m_pParent;
}
