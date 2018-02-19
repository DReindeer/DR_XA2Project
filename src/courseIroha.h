//================================================================================
//
//	Auter : KENSUKE WATANABE
//	Data  : 2017/12/17
//
//--------------------------------------------------------------------------------
#ifndef _COURSEIROHA_H_
#define _COURSEIROHA_H_

#include "course.h"

//================================================================================
// クラス定義
//--------------------------------------------------------------------------------
// 前方宣言
class CSceneFieldX;
class SceneCheckPoint;
class CScene2D;
class CourseIroha : public CCourse
{
public:
	CourseIroha() {}
	~CourseIroha() {}

	void Init();	// 初期化
	void Uninit();	// 終了
	void Update();	// 更新
	void Draw();	// 描画
	static CourseIroha *Create(int pry = 4);	// 生成

private:

};

#endif
