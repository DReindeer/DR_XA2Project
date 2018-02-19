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
// ������
//--------------------------------------------------------------------------------
void CFontManager::Init(HINSTANCE hInst)
{
	// ���\�[�X�t�@�C������t�H���g�ǂݍ���
	HRSRC hFontResource = FindResource(hInst, "IDR_FONT1", RT_FONT);

	LPVOID lpFontImage = LockResource(hFontResource);

	DWORD dwFontImageSize = SizeofResource(NULL, hFontResource);

	DWORD dwFontCount = 0;

	m_hFont = AddFontMemResourceEx(lpFontImage, dwFontImageSize, NULL, &dwFontCount);
}

//--------------------------------------------------------------------------------
// �I��
//--------------------------------------------------------------------------------
void CFontManager::Uninit()
{
	ReleaseAll();

	RemoveFontMemResourceEx(m_hFont);
}

//--------------------------------------------------------------------------------
// ���
//--------------------------------------------------------------------------------
bool CFontManager::Release(const std::string &strFilePass)
{
	// �󂩊m���߂�
	if (strFilePass.empty()) { return false; }

	// �v�f�̗L�����m���߂�
	if (m_unmapFont.count(strFilePass) == 0) { return false; }

	FONT *pFont = m_unmapFont.at(strFilePass);

	// ����܂���
	if (pFont->pTex)
	{
		pFont->pTex->Release();	// 
		pFont->pTex = nullptr;	//  �e�N�X�`���o�b�t�@�̉��
	}
	delete pFont;		// 
	pFont = nullptr;	// �������g�̃o�b�t�@�̉��
	m_unmapFont.erase(strFilePass);			// �v�f�̍폜

	return true;
}

//--------------------------------------------------------------------------------
// �S���
//--------------------------------------------------------------------------------
void CFontManager::ReleaseAll()
{
	for (auto it : m_unmapFont)
	{
		if (it.second->pTex)
		{
			it.second->pTex->Release();	// 
			it.second->pTex = nullptr;	//  �e�N�X�`���o�b�t�@�̉��
		}
		delete it.second;			// 
		it.second = nullptr;		// �������g�̃o�b�t�@�̉��
	}
	// �������g����ɂ���(�S�v�f�N���A)
	m_unmapFont.clear();
}

//--------------------------------------------------------------------------------
// �ݒ菈��
//--------------------------------------------------------------------------------
CFontManager::FONT *CFontManager::SetFontTex(int fontWeight, const wchar_t wcFont[])
{
	// �e�N�X�`���}�l�[�W��
	setlocale(LC_CTYPE, "jpn");
	char ms[MAX_PATH];
	wcstombs(ms, wcFont, MAX_PATH);
	std::string str = ms;

	// �e�N�X�`��������΂����Ԃ�
	if (m_unmapFont.count(str) == 1)
	{
		return m_unmapFont[str];
	}

	// �f�o�C�X�̎擾
	LPDIRECT3DDEVICE9 dev = CManager::GetRenderer()->GetDevice();

	// �t�H���g�̐���
	FONT *pFont = new FONT;

	LOGFONT lf = { FONT_SIZE, 0, 0, 0, fontWeight, 0, 0, 0, SHIFTJIS_CHARSET, OUT_TT_ONLY_PRECIS, CLIP_DEFAULT_PRECIS, PROOF_QUALITY, DEFAULT_PITCH | FF_MODERN, "IDR_FONT1" };
	//	LOGFONT lf = {FONT_SIZE, 0, 0, 0, fontWeight, 0, 0, 0, SHIFTJIS_CHARSET, OUT_TT_ONLY_PRECIS, CLIP_DEFAULT_PRECIS, PROOF_QUALITY, DEFAULT_PITCH | FF_MODERN, _T("�l�r �o�S�V�b�N")};
	
	HFONT hFont = CreateFontIndirect(&lf);
	assert(hFont != nullptr && "hFont��null");	// �t�H���g��null�ł����Ă͂����Ȃ��B

	// �f�o�C�X�Ƀt�H���g���������Ȃ���GetGlyphOutline�֐��̓G���[�ƂȂ�
	HDC hdc = GetDC(NULL);
	HFONT oldFont = (HFONT)SelectObject(hdc, hFont);

	// �t�H���g�r�b�g�}�b�v�擾
	UINT code = (UINT)wcFont[0];
	const int gradFlag = GGO_GRAY4_BITMAP; // GGO_GRAY2_BITMAP or GGO_GRAY4_BITMAP or GGO_GRAY8_BITMAP
	int grad = 0; // �K���̍ő�l
	switch (gradFlag)
	{
	case GGO_GRAY2_BITMAP: grad = 4; break;
	case GGO_GRAY4_BITMAP: grad = 16; break;
	case GGO_GRAY8_BITMAP: grad = 64; break;
	}

	// grad��0�Ȃ̂��_���ł��B
	assert(grad != 0 && "grad��0�ł�");

	GetTextMetrics(hdc, &pFont->tm);
	CONST MAT2 mat = { { 0,1 },{ 0,0 },{ 0,0 },{ 0,1 } };
	DWORD size = GetGlyphOutlineW(hdc, code, gradFlag, &pFont->gm, 0, NULL, &mat);
	BYTE *pMono = new BYTE[size];
	GetGlyphOutlineW(hdc, code, gradFlag, &pFont->gm, size, pMono, &mat);

	// �f�o�C�X�R���e�L�X�g�ƃt�H���g�n���h���͂�������Ȃ��̂ŉ��
	SelectObject(hdc, oldFont);
	ReleaseDC(NULL, hdc);

	// �e�N�X�`���쐬
	pFont->fontWidth = (pFont->gm.gmBlackBoxX + 3) / 4 * 4;
	pFont->fontHeight = pFont->gm.gmBlackBoxY;
	dev->CreateTexture(pFont->fontWidth, pFont->fontHeight, 1, 0, D3DFMT_A8R8G8B8, D3DPOOL_MANAGED, &pFont->pTex, NULL);

	// �e�N�X�`���Ƀt�H���g�r�b�g�}�b�v������������
	D3DLOCKED_RECT lockedRect;
	pFont->pTex->LockRect(0, &lockedRect, NULL, 0);	// ���b�N
	DWORD *m_pTexBuf = (DWORD*)lockedRect.pBits;	// �e�N�X�`���������ւ̃|�C���^

	for (int y = 0; y < pFont->fontHeight; y++)
	{
		for (int x = 0; x < pFont->fontWidth; x++)
		{
			DWORD alpha = pMono[y * pFont->fontWidth + x] * 255 / grad;
			m_pTexBuf[y * pFont->fontWidth + x] = (alpha << 24) | 0x00ffffff;
		}
	}

	pFont->pTex->UnlockRect(0);  // �A�����b�N
	delete[] pMono;

	pFont->relativeVtxPos = pFont->gm.gmptGlyphOrigin.y - pFont->fontHeight;

	// �v�f�̒ǉ�
	m_unmapFont[str] = pFont;

	return pFont;
}
	