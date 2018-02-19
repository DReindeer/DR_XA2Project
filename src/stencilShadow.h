//================================================================================
//
//	title : ポリゴン表示プログラム[ sceneCircleShadow.h ]	Auter : KENSUKE WATANABE
//													Data  : 2016/05/30
//
//--------------------------------------------------------------------------------
#ifndef _STENCILSHADOW_H_
#define _STENCILSHADOW_H_

#include "main.h"
#include <string>
#include "scene2D.h"

//--------------------------------------------------------------------------------
// クラス
//--------------------------------------------------------------------------------
class CStencilShadow : public CScene2D
{
public:
	CStencilShadow(int priority = 7) :CScene2D(priority) { m_classType = CLASSTYPE::SCENE_2D; }
	~CStencilShadow() {}

	void Init();				// 初期化
	void Uninit();				// 終了
	void Update();				// 更新
	void Draw();				// 描画
	static CStencilShadow *Create(const D3DXVECTOR3 &pos, const D3DXVECTOR3 &rot, const D3DXVECTOR3 &size, const std::string &strFilePass, int priority = 4);

protected:

private:
};

#endif
