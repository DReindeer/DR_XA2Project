//================================================================================
//
//	Auter : KENSUKE WATANABE
//	Data  : 2017/08/24
//
//--------------------------------------------------------------------------------
#ifndef _CAR_H_
#define _CAR_H_

#include "main.h"
#include <unordered_map>
#include "sceneFieldX.h"

//--------------------------------------------------------------------------------
// 定数定義
//--------------------------------------------------------------------------------

// 前方宣言
class CSceneModel;
class CSceneCircleShadow;
class XA2SourceVoice3D;

//--------------------------------------------------------------------------------
// クラス
//--------------------------------------------------------------------------------
class CCar
{
public:
	CCar();
	~CCar();

	void Update();

	static void ReleaseAll();

	void Brake();			// ブレーキ
	void SoundEngin();		// エンジンをふかす
	void MoveForward();		// 前進
	void MoveBackward();	// 後進
	void MoveFrictional();	// 慣性
	void TurnRight();
	void TurnLeft();
	void MoveForward(float rT);		// 前進
	void TurnRight(float lx);
	void TurnLeft(float lx);

	const float &GetFrictional()const { return m_frictional; }		// 摩擦
	const D3DXVECTOR3 &GetMove()const { return m_move; }			// 移動量
	const D3DXVECTOR3 &GetCamPosAt()const { return m_camPosAt; }	// カメラの注視点にして欲しい座標

	// 生成
	static CCar* Create(D3DXVECTOR3 pos, D3DXVECTOR3 rot, std::string onlyCarName, int pry = 4);

	// 車をマップから取得
	static CCar *GetCar(std::string name) {
		if (m_umapCarManager.count(name) != 0) { return m_umapCarManager[name]; }
		return nullptr;
	}

	// 取得
	const D3DXVECTOR3 &GetPos()const { return m_pos; }
	const D3DXVECTOR3 &GetPosOld()const { return m_posOld; }
	const D3DXVECTOR3 &GetRot()const { return m_rot; }
	const float &GetSpeed() { return m_speed; }
	const float &GetMaxSpeed() { return m_maxSpeed; }

	// 設定
	void SetPos(const D3DXVECTOR3 &pos) { m_pos = pos; }
	void SetRot(const D3DXVECTOR3 &rot) { m_rot = rot; }

private:
	XA2SourceVoice3D *m_pSourceVoice3D = nullptr;
	IXAudio2SourceVoice *m_pEngine = nullptr;

	CSceneCircleShadow *m_pCircleShadow;

	CSceneModel *m_pBody;		// ボディ
	CSceneModel *m_pTire[4];	// タイヤ - 0:左前, 1:右前, 2:左後, 3:右後

	D3DXVECTOR3 m_pos;			// 車の位置
	D3DXVECTOR3 m_posOld;		// 車の前回の位置
	D3DXVECTOR3 m_move;			// 車の移動量
	D3DXVECTOR3 m_rot;			// 車の回転

	float m_speed;
	float m_maxSpeed, m_moveSpeed, m_moveBackSpeed;
	static CSceneFieldX *m_pField;
	CSceneFieldX::FIELD_POLY m_fieldXPoly;	// 現在いるポリゴンの頂点情報

	float m_rotTire;
	float m_turn;				// タイヤ旋回
	float m_moveTurn;			// タイヤ旋回量
	float m_frictional;			// 摩擦
	float m_brake;
	D3DXVECTOR3 m_camPosAt;		// カメラの注視点にして欲しい位置

	int m_effCnt;
	int m_colSECnt;
	float m_direction;

	bool m_skid, m_idling;
	bool m_brakeSound;
	static std::unordered_map<std::string, CCar*> m_umapCarManager;
};

#endif