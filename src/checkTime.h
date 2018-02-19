//================================================================================
//
// スピードメーター処理 [checkTime.h]
// Author : KENSUKE WATANABE
//
//================================================================================
#ifndef _CHECKTIME_H_
#define _CHECKTIME_H_

#include "main.h"

static const int CHECK_MAX = 4;

//================================================================================
// クラス定義
//--------------------------------------------------------------------------------
// 前方宣言
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

	void Init();	// 初期化
	void Uninit();	// 終了
	void Update();	// 更新
	void Draw();	// 描画
	static CCheckTime *Create(const D3DXVECTOR2 &pos2D, const D3DXVECTOR2 &size2D);	// 生成

private:
	TIME m_allTime;
	TIME m_chekeTime[CHECK_MAX];

	CScore *m_pAllTime[3];				// 全体時間表示
	CScore *m_pChekeTime[3][CHECK_MAX];	// チェック時間表示

	CScene2D *m_pAllTimeTex;
	CScene2D *m_pCheskTimeTex[CHECK_MAX];

	CScene2D *m_pBack;
};

#endif
