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
#include "scene3D.h"
#include "sceneCylinder.h"
#include "input.h"
#include "manager.h"
#include "texManager.h"
#include "renderer.h"

//================================================================================
// 初期化
//--------------------------------------------------------------------------------
void CSceneCylinder::Init()
{
	// 初期化
	SetPos(D3DXVECTOR3(0.f, 0.f, 0.f));
	SetRot(D3DXVECTOR3(0.f, 0.f, 0.f));
	SetScl(D3DXVECTOR3(1.f, 1.f, 1.f));
	SetSize(D3DXVECTOR3(1.f, 1.f, 1.f));
	m_move = D3DXVECTOR3(0.f, 0.f, 0.f);
	m_strFilePass.clear();
}

//--------------------------------------------------------------------------------
// 終了処理
//--------------------------------------------------------------------------------
void CSceneCylinder::Uninit()
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
void CSceneCylinder::Update()
{
	// スレッドセーフ(ロック)
#ifdef THREAD_ON
	std::unique_lock<std::recursive_mutex> locker = CManager::Locker();
#endif

	VERTEX_3D* pVtx;	// バッファをロックする
	m_pVtxBuffer->Lock(0, 0, (void**)&pVtx, 0);

	// 1ブロックの幅と高さ
	D3DXVECTOR3 size = GetSize();

	// 頂点の設定
	float x, y, z;

	float phi = (D3DX_PI * 2) / m_stacks;			// θの角度

	for (int cntSlices = 0; cntSlices <= m_slices; cntSlices++)
	{
		for (int cntStacks = 0; cntStacks <= m_stacks; cntStacks++)
		{
			// 座標
			x = cos(phi * cntStacks) * size.x,				// X
				y = (float)m_slices - cntSlices * size.y,	// Y
				z = sin(phi * cntStacks) * size.z;			// Z
			pVtx[0].pos = D3DXVECTOR3(x, y, z);
			// 法線
			pVtx[0].nom = D3DXVECTOR3(0.f, 1.f, 0.f);
			// 色
			pVtx[0].col = D3DXVECTOR4(1.f, 1.f, 1.f, 1.f);
			// テクスチャ座標
			pVtx[0].tex = D3DXVECTOR2((1.f / m_stacks) * cntStacks, (1.f / m_slices) * cntSlices);
			pVtx++;
		}
	}
	// ロックの解除
	m_pVtxBuffer->Unlock();


	WORD *pIdx;			// バッファをロックする
	m_pIdxBuffer->Lock(0, 0, (void**)&pIdx, 0);

	int nCnt = 0;
	for (int nCntY = 0; nCntY < m_slices; nCntY++)
	{
		if (nCntY != 0)	// 一番初めじゃなかったら
		{
			// 一点打つ
			pIdx[nCnt] = static_cast<WORD>((m_stacks + 1) * nCntY + (m_stacks + 1));
			nCnt++;
		}
		// 2点打つ(上下)
		pIdx[nCnt] = static_cast<WORD>((m_stacks + 1) * nCntY);
		nCnt++;
		pIdx[nCnt] = static_cast<WORD>((m_stacks + 1) * (nCntY + 1));
		nCnt++;

		for (int nCntX = 0; nCntX < m_stacks; nCntX++)
		{
			// 2点打つ(上下)
			pIdx[nCnt] = static_cast<WORD>((m_stacks + 1) * nCntY + (nCntX + 1));
			nCnt++;
			pIdx[nCnt] = static_cast<WORD>(((m_stacks + 1) * (nCntY + 1)) + (nCntX + 1));
			nCnt++;
		}
		if (nCntY != m_slices - 1)	// 最後じゃなかったら
		{
			// 一点打つ
			pIdx[nCnt] = static_cast<WORD>(m_stacks + (nCntY * (m_stacks + 1)));
			nCnt++;
		}
	}

	//ロックの解除
	m_pIdxBuffer->Unlock();
}

//================================================================================
// ポリゴン描画
//--------------------------------------------------------------------------------
void CSceneCylinder::Draw()
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

	pDevice->DrawIndexedPrimitive(D3DPT_TRIANGLESTRIP, 0, 0, NUM_POLYGON, 0, GetNumIndex() - 2);
}

//--------------------------------------------------------------------------------
// 頂点作成
//--------------------------------------------------------------------------------
HRESULT CSceneCylinder::SetVtx(int stacks, int slices)
{
	m_slices = slices;
	m_stacks = stacks;

	int numVtx = ((slices + 1) * (stacks + 1));						// 頂点数
	int numIndex = ((stacks + 1) * 2 * slices) + (slices - 1) * 2;	// インデックス数(三角形部 + 帯部(縮退ポリゴン))
	SetNumVtx(numVtx);
	SetNumIndex(numIndex);

	// スレッドセーフ(ロック)
#ifdef THREAD_ON
	std::unique_lock<std::recursive_mutex> locker = CManager::Locker();
#endif

	// D3Deviceの取得
	LPDIRECT3DDEVICE9 pDevice = CManager::GetRenderer()->GetDevice();

	// 頂点情報の作成
	//---------------------
	if (FAILED(pDevice->CreateVertexBuffer(
		sizeof(VERTEX_3D) * numVtx,			// メモリのサイズ
		D3DUSAGE_WRITEONLY,					// 使用方法(書き込みのみ)
		FVF_VERTEX_3D,						// 使用する頂点フォーマット(見ない)
		D3DPOOL_MANAGED,					// バッファの管理方法(勝手に復旧してくれる)
		&m_pVtxBuffer,							// 
		nullptr)))
	{
		// 失敗したらエラーを返す
		MessageBox(nullptr, "3D用VtxBufferの作成に失敗しました", "警告", MB_OK);
		return E_FAIL;
	}

	if (FAILED(pDevice->CreateIndexBuffer(
		sizeof(WORD) * numIndex,		// バッファ量
		D3DUSAGE_WRITEONLY,			// 使用フラグ
		D3DFMT_INDEX16,				// 必要なバッファ量
		D3DPOOL_MANAGED,			// メモリの管理方法
		&m_pIdxBuffer,
		nullptr)))
	{
		// 失敗したらエラーを返す
		MessageBox(nullptr, "IdxBufferの作成に失敗しました", "警告", MB_OK);
		return E_FAIL;
	}

	return NOERROR;
}

//--------------------------------------------------------------------------------
// 作成処理
//--------------------------------------------------------------------------------
CSceneCylinder *CSceneCylinder::Create(const D3DXVECTOR3 &pos, const D3DXVECTOR3 &rot, const D3DXVECTOR3 &size, const std::string &strFilePass)
{
	// CSceneCylinderの生成
	CSceneCylinder *pSceneCylinder;
	pSceneCylinder = new CSceneCylinder();
	pSceneCylinder->Init();

	pSceneCylinder->SetPos(pos);
	pSceneCylinder->SetRot(rot);
	pSceneCylinder->SetSize(size);
	pSceneCylinder->m_strFilePass = strFilePass;

	// 頂点設定処理
	pSceneCylinder->SetVtx(32, 1);
	
	// テクスチャのセット
	CManager::GetTexManager()->SetTex(strFilePass);

	return pSceneCylinder;
}