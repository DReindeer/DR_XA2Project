//================================================================================
//
//	Auter : KENSUKE WATANABE
//	Data  : 2017/12/17
//
//--------------------------------------------------------------------------------
#ifndef _COURSEAKINA_H_
#define _COURSEAKINA_H_

#include "course.h"

//================================================================================
// クラス定義
//--------------------------------------------------------------------------------
// 前方宣言
class CSceneFieldX;
class SceneCheckPoint;
class CScene2D;
class CourseAkina : public CCourse
{
public:
	CourseAkina() {}
	~CourseAkina() {}

	void Init();	// 初期化
	void Uninit();	// 終了
	void Update();	// 更新
	void Draw();	// 描画
	static CourseAkina *Create(int pry = 4);	// 生成

private:

};

#endif
