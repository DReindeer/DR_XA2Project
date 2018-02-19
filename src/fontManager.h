
#ifndef _FONTMANAGER_H_
#define _FONTMANAGER_H_

#include "main.h"
#include <unordered_map>

class CFontManager
{
public:
	typedef struct
	{
		TEXTMETRIC tm;
		GLYPHMETRICS gm;
		int fontWidth, fontHeight;
		int relativeVtxPos;
		IDirect3DTexture9 *pTex;
	}FONT;

	CFontManager() {};
	~CFontManager() {};

	void Init(HINSTANCE hInst);		// 初期化
	void Uninit();		// 終了

	static FONT *SetFontTex(int fontWeight, const wchar_t wcFont[]);

	FONT *GetFont(const std::string &strFilePass) const { 
		if (m_unmapFont.count(strFilePass) >= 1) {
			return m_unmapFont[strFilePass]; 
		};
		return nullptr;
	}

	bool Release(const std::string &strFilePass);				// 解放
	static void ReleaseAll();									// 全開放

private:
	HANDLE m_hFont;
	static std::unordered_map<std::string, FONT*> m_unmapFont;		// インターフェイス
};

#endif