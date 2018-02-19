//--------------------------------------------------------------------------------
//
//	Auter : KENSUKE WATANABE
//	Data  : 2016/08/30
//
//--------------------------------------------------------------------------------

// インクルード
//--------------------------------------------------------------------------------
#include <math.h>
#include "main.h"
#include "manager.h"
#include "effect2DPoly.h"
#include "modeGame.h"
#include "renderer.h"
#include "texManager.h"

int CEffect2DPoly::m_maxEffect = 0;
int CEffect2DPoly::m_cnt = 0;
std::vector<CEffect2DPoly*> CEffect2DPoly::m_pvecEffect2DPoly;

//--------------------------------------------------------------------------------
// 初期化処理関数
//--------------------------------------------------------------------------------
void CEffect2DPoly::Init()
{
	m_bUse = false;
	m_fRadius = 0.0f;					// 半径
	m_fRadiusValue = 0.0f;				// 半径変化量
	m_colorAlphaValue = 0;				// α値の変化量
	m_nLife = 0;						// 寿命
	m_move = D3DXVECTOR2(0.0f, 0.0f);
}

//--------------------------------------------------------------------------------
// 終了処理
//--------------------------------------------------------------------------------
void CEffect2DPoly::Uninit()
{

}

//--------------------------------------------------------------------------------
// 更新処理関数
//--------------------------------------------------------------------------------
void CEffect2DPoly::Update()
{
	if (m_bUse)
	{
		// スレッドセーフ(ロック)
#ifdef THREAD_ON
		std::unique_lock<std::recursive_mutex> locker = CManager::Locker();
#endif

		VERTEX_2D* pVtx;	//頂点情報書き込み先のポインタ変数
		//バッファをロックする
		m_pVtxBuffer->Lock(0, 0, (void**)&pVtx, 0);

		//使用中のエフェクトの更新
		m_nLife--;	//寿命を減らす
		if (m_nLife <= 0)
		{//寿命が0以下になったら
			m_bUse = false;	//使用フラグオフ
		}
		else
		{
			//色の設定
			m_a -= m_colorAlphaValue;
			if (m_a < 0)
			{//α値が0.0より小さくならないように
				m_a = 0;
			}
		}
		//半径の更新と設定
		m_fRadius -= m_fRadiusValue;
		if (m_fRadius < 0.0f)
		{//半径が0.0より小さくならないように
			m_fRadius = 0.0f;
		}

		// 座標の加算
		m_pos += D3DXVECTOR3(m_move.x, m_move.y, 0.f);

		//頂点ポリゴンのポリゴン座標を座標と半径から算出して書き込む
		SetVertex(pVtx);

		//頂点バッファアンロック
		m_pVtxBuffer->Unlock();
	}
}

//--------------------------------------------------------------------------------
// 描画処理関数
//--------------------------------------------------------------------------------
void CEffect2DPoly::Draw()
{
	// デバイスの取得
	LPDIRECT3DDEVICE9 pDevice = CRenderer::GetDevice();

	pDevice->SetRenderState(D3DRS_LIGHTING, FALSE);				// ライト表示オフ
	pDevice->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_ONE);		// DESTのRGBの係数を１にする
	pDevice->SetRenderState(D3DRS_ZENABLE, D3DZB_FALSE);		// Zバッファ

	//使われているパーティクルの描画
	if (m_bUse)
	{
		// 頂点バッファをデバイスのデータ ストリームにバインドする
		pDevice->SetStreamSource(0, m_pVtxBuffer, 0, sizeof(VERTEX_2D));

		// 頂点フォーマットの設定
		pDevice->SetFVF(FVF_VERTEX_2D);

		// テクスチャの設定
		pDevice->SetTexture(0, CManager::GetTexManager()->GetTexInterface(m_strFilePass));

		// ポリゴンの描画 // ポリゴンの形, ロードする最初の頂点のインデックス, ポリゴン数
		pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, 0, NUM_POLYGON);
	}

	pDevice->SetRenderState(D3DRS_ZENABLE, D3DZB_TRUE);			// Zバッファ
	// ライトの表示を有効にする
	pDevice->SetRenderState(D3DRS_LIGHTING, TRUE);

	//レンダーステートの設定を戻す
	pDevice->SetRenderState(D3DRS_BLENDOP, D3DBLENDOP_ADD);
	pDevice->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
	pDevice->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA);
}

//--------------------------------------------------------------------------------
// 作成
//--------------------------------------------------------------------------------
CEffect2DPoly *CEffect2DPoly::Create(int n, std::string texName)
{
	// テクスチャのセット
	CManager::GetTexManager()->SetTex(texName);

	// エフェクト
	CEffect2DPoly *pEffPoly = nullptr;
	m_pvecEffect2DPoly.clear();

	for (int nCnt = 0; nCnt < n; nCnt++)
	{
		pEffPoly = new CEffect2DPoly(5);
		pEffPoly->Init();
		pEffPoly->m_strFilePass = texName;
		pEffPoly->SetAlpha();
		pEffPoly->m_pvecEffect2DPoly.push_back(pEffPoly);
		pEffPoly->CEffect2DPoly::MakeVertex();
	}
	// キャパシティ設定
	pEffPoly->m_maxEffect = n;
	pEffPoly->m_pvecEffect2DPoly.shrink_to_fit();

	return pEffPoly;
}

//--------------------------------------------------------------------------------
// エフェクト情報設定
//--------------------------------------------------------------------------------
void CEffect2DPoly::SetEffect2DPoly(D3DXVECTOR2 pos, float rot, D3DXVECTOR2 move, float radius, int &r, int &g, int &b, int &a, int life, std::string texName)
{
	// テクスチャのセット
	CManager::GetTexManager()->SetTex(texName);

	for (int i = 0; i < m_maxEffect; i++)
	{
		auto it = m_pvecEffect2DPoly.at(i);
		if (it->m_bUse == false)
		{
			// エフェクトセット
			it->m_strFilePass = texName;
			it->m_pos = D3DXVECTOR3(pos.x, pos.y, 0.f);
			it->m_move = move;
			it->m_rot = D3DXVECTOR3(0.f, 0.f, rot);
			it->m_fRadius = radius;
			it->SetCol(r, g, b, a);
			it->m_nLife = life;

			it->m_bUse = true;
			it->m_fRadiusValue = radius * 0.5f / life;		//寿命が尽きるまでに大きさを半分になるように値を設定
			it->m_colorAlphaValue = 255 / life;				//寿命が尽きるまでに透明になるように値を設定
			break;
		}
	}
}

//--------------------------------------------------------------------------------
// 頂点作成
//--------------------------------------------------------------------------------
HRESULT CEffect2DPoly::MakeVertex()
{
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
		&m_pVtxBuffer,							// 
		nullptr)))
	{
		// 失敗したらエラーを返す
		MessageBox(nullptr, "2D用VtxBufferの作成に失敗しました", "警告", MB_OK);
		return E_FAIL;
	}
	return S_OK;
}

//--------------------------------------------------------------------------------
// 頂点座標設定
//--------------------------------------------------------------------------------
void CEffect2DPoly::SetVertex(VERTEX_2D* pVtx)
{
	//頂点座標の設定
	pVtx[0].pos = D3DXVECTOR3(-m_fRadius + m_pos.x, -m_fRadius + m_pos.y, 0.0f);
	pVtx[1].pos = D3DXVECTOR3(m_fRadius + m_pos.x, -m_fRadius + m_pos.y, 0.0f);
	pVtx[2].pos = D3DXVECTOR3(-m_fRadius + m_pos.x, m_fRadius + m_pos.y, 0.0f);
	pVtx[3].pos = D3DXVECTOR3(m_fRadius + m_pos.x, m_fRadius + m_pos.y, 0.0f);

	//頂点色の設定
	int r = m_r;
	int g = m_g;
	int b = m_b;
	int a = m_a;

	pVtx[0].col = D3DCOLOR_RGBA(r, g, b, a);
	pVtx[1].col = D3DCOLOR_RGBA(r, g, b, a);
	pVtx[2].col = D3DCOLOR_RGBA(r, g, b, a);
	pVtx[3].col = D3DCOLOR_RGBA(r, g, b, a);

	//座標と色設定
	pVtx[0].tex = D3DXVECTOR2(0.0f, 0.0f);
	pVtx[1].tex = D3DXVECTOR2(1.0f, 0.0f);
	pVtx[2].tex = D3DXVECTOR2(0.0f, 1.0f);
	pVtx[3].tex = D3DXVECTOR2(1.0f, 1.0f);

	// rhw
	for (int i = 0; i < 4; i++)
	{
		pVtx[i].rhw = 1.0f;
	}
}
