//================================================================================
//
//	Auter : KENSUKE WATANABE
//	Data  : 2018/02/10
//
//--------------------------------------------------------------------------------
#include "texManager.h"
#include "manager.h"
#include "modeGame.h"
#include "scene2DCircle.h"
#include "input.h"
#include "renderer.h"

#include <assert.h>

//--------------------------------------------------------------------------------
// 初期化
//--------------------------------------------------------------------------------
void CScene2DCircle::Init()
{
	// 変数の初期化
	m_pVtxBuffer = nullptr;
	SetPos(D3DXVECTOR3(0.f, 0.f, 0.f));		// 座標
	SetSize(D3DXVECTOR3(1.f, 1.f, 1.f));	// サイズ
	SetRot(D3DXVECTOR3(0.f, 0.f, 0.f));		// 回転量
	SetScl(D3DXVECTOR3(1.f, 1.f, 1.f));		// 拡大率
	m_posUV = D3DXVECTOR2(0.f, 0.f);
	m_widthUV = 1.f;
	m_heightUV = 1.f;
	m_rotUV = 0.f;
	m_len = 0;
	m_numOutVtx = 16;
}

//--------------------------------------------------------------------------------
// 終了
//--------------------------------------------------------------------------------
void CScene2DCircle::Uninit()
{
	// vtxバッファの解放
	if(m_pVtxBuffer != nullptr)
	{
		m_pVtxBuffer->Release();
		m_pVtxBuffer = nullptr;
	}
}

//--------------------------------------------------------------------------------
// 更新
//--------------------------------------------------------------------------------
void CScene2DCircle::Update()
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
	float ang = D3DX_PI * 2 / (m_numOutVtx);

	pVtx[0].pos = D3DXVECTOR3(pos.x, pos.y, 0.f);	// 頂点座標の設定
	pVtx[0].tex = m_posUV;							// UV座標の設定
	for (int i = 0; i < m_numOutVtx + 1; i++)
	{
		// 頂点座標の設定
		pVtx[i + 1].pos = D3DXVECTOR3(cosf((ang * i) + rot.z) * m_len + pos.x,
			sinf((ang * i) + rot.z) * m_len + pos.y, 0.f);
		// UV座標の設定
		pVtx[i + 1].tex = D3DXVECTOR2(cosf((ang * i) + m_rotUV) * m_lenUV + m_posUV.x,
			sinf((ang * i) + m_rotUV) * m_lenUV + m_posUV.y);
	}
	int r = GetR(); int g = GetB(); int b = GetB(); int a = GetA();
	for (int i = 0; i < m_numOutVtx + 2; i++)
	{
		// rhwの設定
		pVtx[i].rhw = 1.f;
		// 頂点色の設定
		pVtx[i].col = D3DCOLOR_RGBA(r, g, b, a);
	}

	// 設定後バッファのアンロック
	m_pVtxBuffer->Unlock();
}

//--------------------------------------------------------------------------------
// 描画
//--------------------------------------------------------------------------------
void CScene2DCircle::Draw()
{
	// D3Deviceの取得
	LPDIRECT3DDEVICE9 pDevice = CRenderer::GetDevice();
	
	// 頂点バッファをデバイスのデータ ストリームにバインドする
	pDevice->SetStreamSource(0, m_pVtxBuffer, 0, sizeof(VERTEX_2D));

	// 頂点フォーマットの設定
	pDevice->SetFVF(FVF_VERTEX_2D);

	// テクスチャの設定
	pDevice->SetTexture(0, CManager::GetTexManager()->GetTexInterface(m_strFilePass));

	// ポリゴンの描画
	pDevice->DrawPrimitive(D3DPT_TRIANGLEFAN, 0, m_numOutVtx);
}

//--------------------------------------------------------------------------------
// 作成処理
//--------------------------------------------------------------------------------
CScene2DCircle *CScene2DCircle::Create(const D3DXVECTOR3 &pos, const D3DXVECTOR3 &rot, float len, int numOutVtx, D3DXVECTOR2 posUV, float lenUV, const std::string &strFilePass, int priority)
{
	// CScene2DCircleの生成
	CScene2DCircle *pScene2DCircle;
	pScene2DCircle = new CScene2DCircle(priority);
	pScene2DCircle->Init();

	pScene2DCircle->SetPos(pos);
	pScene2DCircle->SetRot(rot);
	pScene2DCircle->SetSize(D3DXVECTOR3(0.f, 0.f, 0.f));
	pScene2DCircle->m_posUV = posUV;
	pScene2DCircle->m_lenUV = lenUV;
	pScene2DCircle->m_len = len;
	pScene2DCircle->m_numOutVtx = max(16, numOutVtx);	// 最低16
	pScene2DCircle->m_strFilePass = strFilePass;

	// スレッドセーフ(ロック)
#ifdef THREAD_ON
	std::unique_lock<std::recursive_mutex> locker = CManager::Locker();
#endif

	// D3Deviceの取得
	LPDIRECT3DDEVICE9 pDevice = CManager::GetRenderer()->GetDevice();

	// 頂点情報の作成
	//---------------------
	if (FAILED(pDevice->CreateVertexBuffer(
		sizeof(VERTEX_2D) * pScene2DCircle->m_numOutVtx + 2,			// メモリのサイズ
		D3DUSAGE_WRITEONLY,					// 使用方法(書き込みのみ)
		FVF_VERTEX_2D,						// 使用する頂点フォーマット(見ない)
		D3DPOOL_MANAGED,					// バッファの管理方法(勝手に復旧してくれる)
		&pScene2DCircle->m_pVtxBuffer,							// 
		nullptr)))
	{
		// 失敗したらエラーを返す
		MessageBox(nullptr, "2D用VtxBufferの作成に失敗しました", "警告", MB_OK);
		return nullptr;
	}
	
	// テクスチャのセット
	CManager::GetTexManager()->SetTex(strFilePass);

	return pScene2DCircle;
}

void CScene2DCircle::SetTexturePos(D3DXVECTOR2 uv, D3DXVECTOR2 length)
{
	m_posUV = uv;
	m_heightUV = length.y;
	m_widthUV = length.x;
}