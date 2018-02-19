//================================================================================
//
//	Auter : KENSUKE WATANABE
//	Data  : 2016/08/30
//
//--------------------------------------------------------------------------------
//--------------------------------------------------------------------------------
// インクルード
//--------------------------------------------------------------------------------
#include <math.h>
#include "main.h"
#include "manager.h"
#include "effect3DPoly.h"
#include "modeGame.h"
#include "renderer.h"
#include "texManager.h"
#include "camera.h"
#include "deferredRendering.h"

int CEffect3DPoly::m_maxEffect = 0;
int CEffect3DPoly::m_cnt = 0;
std::vector<CEffect3DPoly*> CEffect3DPoly::m_pvecEffect3DPoly;

//--------------------------------------------------------------------------------
// 初期化処理関数
//--------------------------------------------------------------------------------
void CEffect3DPoly::Init()
{
	m_bUse = false;
	m_fRadius = 0.f;							// 半径
	m_fRadiusValue = 0.f;						// 半径変化量
	m_fColorAlphaValue = 0.f;					// α値の変化量
	m_nLife = 0;								// 寿命
	m_move = D3DXVECTOR3(0.f, 0.f, 0.f);		// 移動量
}

//--------------------------------------------------------------------------------
// 終了処理
//--------------------------------------------------------------------------------
void CEffect3DPoly::Uninit()
{

}

//--------------------------------------------------------------------------------
// 更新処理関数
//--------------------------------------------------------------------------------
void CEffect3DPoly::Update()
{
	if (m_bUse)
	{
		// スレッドセーフ(ロック)
#ifdef THREAD_ON
		std::unique_lock<std::recursive_mutex> locker = CManager::Locker();
#endif

		//使用中のエフェクトの更新
		m_nLife--;	//寿命を減らす
		if (m_nLife <= 0)
		{//寿命が0以下になったら
			m_bUse = false;	//使用フラグオフ
		}
		else
		{
			//色の設定
			float a = GetA();
		//	if (a > 0.f) AddCol(0, 0, 0, -m_fColorAlphaValue);
		}
		//半径の更新と設定
		m_fRadius -= m_fRadiusValue;
		if (m_fRadius < 0.f)
		{//半径が0.0より小さくならないように
			m_fRadius = 0.f;
		}

		m_pos += m_move;
	}
}

/*
//--------------------------------------------------------------------------------
// 描画処理関数
//--------------------------------------------------------------------------------
void CEffect3DPoly::Draw()
{
	//使われているパーティクルの描画
	if (m_bUse == false)return;

	// デバイスの取得
	LPDIRECT3DDEVICE9 pDevice = CRenderer::GetDevice();

	pDevice->SetRenderState(D3DRS_LIGHTING, FALSE);				// ライト表示オフ

	// アルファブレンド
	pDevice->SetRenderState(D3DRS_ALPHABLENDENABLE, TRUE);
	pDevice->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
	pDevice->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA);

	// アルファテストの有効化
	pDevice->SetRenderState(D3DRS_ALPHATESTENABLE, TRUE);
	pDevice->SetRenderState(D3DRS_ALPHAFUNC, D3DCMP_GREATEREQUAL);
	// 不透明にする値の設定
	pDevice->SetRenderState(D3DRS_ALPHAREF, (DWORD)0x00000001);

	D3DXMATRIX mtxScl;			// 拡大行列
	D3DXMATRIX mtxRot;			// 回転行列
	D3DXMATRIX mtxViewInverse;
	D3DXMATRIX mtxPos;			// 平行行列

	// ワールドマトリクスの作成
	D3DXMatrixIdentity(&mtxScl);			//
	D3DXMatrixIdentity(&mtxRot);			//
	D3DXMatrixIdentity(&mtxPos);			//

	D3DXMATRIX mtxView = CManager::GetCamera()->GetView();		// ビュー行列の取得
	mtxView._41 = 0.f;						//
	mtxView._42 = 0.f;						//
	mtxView._43 = 0.f;						// 平行行列のカット

	D3DXMatrixInverse(&mtxViewInverse, nullptr, &mtxView);	// 逆行列の出力

	D3DXMatrixIdentity(&m_mtxWorld);		// 行列を単位行列にする( 拡大率1,他0で初期化 )

	// ワールド変換行列を設定して変換
	D3DXVECTOR3 scl = GetScl();
	D3DXVECTOR3 rot = m_rot;
	D3DXVECTOR3 pos = m_pos;
	D3DXMatrixScaling(&mtxScl, scl.x, scl.y, scl.z);					// 拡大行列
	D3DXMatrixRotationYawPitchRoll(&mtxRot, rot.y, rot.x, rot.z);		// 回転行列
	D3DXMatrixTranslation(&mtxPos, pos.x, pos.y, pos.z);				// 平行行列

	D3DXMatrixMultiply(&m_mtxWorld, &m_mtxWorld, &mtxViewInverse);
	D3DXMatrixMultiply(&m_mtxWorld, &m_mtxWorld, &mtxScl);
	D3DXMatrixMultiply(&m_mtxWorld, &m_mtxWorld, &mtxRot);
	D3DXMatrixMultiply(&m_mtxWorld, &m_mtxWorld, &mtxPos);

	// デバイスにワールド変換行列を設定
	pDevice->SetTransform(D3DTS_WORLD, &m_mtxWorld);

	// 頂点設定
	SetVertex();

	// 頂点フォーマットの設定
	pDevice->SetFVF(FVF_VERTEX_3D);

	// ストリームの設定
	pDevice->SetStreamSource(0, m_pVtxBuffer, 0, sizeof(VERTEX_3D));

	// テクスチャの設定
	pDevice->SetTexture(0, CManager::GetTexManager()->GetTexInterface(m_strFilePass));

	pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, 0, 2);	//プリミティブ数

	// アルファテストを無効化
	pDevice->SetRenderState(D3DRS_ALPHATESTENABLE, FALSE);

	// ライトの表示を有効にする
	pDevice->SetRenderState(D3DRS_LIGHTING, TRUE);
}
*/

//--------------------------------------------------------------------------------
// デファードレンダリング用
//--------------------------------------------------------------------------------
void CEffect3DPoly::DrawDefferdRendering()
{
	//使われているパーティクルの描画
	if (!m_bUse)return;

	// デバイスの取得
	LPDIRECT3DDEVICE9 pDevice = CRenderer::GetDevice();

	D3DXMATRIX mtxScl;			// 拡大行列
	D3DXMATRIX mtxRot;			// 回転行列
	D3DXMATRIX mtxViewInverse;	// ビルボード用
	D3DXMATRIX mtxPos;			// 平行行列

	// ワールドマトリクスの作成
	D3DXMatrixIdentity(&mtxScl);			// 
	D3DXMatrixIdentity(&mtxRot);			// 
	D3DXMatrixIdentity(&mtxPos);			//

	D3DXMATRIX mtxView = CManager::GetCamera()->GetView();		// ビュー行列の取得
	mtxView._41 = 0.f;						// 
	mtxView._42 = 0.f;						// 
	mtxView._43 = 0.f;						// 平行行列のカット

	D3DXMatrixInverse(&mtxViewInverse, nullptr, &mtxView);		// 逆行列の出力

	D3DXMatrixIdentity(&m_mtxWorld);		// 行列を単位行列にする( 拡大率1,他0で初期化 )

	// ワールド変換行列を設定して変換
	D3DXVECTOR3 scl = GetScl();
	D3DXVECTOR3 rot = m_rot;
	D3DXVECTOR3 pos = m_pos;
	D3DXMatrixScaling(&mtxScl, scl.x, scl.y, scl.z);					// 拡大行列
	D3DXMatrixRotationYawPitchRoll(&mtxRot, rot.y, rot.x, rot.z);		// 回転行列
	D3DXMatrixTranslation(&mtxPos, pos.x, pos.y, pos.z);				// 平行行列

	D3DXMatrixMultiply(&m_mtxWorld, &m_mtxWorld, &mtxViewInverse);
	D3DXMatrixMultiply(&m_mtxWorld, &m_mtxWorld, &mtxScl);
	D3DXMatrixMultiply(&m_mtxWorld, &m_mtxWorld, &mtxRot);
	D3DXMatrixMultiply(&m_mtxWorld, &m_mtxWorld, &mtxPos);

	// デバイスにワールド変換行列を設定
	pDevice->SetTransform(D3DTS_WORLD, &m_mtxWorld);

	// 頂点設定
	SetVertex();

	// デファードレンダリングの取得
	DefferdRendering *pDefferdRendering = CManager::GetDefferdRendering();

	// アルファテストの有効化
	pDevice->SetRenderState(D3DRS_ALPHATESTENABLE, TRUE);
	pDevice->SetRenderState(D3DRS_ALPHAFUNC, D3DCMP_GREATEREQUAL);
	// 不透明にする値の設定
	pDevice->SetRenderState(D3DRS_ALPHAREF, (DWORD)0x00000001);

	// 頂点シェーダーの設定
	//------------------------------------------------------------------------
	pDevice->SetVertexDeclaration(m_pShaderSet->pVertexDeclaration);
	pDefferdRendering->SetVXShader(m_mtxWorld);

	// ピクセルシェーダーの設定
	//------------------------------------------------------------------------
	pDevice->SetTexture(0, CManager::GetTexManager()->GetTexInterface(m_strFilePass));
	pDefferdRendering->SetPXShader(m_pMat);

	// ストリームの設定
	pDevice->SetStreamSource(0, m_pVtxBuffer, 0, sizeof(VERTEX_3D));
	// 描画
	pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, 0, 2);	//プリミティブ数

	pDevice->SetRenderState(D3DRS_ALPHATESTENABLE, FALSE);

	pDevice->SetPixelShader(nullptr);
	pDevice->SetVertexShader(nullptr);
}

//--------------------------------------------------------------------------------
// 作成
//--------------------------------------------------------------------------------
CEffect3DPoly *CEffect3DPoly::Create(int n, std::string texName)
{
	// テクスチャのセット
	CManager::GetTexManager()->SetTex(texName);

	// エフェクト
	CEffect3DPoly *pEffPoly = nullptr;
	m_pvecEffect3DPoly.clear();

	for (int nCnt = 0; nCnt < n; nCnt++)
	{
		pEffPoly = nullptr;
		pEffPoly = new CEffect3DPoly(6);
		pEffPoly->Init();
		pEffPoly->m_strFilePass = texName;
		pEffPoly->SetAlpha();
		pEffPoly->SetOverall();
		pEffPoly->m_pvecEffect3DPoly.push_back(pEffPoly);
		pEffPoly->CEffect3DPoly::MakeVertex();

		// マテリアル生成
		pEffPoly->m_pMat = new D3DMATERIAL9;
	}
	// キャパシティ設定
	pEffPoly->m_maxEffect = n;
	pEffPoly->m_pvecEffect3DPoly.shrink_to_fit();

	return pEffPoly;
}

//--------------------------------------------------------------------------------
// エフェクト情報設定
//--------------------------------------------------------------------------------
void CEffect3DPoly::SetEffect3DPoly(D3DXVECTOR3 pos, D3DXVECTOR3 rot, D3DXVECTOR3 move, float radius, float r, float g, float b, float a, float life, std::string texName)
{
	// テクスチャのセット
	CManager::GetTexManager()->SetTex(texName);

	for (auto it : m_pvecEffect3DPoly)
	{
		if (it->m_bUse == false)
		{
			// エフェクトセット
			it->m_strFilePass = texName;
			it->m_pos = pos;
			it->m_move = move *= CManager::GetMasterScale();	// スケール感を一律に
			it->m_rot = rot;
			it->m_fRadius = radius;
			it->SetCol(r, g, b, a);
			it->m_nLife = life;

			it->m_bUse = true;
			it->m_fRadiusValue = radius * 0.75f / life * CManager::GetMasterScale();	// 寿命が尽きるまでに大きさを半分になるように値を設定
			it->m_fColorAlphaValue = 1.f / life;			// 寿命が尽きるまでに透明になるように値を設定
			break;
		}
	}
}

//--------------------------------------------------------------------------------
// 頂点作成関数
//--------------------------------------------------------------------------------
HRESULT CEffect3DPoly::MakeVertex()
{
	// スレッドセーフ(ロック)
#ifdef THREAD_ON
	std::unique_lock<std::recursive_mutex> locker = CManager::Locker();
#endif

	// D3Deviceの取得
	LPDIRECT3DDEVICE9 pDevice = CManager::GetRenderer()->GetDevice();

	// 頂点情報の作成
	//---------------------
	HRESULT hr = pDevice->CreateVertexBuffer(
		sizeof(VERTEX_3D) * NUM_VERTEX,		// メモリのサイズ
		D3DUSAGE_WRITEONLY,					// 使用方法(書き込みのみ)
		FVF_VERTEX_3D,						// 使用する頂点フォーマット(見ない)
		D3DPOOL_MANAGED,					// バッファの管理方法(勝手に復旧してくれる)
		&m_pVtxBuffer,						// 
		nullptr);
	if (FAILED(hr))
	{
		// 失敗したらエラーを返す
		MessageBox(nullptr, "3D用VtxBufferの作成に失敗しました", "警告", MB_OK);
		return hr;
	}

	return hr;
}

//--------------------------------------------------------------------------------
// 頂点座標設定
//--------------------------------------------------------------------------------
void CEffect3DPoly::SetVertex()
{
	//バッファをロックする
	VERTEX_3D* pVtx;
	m_pVtxBuffer->Lock(0, 0, (void**)&pVtx, 0);

	//頂点座標の設定
	pVtx[0].pos = D3DXVECTOR3(-m_fRadius, m_fRadius, 0.f);
	pVtx[1].pos = D3DXVECTOR3(m_fRadius, m_fRadius, 0.f);
	pVtx[2].pos = D3DXVECTOR3(-m_fRadius, -m_fRadius, 0.f);
	pVtx[3].pos = D3DXVECTOR3(m_fRadius, -m_fRadius, 0.f);

	// 法線
	for (int i = 0; i < 4; i++)
	{
		pVtx[i].nom = D3DXVECTOR3(0.0f, 1.f, 0.0f);
	}

	//頂点色の設定
	float r = GetR();
	float g = GetG();
	float b = GetB();
	float a = GetA();
	pVtx[0].col = D3DXVECTOR4(1.f, 1.f, 1.f, 1.f);
	pVtx[1].col = D3DXVECTOR4(1.f, 1.f, 1.f, 1.f);
	pVtx[2].col = D3DXVECTOR4(1.f, 1.f, 1.f, 1.f);
	pVtx[3].col = D3DXVECTOR4(1.f, 1.f, 1.f, 1.f);

	// ディフューズ色
	m_pMat->Diffuse.a = 1.f;
	m_pMat->Diffuse.r = r;
	m_pMat->Diffuse.g = g;
	m_pMat->Diffuse.b = b;
	m_pMat->Power = 1.f;

	//座標と色設定
	pVtx[0].tex = D3DXVECTOR2(0.0f, 0.0f);
	pVtx[1].tex = D3DXVECTOR2(1.0f, 0.0f);
	pVtx[2].tex = D3DXVECTOR2(0.0f, 1.0f);
	pVtx[3].tex = D3DXVECTOR2(1.0f, 1.0f);

	//頂点バッファアンロック
	m_pVtxBuffer->Unlock();
}

void CEffect3DPoly::FlashAll()
{
	for (auto it : m_pvecEffect3DPoly)
	{
		if (it == nullptr)continue;
		it->m_bUse = false;
	}
}
