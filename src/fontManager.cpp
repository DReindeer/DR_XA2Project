//================================================================================
//
//	Auter : KENSUKE WATANABE
//	Data  : 2018/02/10
//
//--------------------------------------------------------------------------------
#define _CRT_SECURE_NO_WARNINGS
#include "main.h"
#include <tchar.h>
#include <codecvt> 
#include <stdio.h>
#include <assert.h>
#include "fontManager.h"
#include "scene2D.h"
#include "manager.h"
#include "texManager.h"
#include "inputKeyboard.h"
#include "renderer.h"

static const int FONT_SIZE = 64;

std::unordered_map<std::string, CFontManager::FONT*> CFontManager::m_unmapFont;

//--------------------------------------------------------------------------------
// 初期化
//--------------------------------------------------------------------------------
void CFontManager::Init(HINSTANCE hInst)
{
	// リソースファイルからフォント読み込み
	HRSRC hFontResource = FindResource(hInst, "IDR_FONT1", RT_FONT);

	LPVOID lpFontImage = LockResource(hFontResource);

	DWORD dwFontImageSize = SizeofResource(NULL, hFontResource);

	DWORD dwFontCount = 0;

	m_hFont = AddFontMemResourceEx(lpFontImage, dwFontImageSize, NULL, &dwFontCount);
}

//--------------------------------------------------------------------------------
// 終了
//--------------------------------------------------------------------------------
void CFontManager::Uninit()
{
	ReleaseAll();

	RemoveFontMemResourceEx(m_hFont);
}

//--------------------------------------------------------------------------------
// 解放
//--------------------------------------------------------------------------------
bool CFontManager::Release(const std::string &strFilePass)
{
	// 空か確かめる
	if (strFilePass.empty()) { return false; }

	// 要素の有無を確かめる
	if (m_unmapFont.count(strFilePass) == 0) { return false; }

	FONT *pFont = m_unmapFont.at(strFilePass);

	// ありますよ
	if (pFont->pTex)
	{
		pFont->pTex->Release();	// 
		pFont->pTex = nullptr;	//  テクスチャバッファの解放
	}
	delete pFont;		// 
	pFont = nullptr;	// 自分自身のバッファの解放
	m_unmapFont.erase(strFilePass);			// 要素の削除

	return true;
}

//--------------------------------------------------------------------------------
// 全解放
//--------------------------------------------------------------------------------
void CFontManager::ReleaseAll()
{
	for (auto it : m_unmapFont)
	{
		if (it.second->pTex)
		{
			it.second->pTex->Release();	// 
			it.second->pTex = nullptr;	//  テクスチャバッファの解放
		}
		delete it.second;			// 
		it.second = nullptr;		// 自分自身のバッファの解放
	}
	// 自分自身を空にする(全要素クリア)
	m_unmapFont.clear();
}

//--------------------------------------------------------------------------------
// 設定処理
//--------------------------------------------------------------------------------
CFontManager::FONT *CFontManager::SetFontTex(int fontWeight, const wchar_t wcFont[])
{
	// テクスチャマネージャ
	setlocale(LC_CTYPE, "jpn");
	char ms[MAX_PATH];
	wcstombs(ms, wcFont, MAX_PATH);
	std::string str = ms;

	// テクスチャがあればそれを返す
	if (m_unmapFont.count(str) == 1)
	{
		return m_unmapFont[str];
	}

	// デバイスの取得
	LPDIRECT3DDEVICE9 dev = CManager::GetRenderer()->GetDevice();

	// フォントの生成
	FONT *pFont = new FONT;

	LOGFONT lf = { FONT_SIZE, 0, 0, 0, fontWeight, 0, 0, 0, SHIFTJIS_CHARSET, OUT_TT_ONLY_PRECIS, CLIP_DEFAULT_PRECIS, PROOF_QUALITY, DEFAULT_PITCH | FF_MODERN, "IDR_FONT1" };
	//	LOGFONT lf = {FONT_SIZE, 0, 0, 0, fontWeight, 0, 0, 0, SHIFTJIS_CHARSET, OUT_TT_ONLY_PRECIS, CLIP_DEFAULT_PRECIS, PROOF_QUALITY, DEFAULT_PITCH | FF_MODERN, _T("ＭＳ Ｐゴシック")};
	
	HFONT hFont = CreateFontIndirect(&lf);
	assert(hFont != nullptr && "hFontがnull");	// フォントがnullであってはいけない。

	// デバイスにフォントを持たせないとGetGlyphOutline関数はエラーとなる
	HDC hdc = GetDC(NULL);
	HFONT oldFont = (HFONT)SelectObject(hdc, hFont);

	// フォントビットマップ取得
	UINT code = (UINT)wcFont[0];
	const int gradFlag = GGO_GRAY4_BITMAP; // GGO_GRAY2_BITMAP or GGO_GRAY4_BITMAP or GGO_GRAY8_BITMAP
	int grad = 0; // 階調の最大値
	switch (gradFlag)
	{
	case GGO_GRAY2_BITMAP: grad = 4; break;
	case GGO_GRAY4_BITMAP: grad = 16; break;
	case GGO_GRAY8_BITMAP: grad = 64; break;
	}

	// gradが0なのもダメです。
	assert(grad != 0 && "gradが0です");

	GetTextMetrics(hdc, &pFont->tm);
	CONST MAT2 mat = { { 0,1 },{ 0,0 },{ 0,0 },{ 0,1 } };
	DWORD size = GetGlyphOutlineW(hdc, code, gradFlag, &pFont->gm, 0, NULL, &mat);
	BYTE *pMono = new BYTE[size];
	GetGlyphOutlineW(hdc, code, gradFlag, &pFont->gm, size, pMono, &mat);

	// デバイスコンテキストとフォントハンドルはもういらないので解放
	SelectObject(hdc, oldFont);
	ReleaseDC(NULL, hdc);

	// テクスチャ作成
	pFont->fontWidth = (pFont->gm.gmBlackBoxX + 3) / 4 * 4;
	pFont->fontHeight = pFont->gm.gmBlackBoxY;
	dev->CreateTexture(pFont->fontWidth, pFont->fontHeight, 1, 0, D3DFMT_A8R8G8B8, D3DPOOL_MANAGED, &pFont->pTex, NULL);

	// テクスチャにフォントビットマップ情報を書き込み
	D3DLOCKED_RECT lockedRect;
	pFont->pTex->LockRect(0, &lockedRect, NULL, 0);	// ロック
	DWORD *m_pTexBuf = (DWORD*)lockedRect.pBits;	// テクスチャメモリへのポインタ

	for (int y = 0; y < pFont->fontHeight; y++)
	{
		for (int x = 0; x < pFont->fontWidth; x++)
		{
			DWORD alpha = pMono[y * pFont->fontWidth + x] * 255 / grad;
			m_pTexBuf[y * pFont->fontWidth + x] = (alpha << 24) | 0x00ffffff;
		}
	}

	pFont->pTex->UnlockRect(0);  // アンロック
	delete[] pMono;

	pFont->relativeVtxPos = pFont->gm.gmptGlyphOrigin.y - pFont->fontHeight;

	// 要素の追加
	m_unmapFont[str] = pFont;

	return pFont;
}
	