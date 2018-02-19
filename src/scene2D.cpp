#include "texManager.h"
#include "manager.h"
#include "modeGame.h"
#include "scene2D.h"
#include "input.h"
#include "renderer.h"

//--------------------------------------------------------------------------------
// デストラクタ
//--------------------------------------------------------------------------------
CScene2D::~CScene2D()
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
void CScene2D::Init()
{
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
void CScene2D::Uninit()
{

}

//--------------------------------------------------------------------------------
// 更新
//--------------------------------------------------------------------------------
void CScene2D::Update()
{
	// スレッドセーフ(ロック)
#ifdef THREAD_ON
	std::unique_lock<std::recursive_mutex> locker = CManager::Locker();
#endif

	// 仮想アドレスを取得するための変数
	VERTEX_2D* pVtx;
	// バッファをロックし仮想アドレスを取得する(0の場所から0個(全部)ロック)
	m_pVtxBuffer->Lock(0, 0, (void**)&pVtx, 0);

	D3DXVECTOR3 pos = GetPos();
	D3DXVECTOR3 size = GetSize();
	D3DXVECTOR3 rot = GetRot();
	float len = sqrt(size.x * size.x + size.y * size.y);
	float ang = atan2(size.y, size.x);
	pVtx[0].pos = D3DXVECTOR3(cosf(ang + D3DX_PI + rot.z) * len + pos.x,
		sinf(ang + D3DX_PI + rot.z) * len + pos.y, 0.f);
	pVtx[1].pos = D3DXVECTOR3(cosf(-ang + rot.z) * len + pos.x,
		sinf(-ang + rot.z) * len + pos.y, 0.f);
	pVtx[2].pos = D3DXVECTOR3(cosf(-ang + D3DX_PI + rot.z) * len + pos.x,
		sinf(-ang + D3DX_PI + rot.z) * len + pos.y, 0.f);
	pVtx[3].pos = D3DXVECTOR3(cosf(ang + rot.z) * len + pos.x,
		sinf(ang + rot.z) * len + pos.y, 0.f);

	// UV座標の設定
	pVtx[0].tex = D3DXVECTOR2(m_posUV.x, m_posUV.y);
	pVtx[1].tex = D3DXVECTOR2(m_posUV.x + m_widthUV, m_posUV.y);
	pVtx[2].tex = D3DXVECTOR2(m_posUV.x, m_posUV.y + m_heightUV);
	pVtx[3].tex = D3DXVECTOR2(m_posUV.x + m_widthUV, m_posUV.y + m_heightUV);

	// 頂点色の設定
	pVtx[0].col = D3DCOLOR_RGBA(m_r, m_g, m_b, m_a);
	pVtx[1].col = D3DCOLOR_RGBA(m_r, m_g, m_b, m_a);
	pVtx[2].col = D3DCOLOR_RGBA(m_r, m_g, m_b, m_a);
	pVtx[3].col = D3DCOLOR_RGBA(m_r, m_g, m_b, m_a);

	// rhwの設定
	pVtx[0].rhw = 1.f;
	pVtx[1].rhw = 1.f;
	pVtx[2].rhw = 1.f;
	pVtx[3].rhw = 1.f;

	// 設定後バッファのアンロック
	m_pVtxBuffer->Unlock();
}

//--------------------------------------------------------------------------------
// 描画
//--------------------------------------------------------------------------------
void CScene2D::Draw()
{
	// スレッドセーフ(ロック)
#ifdef THREAD_ON
	std::unique_lock<std::recursive_mutex> locker = CManager::Locker();
#endif

	// D3Deviceの取得
	LPDIRECT3DDEVICE9 pDevice = CRenderer::GetDevice();

	// 頂点フォーマットの設定
	pDevice->SetFVF(FVF_VERTEX_2D);
	
	// 頂点バッファをデバイスのデータ ストリームにバインドする
	pDevice->SetStreamSource(0, m_pVtxBuffer, 0, sizeof(VERTEX_2D));

	// テクスチャの設定
	if (m_pUseTexManager)
	{
		pDevice->SetTexture(0, m_pUseTexManager->GetTexInterface(m_strFilePass));
	}
	else
	{
		pDevice->SetTexture(0, CManager::GetTexManager()->GetTexInterface(m_strFilePass));
	}

	// ポリゴンの描画
	pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, 0, NUM_POLYGON);
}

//--------------------------------------------------------------------------------
// 作成処理
//--------------------------------------------------------------------------------
CScene2D *CScene2D::Create(const D3DXVECTOR3 &pos, const D3DXVECTOR3 &rot, const D3DXVECTOR3 &size, const std::string &strFilePass, int priority, CTexManager *pUseTexManager)
{
	// CScene2Dの生成
	CScene2D *pScene2D;
	pScene2D = new CScene2D(priority);

	pScene2D->Init();

	pScene2D->SetPos(pos);
	pScene2D->SetRot(rot);
	pScene2D->SetSize(size);
	pScene2D->m_strFilePass = strFilePass;

	pScene2D->m_pUseTexManager = pUseTexManager;

	// スレッドセーフ(ロック)
#ifdef THREAD_ON
	std::unique_lock<std::recursive_mutex> locker = CManager::Locker();
#endif

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

	// テクスチャの設定
	if (pScene2D->m_pUseTexManager)
	{
		pScene2D->m_pUseTexManager->SetTex(strFilePass);
	}
	else
	{
		CManager::GetTexManager()->SetTex(strFilePass);
	}
	// テクスチャのセット

	return pScene2D;
}

void CScene2D::SetTexturePos(D3DXVECTOR2 uv, D3DXVECTOR2 length)
{
	m_posUV = uv;
	m_heightUV = length.y;
	m_widthUV = length.x;
}
