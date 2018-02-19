//================================================================================
//
//	Auter : KENSUKE WATANABE
//	Data  : 2018/02/10
//
//--------------------------------------------------------------------------------
#ifndef _MODERESULT_H_
#define _MODERESULT_H_

#include "mode.h"

//================================================================================
// �N���X��`
//--------------------------------------------------------------------------------
// �O���錾
class CScore;
class CScene2D;
class CEffectPoly;
class CModeResult : public CMode
{
public:
	CModeResult() { SetModeType(MODETYPE::RESULT); }
	CModeResult(int score) : m_score(score) {}
	~CModeResult();

	void Init();	// ������
	void Uninit();	// �I��
	void Update();	// �X�V
	void Draw();	// �`��
	static void Create(CModeResult *pResult);

private:
	int m_score;
	CScene2D *m_resultButton;
	CEffectPoly *m_resultPoly, *m_resultEnd;
	CScene2D *m_pScoreTex;
	CScore *m_pScore;		// �X�R�A
};

#endif
