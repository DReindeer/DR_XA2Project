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

static const D3DXVECTOR3 CAM_LEN = D3DXVECTOR3(0.f, 4.f, -10.f);		// �J�������W

//--------------------------------------------------------------------------------
// �N���X
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

	// �J�����ɑ΂���ړ�����
	static float BEFORE(const float &y) { return y; }						// �O
	static float BACK(const float &y) { return y + D3DX_PI; }				// ���
	static float RIGHT(const float &y) { return y - D3DX_PI * 0.5f; }		// �E
	static float LEFT(const float &y) { return y + D3DX_PI * 0.5f; }		// ��
	static float RBEFORE(const float &y) { return y - D3DX_PI * 0.25f; }	// �E��
	static float RBACK(const float &y) { return y - D3DX_PI * 0.75f; }		// �E��
	static float LBEFORE(const float &y) { return y + D3DX_PI * 0.25f; }	// ����
	static float LBACK(const float &y) { return y + D3DX_PI * 0.75f; }		// ����

	// �J������]
	void AddCamRoate(const float &moveRot);
	void SetLockAt(D3DXVECTOR3 lookAt);

	// �ݒ�
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

	// ���X�i�[
	XA2Listener *GetListener() { return m_pListener; }

private:
	float m_len = 0.f;
	AT_OBJTYPE m_atObjType;
	CSceneField* m_pField;

	// ���X�i�[
	XA2Listener *m_pListener = nullptr;

	bool m_mapCap = false;

	D3DXVECTOR3 m_pos = D3DXVECTOR3(0.f, 0.f, 0.f);			// ���W
	D3DXVECTOR3 m_posOld = D3DXVECTOR3(0.f, 0.f, 0.f);		// ���W
	D3DXVECTOR3 m_posAt = D3DXVECTOR3(0.f, 0.f, 0.f);		// �����_
	D3DXVECTOR3 m_vecUp = D3DXVECTOR3(0.f, 1.f, 0.f);		// �㕔

	D3DXVECTOR3 m_camLen = D3DXVECTOR3(0.f, 0.f, 0.f);
	D3DXVECTOR3 m_rot = D3DXVECTOR3(0.f, 0.f, 0.f);
	D3DXVECTOR3 m_moveRot = D3DXVECTOR3(0.f, 0.f, 0.f);

	float m_atAngle = 0.f;									// �����p
	float m_moveRotFlame = 0.98f;							// �J�����x��
	D3DXMATRIX m_mtxView;									// �r���[�s��
	D3DXMATRIX m_mtxProj;									// �v���W�F�N�V�����s��

	float m_near, m_far;
};

#endif
