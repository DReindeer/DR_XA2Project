//================================================================================
//
//	Auter : KENSUKE WATANABE
//	Data  : 2018/01/15
//
//--------------------------------------------------------------------------------
#include "checkTime.h"
#include "scene2D.h"
#include "score.h"
#include "mainController.h"
#include "mode.h"
#include "modeGame.h"
#include "course.h"

static const char* TEXNAME_WHITE = "data/TEXTURE/black.png";		// 背景
static const char* TEXNAME_TIME = "data/TEXTURE/GameUI/time.png";	// 時間
static const int DIGIT = 4;

static const int ALL_R = 208;
static const int ALL_G = 208;
static const int ALL_B = 32;
static const int ALL_A = 255;

static const int C_DEF_R = 255;
static const int C_DEF_G = 255;
static const int C_DEF_B = 255;
static const int C_DEF_A = 32;

static const int C_CURRENT_R = 255;
static const int C_CURRENT_G = 32;
static const int C_CURRENT_B = 32;
static const int C_CURRENT_A = 255;

//================================================================================
// 初期化
//--------------------------------------------------------------------------------
void CCheckTime::Init()
{
	m_allTime.flame = 0;
	m_allTime.min = 0;
	m_allTime.sec = 0;
	for (int i = 0; i < CHECK_MAX; i++)
	{
		m_chekeTime[i].flame = 0;
		m_chekeTime[i].min = 0;
		m_chekeTime[i].sec = 0;
	}
}

//================================================================================
// 終了
//--------------------------------------------------------------------------------
void CCheckTime::Uninit()
{

}

//================================================================================
// 更新
//--------------------------------------------------------------------------------
void CCheckTime::Update()
{
	// モード取得
	CMode *pMode = CManager::GetMode();
	if (!pMode)return;

	CCourse *pCourse;				// コース
	CModeGame *pModeGame = nullptr;	// モードゲーム
	
	// 開始前と終了後はタイムをカウントしない
	if (pMode->GetModeType() == CMode::GAME)
	{
		pModeGame = static_cast<CModeGame*>(CManager::GetMode());
		pCourse = pModeGame->GetCourse();
		bool noGoal = pCourse->GetGoalCount() == 0;
		bool start = pCourse->GetReadyCount() == 0;
		if (!noGoal || !start) return;
	}

	pCourse = pModeGame->GetCourse();
	int index = pCourse->GetCurrentCheckPoint();

	// 全体時間の加算
	if (m_allTime.min < 100)
	{
		m_allTime.flame++;
		m_pAllTime[2]->SetScore(m_allTime.flame);
		if (m_allTime.flame % 60 == 0)
		{
			m_allTime.flame = 0;
			m_allTime.sec++;
			m_pAllTime[2]->SetScore(m_allTime.flame);
			m_pAllTime[1]->SetScore(m_allTime.sec);
			if (m_allTime.sec % 60 == 0)
			{
				m_allTime.sec = 0;
				m_allTime.min++;
				m_pAllTime[1]->SetScore(m_allTime.sec);
				m_pAllTime[0]->SetScore(m_allTime.min);
			}
		}
	}

	// チェックポイント毎の時間の加算
	if (m_chekeTime[index].min < 100)
	{
		m_chekeTime[index].flame++;
		m_pChekeTime[2][index]->SetScore(m_chekeTime[index].flame);
		if (m_chekeTime[index].flame % 60 == 0)
		{
			m_chekeTime[index].flame = 0;
			m_chekeTime[index].sec++;
			m_pChekeTime[2][index]->SetScore(m_chekeTime[index].flame);
			m_pChekeTime[1][index]->SetScore(m_chekeTime[index].sec);
			if (m_chekeTime[index].sec % 60 == 0)
			{
				m_chekeTime[index].sec = 0;
				m_chekeTime[index].min++;
				m_pChekeTime[1][index]->SetScore(m_chekeTime[index].sec);
				m_pChekeTime[0][index]->SetScore(m_chekeTime[index].min);
			}
		}
	}
	// 色設定
	for (int i = 0; i < CHECK_MAX; i++)
	{
		if (i == index)
		{
			m_pCheskTimeTex[i]->SetCol(C_CURRENT_R, C_CURRENT_G, C_CURRENT_B, C_CURRENT_A);
			m_pChekeTime[0][i]->SetCol(C_CURRENT_R, C_CURRENT_G, C_CURRENT_B, C_CURRENT_A);
			m_pChekeTime[1][i]->SetCol(C_CURRENT_R, C_CURRENT_G, C_CURRENT_B, C_CURRENT_A);
			m_pChekeTime[2][i]->SetCol(C_CURRENT_R, C_CURRENT_G, C_CURRENT_B, C_CURRENT_A);
			continue;
		}
		else if (i > index)
		{
			m_pCheskTimeTex[i]->SetCol(C_DEF_R, C_DEF_G, C_DEF_B, C_DEF_A);
			m_pChekeTime[0][i]->SetCol(C_DEF_R, C_DEF_G, C_DEF_B, C_DEF_A);
			m_pChekeTime[1][i]->SetCol(C_DEF_R, C_DEF_G, C_DEF_B, C_DEF_A);
			m_pChekeTime[2][i]->SetCol(C_DEF_R, C_DEF_G, C_DEF_B, C_DEF_A);
		}
		else if (i < index)
		{
			m_pCheskTimeTex[i]->SetCol(255, 255, 255, 255);
			m_pChekeTime[0][i]->SetCol(255, 255, 255, 255);
			m_pChekeTime[1][i]->SetCol(255, 255, 255, 255);
			m_pChekeTime[2][i]->SetCol(255, 255, 255, 255);
		}
	}
}

//================================================================================
// 描画
//--------------------------------------------------------------------------------
void CCheckTime::Draw()
{

}

//================================================================================
// 生成
//--------------------------------------------------------------------------------
CCheckTime *CCheckTime::Create(const D3DXVECTOR2 &pos2D, const D3DXVECTOR2 &size2D)
{
	CCheckTime *pCheck = new CCheckTime();
	pCheck->Init();

	// 背景
	D3DXVECTOR3 pos = D3DXVECTOR3(pos2D.x, pos2D.y, 0.f);
	D3DXVECTOR3 size = D3DXVECTOR3(size2D.x, size2D.y, 0.f);
	pCheck->m_pBack = CScene2D::Create(pos, D3DXVECTOR3(0.f, 0.f, 0.f), size, TEXNAME_WHITE, 6);
	pCheck->m_pBack->SetCol(255, 255, 255, 192);

	// タイム表示
	D3DXVECTOR3 timeSize = D3DXVECTOR3(size.x * 0.1f, size.y * 2 * 0.15f, 0.f);
	D3DXVECTOR2 interval = D3DXVECTOR2(size.x * 0.1f, size.y * 2 * 0.25f * 0.166666f);

	// 左の文字
	D3DXVECTOR3 texPos = D3DXVECTOR3((pos.x - size.x * 0.5f), (pos.y - size.y + timeSize.y * 0.5f + interval.y), 0.f);
	D3DXVECTOR3 texSize = D3DXVECTOR3(size.x * 0.5f, timeSize.y * 0.8f, 0.f);
	pCheck->m_pAllTimeTex = CScene2D::Create(texPos, D3DXVECTOR3(0.f, 0.f, 0.f), texSize, TEXNAME_TIME, 7);
	pCheck->m_pAllTimeTex->SetTexturePos(D3DXVECTOR2(0.f, 0.f), D3DXVECTOR2(1.f, 0.2f));
	pCheck->m_pAllTimeTex->SetCol(ALL_R, ALL_G, ALL_B, ALL_A);

	// 全フレーム
	D3DXVECTOR3 timePos;
	for (int x = 0; x < 10; x++)
	{
		if (x == 1)
		{
			timePos = D3DXVECTOR3((pos.x) + interval.x * x, (pos.y - size.y) + interval.y, 0.f);
			pCheck->m_pAllTime[0] = CScore::Create(timePos, 0.f, timeSize, 2, 7);
				pCheck->m_pAllTime[0]->SetCol(ALL_R, ALL_G, ALL_B, ALL_A);
		}
		if (x == 4)
		{
			timePos = D3DXVECTOR3((pos.x) + interval.x * x, (pos.y - size.y) + interval.y, 0.f);
			pCheck->m_pAllTime[1] = CScore::Create(timePos, 0.f, timeSize, 2, 7);
			pCheck->m_pAllTime[1]->SetCol(ALL_R, ALL_G, ALL_B, ALL_A);
		}
		if (x == 7)
		{
			timePos = D3DXVECTOR3((pos.x) + interval.x * x, (pos.y - size.y) + interval.y, 0.f);
			pCheck->m_pAllTime[2] = CScore::Create(timePos, 0.f, timeSize, 2, 7);
			pCheck->m_pAllTime[2]->SetCol(ALL_R, ALL_G, ALL_B, ALL_A);
		}
	}

	// チェックごとのフレーム
	float timePosY = (pos.y - size.y) + timeSize.y + interval.y * 2;
	float texPosY = (pos.y - size.y + timeSize.y * 0.5f + interval.y) + timeSize.y + interval.y;
	for (int y = 0; y < CHECK_MAX; y++)
	{
		// 左の文字
		texPos = D3DXVECTOR3((pos.x - size.x * 0.5f), texPosY + ((timeSize.y + interval.y) * y), 0.f);
		pCheck->m_pCheskTimeTex[y] = CScene2D::Create(texPos, D3DXVECTOR3(0.f, 0.f, 0.f), texSize, TEXNAME_TIME, 7);
		pCheck->m_pCheskTimeTex[y]->SetTexturePos(D3DXVECTOR2(0.f, 0.2f + 0.2f * y), D3DXVECTOR2(1.f, 0.2f));
		pCheck->m_pCheskTimeTex[y]->SetCol(C_DEF_R, C_DEF_G, C_DEF_B, C_DEF_A);

		for (int x = 0; x < 10; x++)
		{
			if (x == 1)
			{
				timePos = D3DXVECTOR3((pos.x) + interval.x * x, timePosY + ((timeSize.y + interval.y) * y), 0.f);
				pCheck->m_pChekeTime[0][y] = CScore::Create(timePos, 0.f, timeSize, 2, 7);
				pCheck->m_pChekeTime[0][y]->SetCol(C_DEF_R, C_DEF_G, C_DEF_B, C_DEF_A);
			}
			if (x == 4)
			{
				timePos = D3DXVECTOR3((pos.x) + interval.x * x, timePosY + ((timeSize.y + interval.y) * y), 0.f);
				pCheck->m_pChekeTime[1][y] = CScore::Create(timePos, 0.f, timeSize, 2, 7);
				pCheck->m_pChekeTime[1][y]->SetCol(C_DEF_R, C_DEF_G, C_DEF_B, C_DEF_A);
			}
			if (x == 7)
			{
				timePos = D3DXVECTOR3((pos.x) + interval.x * x, timePosY + ((timeSize.y + interval.y) * y), 0.f);
				pCheck->m_pChekeTime[2][y] = CScore::Create(timePos, 0.f, timeSize, 2, 7);
				pCheck->m_pChekeTime[2][y]->SetCol(C_DEF_R, C_DEF_G, C_DEF_B, C_DEF_A);
			}
		}
	}

	return pCheck;
}
