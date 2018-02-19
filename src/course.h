//================================================================================
//
//	Auter : KENSUKE WATANABE
//	Data  : 2017/12/17
//
//--------------------------------------------------------------------------------
#ifndef _COURSE_H_
#define _COURSE_H_

#include "main.h"
#include "sceneFieldX.h"
#include <assert.h>

//================================================================================
// クラス定義
//--------------------------------------------------------------------------------
// 前方宣言
class CSceneFieldX;
class SceneCheckPoint;
class CMiniMap;
class CScene2D;
class CCourse
{
public:
	CCourse() : m_currentCheckPoint(0), m_goal(false), m_pMiniMap(nullptr) {}
	~CCourse() {}

	void Init();	// 初期化
	void Uninit();	// 終了
	void Update();	// 更新
	void Draw();	// 描画
	static CCourse *Create(int pry = 4);	// 生成

	const D3DXVECTOR3 &GetCheckPoint(const int &index);
	const D3DXVECTOR3 &GetCourseSize() { return m_pCourse->GetSize(); }
	const D3DXVECTOR3 &GetCourseCenterPos() { return m_pCourse->GetCenterPos(); }
	const D3DXVECTOR3 &GetStartPos() { return m_startPos; }
	const D3DXVECTOR2 &GetMapWorldSize() { return m_mapWorldSize; }
	const int &GetCurrentCheckPoint() { return m_currentCheckPoint; }

	const bool &GetGoal()const { return m_goal; }

	int GetGoalCount() { return m_goalCount; }
	int GetReadyCount() { return m_readyCount; }

protected:
	CSceneFieldX *m_pCourse;			// コース
	SceneCheckPoint *m_checkPoint[5];	// チェックポイント
	CMiniMap *m_pMiniMap;				// ミニマップ

	D3DXVECTOR3 m_startPos;				// スタート位置
	D3DXVECTOR2 m_mapWorldSize;			// コースの境界箱のサイズ - ミニマップで使用

	int m_currentCheckPoint;			// 現在経過したチェックポイント数
	bool m_goal;						// ゴール判定

	CScene2D *m_countDown;				// カウントダウン
	CScene2D *m_sign;					// スタート,ゴール
	CScene2D *m_lapSign;				// ラップを知らせる

	int m_goalCount;
	int m_startCount;
	int m_readyCount;
	int m_signDrawCount;
	bool m_signDrawFlag;
};

#endif
