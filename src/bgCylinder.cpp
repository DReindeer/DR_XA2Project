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
#include "manager.h"
#include "utility.h"
#include "sceneCylinder.h"
#include "effect3DPoly.h"
#include "bgCylinder.h"
#include "input.h"
#include "modeGame.h"
#include "texManager.h"
#include "renderer.h"
#include <random>

//================================================================================
// 初期化
//--------------------------------------------------------------------------------
void CBgCylinder::Init()
{
	// 初期化
	SetPos(D3DXVECTOR3(0.f, 0.f, 0.f));
	SetRot(D3DXVECTOR3(0.f, 0.f, 0.f));
	SetScl(D3DXVECTOR3(1.f, 1.f, 1.f));
	SetSize(D3DXVECTOR3(1.f, 1.f, 1.f));
	SetCol(0, 0, 0, 255);
	m_moveUV = D3DXVECTOR2(0.f, 0.f);
	m_strFilePass.clear();
}

//--------------------------------------------------------------------------------
// 終了処理
//--------------------------------------------------------------------------------
void CBgCylinder::Uninit()
{
	// 親クラスの終了
	CSceneCylinder::Uninit();
}

//================================================================================
// 更新処理
//--------------------------------------------------------------------------------
void CBgCylinder::Update()
{
	// スレッドセーフ(ロック)
#ifdef THREAD_ON
	std::unique_lock<std::recursive_mutex> locker = CManager::Locker();
#endif

	int stacks = GetStacks();
	int slices = GetSlices();
	float r = 255.f / GetR();
	float g = 255.f / GetG();
	float b = 255.f / GetB();

	VERTEX_3D* pVtx;	// バッファをロックする
	m_pVtxBuffer->Lock(0, 0, (void**)&pVtx, 0);
	for (int cntSlices = 0; cntSlices <= slices; cntSlices++)
	{
		for (int cntStacks = 0; cntStacks <= stacks; cntStacks++)
		{
			// 色
			pVtx[0].col = D3DXVECTOR4(r, g, b, 0.f);
			pVtx++;
		}
	}
	// ロックの解除
	m_pVtxBuffer->Unlock();
}

//================================================================================
// ポリゴン描画
//--------------------------------------------------------------------------------
void CBgCylinder::Draw()
{
	// デバイスの取得
	LPDIRECT3DDEVICE9 pDevice = CRenderer::GetDevice();

	pDevice->SetRenderState(D3DRS_LIGHTING, FALSE);						// ライト表示オフ
	pDevice->SetSamplerState(0, D3DSAMP_ADDRESSU, D3DTADDRESS_CLAMP);	// テクスチャＵ値の繰り返し設定
	pDevice->SetSamplerState(0, D3DSAMP_ADDRESSV, D3DTADDRESS_CLAMP);	// テクスチャＶ値の繰り返し設定

																		// 親の描画
	CSceneCylinder::Draw();

	// レンダーステートの設定を戻す
	pDevice->SetSamplerState(0, D3DSAMP_ADDRESSU, D3DTADDRESS_WRAP);	// テクスチャＵ値の繰り返し設定
	pDevice->SetSamplerState(0, D3DSAMP_ADDRESSV, D3DTADDRESS_WRAP);	// テクスチャＶ値の繰り返し設定
	pDevice->SetRenderState(D3DRS_LIGHTING, TRUE);						// ライトの表示を有効にする
}

//--------------------------------------------------------------------------------
// 頂点作成
//--------------------------------------------------------------------------------
HRESULT CBgCylinder::SetVtx(int stacks, int slices)
{
	float phi = (D3DX_PI * 2) / stacks;			// θの角度

	int numVtx = (slices + 1) * (stacks + 1);							// 頂点数
	int numIndex = ((stacks + 1) * 2 * slices) + (slices - 1) * 2;		// インデックス数(三角形部 + 帯部(縮退ポリゴン))
	SetStacks(stacks);
	SetSlices(slices);
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

	VERTEX_3D* pVtx;	// バッファをロックする
	m_pVtxBuffer->Lock(0, 0, (void**)&pVtx, 0);

	// 1ブロックの幅と高さ
	D3DXVECTOR3 size = GetSize();

	// 頂点の設定
	float x, y, z;

	for (int cntSlices = 0; cntSlices <= slices; cntSlices++)
	{
		for (int cntStacks = 0; cntStacks <= stacks; cntStacks++)
		{
			// 座標
			x = cos(phi * cntStacks) * size.x,			// X
			y = cntSlices * size.y,						// Y
			z = sin(phi * cntStacks) * size.z;			// Z
			pVtx[0].pos = D3DXVECTOR3(x, y, z);
			// 法線
			pVtx[0].nom = D3DXVECTOR3(0.f, 1.f, 0.f);
			// 色
			pVtx[0].col = D3DXVECTOR4(1.f, 1.f, 1.f, 0.f);
			// テクスチャ座標
			pVtx[0].tex = D3DXVECTOR2((1.f / stacks) * cntStacks, 1.f - ((1.f / slices) * cntSlices));
			pVtx++;
		}
	}
	// ロックの解除
	m_pVtxBuffer->Unlock();

	// インデックスバッファ
	WORD *pIdx;			// バッファをロックする
	m_pIdxBuffer->Lock(0, 0, (void**)&pIdx, 0);

	int nCnt = 0;
	for (int nCntY = 0; nCntY < slices; nCntY++)
	{
		if (nCntY != 0)	// 一番初めでなかったら
		{
			// 一点打つ
			pIdx[nCnt] = static_cast<WORD>((stacks + 1) * nCntY + (stacks));
			nCnt++;
		}
		// 2点打つ(上下)
		pIdx[nCnt] = static_cast<WORD>((stacks + 1) * (nCntY + 1));
		nCnt++;
		pIdx[nCnt] = static_cast<WORD>((stacks + 1) * nCntY);
		nCnt++;

		for (int nCntX = 0; nCntX < stacks; nCntX++)
		{
			// 2点打つ(上下)
			pIdx[nCnt] = static_cast<WORD>(((stacks + 1) * (nCntY + 1)) + (nCntX + 1));
			nCnt++;
			pIdx[nCnt] = static_cast<WORD>((stacks + 1) * nCntY + (nCntX + 1));
			nCnt++;
		}
		if (nCntY != slices - 1)	// 最後でなかったら
		{
			// 一点打つ
			pIdx[nCnt] = static_cast<WORD>(stacks + (nCntY * (stacks)));
			nCnt++;
		}
	}

	//ロックの解除
	m_pIdxBuffer->Unlock();

	return NOERROR;
}

//--------------------------------------------------------------------------------
// 作成処理
//--------------------------------------------------------------------------------
CBgCylinder *CBgCylinder::Create(const D3DXVECTOR3 &pos, const D3DXVECTOR3 &rot, const D3DXVECTOR3 &size, const std::string &strFilePass)
{
	// CBgCylinderの生成
	CBgCylinder *pSceneCylinder;
	pSceneCylinder = new CBgCylinder(1);
	pSceneCylinder->Init();

	pSceneCylinder->SetPos(pos);
	pSceneCylinder->SetRot(rot);
	pSceneCylinder->SetSize(size);
	pSceneCylinder->m_strFilePass = strFilePass;
	pSceneCylinder->SetCol(255, 255, 255, 255);

	// 頂点設定処理
	pSceneCylinder->SetVtx(32, 1);

	// テクスチャのセット
	CManager::GetTexManager()->SetTex(strFilePass);

	return pSceneCylinder;
}