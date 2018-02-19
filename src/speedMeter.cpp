//================================================================================
//
//	Auter : KENSUKE WATANABE
//	Data  : 2018/01/15
//
//--------------------------------------------------------------------------------
#include "speedMeter.h"
#include "scene2D.h"
#include "score.h"
#include "mainController.h"

static const char* TEXNAME_METER = "data/TEXTURE/GameUI/speedMeterB.png";				// メーター
static const char* TEXNAME_METER_NEEDLE = "data/TEXTURE/GameUI/speedMeterNeedle.png";	// 針

static const int MAX_SPEED_PAR_H = 120;

//================================================================================
// 初期化
//--------------------------------------------------------------------------------
void CSpeedMeter::Init()
{
	m_needleRot = 0.f;
}

//================================================================================
// 終了
//--------------------------------------------------------------------------------
void CSpeedMeter::Uninit()
{

}

//================================================================================
// 更新
//--------------------------------------------------------------------------------
void CSpeedMeter::Update()
{
	// 現在速度
	D3DXVECTOR3 move = CManager::GetMainController()->GetMove();
	D3DXVECTOR2 moveV2 = D3DXVECTOR2(move.x, move.z);
	float speed = D3DXVec2Length(&moveV2) / CManager::GetMainController()->GetMaxSpeed();

	m_needleRot = (D3DX_PI * 1.5f) * speed;
	m_pSpeedMeterNeedle->SetRot(D3DXVECTOR3(0.f, 0.f, m_needleRot));

	// 速度デジタル表示
	m_pSpeed->SetScore(static_cast<int>(MAX_SPEED_PAR_H * speed));
}

//================================================================================
// 描画
//--------------------------------------------------------------------------------
void CSpeedMeter::Draw()
{

}

//================================================================================
// 生成
//--------------------------------------------------------------------------------
CSpeedMeter *CSpeedMeter::Create(const D3DXVECTOR2 &pos2D, const float &len)
{
	CSpeedMeter *pMeter = new CSpeedMeter();
	pMeter->Init();

	D3DXVECTOR3 pos = D3DXVECTOR3(pos2D.x, pos2D.y, 0.f);
	D3DXVECTOR3 size = D3DXVECTOR3(len, len, 0.f);
	pMeter->m_needleRot = 0.f;

	pMeter->m_pSpeedMeter = CScene2D::Create(pos, D3DXVECTOR3(0.f, 0.f, 0.f), size, TEXNAME_METER, 7);
	pMeter->m_pSpeedMeterNeedle = CScene2D::Create(pos, D3DXVECTOR3(0.f, 0.f, pMeter->m_needleRot), size, TEXNAME_METER_NEEDLE, 8);

	D3DXVECTOR3 speedSize = D3DXVECTOR3(len * 0.3f, len * 0.2f, 0.f);
	D3DXVECTOR3 speedPos = D3DXVECTOR3(pos.x - speedSize.x * 1.5f, pos.y + len * 0.6f, 0.f);

	pMeter->m_pSpeed = CScore::Create(speedPos, 0.f, speedSize, 3, 8);

	// スピードメーター表示速度
	float parSpeed = (MAX_SPEED_PAR_H / 12.0f);
	float x, y;
	int disit = 1;
	float parSpeedValueX = len / 10.f * 1.15f;
	float parSpeedValueY = len / 10.f;
	for (int i = 0; i < 13; i++)
	{
		float speedValue = parSpeed * i;
		float speedRot = (D3DX_PI * 1.5f) * (speedValue / MAX_SPEED_PAR_H);
		if (speedValue >= 10)disit = 2;
		if (speedValue >= 100)disit = 3;
		x = cos((D3DX_PI * 0.75f) + speedRot) * len * 0.6f + pos.x - parSpeedValueX - ((parSpeedValueX / 3) * (disit - 1));
		y = sin((D3DX_PI * 0.75f) + speedRot) * len * 0.6f + pos.y - parSpeedValueY;
		CScore *pScore = CScore::Create(D3DXVECTOR3(x, y, 0), 0.f, D3DXVECTOR3(parSpeedValueX, parSpeedValueY, 1.f), disit, 8);
		pScore->SetScore((int)speedValue);
	}

	return pMeter;
}
