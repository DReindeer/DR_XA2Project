//================================================================================
//
// �X�s�[�h���[�^�[���� [checkTime.h]
// Author : KENSUKE WATANABE
//
//================================================================================
#ifndef _CHECKTIME_H_
#define _CHECKTIME_H_

#include "main.h"

static const int CHECK_MAX = 4;

//================================================================================
// �N���X��`
//--------------------------------------------------------------------------------
// �O���錾
class CScene2D;
class CScore;
class CCheckTime
{
public:

	typedef struct
	{
		int flame, sec, min;
	}TIME;

	CCheckTime() : m_pBack(nullptr){}
	~CCheckTime() {}

	void Init();	// ������
	void Uninit();	// �I��
	void Update();	// �X�V
	void Draw();	// �`��
	static CCheckTime *Create(const D3DXVECTOR2 &pos2D, const D3DXVECTOR2 &size2D);	// ����

private:
	TIME m_allTime;
	TIME m_chekeTime[CHECK_MAX];

	CScore *m_pAllTime[3];				// �S�̎��ԕ\��
	CScore *m_pChekeTime[3][CHECK_MAX];	// �`�F�b�N���ԕ\��

	CScene2D *m_pAllTimeTex;
	CScene2D *m_pCheskTimeTex[CHECK_MAX];

	CScene2D *m_pBack;
};

#endif
