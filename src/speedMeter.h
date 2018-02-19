//================================================================================
//
// スピードメーター処理 [speedMeter.h]
// Author : KENSUKE WATANABE
//
//================================================================================
#ifndef _SPEEDMATER_H_
#define _SPEEDMATER_H_

#include "main.h"

//================================================================================
// クラス定義
//--------------------------------------------------------------------------------
// 前方宣言
class CScene2D;
class CScore;
class CSpeedMeter
{
public:
	CSpeedMeter() : m_needleRot(0.f), m_pSpeedMeter(nullptr), m_pSpeedMeterNeedle(nullptr){}
	~CSpeedMeter() {}

	void Init();	// 初期化
	void Uninit();	// 終了
	void Update();	// 更新
	void Draw();	// 描画
	static CSpeedMeter *Create(const D3DXVECTOR2 &pos2D, const float &len);	// 生成

//	static const CScore &GetScore(){ return *m_pScore; }		// スコアの取得

private:
	float m_needleRot;

	CScene2D *m_pSpeedMeter;
	CScene2D *m_pSpeedMeterNeedle;
	CScore *m_pSpeed;				// スピード表示
};

#endif
