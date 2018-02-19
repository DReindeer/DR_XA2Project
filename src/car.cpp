//================================================================================
//
//	Auter : KENSUKE WATANABE
//	Data  : 2017/08/24
//
//--------------------------------------------------------------------------------
#include <assert.h>
#include "car.h"
#include "manager.h"
#include "renderer.h"
#include "camera.h"
#include "debugStr.h"
#include "sceneModel.h"
#include "sceneFieldX.h"
#include "sceneCircleShadow.h"
#include "effect3DPoly.h"
#include "stencilShadow.h"
// XAudio2
#include "xa2SourceVoiceManager.h"
#include "xa2SourceVoice2D.h"
#include "xa2SourceVoice3D.h"

//================================================================================
// 定数定義
//--------------------------------------------------------------------------------
static const char* MODELNAME_STANDARDCAR_BODY = "data/MODEL/car/Sbody.x";
static const char* MODELNAME_STANDARDCAR_TIRE = "data/MODEL/car/Stire.x";
static const char* TEXNAME_EFFECT = "data/TEXTURE/smoke.png";

static const D3DXVECTOR3 STANDARDCAR_RF = D3DXVECTOR3(0.35f, 0.1f, -0.50f);
static const D3DXVECTOR3 STANDARDCAR_RB = D3DXVECTOR3(0.35f, 0.1f, 0.67f);

static const float STANDARDCAR_BASEPOS_Y = 1.f;
static const float MAX_SPEED = 1.1f;
static const float BREAK = 0.97f;
static const float FRICTIONAL = 0.993f;
static const float MOVE_SPEED = 0.03f;					// プレイヤー移動速度
static const float MOVE_BACK_SPEED = 0.003f;			// 
static const float MOVE_ROT_SPEED = D3DX_PI * 0.001f;	// プレイヤー旋回速度
static const float ROT_FRICTIONAL = 0.95f;

static const float ROT_LIMIT = D3DX_PI * 0.015f;		// プレイヤー旋回限界の絶対値

//================================================================================
// 静的メンバ変数
//--------------------------------------------------------------------------------
std::unordered_map<std::string, CCar*> CCar::m_umapCarManager;
CSceneFieldX *CCar::m_pField = nullptr;

//================================================================================
// コンストラクタ
//--------------------------------------------------------------------------------
CCar::CCar()
	: m_pBody(nullptr), m_pos(D3DXVECTOR3(0.f, 0.f, 0.f)), m_posOld(D3DXVECTOR3(0.f, 0.f, 0.f)), 
	m_rot(D3DXVECTOR3(0.f, 0.f, 0.f)), m_move(D3DXVECTOR3(0.f, 0.f, 0.f)), m_turn(0.f),
	m_moveTurn(0.f), m_frictional(FRICTIONAL), m_camPosAt(D3DXVECTOR3(0.f, 0.f, 0.f)),
	m_rotTire(0.f), m_brake(BREAK), m_speed(0.f), m_effCnt(0),
	m_maxSpeed(MAX_SPEED * CManager::GetMasterScale()), m_moveSpeed(MOVE_SPEED * CManager::GetMasterScale()), m_moveBackSpeed(MOVE_BACK_SPEED * CManager::GetMasterScale()),
	m_direction(0.f), m_skid(false), m_idling(false), m_brakeSound(false), m_colSECnt(0)
{
	for (int n = 0; n < 4; n++) { m_pTire[n] = nullptr; }	// タイヤ *4
	m_pField = nullptr;
}

//================================================================================
// デストラクタ
//--------------------------------------------------------------------------------
CCar::~CCar()
{

}

//================================================================================
// 生成
//--------------------------------------------------------------------------------
CCar* CCar::Create(D3DXVECTOR3 pos, D3DXVECTOR3 rot, std::string onlyCarName, int pry)
{
	CCar* pCar = new CCar();
	pCar->m_pos = pos;
	pCar->m_rot = rot;

	// ボディ
	pCar->m_pBody = CSceneModel::Create(D3DXVECTOR3(0.f, 0.f, 0.f), D3DXVECTOR3(0.f, 0.f, 0.f), D3DXVECTOR3(2.5f, 2.5f, 2.5f), MODELNAME_STANDARDCAR_BODY, pry);
	pCar->m_pBody->SetBasePos(D3DXVECTOR3(0.f, 0.4f * CManager::GetMasterScale(), 0.0f * CManager::GetMasterScale()));
	pCar->m_pBody->SetObjType(CScene::OBJTYPE::PLAYER);
	pCar->m_pBody->SetParent(nullptr);

	// タイヤ
	// LF
	D3DXVECTOR3 tirePos = D3DXVECTOR3(-STANDARDCAR_RF.x, STANDARDCAR_RF.y, STANDARDCAR_RF.z);
	pCar->m_pTire[0] = CSceneModel::Create(tirePos, D3DXVECTOR3(0.f, 0.f, 0.f), D3DXVECTOR3(1.f, 1.f, 1.f), MODELNAME_STANDARDCAR_TIRE, pry);
	pCar->m_pTire[0]->SetParent(pCar->m_pBody);

	// RF
	tirePos = D3DXVECTOR3(STANDARDCAR_RF.x, STANDARDCAR_RF.y, STANDARDCAR_RF.z);
	pCar->m_pTire[1] = CSceneModel::Create(tirePos, D3DXVECTOR3(0.f, 0.f, 0.f), D3DXVECTOR3(1.f, 1.f, 1.f), MODELNAME_STANDARDCAR_TIRE, pry);
	pCar->m_pTire[1]->SetParent(pCar->m_pBody);

	// LB
	tirePos = D3DXVECTOR3(-STANDARDCAR_RB.x, STANDARDCAR_RB.y, STANDARDCAR_RB.z);
	pCar->m_pTire[2] = CSceneModel::Create(tirePos, D3DXVECTOR3(0.f, 0.f, 0.f), D3DXVECTOR3(1.f, 1.f, 1.f), MODELNAME_STANDARDCAR_TIRE, pry);
	pCar->m_pTire[2]->SetParent(pCar->m_pBody);

	// RB
	tirePos = D3DXVECTOR3(STANDARDCAR_RB.x, STANDARDCAR_RB.y, STANDARDCAR_RB.z);
	pCar->m_pTire[3] = CSceneModel::Create(tirePos, D3DXVECTOR3(0.f, 0.f, 0.f), D3DXVECTOR3(1.f, 1.f, 1.f), MODELNAME_STANDARDCAR_TIRE, pry);
	pCar->m_pTire[3]->SetParent(pCar->m_pBody);

	// コンテナにつめる
	m_umapCarManager[onlyCarName] = pCar;

	// ソースボイス作成
	XA2LoadWave *pLoadWave = CManager::GetXA2Manager()->GetLoadWaveOnAll();
	pCar->m_pSourceVoice3D = new XA2SourceVoice3D;
	pCar->m_pEngine = pCar->m_pSourceVoice3D->Create("data/SE/engine001.wav", -1, pLoadWave);			// エンジン
	pCar->m_pSourceVoice3D->Create("data/SE/accel.wav", 0, pLoadWave);				// アクセル
	pCar->m_pSourceVoice3D->Create("data/SE/car-idling.wav", -1, pLoadWave);		// アイドリング
	pCar->m_pSourceVoice3D->Create("data/SE/crash.wav", 0, pLoadWave);				// クラッシュ
	pCar->m_pSourceVoice3D->Create("data/SE/TireSkid_loop.wav", -1, pLoadWave);		// スキット
	
	// サウンドオブジェクト追加
	CManager::GetXA2Manager()->GetSourceVoiceManager()->SetXA2SoundObject(pCar->m_pSourceVoice3D, pCar);

	XA2SourceVoiceManager *pSoundObjectManager = CManager::GetXA2Manager()->GetSourceVoiceManager();
	pSoundObjectManager->GetXA2SourceVoice(pCar)->Play("data/SE/engine001.wav");

	return pCar;
}

//================================================================================
// 更新
//--------------------------------------------------------------------------------
void CCar::Update()
{
	m_move.y -= 0.1f * CManager::GetMasterScale();			// 落下！！！

	m_moveTurn *= ROT_FRICTIONAL;

	// 速度制限
	D3DXVECTOR2 vecMove = D3DXVECTOR2(m_move.x, m_move.z);
	m_direction = D3DXVec2Length(&vecMove);
	if (m_direction > m_maxSpeed) {
		D3DXVECTOR3 limit;
		D3DXVec3Normalize(&limit, &m_move);
		m_move.x = limit.x * m_maxSpeed;
		m_move.z = limit.z * m_maxSpeed;
	}

	static int i;
	if (++i % 3 == 0)
	{
		m_pSourceVoice3D->SetPitch("data/SE/engine001.wav", 0.5f + m_direction * 5.f);
		m_pSourceVoice3D->SetVolume("data/SE/engine001.wav", 0.5f + m_direction * 1.0f);
		m_pSourceVoice3D->SetVolume("data/SE/crash.wav", 0.1f + m_direction);
		m_pSourceVoice3D->SetVolume("data/SE/TireSkid_loop.wav", 0.1f + m_direction);
	}

	// 移動量の加算
	m_pos += m_move;

	// ブラー
	CRenderer *pRenderer = CManager::GetRenderer();
	if (m_direction >= 0.7f * CManager::GetMasterScale()) {
		pRenderer->SetBlur(m_direction - 0.7f);
	}
	else {
		pRenderer->SetBlur(0.f);
	}

	m_effCnt = ++m_effCnt % 8;
	// 減速時はタイヤの旋回量を自動的に元に戻す
	if (m_direction <= 0.001f * CManager::GetMasterScale()) {
		m_turn = 0.f;
	}
	else if(m_direction >= 0.2f * CManager::GetMasterScale())
	{
		// 土煙を出す
		if (m_effCnt == 0)
		{
			float rotR = m_rot.y * -1.f + D3DX_PI * 1.65f;
			float rotL = m_rot.y * -1.f + D3DX_PI * 1.35f;
			float dir = m_direction + 0.5f * CManager::GetMasterScale();
			float len = 1.5f * CManager::GetMasterScale();
			float rad = 0.5f * CManager::GetMasterScale();
			D3DXVECTOR3 RB = D3DXVECTOR3(cos(rotR) * len, 0.f, sin(rotR) * len);	// 右後ろ
			D3DXVECTOR3 LB = D3DXVECTOR3(cos(rotL) * len, 0.f, sin(rotL) * len);	// 左後ろ
			CEffect3DPoly::SetEffect3DPoly(m_pos + RB, D3DXVECTOR3(0.f, 0.f, 0.f),
				D3DXVECTOR3(cos(rotR + D3DX_PI * 0.5f) * 0.15f * dir, 0.1f * dir, sin(rotR + D3DX_PI * 0.5f) * 0.15f * dir),
				rad, 0.1f, 0.1f, 0.1f, 1.f, 16.f, TEXNAME_EFFECT);
			CEffect3DPoly::SetEffect3DPoly(m_pos + LB, D3DXVECTOR3(0.f, 0.f, 0.f),
				D3DXVECTOR3(cos(rotL - D3DX_PI * 0.5f) * 0.15f * dir, 0.1f * dir, sin(rotL - D3DX_PI * 0.5f) * 0.15f * dir),
				rad, 0.1f, 0.1f, 0.1f, 1.f, 16.f, TEXNAME_EFFECT);
		}
	}

	m_colSECnt = ++m_colSECnt % 16;
	// 衝突判定
	CSceneFieldX::FIELD_POLY pFieldXPoly;
	pFieldXPoly.p[0] = pFieldXPoly.p[1] = pFieldXPoly.p[2] = D3DXVECTOR3();
	for (int n = 0; n < MAX_SCENE_PRIORITY; n++)
	{
		std::list<CScene*> *lisScene = CScene::GetScene(n);
		for (CScene* pScene : *lisScene)
		{
			if (pScene == nullptr)continue;
			// タイプの取得
			CScene::OBJTYPE objType = pScene->GetObjType();

			// フィールドの場合
			if (objType == CScene::OBJTYPE::FIELDX)
			{
				// ダウンキャスト
				m_pField = ((CSceneFieldX*)pScene);
				float py = 0.f;
				bool in = m_pField->GetHeight(m_pos, &py, &pFieldXPoly);
				// ポリゴン内
				if (in == true)
				{
					// 指定の高さにあった場合
					if (m_pos.y < py + 1.f && m_pos.y >= py - m_move.y - 5.f)
					{
						m_pos = D3DXVECTOR3(m_pos.x, py, m_pos.z);
						m_move.y = 0.f;
						// 現在いるポリゴンの頂点情報を更新
						m_fieldXPoly = pFieldXPoly;
					}
					continue;
				}
				// ポリゴン外
				// 
				// 戻る処理
				if (m_pField)
				{
					// 最近辺を探して2頂点を取得
					D3DXVECTOR3 outPosS, outPosG;
					outPosS = outPosG = D3DXVECTOR3(0.f, 0.f, 0.f);
					m_pField->TriangleNearLine(m_pos, &m_fieldXPoly, &outPosS, &outPosG);
					D3DXVECTOR3 vecSG = outPosG - outPosS;
					// 真上から見た時の、辺に対する法線ベクトルを取得 - nom
					float rotSG = atan2(vecSG.z, vecSG.x);
					D3DXVECTOR3 nom(cosf(rotSG - D3DX_PI * 0.5f), 0, sinf(rotSG - D3DX_PI * 0.5f));
					D3DXVec3Normalize(&nom, &nom);
					// 内積を使用し、衝突点に戻す
					D3DXVECTOR3 vecPG(outPosG - m_pos);
					D3DXVECTOR3 colPos;
					colPos = m_posOld + D3DXVec3Dot(&vecPG, &nom) * nom;
					m_pos = colPos;
					m_move.y = 0.f;
					m_move *= 0.96f;
				}
				// SE_衝突
				// -------- -------- -------- --------
				if (m_colSECnt == 0)
				{
					XA2SourceVoiceManager *pSoundObjectManager = CManager::GetXA2Manager()->GetSourceVoiceManager();
					pSoundObjectManager->GetXA2SourceVoice(this)->Play("data/SE/crash.wav");
				}
			}
		}
	}
	m_posOld = m_pos;

	// 旋回量の加算
	 m_turn += m_moveTurn;

	// 旋回量限界値の判定
	if (m_turn > ROT_LIMIT) {
		m_turn = ROT_LIMIT;
	}
	else if (m_turn < ROT_LIMIT * -1.f) {
		m_turn = ROT_LIMIT * -1.f;
	}

	// 座標と回転量はボディに代入
	m_pBody->SetPos(m_pos);
	m_pBody->SetRot(m_rot);

	CCamera *pCam = CManager::GetCamera();
	if (pCam->GetAtObjType() == CCamera::AT_OBJTYPE::CONTROL_PLAYER)
	{
		// 注視点にして欲しい座標を徐々に車の座標に寄せる
		if (m_camPosAt != m_pos)m_camPosAt += (m_pos - m_camPosAt) * 0.8f;
		// カメラの回転
		pCam->AddCamRoate(m_turn * m_direction * -1.f);
		// カメラ注視点の設定
		m_camPosAt = m_pos + m_move * 0.5f;

		pCam->SetAtAngle(m_direction / CManager::GetMasterScale() * -1.25f);
	}


	//================================================================================
	//... 音 ...
	//--------------------------------------------------------------------------------
	// メインリスナー座標の更新
	// -------- -------- -------- --------
	XA2SourceVoiceManager *pSoundObjectManager = CManager::GetXA2Manager()->GetSourceVoiceManager();
	if (m_pSourceVoice3D)
	{
		m_pSourceVoice3D->SetPos(m_pos);
		m_pSourceVoice3D->SetFront(D3DXVECTOR3(cos(m_rot.y - D3DX_PI * 0.5f), 0.f, sin(m_rot.y - D3DX_PI * 0.5f)));
		m_pSourceVoice3D->SetTop(D3DXVECTOR3(0.f, 1.f, 0.f));

	}

	if (m_direction < 0.01f * CManager::GetMasterScale() && m_idling == false)
	{
		// SE_アイドリング
		// -------- -------- -------- --------
		pSoundObjectManager->GetXA2SourceVoice(this)->Play("data/SE/car-idling.wav");
		m_idling = true;
	}
	else if (m_direction > 0.01f * CManager::GetMasterScale())
	{
		// SE_アイドリング ストップ
		// -------- -------- -------- --------
		pSoundObjectManager->GetXA2SourceVoice(this)->Stop("data/SE/car-idling.wav");
		m_idling = false;
	}

	if ((m_turn <= -0.015f || m_turn >= 0.015f) && m_skid == false)
	{
		// SE_スキッド
		// -------- -------- -------- --------
		pSoundObjectManager->GetXA2SourceVoice(this)->Play("data/SE/TireSkid_loop.wav");
		m_skid = true;
	}
	else if (m_direction < 0.1f * CManager::GetMasterScale() && m_brakeSound)
	{
		// SE_スキッド ストップ
		// -------- -------- -------- --------
		pSoundObjectManager->GetXA2SourceVoice(this)->Stop("data/SE/TireSkid_loop.wav");
		m_skid = false;
	}
	else if ((m_turn >= -0.02f && m_turn <= 0.02f && m_skid))
	{
		// SE_スキッド ストップ
		// -------- -------- -------- --------
		pSoundObjectManager->GetXA2SourceVoice(this)->Stop("data/SE/TireSkid_loop.wav");
		m_skid = false;
	}

	// RTCP値　SPEED
	// -------- -------- -------- --------
	m_speed = m_maxSpeed / m_direction * 100.f;	// 速度
	if (m_direction == 0.f)m_speed = m_direction;
//	pXA2Master->SetGameParam(GAME_PARAMETERS::SPEED, m_speed);
}

//================================================================================
// 前進
//--------------------------------------------------------------------------------
void CCar::MoveForward()
{
	// 現在の移動量に合わせて車体を旋回させる
	m_turn *= 0.3f;
	float turning = 0.f;
	D3DXVECTOR2 vecMove = D3DXVECTOR2(m_move.x, m_move.z) * 0.00001f;
	float saveTurning = D3DXVec2Length(&vecMove);
	if (m_turn > 0.f) {
		turning = m_turn - saveTurning;
		if (turning <= 0.f)turning = 0.f;
	}
	else if (m_turn < 0.f) {
		turning = m_turn + saveTurning;
		if (turning >= 0.f)turning = 0.f;
	}
	m_rot.y += turning * -1.f;

	m_move.x += cosf(m_rot.y * -1.f - D3DX_PI * 0.5f) * m_moveSpeed;
	m_move.z += sinf(m_rot.y * -1.f - D3DX_PI * 0.5f) * m_moveSpeed;

	// タイヤの回転
	vecMove = D3DXVECTOR2(m_move.x, m_move.z);
	m_rotTire += D3DXVec2Length(&vecMove) * -0.2f;
	m_pTire[0]->SetRot(D3DXVECTOR3(m_rotTire, m_moveTurn * -20.f, 0.f));
	m_pTire[1]->SetRot(D3DXVECTOR3(m_rotTire, m_moveTurn * -20.f, 0.f));
	m_pTire[2]->SetRot(D3DXVECTOR3(m_rotTire, 0.f, 0.f));
	m_pTire[3]->SetRot(D3DXVECTOR3(m_rotTire, 0.f, 0.f));

	if (m_direction < 0.1f * CManager::GetMasterScale())
	{
		// SE_アクセル
		// -------- -------- -------- --------
		XA2SourceVoiceManager *pSoundObjectManager = CManager::GetXA2Manager()->GetSourceVoiceManager();
		pSoundObjectManager->GetXA2SourceVoice(this)->Play("data/SE/accel.wav");
	}

	m_brakeSound = false;
}

//================================================================================
// 前進
//--------------------------------------------------------------------------------
void CCar::MoveForward(float rT)
{
	// 現在の移動量に合わせて車体を旋回させる
	m_turn *= 0.3f;
	float turning = 0.f;
	D3DXVECTOR2 vecMove = D3DXVECTOR2(m_move.x, m_move.z) * 0.00001f;
	float saveTurning = D3DXVec2Length(&vecMove);
	if (m_turn > 0.f) {
		turning = m_turn - saveTurning;
		if (turning <= 0.f)turning = 0.f;
	}
	else if (m_turn < 0.f) {
		turning = m_turn + saveTurning;
		if (turning >= 0.f)turning = 0.f;
	}
	m_rot.y += turning * -1.f;

	m_move.x += cosf(m_rot.y * -1.f - D3DX_PI * 0.5f) * m_moveSpeed * rT;
	m_move.z += sinf(m_rot.y * -1.f - D3DX_PI * 0.5f) * m_moveSpeed * rT;

	// タイヤの回転
	vecMove = D3DXVECTOR2(m_move.x, m_move.z);
	m_rotTire += D3DXVec2Length(&vecMove) * -0.2f;
	m_pTire[0]->SetRot(D3DXVECTOR3(m_rotTire, m_moveTurn * -20.f, 0.f));
	m_pTire[1]->SetRot(D3DXVECTOR3(m_rotTire, m_moveTurn * -20.f, 0.f));
	m_pTire[2]->SetRot(D3DXVECTOR3(m_rotTire, 0.f, 0.f));
	m_pTire[3]->SetRot(D3DXVECTOR3(m_rotTire, 0.f, 0.f));

	if (m_direction < 0.1f * CManager::GetMasterScale())
	{
		// SE_アクセル
		// -------- -------- -------- --------
		XA2SourceVoiceManager *pSoundObjectManager = CManager::GetXA2Manager()->GetSourceVoiceManager();
		pSoundObjectManager->GetXA2SourceVoice(this)->Play("data/SE/accel.wav");
	}

	m_brakeSound = false;
}

//================================================================================
// 後進
//--------------------------------------------------------------------------------
void CCar::MoveBackward()
{
	// 現在の移動量に合わせて車体を旋回させる
	m_turn *= 0.3f;
	float turning = 0.f;
	D3DXVECTOR2 vecMove = D3DXVECTOR2(m_move.x, m_move.z) * 0.00001f;
	float saveTurning = D3DXVec2Length(&vecMove);
	if (m_turn > 0.f) {
		turning = m_turn - saveTurning;
		if (turning <= 0.f)turning = 0.f;
	}
	else if (m_turn < 0.f) {
		turning = m_turn + saveTurning;
		if (turning >= 0.f)turning = 0.f;
	}
	m_rot.y += turning * 1.1f;

	m_move.x += cosf(m_rot.y * -1.f + D3DX_PI * 0.5f) * m_moveBackSpeed;
	m_move.z += sinf(m_rot.y * -1.f + D3DX_PI * 0.5f) * m_moveBackSpeed;

	// タイヤの回転
	vecMove = D3DXVECTOR2(m_move.x, m_move.z);
	m_rotTire += D3DXVec2Length(&vecMove) * 0.2f;
	m_pTire[0]->SetRot(D3DXVECTOR3(m_rotTire, m_moveTurn * -20.f, 0.f));
	m_pTire[1]->SetRot(D3DXVECTOR3(m_rotTire, m_moveTurn * -20.f, 0.f));
	m_pTire[2]->SetRot(D3DXVECTOR3(m_rotTire, 0.f, 0.f));
	m_pTire[3]->SetRot(D3DXVECTOR3(m_rotTire, 0.f, 0.f));

	m_brakeSound = false;
}

//================================================================================
// ブレーキ
//--------------------------------------------------------------------------------
void CCar::Brake()
{
	m_moveTurn *= 0.9f;
	D3DXVECTOR2 vecMove = D3DXVECTOR2(m_move.x, m_move.z);
	float length = D3DXVec2Length(&vecMove);

	// 旋回量を移動量によって減衰させる
	if (length > m_maxSpeed) m_turn *= m_maxSpeed - m_maxSpeed / length;
	else m_turn *= length;

	// 減衰しながら進行
	CCamera *pCamera = CManager::GetCamera();
	float camY = pCamera->GetRot().y;
	D3DXVECTOR2 cam(cos(camY), sin(camY));
	D3DXVECTOR2 car(cos(m_rot.y), sin(m_rot.y));
	float dot = D3DXVec2Dot(&car, &cam);
	if (dot >= -1.f)
	{
		m_move.x = cosf(m_rot.y * -1.f - D3DX_PI * 0.5f - m_turn * 20.f) * length * m_brake;
		m_move.z = sinf(m_rot.y * -1.f - D3DX_PI * 0.5f - m_turn * 20.f) * length * m_brake;
	}
	else
	{
		m_move.x = cosf(m_rot.y * -1.f + D3DX_PI * 0.5f - m_turn) * length * m_brake;
		m_move.z = sinf(m_rot.y * -1.f + D3DX_PI * 0.5f - m_turn) * length * m_brake;
	}

	if (!m_skid && m_direction > 0.1f * CManager::GetMasterScale())
	{
		// SE_スキッド
		// -------- -------- -------- --------
		XA2SourceVoiceManager *pSoundObjectManager = CManager::GetXA2Manager()->GetSourceVoiceManager();
		pSoundObjectManager->GetXA2SourceVoice(this)->Play("data/SE/TireSkid_loop.wav");

		m_skid = true;
		m_brakeSound = true;
	}
}

//================================================================================
// 慣性
//--------------------------------------------------------------------------------
void CCar::MoveFrictional()
{
	CCamera *pCamera = CManager::GetCamera();

	m_moveTurn *= 0.9f;
	D3DXVECTOR2 vecMove = D3DXVECTOR2(m_move.x, m_move.z);
	float length = D3DXVec2Length(&vecMove);

	// 旋回量を移動量によって減衰させる
	if(length > m_maxSpeed) m_turn *= (m_maxSpeed - m_maxSpeed / length);
	else m_turn *= length / CManager::GetMasterScale();

	if (length > 0.1f * CManager::GetMasterScale())
	{
		// 現在の移動量に合わせて車体を旋回させる
		float turning = 0.f;
		float saveTurning = length * 0.000001f / CManager::GetMasterScale();
		if (m_turn > 0.f) {
			turning = m_turn - saveTurning;
			if (turning <= 0.f)turning = 0.f;
		}
		else if (m_turn < 0.f) {
			turning = m_turn + saveTurning;
			if (turning >= 0.f)turning = 0.f;
		}
		m_rot.y += turning * -1.f;
	}

	// 減衰しながら進行
	float camY = pCamera->GetRot().y;
	D3DXVECTOR2 cam(cos(camY), sin(camY));
	D3DXVECTOR2 car(cos(m_rot.y), sin(m_rot.y));
	float dot = D3DXVec2Dot(&car, &cam);
	if (dot >= -1.f)
	{
		m_move.x = cosf(m_rot.y * -1.f - D3DX_PI * 0.5f - m_turn * 15.f) * length * m_frictional;
		m_move.z = sinf(m_rot.y * -1.f - D3DX_PI * 0.5f - m_turn * 15.f) * length * m_frictional;
	}
	else
	{
		m_move.x = cosf(m_rot.y * -1.f + D3DX_PI * 0.5f - m_turn) * length * m_frictional;
		m_move.z = sinf(m_rot.y * -1.f + D3DX_PI * 0.5f - m_turn) * length * m_frictional;
	}

	// タイヤの回転
	vecMove = D3DXVECTOR2(m_move.x, m_move.z);
	m_rotTire += D3DXVec2Length(&vecMove) * -0.2f;
	m_pTire[0]->SetRot(D3DXVECTOR3(m_rotTire, m_moveTurn * -20.f, 0.f));
	m_pTire[1]->SetRot(D3DXVECTOR3(m_rotTire, m_moveTurn * -20.f, 0.f));
	m_pTire[2]->SetRot(D3DXVECTOR3(m_rotTire, 0.f, 0.f));
	m_pTire[3]->SetRot(D3DXVECTOR3(m_rotTire, 0.f, 0.f));

	m_brakeSound = false;
}

//================================================================================
// 右向きにタイヤを回す
//--------------------------------------------------------------------------------
void CCar::TurnRight()
{
	m_moveTurn += MOVE_ROT_SPEED;
}

//================================================================================
// 左向きにタイヤを回す
//--------------------------------------------------------------------------------
void CCar::TurnLeft()
{
	m_moveTurn -= MOVE_ROT_SPEED;
}

void CCar::TurnRight(float lx)
{
	m_moveTurn -= MOVE_ROT_SPEED * lx;
}

void CCar::TurnLeft(float lx)
{
	m_moveTurn += MOVE_ROT_SPEED * lx;
}

//================================================================================
// エンジンをふかす
//--------------------------------------------------------------------------------
void CCar::SoundEngin()
{
	// SE_アクセル
	// -------- -------- -------- --------
	XA2SourceVoiceManager *pSoundObjectManager = CManager::GetXA2Manager()->GetSourceVoiceManager();
	pSoundObjectManager->GetXA2SourceVoice(this)->Play("data/SE/accel.wav");
}

//================================================================================
// 全解放
//--------------------------------------------------------------------------------
void CCar::ReleaseAll()
{
	// サウンドオブジェクトマネージャー
	XA2SourceVoiceManager *pSoundObjectManager = CManager::GetXA2Manager()->GetSourceVoiceManager();

	for (auto car : m_umapCarManager)
	{
		if (car.second != nullptr)
		{
			// 音全停止
			XA2SourceVoiceInterface *pSourceVoice = pSoundObjectManager->GetXA2SourceVoice(car.second);
			if (pSourceVoice)pSourceVoice->StopAll();

			// 解放処理
			delete car.second;
			car.second = nullptr;
		}
	}
	// コンテナを空にする
	m_umapCarManager.clear();
}
