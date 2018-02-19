//================================================================================
//
//	Auter : KENSUKE WATANABE
//	Data  : 2017/04/27
//
//--------------------------------------------------------------------------------
//--------------------------------------------------------------------------------
// インクルードファイル
//--------------------------------------------------------------------------------
#include <math.h>
#include "main.h"
#include "sceneBoxX.h"
#include "manager.h"
#include "modelManager.h"
#include "texManager.h"
#include "camera.h"
#include "light.h"
#include "lightcamera.h"
#include "depthShadow.h"
#include "deferredRendering.h"
#include "renderer.h"
#include "utility.h"

//================================================================================
// 初期化処理
//--------------------------------------------------------------------------------
void CSceneBoxX::Init()
{
	SetPos(D3DXVECTOR3(0.f, 0.f, 0.f));		// 座標
	SetRot(D3DXVECTOR3(0.f, 0.f, 0.f));		// 角度
	SetScl(D3DXVECTOR3(1.f, 1.f, 1.f));		// 拡大率
	SetSize(D3DXVECTOR3(1.f, 1.f, 1.f));	// サイズ
	m_pIdxBuffer = nullptr;					// インデックスバッファ
	m_pVtxBuffer = nullptr;					// 頂点バッファ
	m_strFilePass.clear();					// テクスチャの名前

	// 親クラスの初期化
	CScene3D::Init();
}

//--------------------------------------------------------------------------------
// 終了処理
//--------------------------------------------------------------------------------
void CSceneBoxX::Uninit()
{
	// 親クラスの終了
	CScene3D::Uninit();
}

//================================================================================
// 更新処理
//--------------------------------------------------------------------------------
void CSceneBoxX::Update()
{
	AddRot(D3DXVECTOR3(0.f, 0.f, 0.f));
	SetPos(CManager::GetCamera()->GetPos());
}

/*
//================================================================================
// ポリゴン描画
//--------------------------------------------------------------------------------
void CSceneBoxX::Draw()
{
	// デバイスの取得
	LPDIRECT3DDEVICE9 pDevice = CRenderer::GetDevice();

	// ワールドマトリクスの作成
	D3DXMATRIX mtxScl;				// 拡大行列
	D3DXMATRIX mtxRot;				// 回転行列
	D3DXMATRIX mtxPos;				// 平行行列

	D3DXMatrixIdentity(&m_mtxWorld);//
	D3DXMatrixIdentity(&mtxScl);	//
	D3DXMatrixIdentity(&mtxRot);	//
	D3DXMatrixIdentity(&mtxPos);	// 行列を単位行列にする( 拡大率1,他0で初期化 )

	D3DXVECTOR3 scl = GetScl();

	// ワールド変換行列を設定して変換
	D3DXMatrixScaling(&mtxScl, scl.x, scl.y, scl.z);					// 拡大行列
	D3DXMatrixRotationYawPitchRoll(&mtxRot, m_rot.y, m_rot.x, m_rot.z);	// 回転行列
	D3DXMatrixTranslation(&mtxPos, m_pos.x, m_pos.y, m_pos.z);			// 平行行列

	D3DXMatrixMultiply(&m_mtxWorld, &m_mtxWorld, &mtxScl);
	D3DXMatrixMultiply(&m_mtxWorld, &m_mtxWorld, &mtxRot);
	D3DXMatrixMultiply(&m_mtxWorld, &m_mtxWorld, &mtxPos);

	// デバイスにワールド変換行列を設定
	pDevice->SetTransform(D3DTS_WORLD, &m_mtxWorld);

	// モデルの取得
	CModelManager::MODEL* pModel = CManager::GetModelManager()->GetModel(m_strFilePass);

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

	D3DXVECTOR4 lightAmb = D3DXVECTOR4(0.2f, 0.2f, 0.2f, 1.f);
	m_pShaderSet->pVConstantTable->SetValue(pDevice, "ambent", lightAmb, sizeof(D3DXVECTOR4));

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

	// 頂点シェーダーの設定
	//------------------------------------------------------------------------
	pDevice->SetVertexDeclaration(m_pShaderSet->pVertexDeclaration);
	pDevice->SetVertexShader(m_pShaderSet->pVertexShader);

	// 深度マップの取得
	LPDIRECT3DTEXTURE9 depthMap = CManager::GetDepthShadow()->GetTexture();
	UINT sampleIndex = m_pShaderSet->pPConstantTable->GetSamplerIndex("depthSampler");
	pDevice->SetSamplerState(sampleIndex, D3DSAMP_ADDRESSU, D3DTADDRESS_CLAMP);	// テクスチャＵ値の繰り返し設定
	pDevice->SetSamplerState(sampleIndex, D3DSAMP_ADDRESSV, D3DTADDRESS_CLAMP);	// テクスチャＶ値の繰り返し設定
	pDevice->SetTexture(sampleIndex, depthMap);

	// モデルのマテリアルごとに設定
	for (int nCntMat = 0; nCntMat < (int)CManager::GetModelManager()->GetModelNumMat(m_strFilePass); nCntMat++)
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

	pDevice->SetSamplerState(sampleIndex, D3DSAMP_ADDRESSU, D3DTADDRESS_WRAP);	// テクスチャＵ値の繰り返し設定
	pDevice->SetSamplerState(sampleIndex, D3DSAMP_ADDRESSV, D3DTADDRESS_WRAP);	// テクスチャＶ値の繰り返し設定

	pDevice->SetMaterial(&matDef);	// 元に戻す

	pDevice->SetPixelShader(nullptr);
	pDevice->SetVertexShader(nullptr);
}
*/

//--------------------------------------------------------------------------------
// デファードレンダリング用
//--------------------------------------------------------------------------------
void CSceneBoxX::DrawDefferdRendering()
{
	// デバイスの取得
	LPDIRECT3DDEVICE9 pDevice = CRenderer::GetDevice();

	// ワールドマトリクスの作成
	D3DXMATRIX mtxScl;						// 拡大行列
	D3DXMATRIX mtxRot;						// 回転行列
	D3DXMATRIX mtxPos;						// 平行行列

	D3DXMatrixIdentity(&m_mtxWorld);// 
	D3DXMatrixIdentity(&mtxScl);	// 
	D3DXMatrixIdentity(&mtxRot);	// 
	D3DXMatrixIdentity(&mtxPos);	// 行列を単位行列にする( 拡大率1,他0で初期化 )

	D3DXVECTOR3 scl = GetScl();

	// ワールド変換行列を設定して変換
	D3DXMatrixScaling(&mtxScl, scl.x, scl.y, scl.z);					// 拡大行列
	D3DXMatrixRotationYawPitchRoll(&mtxRot, m_rot.y, m_rot.x, m_rot.z);	// 回転行列
	D3DXMatrixTranslation(&mtxPos, m_pos.x, m_pos.y, m_pos.z);			// 平行行列

	D3DXMatrixMultiply(&m_mtxWorld, &m_mtxWorld, &mtxScl);
	D3DXMatrixMultiply(&m_mtxWorld, &m_mtxWorld, &mtxRot);
	D3DXMatrixMultiply(&m_mtxWorld, &m_mtxWorld, &mtxPos);

	// モデルの取得
	CModelManager::MODEL* pModel = CManager::GetModelManager()->GetModel(m_strFilePass);

	// 現在デバイスに設定されているマテリアル情報の取得
	D3DXMATERIAL* pMat = pModel->pMat;
	D3DMATERIAL9 matDef;
	pDevice->GetMaterial(&matDef);

	// デファードレンダリングの取得
	DefferdRendering *pDefferdRendering = CManager::GetDefferdRendering();

	// 現在デバイスに設定されているマテリアル情報の取得
	pDevice->GetMaterial(&matDef);

	// 頂点シェーダーの設定
	//------------------------------------------------------------------------
	pDevice->SetVertexDeclaration(m_pShaderSet->pVertexDeclaration);
	pDefferdRendering->SetVXShader(m_mtxWorld);

	// モデルのマテリアルごとに設定
	for (int nCntMat = 0; nCntMat < (int)CManager::GetModelManager()->GetModelNumMat(m_strFilePass); nCntMat++)
	{
		// デバイスにマテリアルを設定
		pDevice->SetMaterial(&pMat[nCntMat].MatD3D);

		// pMat[nCntMat].MatD3D.Diffuse.a = 0.f;

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

	pDevice->SetMaterial(&matDef);	// 元に戻す

	pDevice->SetPixelShader(nullptr);
	pDevice->SetVertexShader(nullptr);
}

//--------------------------------------------------------------------------------
// 作成処理
//--------------------------------------------------------------------------------
CSceneBoxX *CSceneBoxX::Create(const D3DXVECTOR3 &pos, const D3DXVECTOR3 &rot, const D3DXVECTOR3 &scl, const std::string &strFilePass, int pry)
{
	// スレッドセーフ(ロック)
#ifdef THREAD_ON
	std::unique_lock<std::recursive_mutex> locker = CManager::Locker();
#endif

	// CSceneBoxXの生成
	CSceneBoxX *pFieldX = new CSceneBoxX(pry);
	pFieldX->Init();

	pFieldX->SetPos(pos);
	pFieldX->SetRot(rot);
	pFieldX->SetScl(scl);
	pFieldX->SetFilePass(strFilePass);

	//---------------------
	// モデルのセット
	//---------------------
	CModelManager *modelManager = CManager::GetModelManager();
	if (modelManager->SetModel(strFilePass))
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
		// モデル頂点の取得
		//---------------------------------------------
		LPD3DXMESH pMesh = nullptr;
		pMesh = modelManager->GetModelMesh(strFilePass);

		// Vtxバッファをロックする
		BYTE *pVtx = nullptr;							// 頂点バッファ
		pMesh->LockVertexBuffer(D3DLOCK_READONLY, (void**)&pVtx);
		DWORD FVF = pMesh->GetFVF();					// 頂点フォーマットを取得する。
		DWORD VertexSize = D3DXGetFVFVertexSize(FVF);	// 頂点のサイズを取得する。

		// モデルの頂点の最大値、最小値の取得
		D3DXVECTOR3 Vertex;
		D3DXVECTOR3 min = D3DXVECTOR3(0.f, 0.f, 0.f);
		D3DXVECTOR3 max = D3DXVECTOR3(0.f, 0.f, 0.f);
		for (int cntVtx = 0; cntVtx < static_cast<int>(pMesh->GetNumVertices()); cntVtx++)
		{
			Vertex = *(D3DXVECTOR3*)pVtx * CManager::GetMasterScale();				// 頂点座標を取得する。
			pFieldX->m_centerPos += Vertex;						// 全頂点の座標を足しこむ
			min.x = RETURN_MIN(min.x, Vertex.x);		// X最小値
			max.x = RETURN_MAX(max.x, Vertex.x);		// X最大値
			min.y = RETURN_MIN(min.y, Vertex.y);		// Y最小値
			max.y = RETURN_MAX(max.y, Vertex.y);		// Y最大値
			min.z = RETURN_MIN(min.z, Vertex.z);		// Z最小値
			max.z = RETURN_MAX(max.z, Vertex.z);		// Z最大値
			pVtx += VertexSize;							// 進める
		}
		// 幅,高さ,奥行の取得
		pFieldX->SetSize(D3DXVECTOR3(max.x - min.x, max.y - min.y, max.z - min.z));
		// Vtxバッファのアンロック
		pMesh->UnlockVertexBuffer();

		pFieldX->m_centerPos /= static_cast<FLOAT>(pMesh->GetNumVertices());
	}

	return pFieldX;
}
