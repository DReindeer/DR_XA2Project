//--------------------------------------------------------------------------------
//
//	title : ポリゴン表示プログラム[ CSceneFont.h ]	Auter : KENSUKE WATANABE
//													Data  : 2016/06/18
//
//--------------------------------------------------------------------------------
#ifndef _SCENEFONT_H_
#define _SCENEFONT_H_

#include "main.h"
#include <string>
#include "scene.h"
#include "scene2D.h"

//--------------------------------------------------------------------------------
// クラス
//--------------------------------------------------------------------------------
class CSceneFont : public CScene
{
public:
	CSceneFont(int priority = 4) : CScene(priority){}
	~CSceneFont() {}

	void Init();				// 初期化
	void Uninit();				// 終了
	void Update();				// 更新
	void Draw();				// 描画
	static CSceneFont *Create(const D3DXVECTOR3 &pos, const D3DXVECTOR3 &rot, const float &height, const int &weight, const wchar_t *wcFont);

private:
	std::string m_strFilePass;	// テクスチャFilePass
	GLYPHMETRICS m_gm;
	int m_fontWidth, m_fontHeight, m_fontRelativeVtxPos;
};

#endif
