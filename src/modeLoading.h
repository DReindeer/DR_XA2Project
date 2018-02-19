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
// クラス
//--------------------------------------------------------------------------------
class ModeLoading : public CMode
{
public:
	ModeLoading(CMode *nextMode);		// コンストラクタ
	~ModeLoading();						// デストラクタ

	void Init();		// 初期化
	void Uninit();		// 終了
	void Update();		// 更新
	void Draw();		// 描画

private:
	CScene2D *m_loading2D[LOADING_MAX_POLYGON];		// 
	float moveRot;

	CMode *m_nextMode;
};

#endif
