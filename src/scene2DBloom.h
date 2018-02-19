//================================================================================
//
//	title : ポリゴン表示プログラム[ CScene2D.h ]	Auter : KENSUKE WATANABE
//													Data  : 2016/05/30
//
//--------------------------------------------------------------------------------
#ifndef _SCENE2DBLOOM_H_
#define _SCENE2DBLOOM_H_

#include "scene2D.h"

//--------------------------------------------------------------------------------
// 前方宣言
//--------------------------------------------------------------------------------
class CTexManager;

//--------------------------------------------------------------------------------
// クラス
//--------------------------------------------------------------------------------
class CScene2DBloom : public CScene2D
{
public:
	CScene2DBloom(int priority = 4) :CScene2D(priority) { m_classType = CLASSTYPE::SCENE_2D; }
	~CScene2DBloom();

	void Init();				// 初期化
	void Uninit();				// 終了
	void Update();				// 更新
	void Draw();				// 描画
	static CScene2DBloom *Create(const D3DXVECTOR3 &pos, const D3DXVECTOR3 &rot, const D3DXVECTOR3 &size, const std::string &strFilePass, int priority = 4, CTexManager *pUseTexManager = nullptr);
	
};

#endif
