//================================================================================
//
//	Auter : KENSUKE WATANABE
//	Data  : 2018/01/15
//
//--------------------------------------------------------------------------------
#ifndef _LOADING_H_
#define _LOADING_H_

#include "mode.h"

class CScene2D;
class CTexManager;

#define LOADING_MAX_POLYGON (5)

//--------------------------------------------------------------------------------
// �N���X
//--------------------------------------------------------------------------------
class ModeLoading : public CMode
{
public:
	ModeLoading(CMode *nextMode);		// �R���X�g���N�^
	~ModeLoading();						// �f�X�g���N�^

	void Init();		// ������
	void Uninit();		// �I��
	void Update();		// �X�V
	void Draw();		// �`��

private:
	CScene2D *m_loading2D[LOADING_MAX_POLYGON];		// 
	float moveRot;

	CMode *m_nextMode;
};

#endif
