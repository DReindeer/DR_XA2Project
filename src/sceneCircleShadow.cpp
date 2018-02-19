//================================================================================
//
//	Auter : KENSUKE WATANABE
//	Data  : 2018/02/10
//
//--------------------------------------------------------------------------------
#include "manager.h"
#include "texManager.h"
#include "modelManager.h"
#include "scene3D.h"
#include "sceneCircleShadow.h"
#include "input.h"
#include "renderer.h"

#include <assert.h>

//--------------------------------------------------------------------------------
// 初期化
//--------------------------------------------------------------------------------
void CSceneCircleShadow::Init()
{
	CSceneModel::Init();
}

//--------------------------------------------------------------------------------
// 終了
//--------------------------------------------------------------------------------
void CSceneCircleShadow::Uninit()
{
	CSceneModel::Uninit();
}

//--------------------------------------------------------------------------------
// 更新
//--------------------------------------------------------------------------------
void CSceneCircleShadow::Update()
{
	CSceneModel::Update();
}

//--------------------------------------------------------------------------------
// 描画
//--------------------------------------------------------------------------------
void CSceneCircleShadow::Draw()
{
	// デバイスの取得
	LPDIRECT3DDEVICE9 pDevice = CRenderer::GetDevice();

	D3DXMatrixIdentity(&m_mtxWorld);// 行列を単位行列にする( 拡大率1,他0で初期化 )
	D3DXMatrixIdentity(&m_mtxScl);	// 行列を単位行列にする( 拡大率1,他0で初期化 )
	D3DXMatrixIdentity(&m_mtxRot);	// 行列を単位行列にする( 拡大率1,他0で初期化 )
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

	// 頂点フォーマットの設定
	pDevice->SetFVF(FVF_VERTEX_3D);

	// 現在デバイスに設定されているマテリアル情報の取得
	D3DXMATERIAL* pMat = CManager::GetModelManager()->GetModelMat(m_strFilePass);
	D3DMATERIAL9 matDef;
	pDevice->GetMaterial(&matDef);

	// 表面
	pDevice->SetRenderState(D3DRS_STENCILENABLE, TRUE);
	pDevice->SetRenderState(D3DRS_ZWRITEENABLE, FALSE);
	pDevice->SetRenderState(D3DRS_COLORWRITEENABLE, 0);
	pDevice->SetRenderState(D3DRS_STENCILFUNC, D3DCMP_ALWAYS);
	pDevice->SetRenderState(D3DRS_STENCILREF, 1);
	pDevice->SetRenderState(D3DRS_CULLMODE, D3DCULL_CCW);
	pDevice->SetRenderState(D3DRS_STENCILPASS, D3DSTENCILOP_INCR);

	// モデルのマテリアルごとに設定
	for (int nCntMat = 0; nCntMat < (int)CManager::GetModelManager()->GetModelNumMat(m_strFilePass); nCntMat++)
	{
		// デバイスにマテリアルを設定
		pDevice->SetMaterial(&pMat->MatD3D);

		// テクスチャ情報の設定
		if (pMat[nCntMat].pTextureFilename != nullptr){
			pDevice->SetTexture(0, CManager::GetTexManager()->GetTexInterface(pMat[nCntMat].pTextureFilename));
		}
		else{// テクスチャがなければnull
			pDevice->SetTexture(0, nullptr);
		}

		// 描画
		CManager::GetModelManager()->GetModelMesh(m_strFilePass)->DrawSubset(nCntMat);

	//	pDevice->SetMaterial(&matDef);	// 元に戻す
	}

	// 裏面
	pDevice->SetRenderState(D3DRS_STENCILPASS, D3DSTENCILOP_DECR);
	pDevice->SetRenderState(D3DRS_CULLMODE, D3DCULL_CW);

//	pDevice->GetMaterial(&matDef);

	// モデルのマテリアルごとに設定
	for (int nCntMat = 0; nCntMat < (int)CManager::GetModelManager()->GetModelNumMat(m_strFilePass); nCntMat++)
	{
		// デバイスにマテリアルを設定
		pDevice->SetMaterial(&pMat->MatD3D);

		// テクスチャ情報の設定
		if (pMat[nCntMat].pTextureFilename != nullptr) {
			pDevice->SetTexture(0, CManager::GetTexManager()->GetTexInterface(pMat[nCntMat].pTextureFilename));
		}
		else {// テクスチャがなければnull
			pDevice->SetTexture(0, nullptr);
		}

		// 描画
		CManager::GetModelManager()->GetModelMesh(m_strFilePass)->DrawSubset(nCntMat);
	}

	pDevice->SetRenderState(D3DRS_COLORWRITEENABLE, 0xf);
	pDevice->SetRenderState(D3DRS_CULLMODE, D3DCULL_CCW);
	pDevice->SetRenderState(D3DRS_ZWRITEENABLE, TRUE);

	pDevice->SetRenderState(D3DRS_STENCILENABLE, FALSE);

	pDevice->SetMaterial(&matDef);	// 元に戻す
}

//--------------------------------------------------------------------------------
// 作成処理
//--------------------------------------------------------------------------------
CSceneCircleShadow *CSceneCircleShadow::Create(const D3DXVECTOR3 &pos, const D3DXVECTOR3 &rot, const D3DXVECTOR3 &scl, const std::string &strFilePass, int pry)
{
	// CScene3Dの生成
	CSceneCircleShadow *pCircleShadow;
	pCircleShadow = new CSceneCircleShadow(pry);
	pCircleShadow->Init();

	pCircleShadow->SetPos(pos);
	pCircleShadow->SetRot(rot);
	pCircleShadow->SetBasePos(D3DXVECTOR3(0.f, 0.f, 0.f));
	pCircleShadow->SetBaseRot(D3DXVECTOR3(0.f, 0.f, 0.f));
	pCircleShadow->SetScl(scl);
	pCircleShadow->m_strFilePass = strFilePass;
	pCircleShadow->m_strPartsName = strFilePass;

	// モデルの生成
	pCircleShadow->CreateModel(pCircleShadow->m_strFilePass);

	return pCircleShadow;
}
