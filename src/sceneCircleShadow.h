//================================================================================
//
//	title : ポリゴン表示プログラム[ sceneCircleShadow.h ]	Auter : KENSUKE WATANABE
//													Data  : 2016/05/30
//
//--------------------------------------------------------------------------------
#ifndef _SCENECIRCLESHADOW_H_
#define _SCENECIRCLESHADOW_H_

#include "main.h"
#include <string>
#include "sceneModel.h"

//--------------------------------------------------------------------------------
// クラス
//--------------------------------------------------------------------------------
class CSceneCircleShadow : public CSceneModel
{
public:
	CSceneCircleShadow(int priority = 4) : CSceneModel(priority) { m_classType = CLASSTYPE::SCENE_2D; }
	~CSceneCircleShadow() {}

	void Init();
	void Uninit();
	void Update();
	void Draw();

	// モデル生成
	static CSceneCircleShadow *Create(const D3DXVECTOR3 &pos, const D3DXVECTOR3 &rot, const D3DXVECTOR3 &scl, const std::string &strFilePass, int pry = 0);

private:

};

#endif
