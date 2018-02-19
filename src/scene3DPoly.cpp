//================================================================================
//
//	Auter : KENSUKE WATANABE
//	Data  : 2018/02/10
//
//--------------------------------------------------------------------------------
#include "scene3D.h"
#include "scene3DPoly.h"
#include "manager.h"
#include "utility.h"
#include "input.h"
#include "modeGame.h"
#include "texManager.h"
#include "renderer.h"
#include "camera.h"
#include "depthShadow.h"

//--------------------------------------------------------------------------------
// 初期化
//--------------------------------------------------------------------------------
void CScene3DPoly::Init()
{
	// 変数の初期化
	//---------------------
	SetPos(D3DXVECTOR3(0.f, 0.f, 0.f));		// 座標
	SetSize(D3DXVECTOR3(1.f, 1.f, 1.f));	// サイズ
	SetRot(D3DXVECTOR3(0.f, 0.f, 0.f));		// 回転量
	SetScl(D3DXVECTOR3(1.f, 1.f, 1.f));		// 拡大率
}

//--------------------------------------------------------------------------------
// 終了
//--------------------------------------------------------------------------------
void CScene3DPoly::Uninit()
{
	// Vtxバッファの解放
	if (m_pVtxBuffer != nullptr)
	{
		m_pVtxBuffer->Release();
		m_pVtxBuffer = nullptr;
	}
}

//--------------------------------------------------------------------------------
// 更新
//--------------------------------------------------------------------------------
void CScene3DPoly::Update()
{
	// スレッドセーフ(ロック)
#ifdef THREAD_ON
	std::unique_lock<std::recursive_mutex> locker = CManager::Locker();
#endif

	// バッファをロックし仮想アドレスを取得
	VERTEX_3D* pVtx;
	m_pVtxBuffer->Lock(0, 0, (void**)&pVtx, 0);

	D3DXVECTOR3 size = GetSize();
	float r = 255.f / GetR();
	float g = 255.f / GetG();
	float b = 255.f / GetB();

	// 頂点座標
	pVtx[0].pos = D3DXVECTOR3(-size.x * 0.5f, 0.f, size.z * 0.5f);
	pVtx[1].pos = D3DXVECTOR3(size.x * 0.5f, 0.f, size.z * 0.5f);
	pVtx[2].pos = D3DXVECTOR3(-size.x * 0.5f, 0.f, -size.z * 0.5f);
	pVtx[3].pos = D3DXVECTOR3(size.x * 0.5f, 0.f, -size.z * 0.5f);
	// テクスチャ座標
	pVtx[0].tex = D3DXVECTOR2(0.f, 0.f);
	pVtx[1].tex = D3DXVECTOR2(1.f, 0.f);
	pVtx[2].tex = D3DXVECTOR2(0.f, 1.f);
	pVtx[3].tex = D3DXVECTOR2(1.f, 1.f);

	for (int i = 0; i < 4; i++)
	{
		pVtx[i].col = D3DXVECTOR4(r, g, b, 0.f);
		pVtx[i].nom = D3DXVECTOR3(0.f, 1.f, 0.f);
	}

	// 設定後バッファのアンロック
	m_pVtxBuffer->Unlock();
}
/*
//--------------------------------------------------------------------------------
// 描画
//--------------------------------------------------------------------------------
void CScene3DPoly::Draw()
{
	// デバイスの取得
	LPDIRECT3DDEVICE9 pDevice = CRenderer::GetDevice();

	// 変数宣言
	D3DXMATRIX mtxScl;			// 拡大行列
	D3DXMATRIX mtxRot;			// 回転行列
	D3DXMATRIX mtxPos;			// 平行行列

	// ワールドマトリクスの作成
	D3DXMatrixIdentity(&m_mtxWorld);		// 行列を単位行列にする( 拡大率1,他0で初期化 )
	D3DXMatrixIdentity(&mtxScl);			// 
	D3DXMatrixIdentity(&mtxRot);			// 
	D3DXMatrixIdentity(&mtxPos);			//

	// ワールド変換行列を設定して変換
	D3DXVECTOR3 scl = GetScl();
	D3DXVECTOR3 rot = GetRot();
	D3DXVECTOR3 pos = GetPos();
	D3DXMatrixScaling(&mtxScl, scl.x, scl.y, scl.z);					// 拡大行列
	D3DXMatrixRotationYawPitchRoll(&mtxRot, rot.y, rot.x, rot.z);		// 回転行列
	D3DXMatrixTranslation(&mtxPos, pos.x, pos.y, pos.z);				// 平行行列

	D3DXMatrixMultiply(&m_mtxWorld, &m_mtxWorld, &mtxScl);
	D3DXMatrixMultiply(&m_mtxWorld, &m_mtxWorld, &mtxRot);
	D3DXMatrixMultiply(&m_mtxWorld, &m_mtxWorld, &mtxPos);

	// デバイスにワールド変換行列を設定
	pDevice->SetTransform(D3DTS_WORLD, &m_mtxWorld);

	// 頂点フォーマットの設定
	pDevice->SetFVF(FVF_VERTEX_3D);

	// ストリームの設定
	pDevice->SetStreamSource(0, m_pVtxBuffer, 0, sizeof(VERTEX_3D));

	// テクスチャの設定
	pDevice->SetTexture(0, CManager::GetTexManager()->GetTexInterface(m_strFilePass));

	// ポリゴンの描画 // ポリゴンの形, ポリゴンの数, 先頭アドレス, サイズ
	pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, 0, NUM_POLYGON);
}
*/

//--------------------------------------------------------------------------------
// デファードレンダリング用
//--------------------------------------------------------------------------------
void CScene3DPoly::DrawDefferdRendering()
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

	// ワールド変換行列を設定して変換
	D3DXVECTOR3 scl = GetScl();
	D3DXMatrixScaling(&mtxScl, scl.x, scl.y, scl.z);						// 拡大行列
	D3DXMatrixRotationYawPitchRoll(&mtxRot, m_rot.y, m_rot.x, m_rot.z);		// 回転行列
	D3DXMatrixTranslation(&mtxPos, m_pos.x, m_pos.y, m_pos.z);				// 平行行列

	D3DXMatrixMultiply(&m_mtxWorld, &m_mtxWorld, &mtxScl);
	D3DXMatrixMultiply(&m_mtxWorld, &m_mtxWorld, &mtxRot);
	D3DXMatrixMultiply(&m_mtxWorld, &m_mtxWorld, &mtxPos);

	// デバイスにワールド変換行列を設定
	pDevice->SetTransform(D3DTS_WORLD, &m_mtxWorld);

	// アルファテストの有効化
	pDevice->SetRenderState(D3DRS_ALPHATESTENABLE, TRUE);
	pDevice->SetRenderState(D3DRS_ALPHAFUNC, D3DCMP_GREATEREQUAL);
	// 不透明にする値の設定
	pDevice->SetRenderState(D3DRS_ALPHAREF, (DWORD)0xA0);

	// 開始
	CManager::GetDepthShadow()->DrawBigin(m_mtxWorld);

	// 頂点フォーマットの設定
	pDevice->SetFVF(FVF_VERTEX_3D);

	// ストリームの設定
	pDevice->SetStreamSource(0, m_pVtxBuffer, 0, sizeof(VERTEX_3D));

	// ポリゴンの描画 // ポリゴンの形, ポリゴンの数, 先頭アドレス, サイズ
	pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, 0, NUM_POLYGON);

	// 終了
	CManager::GetDepthShadow()->DrawEnd();

	pDevice->SetRenderState(D3DRS_ALPHATESTENABLE, FALSE);
}

//--------------------------------------------------------------------------------
// 作成処理
//--------------------------------------------------------------------------------
CScene3DPoly *CScene3DPoly::Create(const D3DXVECTOR3 &pos, const D3DXVECTOR3 &rot, const D3DXVECTOR3 &size, const std::string &strFilePass, int priority)
{
	// CScene3DPolyの生成
	CScene3DPoly *pScene3DPoly;
	pScene3DPoly = new CScene3DPoly(priority);
	pScene3DPoly->Init();

	pScene3DPoly->SetPos(pos);
	pScene3DPoly->SetRot(rot);
	pScene3DPoly->SetSize(size);
	pScene3DPoly->m_strFilePass = strFilePass;

	// スレッドセーフ(ロック)
#ifdef THREAD_ON
	std::unique_lock<std::recursive_mutex> locker = CManager::Locker();
#endif

	// D3Deviceの取得
	LPDIRECT3DDEVICE9 pDevice = CManager::GetRenderer()->GetDevice();

	// 頂点情報の作成
	//---------------------
	if (FAILED(pDevice->CreateVertexBuffer(
		sizeof(VERTEX_3D) * NUM_VERTEX,		// メモリのサイズ
		D3DUSAGE_WRITEONLY,					// 使用方法(書き込みのみ)
		FVF_VERTEX_3D,						// 使用する頂点フォーマット(見ない)
		D3DPOOL_MANAGED,					// バッファの管理方法(勝手に復旧してくれる)
		&pScene3DPoly->m_pVtxBuffer,		// 
		nullptr)))
	{
		// 失敗したらエラーを返す
		MessageBox(nullptr, "3D用VtxBufferの作成に失敗しました", "警告", MB_OK);
		return nullptr;
	}

	// テクスチャのセット
	CManager::GetTexManager()->SetTex(strFilePass);

	return pScene3DPoly;
}