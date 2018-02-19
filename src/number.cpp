//================================================================================
//
//	Auter : KENSUKE WATANABE
//	Data  : 2018/02/10
//
//--------------------------------------------------------------------------------
#include "main.h"
#include "manager.h"
#include "texManager.h"
#include "renderer.h"
#include "scene2D.h"
#include "input.h"
#include "number.h"

//--------------------------------------------------------------------------------
// 定数定義
//--------------------------------------------------------------------------------
#define TEXNAME_NUMBER "data/TEXTURE/number000.png"	// ナンバーのテクスチャ

//--------------------------------------------------------------------------------
// CNumber::コンストラクタ
//--------------------------------------------------------------------------------
CNumber::CNumber()
{
	m_pVtxBuffer = nullptr;
}

//--------------------------------------------------------------------------------
// CEnemy::デストラクタ
//--------------------------------------------------------------------------------
CNumber::~CNumber()
{

}

//--------------------------------------------------------------------------------
// CNumber::初期化処理
//--------------------------------------------------------------------------------
HRESULT CNumber::Init(D3DXVECTOR3 pos, float rot, D3DXVECTOR3 size)
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

	m_pos = pos;
	m_size = size;
	m_rot = rot;

	// スレッドセーフ(ロック)
#ifdef THREAD_ON
	std::unique_lock<std::recursive_mutex> locker = CManager::Locker();
#endif

	// 頂点情報を設定
	VERTEX_2D* pVtx;	//頂点情報書き込み先のポインタ変数
	// バッファをロックする
	m_pVtxBuffer->Lock(0, 0, (void**)&pVtx, 0);
	
	// 頂点座標
	pVtx[0].pos = D3DXVECTOR3(m_pos.x, m_pos.y, 0.0f);
	pVtx[1].pos = D3DXVECTOR3(m_pos.x + m_size.x, m_pos.y, 0.0f);
	pVtx[2].pos = D3DXVECTOR3(m_pos.x, m_pos.y + m_size.y, 0.0f);
	pVtx[3].pos = D3DXVECTOR3(m_pos.x + m_size.x, m_pos.y + m_size.y, 0.0f);

	// UV
	pVtx[0].tex = D3DXVECTOR2(0.0f, 0.0f);
	pVtx[1].tex = D3DXVECTOR2(0.1f, 0.0f);
	pVtx[2].tex = D3DXVECTOR2(0.0f, 1.0f);
	pVtx[3].tex = D3DXVECTOR2(0.1f, 1.0f);

	// 色
	pVtx[0].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
	pVtx[1].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
	pVtx[2].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
	pVtx[3].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);

	// 描画優先度
	pVtx[0].rhw = 255;
	pVtx[1].rhw = 255;
	pVtx[2].rhw = 255;
	pVtx[3].rhw = 255;

	//ロックの解除
	m_pVtxBuffer->Unlock();
	return S_OK;
}

//--------------------------------------------------------------------------------
// CNumber::終了処理
//--------------------------------------------------------------------------------
void CNumber::Uninit()
{
	// 頂点バッファの破棄
	if(m_pVtxBuffer != nullptr)
	{
		m_pVtxBuffer->Release();
		m_pVtxBuffer = nullptr;
	}
}

//--------------------------------------------------------------------------------
// CNumber::更新処理
//--------------------------------------------------------------------------------
void CNumber::Update()
{
	// スレッドセーフ(ロック)
#ifdef THREAD_ON
	std::unique_lock<std::recursive_mutex> locker = CManager::Locker();
#endif

	// 頂点情報を設定
	VERTEX_2D* pVtx;	//頂点情報書き込み先のポインタ変数
	// バッファをロックする
	m_pVtxBuffer->Lock(0, 0, (void**)&pVtx, 0);

	//頂点色の設定
	pVtx[0].col = D3DCOLOR_RGBA(m_r, m_g, m_b, m_a);
	pVtx[1].col = D3DCOLOR_RGBA(m_r, m_g, m_b, m_a);
	pVtx[2].col = D3DCOLOR_RGBA(m_r, m_g, m_b, m_a);
	pVtx[3].col = D3DCOLOR_RGBA(m_r, m_g, m_b, m_a);

	//ロックの解除
	m_pVtxBuffer->Unlock();
}

//--------------------------------------------------------------------------------
// CNumber::描画処理
//--------------------------------------------------------------------------------
void CNumber::Draw()
{
	// デバイスの取得
	LPDIRECT3DDEVICE9 pDevice = CManager::GetRenderer()->GetDevice();

	// 頂点バッファをデータストリームに設定
	pDevice->SetStreamSource(0, m_pVtxBuffer, 0, sizeof(VERTEX_2D));

	// 頂点フォーマットの設定
	pDevice->SetFVF(FVF_VERTEX_2D);	//デバイスに頂点の設定を教える

	// テクスチャの設定
	pDevice->SetTexture(0, CManager::GetTexManager()->GetTexInterface(TEXNAME_NUMBER));

	// ポリゴンの描画
	pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, 0, NUM_POLYGON);
}

//--------------------------------------------------------------------------------
// CNumber::作成処理
//--------------------------------------------------------------------------------
CNumber *CNumber::Create(D3DXVECTOR3 pos, float rot, D3DXVECTOR3 size)
{
	// 生成
	CNumber *pNumber;
	pNumber = new CNumber;
	pNumber->Init(pos, rot, size);		// 初期化
	pNumber->SetNumber(0);
	pNumber->SetCol(255, 255, 255, 255);

	// テクスチャの設定
	CManager::GetTexManager()->SetTex(TEXNAME_NUMBER);

	return pNumber;
}

CNumber *CNumber::Create(D3DXVECTOR3 pos, D3DXVECTOR3 rot, D3DXVECTOR3 size, const int disit)
{
	// 生成
	CNumber *pNumber = new CNumber[disit];
	for (int num = 0; num < disit; num++)
	{
		pNumber[num].Init(D3DXVECTOR3(pos.x + num * size.x, pos.y, 0.0f), rot.z, size);		// 初期化
		pNumber[num].SetNumber(0);
		pNumber[num].SetCol(255, 255, 255, 255);
	}

	// テクスチャの設定
	CManager::GetTexManager()->SetTex(TEXNAME_NUMBER);

	return pNumber;
}

//--------------------------------------------------------------------------------
// CNumber::設定処理
//--------------------------------------------------------------------------------
void CNumber::SetNumber(int nNumber)
{
	// スレッドセーフ(ロック)
#ifdef THREAD_ON
	std::unique_lock<std::recursive_mutex> locker = CManager::Locker();
#endif

	VERTEX_2D* pVtx;	// バッファをロックする
	m_pVtxBuffer->Lock(0, 0, (void**)&pVtx, 0);

	// テクスチャのuv値の設定
	pVtx[0].tex = D3DXVECTOR2(0.1f * nNumber, 0.0f);
	pVtx[1].tex = D3DXVECTOR2(0.1f * nNumber + 0.1f, 0.0f);
	pVtx[2].tex = D3DXVECTOR2(0.1f * nNumber, 1.0f);
	pVtx[3].tex = D3DXVECTOR2(0.1f * nNumber + 0.1f, 1.0f);
	// ポリゴンの座標の設定
	pVtx[0].pos = D3DXVECTOR3(m_pos.x, m_pos.y, 0.0f);
	pVtx[1].pos = D3DXVECTOR3(m_pos.x + m_size.x, m_pos.y, 0.0f);
	pVtx[2].pos = D3DXVECTOR3(m_pos.x, m_pos.y + m_size.y, 0.0f);
	pVtx[3].pos = D3DXVECTOR3(m_pos.x + m_size.x, m_pos.y + m_size.y, 0.0f);

	//頂点バッファアンロック
	m_pVtxBuffer->Unlock();
}
