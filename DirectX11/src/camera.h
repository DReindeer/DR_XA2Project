// ======== ======== ======== ======== ======== ======== ======== ========
//
//
//	title : カメラ
//	Auter : KENSUKE WATANABE
//
//
// ======== ======== ======== ======== ======== ======== ======== ========
#ifndef _CAMERA_H_
#define _CAMERA_H_

#define CAM_NEAR (0.1f)
#define CAM_FAR (2000.f)

#include "Wmath.h"

static const XMFLOAT3 CAM_LEN = XMFLOAT3(0.f, 7.f, 7.f);		// カメラ座標

// -------- -------- -------- -------- -------- -------- -------- --------
// クラス
// -------- -------- -------- -------- -------- -------- -------- --------
class CSceneField;
class CCamera
{
public:
	struct ConstantBuffer
	{
		XMMATRIX mView;			// ビュー変換行列
		XMMATRIX mProjection;	// 透視射影変換行列
	};

	struct CBufferModel
	{
		XMMATRIX mWorld;		// ワールド変換行列
		XMFLOAT4 mLight;		// ライト
	};

	typedef enum
	{
		NONE = 0,
		CONTROL_PLAYER,
		MAX
	}AT_OBJTYPE;

	CCamera();
	~CCamera() {};

	void Init(void);
	void Uninit(void);
	void Update(void);
	void Set(void);

	// カメラに対する移動方向
	static float BEFORE(const float &y) { return y; }				// 前
	static float BACK(const float &y) { return y + PI; }			// 後ろ
	static float RIGHT(const float &y) { return y - PI * 0.5f; }	// 右
	static float LEFT(const float &y) { return y + PI * 0.5f; }		// 左
	static float RBEFORE(const float &y) { return y - PI * 0.25f; }	// 右上
	static float RBACK(const float &y) { return y - PI * 0.75f; }	// 右下
	static float LBEFORE(const float &y) { return y + PI * 0.25f; }	// 左上
	static float LBACK(const float &y) { return y + PI * 0.75f; }	// 左下

	// カメラ回転
	void AddCamRoateY(const float &moveRot);
	void AddCamPosY(const float &move);
	void SetLockAt(XMFLOAT3 lookAt);

	// 設定
	void SetatObjType(AT_OBJTYPE atObjType) { m_atObjType = atObjType; }
	void SetMoveRotFlame(float moveRot) { m_moveRotFlame = moveRot; }
	void SetCamPos(XMFLOAT3 pos) { m_pos = pos; }
	void SetCamPosAt(XMFLOAT3 posAt) { m_posAt = posAt; }
	void SetVecUp(XMFLOAT3 up) { m_vecUp = up; }

	const XMFLOAT3 &GetRot(void)const { return m_rot; }
	const XMFLOAT3 &GetPos(void)const { return m_pos; }
	const XMFLOAT3 &GetPosAt(void)const { return m_posAt; }
	const AT_OBJTYPE &GetAtObjType(void)const { return m_atObjType; }
	const XMFLOAT4X4 &GetWorld(void)const { return m_mtxWorld; }
	const XMFLOAT4X4 &GetView(void)const { return m_mtxView; }
	const XMFLOAT4X4 &GetProj(void)const { return m_mtxProj; }
	const XMFLOAT4 &GetLightPos(void)const { return m_lightPos; }
	ID3D11Buffer *GetConstantBuffer(void)const { return m_pConstantBuffer; }

private:
	XMFLOAT3 m_pos;						// 座標
	XMFLOAT3 m_posAt;					// 注視点
	XMFLOAT3 m_vecUp;					// 上部
	XMFLOAT4X4 m_mtxWorld;				// ワールド行列
	XMFLOAT4X4 m_mtxView;				// ビュー行列
	XMFLOAT4X4 m_mtxProj;				// プロジェクション行列
	XMFLOAT4 m_lightPos;

	// コンスタントバッファ
	ID3D11Buffer *m_pConstantBuffer;	// カメラ
	ID3D11Buffer *m_pConstantBufferModel;				// オブジェクト

	float m_len;
	AT_OBJTYPE m_atObjType;
	CSceneField* m_pField;

	bool m_mapCap;

	XMFLOAT3 m_rot;
	XMFLOAT3 m_moveRot;
	float m_moveRotFlame;
};

#endif
