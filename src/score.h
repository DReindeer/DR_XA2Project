//--------------------------------------------------------------------------------
//
// シーン2D処理 [score.h]
// Author : KENSUKE WATANABE
//
//--------------------------------------------------------------------------------
#ifndef _SCORE_H_
#define _SCORE_H_

//--------------------------------------------------------------------------------
// ヘッダファイル
//--------------------------------------------------------------------------------
#include "Scene.h"
#include "number.h"

//--------------------------------------------------------------------------------
// 定数定義
//--------------------------------------------------------------------------------
#define MAX_DIGIT (8)

//--------------------------------------------------------------------------------
// クラス定義
//--------------------------------------------------------------------------------
class CScore : public CScene
{
public:
	CScore(int priority = 4) :CScene(priority), m_disit(MAX_DIGIT), m_score(0){};			// コンストラクタ
	CScore(int digit, int priority = 4) :CScene(priority), m_disit(digit), m_score(0) {};	// コンストラクタ
	~CScore() {};												// デストラクタ

	void Init();	// 初期化
	void Uninit();	// 終了
	void Update();	// 更新
	void Draw();	// 描画

	static CScore *Create(D3DXVECTOR3 pos, float rot, D3DXVECTOR3 size, int digit, int priority = 4);	// 生成
	
	void SetScore(int nValue) { m_score = nValue; }		// スコアの設定
	void AddScore(int nValue) { m_score += nValue; }	// スコアの加算
	int* GetScore() { return &m_score; }				// スコアの取得

	void SetCol(int r, int g, int b, int a) { 
		for (int i = 0; i < m_disit; i++) { m_apNumber[i].SetCol(r, g, b, a); } 
	}

private:
	const int m_disit;
	CNumber* m_apNumber;	// ナンバー
	int m_score;			// スコア総数
};

#endif
