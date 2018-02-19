//================================================================================
//
//	Auter : KENSUKE WATANABE
//	Data  : 2018/01/15
//
//--------------------------------------------------------------------------------
#include "miniMap.h"
#include "scene2D.h"
#include "scene2DCircle.h"
#include "scene2DCircleMap.h"
#include "manager.h"
#include "mainController.h"

// 
//--------------------------------------------------------------------------------
static const char* TEXNAME_UNDERLIGHT = "data/TEXTURE/whiteCircle.png";

// 初期化
//--------------------------------------------------------------------------------
void CMiniMap::Init()
{
	m_pos = D3DXVECTOR2(0.f, 0.f);
	m_rot = 0.f;
	m_len = 0.f;
	m_wToMap = D3DXVECTOR2(0.f, 0.f);
}

// 終了
//--------------------------------------------------------------------------------
void CMiniMap::Uninit()
{

}

// 更新
//--------------------------------------------------------------------------------
void CMiniMap::Update()
{
	// プレイヤー座標をミニマップ上の座標に変換
	//--------------------------------------------------------------------------------
	CMainController *mainController = CManager::GetMainController();
	D3DXVECTOR3 mainControllerPos = mainController->GetPos();

	// マップ上のプレイヤー座標算出
	D3DXVECTOR2 mapPlayerPos = D3DXVECTOR2(0.f, 0.f);
	mapPlayerPos.x = mainControllerPos.x * m_wToMap.x * -1.f + 0.5f;
	mapPlayerPos.y = mainControllerPos.z * m_wToMap.y + 0.5f;

	// 座標と回転量をマップに反映
	m_pMap->SetPosUV(mapPlayerPos);
	m_pMap->SetRotUV(mainController->GetRot().y);
}

// 描画
//--------------------------------------------------------------------------------
void CMiniMap::Draw()
{

}

// 生成
//--------------------------------------------------------------------------------
CMiniMap *CMiniMap::Create(const D3DXVECTOR2 &pos, const float &rot, const float &len, const D3DXVECTOR2 &mapWorldSize, std::string mapFilePass)
{
	CMiniMap *pMiniMap = new CMiniMap();
	pMiniMap->Init();

	// 引数の代入
	pMiniMap->m_pos = pos;
	pMiniMap->m_rot = rot;
	pMiniMap->m_len = len;
	pMiniMap->m_filePass = mapFilePass;

	D3DXVECTOR2 screenSize = CManager::GetRenderer()->GetScreenSize();

	// ミニマップ枠生成
	pMiniMap->m_pMapFrame = CScene2DCircleMap::Create(D3DXVECTOR3(pos.x, pos.y, 0.f), D3DXVECTOR3(0.f, 0.f, rot), len, 32, D3DXVECTOR2(0.5f, 0.5f), 0.5f, TEXNAME_UNDERLIGHT, 7);
	pMiniMap->m_pMapFrame->SetCol(0, 0, 0, 128);

	// ミニマップ生成
	pMiniMap->m_pMap = CScene2DCircleMap::Create(D3DXVECTOR3(pos.x, pos.y, 0.f), D3DXVECTOR3(0.f, 0.f, rot), len, 32, D3DXVECTOR2(0.f, 0.f), 0.2f , mapFilePass, 8);

	// 縮尺計算
	pMiniMap->m_wToMap.x = 1.f / mapWorldSize.x;
	pMiniMap->m_wToMap.y = 1.f / mapWorldSize.y;

	// プレイヤー位置の丸を作成
	pMiniMap->m_pPlayerPoint = CScene2D::Create(D3DXVECTOR3(pos.x, pos.y, 0.f), D3DXVECTOR3(0.f, 0.f, rot), D3DXVECTOR3(4.f, 4.f, 0.f), TEXNAME_UNDERLIGHT, 9);
	pMiniMap->m_pPlayerPoint->SetCol(255, 8, 8, 255);

	return pMiniMap;
}
