//================================================================================
//
//	Auter : KENSUKE WATANABE
//	Data  : 2018/02/10
//
//--------------------------------------------------------------------------------
#include "texManager.h"
#include "manager.h"
#include "modeGame.h"
#include "scene2DCircleMap.h"
#include "input.h"
#include "renderer.h"

//--------------------------------------------------------------------------------
// 初期化
//--------------------------------------------------------------------------------
void CScene2DCircleMap::Init()
{
	// 変数の初期化
	m_pVtxBuffer = nullptr;
	m_pIdxBufferParAlpha[0] = nullptr;
	m_pIdxBufferParAlpha[1] = nullptr;
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
	m_bInitDeviceLock = false;
}

//--------------------------------------------------------------------------------
// 終了
//--------------------------------------------------------------------------------
void CScene2DCircleMap::Uninit()
{
	// vtxバッファの解放
	if(m_pVtxBuffer != nullptr)
	{
		m_pVtxBuffer->Release();
		m_pVtxBuffer = nullptr;
	}
	// idxバッファの解放
	for (int i = 0; i < 2; i++)
	{
		if (m_pIdxBufferParAlpha[i] != nullptr)
		{
			m_pIdxBufferParAlpha[i]->Release();
			m_pIdxBufferParAlpha[i] = nullptr;
		}
	}
}

//--------------------------------------------------------------------------------
// 更新
//--------------------------------------------------------------------------------
void CScene2DCircleMap::Update()
{
	// スレッドセーフ(ロック)
#ifdef THREAD_ON
	std::unique_lock<std::recursive_mutex> locker = CManager::Locker();
#endif

	// 更新処理の初期に行う初期化(Deviceのロック.アンロック)
	if (m_bInitDeviceLock == false)
	{
		InitDeviceLock();
		m_bInitDeviceLock = true;
	}

	// 仮想アドレスを取得するための変数
	VERTEX_2D* pVtx;
	// バッファをロックし仮想アドレスを取得する(0の場所から0個(全部)ロック)
	m_pVtxBuffer->Lock(0, 0, (void**)&pVtx, 0);

	D3DXVECTOR3 pos = GetPos();
	D3DXVECTOR3 size = GetSize();
	D3DXVECTOR3 rot = GetRot();
	float ang = D3DX_PI * 2 / (m_numOutVtx);

	float alpha255Len = m_len * 0.8f;
	float alpha255LenUV = m_lenUV * 0.8f;

	pVtx[0].pos = D3DXVECTOR3(pos.x, pos.y, 0.f);		// 頂点座標の設定
	pVtx[0].tex = m_posUV;								// UV座標の設定
	int r = GetR(); int g = GetB(); int b = GetB(); int a = GetA();
	pVtx[0].col = D3DCOLOR_RGBA(r, g, b, a);

	// 内側
	for (int i = 0; i < m_numOutVtx + 1; i++)
	{
		// 頂点座標の設定
		pVtx[i + 1].pos = D3DXVECTOR3(cosf((ang * i) + rot.z) * alpha255Len + pos.x,
			sinf((ang * i) + rot.z) * alpha255Len + pos.y, 0.f);
		// UV座標の設定
		pVtx[i + 1].tex = D3DXVECTOR2(cosf((ang * i) + m_rotUV) * alpha255LenUV + m_posUV.x,
			sinf((ang * i) + m_rotUV) * alpha255LenUV + m_posUV.y);
		// 頂点色の設定
		pVtx[i + 1].col = D3DCOLOR_RGBA(r, g, b, a);
	}

	// 外側
	for (int i = 0; i < m_numOutVtx + 1; i++)
	{
		// 頂点座標の設定
		pVtx[i + 1 + m_numOutVtx + 1].pos = D3DXVECTOR3(cosf((ang * i) + rot.z) * m_len + pos.x,
			sinf((ang * i) + rot.z) * m_len + pos.y, 0.f);
		// UV座標の設定
		pVtx[i + 1 + m_numOutVtx + 1].tex = D3DXVECTOR2(cosf((ang * i) + m_rotUV) * m_lenUV + m_posUV.x,
			sinf((ang * i) + m_rotUV) * m_lenUV + m_posUV.y);
		// 頂点色の設定
		pVtx[i + 1 + m_numOutVtx + 1].col = D3DCOLOR_RGBA(r, g, b, static_cast<int>(a * 0.1f));
	}

	// 設定後バッファのアンロック
	m_pVtxBuffer->Unlock();
}

//--------------------------------------------------------------------------------
// 描画
//--------------------------------------------------------------------------------
void CScene2DCircleMap::Draw()
{
	// D3Deviceの取得
	LPDIRECT3DDEVICE9 pDevice = CRenderer::GetDevice();

	// サンプラーステートの設定
	pDevice->SetSamplerState(0, D3DSAMP_ADDRESSU, D3DTADDRESS_BORDER);	// テクスチャＵ値の繰り返し設定
	pDevice->SetSamplerState(0, D3DSAMP_ADDRESSV, D3DTADDRESS_BORDER);	// テクスチャＶ値の繰り返し設定
	
	// 頂点バッファをデバイスのデータ ストリームにバインドする
	pDevice->SetStreamSource(0, m_pVtxBuffer, 0, sizeof(VERTEX_2D));

	// 頂点フォーマットの設定
	pDevice->SetFVF(FVF_VERTEX_2D);

	// テクスチャの設定
	pDevice->SetTexture(0, CManager::GetTexManager()->GetTexInterface(m_strFilePass));

	// ポリゴンの描画
	pDevice->DrawPrimitive(D3DPT_TRIANGLEFAN, 0, m_numOutVtx);

	// インデックス番号をデバイスに設定
	pDevice->SetIndices(m_pIdxBufferParAlpha[0]);
	// プリミティブの設定
	pDevice->DrawIndexedPrimitive(D3DPT_TRIANGLESTRIP, 0, 0, NUM_POLYGON, 0, (m_numOutVtx + 1) * 2 - 2);

	// サンプラーステートの設定
	pDevice->SetSamplerState(0, D3DSAMP_ADDRESSU, D3DTADDRESS_WRAP);	// テクスチャＵ値の繰り返し設定
	pDevice->SetSamplerState(0, D3DSAMP_ADDRESSV, D3DTADDRESS_WRAP);	// テクスチャＶ値の繰り返し設定
}

//--------------------------------------------------------------------------------
// 作成処理
//--------------------------------------------------------------------------------
CScene2DCircleMap *CScene2DCircleMap::Create(const D3DXVECTOR3 &pos, const D3DXVECTOR3 &rot, float len, int numOutVtx, D3DXVECTOR2 posUV, float lenUV, const std::string &strFilePass, int priority)
{
	// CScene2DCircleMapの生成
	CScene2DCircleMap *pScene2DCircleMap;
	pScene2DCircleMap = new CScene2DCircleMap(priority);
	pScene2DCircleMap->Init();

	pScene2DCircleMap->SetPos(pos);
	pScene2DCircleMap->SetRot(rot);
	pScene2DCircleMap->SetSize(D3DXVECTOR3(0.f, 0.f, 0.f));
	pScene2DCircleMap->m_posUV = posUV;
	pScene2DCircleMap->m_lenUV = lenUV;
	pScene2DCircleMap->m_len = len;
	pScene2DCircleMap->m_numOutVtx = max(16, numOutVtx);	// 最低16
	pScene2DCircleMap->m_strFilePass = strFilePass;

	// スレッドセーフ(ロック)
#ifdef THREAD_ON
	std::unique_lock<std::recursive_mutex> locker = CManager::Locker();
#endif

	// D3Deviceの取得
	LPDIRECT3DDEVICE9 pDevice = CManager::GetRenderer()->GetDevice();

	// 頂点情報の作成
	//---------------------
	if (FAILED(pDevice->CreateVertexBuffer(
		sizeof(VERTEX_2D) * (1 + (pScene2DCircleMap->m_numOutVtx + 1) * 2),			// メモリのサイズ
		D3DUSAGE_WRITEONLY,					// 使用方法(書き込みのみ)
		FVF_VERTEX_2D,						// 使用する頂点フォーマット(見ない)
		D3DPOOL_MANAGED,					// バッファの管理方法(勝手に復旧してくれる)
		&pScene2DCircleMap->m_pVtxBuffer,	// 
		nullptr)))
	{
		// 失敗したらエラーを返す
		MessageBox(nullptr, "2D用VtxBufferの作成に失敗しました", "警告", MB_OK);
		return nullptr;
	}
	if (FAILED(pDevice->CreateIndexBuffer(
		sizeof(WORD) * (pScene2DCircleMap->m_numOutVtx + 1) * 2,		// バッファ量
		D3DUSAGE_WRITEONLY,					// 使用フラグ
		D3DFMT_INDEX16,						// 必要なバッファ量
		D3DPOOL_MANAGED,					// メモリの管理方法
		&pScene2DCircleMap->m_pIdxBufferParAlpha[0],
		nullptr)))
	{
		// 失敗したらエラーを返す
		MessageBox(nullptr, "IdxBufferの作成に失敗しました", "警告", MB_OK);
		return nullptr;
	}
	
	// テクスチャのセット
	CManager::GetTexManager()->SetTex(strFilePass);

	return pScene2DCircleMap;
}

void CScene2DCircleMap::SetTexturePos(D3DXVECTOR2 uv, D3DXVECTOR2 length)
{
	m_posUV = uv;
	m_heightUV = length.y;
	m_widthUV = length.x;
}

//--------------------------------------------------------------------------------
// 更新処理の最初に行う処理
//--------------------------------------------------------------------------------
void CScene2DCircleMap::InitDeviceLock()
{
	// 仮想アドレスを取得するための変数
	VERTEX_2D* pVtx;
	// バッファをロックし仮想アドレスを取得する(0の場所から0個(全部)ロック)
	m_pVtxBuffer->Lock(0, 0, (void**)&pVtx, 0);

	D3DXVECTOR3 pos = GetPos();
	D3DXVECTOR3 size = GetSize();
	D3DXVECTOR3 rot = GetRot();
	float ang = D3DX_PI * 2 / (m_numOutVtx);

	float alpha255Len = m_len * 0.8f;
	float alpha255LenUV = m_lenUV * 0.8f;

	pVtx[0].pos = D3DXVECTOR3(pos.x, pos.y, 0.f);		// 頂点座標の設定
	pVtx[0].tex = m_posUV;								// UV座標の設定
	pVtx[0].col = D3DCOLOR_RGBA(255, 255, 255, 255);	// 頂点色の設定
	pVtx[0].rhw = 1.f;									// rhwの設定
	for (int i = 0; i < m_numOutVtx + 1; i++)
	{
		// 頂点座標の設定
		pVtx[i + 1].pos = D3DXVECTOR3(cosf((ang * i) + rot.z) * alpha255Len + pos.x,
			sinf((ang * i) + rot.z) * alpha255Len + pos.y, 0.f);
		// UV座標の設定
		pVtx[i + 1].tex = D3DXVECTOR2(cosf((ang * i) + m_rotUV) * alpha255LenUV + m_posUV.x,
			sinf((ang * i) + m_rotUV) * alpha255LenUV + m_posUV.y);
		// 頂点色の設定
		pVtx[i + 1].col = D3DCOLOR_RGBA(255, 255, 255, 255);
		// rhwの設定
		pVtx[i + 1].rhw = 1.f;
	}
	for (int i = 0; i < m_numOutVtx + 1; i++)
	{
		// 頂点座標の設定
		pVtx[i + 1 + m_numOutVtx + 1].pos = D3DXVECTOR3(cosf((ang * i) + rot.z) * m_len + pos.x,
			sinf((ang * i) + rot.z) * m_len + pos.y, 0.f);
		// UV座標の設定
		pVtx[i + 1 + m_numOutVtx + 1].tex = D3DXVECTOR2(cosf((ang * i) + m_rotUV) * m_lenUV + m_posUV.x,
			sinf((ang * i) + m_rotUV) * m_lenUV + m_posUV.y);
		// 頂点色の設定
		pVtx[i + 1 + m_numOutVtx + 1].col = D3DCOLOR_RGBA(255, 255, 255, 64);
		// rhwの設定
		pVtx[i + 1 + m_numOutVtx + 1].rhw = 1.f;
	}

	// 設定後バッファのアンロック
	m_pVtxBuffer->Unlock();

	// インデックス
	WORD *pIdx;				// バッファをロックする
	m_pIdxBufferParAlpha[0]->Lock(0, 0, (void**)&pIdx, 0);

	for (int i = 0; i < (m_numOutVtx + 1) * 2; i += 2)
	{
		pIdx[i] = static_cast<WORD>((i + 1) / 2 + 1);
		pIdx[i + 1] = static_cast<WORD>(pIdx[i] + (m_numOutVtx + 1));
	}

	m_pIdxBufferParAlpha[0]->Unlock();
}