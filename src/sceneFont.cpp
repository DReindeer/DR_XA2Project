//================================================================================
//
//	Auter : KENSUKE WATANABE
//	Data  : 2018/02/10
//
//--------------------------------------------------------------------------------
#include "texManager.h"
#include "fontManager.h"
#include "manager.h"
#include "scene2D.h"
#include "sceneFont.h"
#include "input.h"
#include "renderer.h"

//--------------------------------------------------------------------------------
// 初期化
//--------------------------------------------------------------------------------
void CSceneFont::Init()
{
	SetPos(D3DXVECTOR3(0.0f, 0.0f, 0.0f));		// 座標
	SetRot(D3DXVECTOR3(0.0f, 0.0f, 0.0f));		// 回転量
	SetSize(D3DXVECTOR3(1.0f, 1.0f, 1.0f));		// サイズ
	SetScl(D3DXVECTOR3(1.0f, 1.0f, 1.0f));		// 拡大率
}

//--------------------------------------------------------------------------------
// 終了
//--------------------------------------------------------------------------------
void CSceneFont::Uninit()
{
	// vtxバッファの解放
	if (m_pVtxBuffer != nullptr)
	{
		m_pVtxBuffer->Release();
		m_pVtxBuffer = nullptr;
	}
}

//--------------------------------------------------------------------------------
// 更新
//--------------------------------------------------------------------------------
void CSceneFont::Update()
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
	int r = GetR(), g = GetG(), b = GetB(), a = GetA();

	// rhwの設定
	pVtx[0].rhw = 1.0f;
	pVtx[1].rhw = 1.0f;
	pVtx[2].rhw = 1.0f;
	pVtx[3].rhw = 1.0f;

	// 頂点座標の設定
	pVtx[0].pos = D3DXVECTOR3(pos.x, pos.y - m_fontRelativeVtxPos, 0.0f);
	pVtx[1].pos = D3DXVECTOR3(pos.x, pos.y - m_fontHeight - m_fontRelativeVtxPos, 0.0f);
	pVtx[2].pos = D3DXVECTOR3(m_fontWidth + pos.x, pos.y - m_fontRelativeVtxPos, 0.0f);
	pVtx[3].pos = D3DXVECTOR3(m_fontWidth + pos.x, pos.y - m_fontHeight - m_fontRelativeVtxPos, 0.0f);

	// 頂点色の設定
	pVtx[0].col = D3DCOLOR_RGBA(r, g, b, a);
	pVtx[1].col = D3DCOLOR_RGBA(r, g, b, a);
	pVtx[2].col = D3DCOLOR_RGBA(r, g, b, a);
	pVtx[3].col = D3DCOLOR_RGBA(r, g, b, a);

	// UVの設定
	pVtx[0].tex = D3DXVECTOR2(0.f, 0.f);
	pVtx[1].tex = D3DXVECTOR2(1.f, 0.f);
	pVtx[2].tex = D3DXVECTOR2(0.f, 1.f);
	pVtx[3].tex = D3DXVECTOR2(1.f, 1.f);

	// 設定後バッファのアンロック
	m_pVtxBuffer->Unlock();
}

//--------------------------------------------------------------------------------
// 描画
//--------------------------------------------------------------------------------
void CSceneFont::Draw()
{
	// デバイスの取得
	LPDIRECT3DDEVICE9 dev = CManager::GetRenderer()->GetDevice();

	// αブレンド設定
	dev->SetRenderState(D3DRS_ALPHABLENDENABLE, TRUE);
	dev->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
	dev->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA);
	dev->SetSamplerState(0, D3DSAMP_ADDRESSU, D3DTADDRESS_CLAMP);	// テクスチャＵ値の繰り返し設定
	dev->SetSamplerState(0, D3DSAMP_ADDRESSV, D3DTADDRESS_CLAMP);	// テクスチャＶ値の繰り返し設定

	// 頂点バッファをデバイスのデータ ストリームにバインドする
	dev->SetStreamSource(0, m_pVtxBuffer, 0, sizeof(VERTEX_2D));

	// 頂点フォーマットの設定
	dev->SetFVF(FVF_VERTEX_2D);

	// テクスチャの設定
	dev->SetTexture(0, CManager::GetFontManager()->GetFont(m_strFilePass)->pTex);

	// ポリゴンの描画 // ポリゴンの形, ロードする最初の頂点のインデックス, ポリゴン数
	dev->DrawPrimitive(D3DPT_TRIANGLESTRIP, 0, NUM_POLYGON);

	dev->SetSamplerState(0, D3DSAMP_ADDRESSU, D3DTADDRESS_WRAP);	// テクスチャＵ値の繰り返し設定
	dev->SetSamplerState(0, D3DSAMP_ADDRESSV, D3DTADDRESS_WRAP);	// テクスチャＶ値の繰り返し設定
	dev->SetRenderState(D3DRS_ALPHATESTENABLE, FALSE);
}

//--------------------------------------------------------------------------------
// 作成処理
//--------------------------------------------------------------------------------
CSceneFont *CSceneFont::Create(const D3DXVECTOR3 &pos, const D3DXVECTOR3 &rot, const float &height, const int &weight, const wchar_t *wcFont)
{
	// CSceneFontの生成
	CSceneFont *pSceneFont;
	pSceneFont = new CSceneFont;
	pSceneFont->Init();

	// テクスチャ生成&セット
	CFontManager::FONT *pFont = CFontManager::SetFontTex(weight, wcFont);
	if (pFont)
	{
		pSceneFont->m_gm = pFont->gm;
		pSceneFont->m_fontWidth = pFont->fontWidth;
		pSceneFont->m_fontHeight = pFont->fontHeight;
		pSceneFont->m_fontRelativeVtxPos = pFont->relativeVtxPos;
	}

	// ポリゴンの頂点の設定
	pSceneFont->SetPos(D3DXVECTOR3(pos.x, pos.y + SCREEN_HEIGHT, 0.0f));
	pSceneFont->SetRot(rot);
	pSceneFont->SetSize(D3DXVECTOR3(height, height, 0.0f));

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
		&pSceneFont->m_pVtxBuffer,							// 
		nullptr)))
	{
		// 失敗したらエラーを返す
		MessageBox(nullptr, "2D用VtxBufferの作成に失敗しました", "警告", MB_OK);
		return nullptr;
	}

	setlocale(LC_CTYPE, "jpn");
	char ms[256];
	size_t size;
	wcstombs_s(&size, ms, (size_t)256, wcFont , (size_t)256);
	pSceneFont->m_strFilePass = ms;

	return pSceneFont;
}
