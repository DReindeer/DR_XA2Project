//================================================================================
//
//	title : classMake[ camera.h ]			Auter : KENSUKE WATANABE
//														Data  : 2017/04/27
//
//--------------------------------------------------------------------------------
#ifndef _CAMERA_H_
#define _CAMERA_H_

#include "scene.h"

#define CAM_NEAR (0.1f)
#define CAM_FAR (2000.f)

static const D3DXVECTOR3 CAM_LEN = D3DXVECTOR3(0.f, 4.f, -10.f);		// カメラ座標

//--------------------------------------------------------------------------------
// クラス
//--------------------------------------------------------------------------------
class XA2Listener;
class CSceneField;
class CCamera
{
public:
	
	typedef enum
	{
		NONE = 0,
		CONTROL_PLAYER,
		MAX
	}AT_OBJTYPE;

	CCamera();
	~CCamera() {};

	void Init();
	void Uninit();
	void UpdateBigin();
	void Update();
	void Draw() {}

	static D3DXVECTOR3* CalcScreenToWorld(D3DXVECTOR3* pout, int Sx, int Sy, float fZ,
		int Screen_w, int Screen_h, const D3DXMATRIX &View, const D3DXMATRIX &Prj);

	// カメラに対する移動方向
	static float BEFORE(const float &y) { return y; }						// 前
	static float BACK(const float &y) { return y + D3DX_PI; }				// 後ろ
	static float RIGHT(const float &y) { return y - D3DX_PI * 0.5f; }		// 右
	static float LEFT(const float &y) { return y + D3DX_PI * 0.5f; }		// 左
	static float RBEFORE(const float &y) { return y - D3DX_PI * 0.25f; }	// 右上
	static float RBACK(const float &y) { return y - D3DX_PI * 0.75f; }		// 右下
	static float LBEFORE(const float &y) { return y + D3DX_PI * 0.25f; }	// 左上
	static float LBACK(const float &y) { return y + D3DX_PI * 0.75f; }		// 左下

	// カメラ回転
	void AddCamRoate(const float &moveRot);
	void SetLockAt(D3DXVECTOR3 lookAt);

	// 設定
	void Set();
	void SetatObjType(AT_OBJTYPE atObjType) { m_atObjType = atObjType; }
	void SetMoveRotFlame(float moveRot) { m_moveRotFlame = moveRot; }
	void SetCamPos(D3DXVECTOR3 pos) { m_pos = pos; }
	void SetCamPosAt(D3DXVECTOR3 posAt) { m_posAt = posAt; }
	void SetVecUp(D3DXVECTOR3 up) { m_vecUp = up; }
	void SetAtAngle(float parSpeed) { m_atAngle -= m_atAngle - D3DX_PI / (4.f + parSpeed); }

	const D3DXVECTOR3 &GetRot()const { return m_rot; }
	const D3DXVECTOR3 &GetPos()const { return m_pos; }
	const D3DXVECTOR3 &GetVec() { D3DXVECTOR3 vec = m_posAt - m_pos; return *D3DXVec3Normalize(&vec, &vec); }
	const D3DXVECTOR3 &GetPosAt(void)const { return m_posAt; }
	const AT_OBJTYPE &GetAtObjType(void)const { return m_atObjType; }

	const D3DXMATRIX &GetView()const { return m_mtxView; }
	const D3DXMATRIX &GetProj()const { return m_mtxProj; }

	const float &GetNear()const { return m_near; }
	const float &GetFar()const { return m_far; }

	// リスナー
	XA2Listener *GetListener() { return m_pListener; }

private:
	float m_len = 0.f;
	AT_OBJTYPE m_atObjType;
	CSceneField* m_pField;

	// リスナー
	XA2Listener *m_pListener = nullptr;

	bool m_mapCap = false;

	D3DXVECTOR3 m_pos = D3DXVECTOR3(0.f, 0.f, 0.f);			// 座標
	D3DXVECTOR3 m_posOld = D3DXVECTOR3(0.f, 0.f, 0.f);		// 座標
	D3DXVECTOR3 m_posAt = D3DXVECTOR3(0.f, 0.f, 0.f);		// 注視点
	D3DXVECTOR3 m_vecUp = D3DXVECTOR3(0.f, 1.f, 0.f);		// 上部

	D3DXVECTOR3 m_camLen = D3DXVECTOR3(0.f, 0.f, 0.f);
	D3DXVECTOR3 m_rot = D3DXVECTOR3(0.f, 0.f, 0.f);
	D3DXVECTOR3 m_moveRot = D3DXVECTOR3(0.f, 0.f, 0.f);

	float m_atAngle = 0.f;									// 注視角
	float m_moveRotFlame = 0.98f;							// カメラ遅延
	D3DXMATRIX m_mtxView;									// ビュー行列
	D3DXMATRIX m_mtxProj;									// プロジェクション行列

	float m_near, m_far;
};

#endif
