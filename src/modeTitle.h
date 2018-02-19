//================================================================================
//
// マネージャー処理 [modeTitle.h]
// Author : KENSUKE WATANABE
//
//================================================================================
#ifndef _MODETITLE_H_
#define _MODETITLE_H_

#include "mode.h"

//================================================================================
// クラス定義
//--------------------------------------------------------------------------------
// 前方宣言
class CScore;
class CSceneField;
class CScene3DPoly;
class CScene2D;
class CSceneModel;
class CModeTitle : public CMode
{
public:
	CModeTitle();
	~CModeTitle() {}

	void Init();	// 初期化
	void Uninit();	// 終了
	void Update();	// 更新
	void Draw();	// 描画
	static void Create(CModeTitle *pTitle);	// 生成

	const int &GetGradR() { return m_gradR; }
	const int &GetGradG() { return m_gradG; }
	const int &GetGradB() { return m_gradB; }
	void SetGrad(const int &r, const int &g, const int &b) { m_gradR = r; m_gradG = g; m_gradB = b; }
	void AddGrad(const int &r, const int &g, const int &b) { m_gradR += r; m_gradG += g; m_gradB += b; }

private:
	int m_createEffectFrame;
	int m_gradR, m_gradG, m_gradB;
	bool m_bR, m_bG, m_bB;
	float m_titleCnt;
	CScene3DPoly *m_pUnderCircle;
	CScene2D *m_pTitleLogo;
	CSceneModel *m_pModel[5];
};

#endif
