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
// クラス定義
//--------------------------------------------------------------------------------
// 前方宣言
class CScore;
class CScene2D;
class CEffectPoly;
class CModeResult : public CMode
{
public:
	CModeResult() { SetModeType(MODETYPE::RESULT); }
	CModeResult(int score) : m_score(score) {}
	~CModeResult();

	void Init();	// 初期化
	void Uninit();	// 終了
	void Update();	// 更新
	void Draw();	// 描画
	static void Create(CModeResult *pResult);

private:
	int m_score;
	CScene2D *m_resultButton;
	CEffectPoly *m_resultPoly, *m_resultEnd;
	CScene2D *m_pScoreTex;
	CScore *m_pScore;		// スコア
};

#endif
