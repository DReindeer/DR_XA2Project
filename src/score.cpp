//================================================================================
//
//	Auter : KENSUKE WATANABE
//	Data  : 2018/02/10
//
//--------------------------------------------------------------------------------
#include "main.h"
#include "manager.h"
#include "scene.h"
#include "score.h"
#include "renderer.h"
#include "number.h"

//--------------------------------------------------------------------------------
// CScore::初期化処理
//--------------------------------------------------------------------------------
void CScore::Init()
{
	m_score = 0;
}

//--------------------------------------------------------------------------------
// CScore::終了処理
//--------------------------------------------------------------------------------
void CScore::Uninit()
{
	for(int nCnt = 0; nCnt < m_disit; nCnt++)
	{
		m_apNumber[nCnt].Uninit();
	}
}

//--------------------------------------------------------------------------------
// CScore::更新処理
//--------------------------------------------------------------------------------
void CScore::Update()
{
	int num, value = m_score;
	for(int nCnt = m_disit - 1; nCnt >= 0; nCnt--)
	{
		num = value % 10;
		m_apNumber[nCnt].SetNumber(num);
		if(value != 0)value /= 10;
	}
	
	for(int nCnt = 0; nCnt < m_disit; nCnt++)
	{
		m_apNumber[nCnt].Update();
	}
}

//--------------------------------------------------------------------------------
// CScore::描画処理
//--------------------------------------------------------------------------------
void CScore::Draw()
{
	for(int nCnt = 0; nCnt < m_disit; nCnt++)
	{
		m_apNumber[nCnt].Draw();
	}
}

//--------------------------------------------------------------------------------
// CScore::作成処理
//--------------------------------------------------------------------------------
CScore *CScore::Create(D3DXVECTOR3 pos, float rot, D3DXVECTOR3 size, int digit, int priority)
{
	// CScoreの生成
	CScore *pScore;
	pScore = new CScore(digit, priority);
	pScore->Init();

	pScore->m_pos = pos;		// 引数より座標を代入
	pScore->m_size = size;		// 大きさ
	pScore->m_rot = D3DXVECTOR3(0.f, 0.f, rot);

	// ナンバーの作成
	pScore->m_apNumber = CNumber::Create(pScore->m_pos, pScore->m_rot, pScore->m_size, pScore->m_disit);

	return pScore;
}
