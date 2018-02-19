//================================================================================
//
//	Auter : KENSUKE WATANABE
//	Data  : 2018/02/10
//
//--------------------------------------------------------------------------------
#include "texManager.h"
#include "manager.h"
#include "modeGame.h"
#include "scene2DDeferred.h"
#include "deferredRendering.h"
#include "renderer.h"

//--------------------------------------------------------------------------------
// 終了
//--------------------------------------------------------------------------------
CScene2DDeferred::~CScene2DDeferred()
{
	// スレッドセーフ(ロック)
#ifdef THREAD_ON
	std::unique_lock<std::recursive_mutex> locker = CManager::Locker();
#endif
	// vtxバッファの解放
	if (m_pVtxBuffer != nullptr)
	{
		m_pVtxBuffer->Release();
		m_pVtxBuffer = nullptr;
	}
}

//--------------------------------------------------------------------------------
// 初期化
//--------------------------------------------------------------------------------
void CScene2DDeferred::Init()
{
	CScene2D::Init();
	// 変数の初期化
	SetPos(D3DXVECTOR3(0.f, 0.f, 0.f));		// 座標
	SetSize(D3DXVECTOR3(1.f, 1.f, 1.f));	// サイズ
	SetRot(D3DXVECTOR3(0.f, 0.f, 0.f));		// 回転量
	SetScl(D3DXVECTOR3(1.f, 1.f, 1.f));		// 拡大率
	m_posUV = D3DXVECTOR2(0.f, 0.f);
	m_widthUV = 1.f;
	m_heightUV = 1.f;
}

//--------------------------------------------------------------------------------
// 終了
//--------------------------------------------------------------------------------
void CScene2DDeferred::Uninit()
{
	CScene2D::Uninit();
}

//--------------------------------------------------------------------------------
// 更新
//--------------------------------------------------------------------------------
void CScene2DDeferred::Update()
{
	CScene2D::Update();
}

//--------------------------------------------------------------------------------
// 描画
//--------------------------------------------------------------------------------
void CScene2DDeferred::Draw()
{
	// スレッドセーフ(ロック)
#ifdef THREAD_ON
	std::unique_lock<std::recursive_mutex> locker = CManager::Locker();
#endif

	// D3Deviceの取得
	LPDIRECT3DDEVICE9 pDevice = CRenderer::GetDevice();

	// 描画
	//--------------------------------------------------------------------------------
	DefferdRendering *pDefferd = CManager::GetDefferdRendering();

	// Gバッファ書き込み開始
	pDefferd->DrawCombineTexture();

	// 頂点フォーマットの設定
	pDevice->SetFVF(FVF_VERTEX_2D);

	// 頂点バッファをデバイスのデータ ストリームにバインドする
	pDevice->SetStreamSource(0, m_pVtxBuffer, 0, sizeof(VERTEX_2D));

	// ポリゴンの描画
	pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, 0, NUM_POLYGON);

	// Gバッファ書き込み終了
	pDefferd->DrawCombineEnd();
}

//--------------------------------------------------------------------------------
// 作成処理
//--------------------------------------------------------------------------------
CScene2DDeferred *CScene2DDeferred::Create(const D3DXVECTOR3 &pos, const D3DXVECTOR3 &rot, const D3DXVECTOR3 &size, const std::string &strFilePass, int priority, CTexManager *pUseTexManager)
{
	// スレッドセーフ(ロック)
#ifdef THREAD_ON
	std::unique_lock<std::recursive_mutex> locker = CManager::Locker();
#endif

	// CScene2DDeferredの生成
	CScene2DDeferred *pScene2D;
	pScene2D = new CScene2DDeferred(priority);

	pScene2D->Init();

	pScene2D->SetPos(pos);
	pScene2D->SetRot(rot);
	pScene2D->SetSize(size);
	pScene2D->m_strFilePass = strFilePass; 
	pScene2D->SetUseTexManager(pUseTexManager);

	// D3Deviceの取得
	LPDIRECT3DDEVICE9 pDevice = CRenderer::GetDevice();

	// 頂点情報の作成
	//---------------------
	if (FAILED(pDevice->CreateVertexBuffer(
		sizeof(VERTEX_2D) * NUM_VERTEX,		// メモリのサイズ
		D3DUSAGE_WRITEONLY,					// 使用方法(書き込みのみ)
		FVF_VERTEX_2D,						// 使用する頂点フォーマット(見ない)
		D3DPOOL_MANAGED,					// バッファの管理方法(勝手に復旧してくれる)
		&pScene2D->m_pVtxBuffer,			// 
		nullptr)))
	{
		// 失敗したらエラーを返す
		MessageBox(nullptr, "2D用VtxBufferの作成に失敗しました", "警告", MB_OK);
		return nullptr;
	}

	return pScene2D;
}
