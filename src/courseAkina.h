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
// �N���X��`
//--------------------------------------------------------------------------------
// �O���錾
class CSceneFieldX;
class SceneCheckPoint;
class CScene2D;
class CourseAkina : public CCourse
{
public:
	CourseAkina() {}
	~CourseAkina() {}

	void Init();	// ������
	void Uninit();	// �I��
	void Update();	// �X�V
	void Draw();	// �`��
	static CourseAkina *Create(int pry = 4);	// ����

private:

};

#endif