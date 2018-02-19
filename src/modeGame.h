//================================================================================
//
// マネージャー処理 [modeGame.h]
// Author : KENSUKE WATANABE
//
//================================================================================
#ifndef _MODEGAME_H_
#define _MODEGAME_H_

#include "mode.h"

// 前方宣言
class CCourse;
class CSpeedMeter;
class CCheckTime;

//================================================================================
// クラス定義
//--------------------------------------------------------------------------------
class CModeGame : public CMode
{
public:
	CModeGame();
	~CModeGame();

	void Init();			// 初期化
	void Uninit();			// 終了
	void Update();			// 更新
	void Draw();			// 描画
	static void Create(CModeGame *pGame);	// 配置

	CCourse *GetCourse() { return m_pCourse; }

private:
	CCourse *m_pCourse;				// コース
	CSpeedMeter *m_pSpeedMeter;		// スピードメーター
	CCheckTime *m_pCheckTime;		// 時間計測
};

#endif
