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
// �N���X��`
//--------------------------------------------------------------------------------
// �O���錾
class CSceneFieldX;
class SceneCheckPoint;
class CScene2D;
class CourseIroha : public CCourse
{
public:
	CourseIroha() {}
	~CourseIroha() {}

	void Init();	// ������
	void Uninit();	// �I��
	void Update();	// �X�V
	void Draw();	// �`��
	static CourseIroha *Create(int pry = 4);	// ����

private:

};

#endif
