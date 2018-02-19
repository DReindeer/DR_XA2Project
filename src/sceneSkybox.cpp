//================================================================================
//
//	Auter : KENSUKE WATANABE
//	Data  : 2017/04/27
//
//--------------------------------------------------------------------------------
#include <math.h>
#include "main.h"
#include "sceneSkybox.h"
#include "input.h"
#include "manager.h"
#include "texManager.h"
#include "renderer.h"
#include "mainController.h"
#include "deferredRendering.h"

//================================================================================
// 定数
//--------------------------------------------------------------------------------
static const int NUM_VTX = 18;			// 頂点数
static const int NUM_IDX = 22;			// インデックス数

//================================================================================
// 初期化
//--------------------------------------------------------------------------------
void CSceneSkybox::Init( void )
{
	CScene3D::Init();

	// 初期化
	SetPos(D3DXVECTOR3(0.f, 0.f, 0.f));
	SetRot(D3DXVECTOR3(0.f, 0.f, 0.f));
	SetScl(D3DXVECTOR3(1.f, 1.f, 1.f));
	m_moveTex = 0.000f;
	m_strFilePass.clear();
	m_numVtx = NUM_VTX;
	m_numIndex = NUM_IDX;
}

//--------------------------------------------------------------------------------
// 終了処理
//--------------------------------------------------------------------------------
void CSceneSkybox::Uninit( void )
{
	// インデックスバッファの解放
	if(m_pIdxBuffer != nullptr)
	{
		m_pIdxBuffer->Release();
		m_pIdxBuffer = nullptr;
	}

	// 親クラスの終了
	CScene3D::Uninit();
}

//================================================================================
// 更新処理
//--------------------------------------------------------------------------------
void CSceneSkybox::Update( void )
{
	AddRot(D3DXVECTOR3(0.f, m_moveTex, 0.f));
	SetPos(CManager::GetCamera()->GetPos());
}

//================================================================================
// ポリゴン描画
//--------------------------------------------------------------------------------
void CSceneSkybox::Draw( void )
{
	// デバイスの取得
	LPDIRECT3DDEVICE9 pDevice = CRenderer::GetDevice();

	D3DXVECTOR3 pos = GetPos();
	D3DXVECTOR3 rot = GetRot();
	D3DXVECTOR3 scl = GetScl();

	// ワールドマトリクスの作成
	D3DXMATRIX mtxScl;						// 拡大行列
	D3DXMATRIX mtxRot;						// 回転行列
	D3DXMATRIX mtxPos;						// 平行行列

	D3DXMatrixIdentity(&m_mtxWorld);		// 行列を単位行列にする( 拡大率1,他0で初期化 )
	D3DXMatrixIdentity(&mtxScl);			// 
	D3DXMatrixIdentity(&mtxRot);			// 
	D3DXMatrixIdentity(&mtxPos);			// 

	// ワールド変換行列を設定して変換
	D3DXMatrixScaling(&mtxScl, scl.x, scl.y, scl.z);				// 拡大行列
	D3DXMatrixRotationYawPitchRoll(&mtxRot, rot.y, rot.x, rot.z);	// 回転行列
	D3DXMatrixTranslation(&mtxPos, pos.x, pos.y, pos.z);			// 平行行列

	D3DXMatrixMultiply(&m_mtxWorld, &m_mtxWorld, &mtxScl);
	D3DXMatrixMultiply(&m_mtxWorld, &m_mtxWorld, &mtxRot);
	D3DXMatrixMultiply(&m_mtxWorld, &m_mtxWorld, &mtxPos);

	pDevice->SetRenderState(D3DRS_LIGHTING, FALSE);			// ライト表示オフ

	// デバイスにワールド変換行列を設定
	pDevice->SetTransform(D3DTS_WORLD, &m_mtxWorld);

	// 頂点フォーマットの設定
	pDevice->SetFVF(FVF_VERTEX_3D);

	// インデックス番号をデバイスに設定
	pDevice->SetIndices(m_pIdxBuffer);

	// テクスチャをパイプラインにセット
	if(!m_strFilePass.empty())
	{
		pDevice->SetTexture(0, CManager::GetTexManager()->GetTexInterface(m_strFilePass));
	}else {pDevice->SetTexture(0, nullptr);}

	// 頂点バッファをデバイスのデータ ストリームにバインドする
	pDevice->SetStreamSource(0, m_pVtxBuffer, 0, sizeof(VERTEX_3D));

	pDevice->DrawIndexedPrimitive(D3DPT_TRIANGLESTRIP, 0, 0, NUM_POLYGON, 0, m_numIndex - 2);

	// レンダーステートの設定を戻す
	pDevice->SetRenderState(D3DRS_LIGHTING, TRUE);			// ライトの表示を有効にする
}

//--------------------------------------------------------------------------------
// デファードレンダリング用
//--------------------------------------------------------------------------------
void CSceneSkybox::DrawDefferdRendering()
{
	// デバイスの取得
	LPDIRECT3DDEVICE9 pDevice = CRenderer::GetDevice();

//	pDevice->SetRenderState(D3DRS_FILLMODE, D3DFILL_WIREFRAME);

	D3DXVECTOR3 pos = GetPos();
	D3DXVECTOR3 rot = GetRot();
	D3DXVECTOR3 scl = GetScl();

	// ワールドマトリクスの作成
	D3DXMATRIX mtxScl;						// 拡大行列
	D3DXMATRIX mtxRot;						// 回転行列
	D3DXMATRIX mtxPos;						// 平行行列

	D3DXMatrixIdentity(&m_mtxWorld);		// 行列を単位行列にする( 拡大率1,他0で初期化 )
	D3DXMatrixIdentity(&mtxScl);			// 
	D3DXMatrixIdentity(&mtxRot);			// 
	D3DXMatrixIdentity(&mtxPos);			// 

	// ワールド変換行列を設定して変換
	D3DXMatrixScaling(&mtxScl, scl.x, scl.y, scl.z);				// 拡大行列
	D3DXMatrixRotationYawPitchRoll(&mtxRot, rot.y, rot.x, rot.z);	// 回転行列
	D3DXMatrixTranslation(&mtxPos, pos.x, pos.y, pos.z);			// 平行行列

	D3DXMatrixMultiply(&m_mtxWorld, &m_mtxWorld, &mtxScl);
	D3DXMatrixMultiply(&m_mtxWorld, &m_mtxWorld, &mtxRot);
	D3DXMatrixMultiply(&m_mtxWorld, &m_mtxWorld, &mtxPos);

	// デバイスにワールド変換行列を設定
	pDevice->SetTransform(D3DTS_WORLD, &m_mtxWorld);

	D3DMATERIAL9 matDef;
	pDevice->GetMaterial(&matDef);

	// デファードレンダリングの取得
	DefferdRendering *pDefferdRendering = CManager::GetDefferdRendering();

	// 頂点シェーダーの設定
	//------------------------------------------------------------------------
	pDevice->SetVertexDeclaration(m_pShaderSet->pVertexDeclaration);
	pDefferdRendering->SetVXShader(m_mtxWorld);

	// デバイスにマテリアルを設定
	pDevice->SetMaterial(m_pMat);

	// ピクセルシェーダーの設定
	//------------------------------------------------------------------------
	if (!m_strFilePass.empty())
	{
		LPDIRECT3DTEXTURE9 pTex = CManager::GetTexManager()->GetTexInterface(m_strFilePass);
		pDevice->SetTexture(0, pTex);
		pDefferdRendering->SetPXShader(m_pMat);
	}
	else 
	{ 
		pDevice->SetTexture(0, nullptr);
		pDefferdRendering->SetNoTexturePXShader(m_pMat);
	}

	// インデックス番号をデバイスに設定
	pDevice->SetIndices(m_pIdxBuffer);

	// 頂点バッファをデバイスのデータ ストリームにバインドする
	pDevice->SetStreamSource(0, m_pVtxBuffer, 0, sizeof(VERTEX_3D));

	pDevice->DrawIndexedPrimitive(D3DPT_TRIANGLESTRIP, 0, 0, NUM_POLYGON, 0, m_numIndex - 2);

	// レンダーステートの設定を戻す
//	pDevice->SetRenderState(D3DRS_FILLMODE, D3DFILL_SOLID);

	pDevice->SetMaterial(&matDef);	// 元に戻す

	pDevice->SetPixelShader(nullptr);
	pDevice->SetVertexShader(nullptr);
}

//--------------------------------------------------------------------------------
// 頂点作成
//--------------------------------------------------------------------------------
HRESULT CSceneSkybox::SetVtx()
{
	// スレッドセーフ(ロック)
#ifdef THREAD_ON
	std::unique_lock<std::recursive_mutex> locker = CManager::Locker();
#endif

	LPDIRECT3DDEVICE9 pDevice = CRenderer::GetDevice();

	// 頂点バッファを作成する--------------------------------
	if (FAILED(pDevice->CreateVertexBuffer(sizeof(VERTEX_3D) * NUM_VTX,
		D3DUSAGE_WRITEONLY, FVF_VERTEX_3D, D3DPOOL_MANAGED, &m_pVtxBuffer, nullptr)))
	{
		return E_FAIL;
	}
	VERTEX_3D* pVtx;	// バッファをロックする
	m_pVtxBuffer->Lock(0, 0, (void**)&pVtx, 0);

	D3DXVECTOR3 scl = GetScl();

	// 側面
	pVtx[0].pos = D3DXVECTOR3(m_size, -m_size, -m_size);			// 座標
	pVtx[0].tex = D3DXVECTOR2(0.f, 0.6666666f);			// テクスチャ座標
	pVtx[1].pos = D3DXVECTOR3(m_size, m_size, -m_size);			// 座標
	pVtx[1].tex = D3DXVECTOR2(0.f, 0.3333333f);			// テクスチャ座標

	pVtx[2].pos = D3DXVECTOR3(-m_size, -m_size, -m_size);		// 座標
	pVtx[2].tex = D3DXVECTOR2(0.25f, 0.6666666f);		// テクスチャ座標
	pVtx[3].pos = D3DXVECTOR3(-m_size, m_size, -m_size);			// 座標
	pVtx[3].tex = D3DXVECTOR2(0.25f, 0.3333333f);		// テクスチャ座標

	pVtx[4].pos = D3DXVECTOR3(-m_size, -m_size, m_size);			// 座標
	pVtx[4].tex = D3DXVECTOR2(0.5f, 0.6666666f);		// テクスチャ座標
	pVtx[5].pos = D3DXVECTOR3(-m_size, m_size, m_size);			// 座標
	pVtx[5].tex = D3DXVECTOR2(0.5f, 0.3333333f);		// テクスチャ座標

	pVtx[6].pos = D3DXVECTOR3(m_size, -m_size, m_size);			// 座標
	pVtx[6].tex = D3DXVECTOR2(0.75f, 0.6666666f);		// テクスチャ座標
	pVtx[7].pos = D3DXVECTOR3(m_size, m_size, m_size);			// 座標
	pVtx[7].tex = D3DXVECTOR2(0.75f, 0.3333333f);		// テクスチャ座標

	pVtx[8].pos = D3DXVECTOR3(m_size, -m_size, -m_size);			// 座標
	pVtx[8].tex = D3DXVECTOR2(1.f, 0.6666666f);			// テクスチャ座標
	pVtx[9].pos = D3DXVECTOR3(m_size, m_size, -m_size);			// 座標
	pVtx[9].tex = D3DXVECTOR2(1.f, 0.3333333f);			// テクスチャ座標

														// 上面
	pVtx[10].pos = D3DXVECTOR3(-m_size, m_size, -m_size);		// 座標
	pVtx[10].tex = D3DXVECTOR2(0.25f, 0.3333333f);		// テクスチャ座標
	pVtx[11].pos = D3DXVECTOR3(m_size, m_size, -m_size);			// 座標
	pVtx[11].tex = D3DXVECTOR2(0.25f, 0.f);				// テクスチャ座標
	pVtx[12].pos = D3DXVECTOR3(-m_size, m_size, m_size);			// 座標
	pVtx[12].tex = D3DXVECTOR2(0.5f, 0.3333333f);		// テクスチャ座標
	pVtx[13].pos = D3DXVECTOR3(m_size, m_size, m_size);			// 座標
	pVtx[13].tex = D3DXVECTOR2(0.5f, 0.f);				// テクスチャ座標

														// 下面
	pVtx[14].pos = D3DXVECTOR3(-m_size, -m_size, m_size);		// 座標
	pVtx[14].tex = D3DXVECTOR2(0.25f, 0.6666666f);		// テクスチャ座標
	pVtx[15].pos = D3DXVECTOR3(m_size, -m_size, m_size);			// 座標
	pVtx[15].tex = D3DXVECTOR2(0.25f, m_size);				// テクスチャ座標
	pVtx[16].pos = D3DXVECTOR3(-m_size, -m_size, -m_size);		// 座標
	pVtx[16].tex = D3DXVECTOR2(0.5f, 0.6666666f);		// テクスチャ座標
	pVtx[17].pos = D3DXVECTOR3(m_size, -m_size, -m_size);		// 座標
	pVtx[17].tex = D3DXVECTOR2(0.5f, m_size);				// テクスチャ座標


	for (int i = 0; i < m_numVtx; i++)
	{
		pVtx[i].nom = D3DXVECTOR3(0.f, 1.f, 0.f);		// 法線
		pVtx[i].col = D3DXVECTOR4(1.f, 1.f, 1.f, 0.f);	// 色
	}

	// ロックの解除
	m_pVtxBuffer->Unlock();

	// インデックスバッファを作成する--------------------------------
	if (FAILED(pDevice->CreateIndexBuffer(
		sizeof(WORD) * m_numIndex,			// バッファ量
		D3DUSAGE_WRITEONLY,					// 使用フラグ
		D3DFMT_INDEX16,						// 必要なバッファ量
		D3DPOOL_MANAGED,					// メモリの管理方法
		&m_pIdxBuffer,
		nullptr)))
	{
		return E_FAIL;
	}

	WORD *pIdx;			// バッファをロックする
	m_pIdxBuffer->Lock(0, 0, (void**)&pIdx, 0);

	pIdx[0] = static_cast<WORD>(0);
	pIdx[1] = static_cast<WORD>(1);
	pIdx[2] = static_cast<WORD>(2);
	pIdx[3] = static_cast<WORD>(3);
	pIdx[4] = static_cast<WORD>(4);
	pIdx[5] = static_cast<WORD>(5);
	pIdx[6] = static_cast<WORD>(6);
	pIdx[7] = static_cast<WORD>(7);
	pIdx[8] = static_cast<WORD>(8);
	pIdx[9] = static_cast<WORD>(9);
	pIdx[10] = static_cast<WORD>(9);
	pIdx[11] = static_cast<WORD>(10);
	pIdx[12] = static_cast<WORD>(10);
	pIdx[13] = static_cast<WORD>(11);
	pIdx[14] = static_cast<WORD>(12);
	pIdx[15] = static_cast<WORD>(13);
	pIdx[16] = static_cast<WORD>(13);
	pIdx[17] = static_cast<WORD>(14);
	pIdx[18] = static_cast<WORD>(14);
	pIdx[19] = static_cast<WORD>(15);
	pIdx[20] = static_cast<WORD>(16);
	pIdx[21] = static_cast<WORD>(17);

	//ロックの解除
	m_pIdxBuffer->Unlock();

	// デフューズ色
	m_pMat = new D3DMATERIAL9;
	m_pMat->Diffuse.a = 0.f;
	m_pMat->Diffuse.r = 1.f;
	m_pMat->Diffuse.g = 1.f;
	m_pMat->Diffuse.b = 1.f;
	m_pMat->Specular.a = 1.f;
	m_pMat->Specular.r = 1.f;
	m_pMat->Specular.g = 1.f;
	m_pMat->Specular.b = 1.f;
	m_pMat->Power = 0.f;

	return NOERROR;
}

//--------------------------------------------------------------------------------
// 作成処理
//--------------------------------------------------------------------------------
CSceneSkybox *CSceneSkybox::Create(const D3DXVECTOR3 &pos, const D3DXVECTOR3 &rot, const float &size, const std::string &strFilePass, int pry)
{
	// CSceneSkyboxの生成
	CSceneSkybox *pSceneSkydome;
	pSceneSkydome = new CSceneSkybox(pry);
	pSceneSkydome->Init();

	pSceneSkydome->SetPos(pos);
	pSceneSkydome->SetRot(rot);
	pSceneSkydome->SetScl(D3DXVECTOR3(1.f, 1.f, 1.f));
	pSceneSkydome->m_strFilePass = strFilePass;

	pSceneSkydome->m_size = size;

	// 頂点設定処理
	pSceneSkydome->SetVtx();
	
	// テクスチャのセット
	CManager::GetTexManager()->SetTex(strFilePass);

	return pSceneSkydome;
}