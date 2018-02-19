//================================================================================
//
//	Auter : KENSUKE WATANABE
//	Data  : 2018/02/10
//
//--------------------------------------------------------------------------------
#include "texManager.h"
#include "manager.h"
#include "modeGame.h"
#include "stencilShadow.h"
#include "input.h"
#include "renderer.h"

#include <assert.h>

//--------------------------------------------------------------------------------
// 初期化
//--------------------------------------------------------------------------------
void CStencilShadow::Init()
{
	CScene2D::Init();
	SetCol(1, 1, 1, 192);
}

//--------------------------------------------------------------------------------
// 終了
//--------------------------------------------------------------------------------
void CStencilShadow::Uninit()
{
	CScene2D::Uninit();
}

//--------------------------------------------------------------------------------
// 更新
//--------------------------------------------------------------------------------
void CStencilShadow::Update()
{
	CScene2D::Update();
}

//--------------------------------------------------------------------------------
// 描画
//--------------------------------------------------------------------------------
void CStencilShadow::Draw()
{
	// D3Deviceの取得
	LPDIRECT3DDEVICE9 pDevice = CRenderer::GetDevice();

	// 頂点バッファをデバイスのデータ ストリームにバインドする
	pDevice->SetStreamSource(0, m_pVtxBuffer, 0, sizeof(VERTEX_2D));

	// 頂点フォーマットの設定
	pDevice->SetFVF(FVF_VERTEX_2D);

	// テクスチャの設定
	pDevice->SetTexture(0, CManager::GetTexManager()->GetTexInterface(m_strFilePass));

	pDevice->SetRenderState(D3DRS_COLORWRITEENABLE, 0xf);
	pDevice->SetRenderState(D3DRS_CULLMODE, D3DCULL_CCW);
	pDevice->SetRenderState(D3DRS_STENCILFUNC, D3DCMP_EQUAL);
	pDevice->SetRenderState(D3DRS_STENCILENABLE, TRUE);
	pDevice->SetRenderState(D3DRS_STENCILREF, 1);
	pDevice->SetRenderState(D3DRS_ZWRITEENABLE, FALSE);

	// ポリゴンの描画 // ポリゴンの形, ロードする最初の頂点のインデックス, ポリゴン数
	pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, 0, NUM_POLYGON);

	pDevice->SetRenderState(D3DRS_ZWRITEENABLE, TRUE);
	pDevice->SetRenderState(D3DRS_STENCILENABLE, FALSE);
}

//--------------------------------------------------------------------------------
// 作成処理
//--------------------------------------------------------------------------------
CStencilShadow *CStencilShadow::Create(const D3DXVECTOR3 &pos, const D3DXVECTOR3 &rot, const D3DXVECTOR3 &size, const std::string &strFilePass, int priority)
{
	// CStencilShadowの生成
	CStencilShadow *pShadow2D;
	pShadow2D = new CStencilShadow(priority);
	pShadow2D->Init();

	pShadow2D->SetPos(pos);
	pShadow2D->SetRot(rot);
	pShadow2D->SetSize(size);
	pShadow2D->m_strFilePass = strFilePass;
	pShadow2D->m_pVtxBuffer = nullptr;

	// スレッドセーフ(ロック)
#ifdef THREAD_ON
	std::unique_lock<std::recursive_mutex> locker = CManager::Locker();
#endif

	// D3Deviceの取得
	LPDIRECT3DDEVICE9 pDevice = CManager::GetRenderer()->GetDevice();

	// 頂点情報の作成
	//---------------------
	if (FAILED(pDevice->CreateVertexBuffer(
		sizeof(VERTEX_2D) * NUM_VERTEX,			// メモリのサイズ
		D3DUSAGE_WRITEONLY,					// 使用方法(書き込みのみ)
		FVF_VERTEX_2D,						// 使用する頂点フォーマット(見ない)
		D3DPOOL_MANAGED,					// バッファの管理方法(勝手に復旧してくれる)
		&pShadow2D->m_pVtxBuffer,							// 
		nullptr)))
	{
		// 失敗したらエラーを返す
		MessageBox(nullptr, "2D用VtxBufferの作成に失敗しました", "警告", MB_OK);
		return nullptr;
	}

	// テクスチャのセット
	CManager::GetTexManager()->SetTex(strFilePass);

	return pShadow2D;
}
