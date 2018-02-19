//================================================================================
//
// �}�l�[�W���[���� [modeGame.h]
// Author : KENSUKE WATANABE
//
//================================================================================
#ifndef _MODEGAME_H_
#define _MODEGAME_H_

#include "mode.h"

// �O���錾
class CCourse;
class CSpeedMeter;
class CCheckTime;

//================================================================================
// �N���X��`
//--------------------------------------------------------------------------------
class CModeGame : public CMode
{
public:
	CModeGame();
	~CModeGame();

	void Init();			// ������
	void Uninit();			// �I��
	void Update();			// �X�V
	void Draw();			// �`��
	static void Create(CModeGame *pGame);	// �z�u

	CCourse *GetCourse() { return m_pCourse; }

private:
	CCourse *m_pCourse;				// �R�[�X
	CSpeedMeter *m_pSpeedMeter;		// �X�s�[�h���[�^�[
	CCheckTime *m_pCheckTime;		// ���Ԍv��
};

#endif
