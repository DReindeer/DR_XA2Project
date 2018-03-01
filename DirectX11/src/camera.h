// ======== ======== ======== ======== ======== ======== ======== ========
//
//
//	title : �J����
//	Auter : KENSUKE WATANABE
//
//
// ======== ======== ======== ======== ======== ======== ======== ========
#ifndef _CAMERA_H_
#define _CAMERA_H_

#define CAM_NEAR (0.1f)
#define CAM_FAR (2000.f)

#include "Wmath.h"

static const XMFLOAT3 CAM_LEN = XMFLOAT3(0.f, 7.f, 7.f);		// �J�������W

// -------- -------- -------- -------- -------- -------- -------- --------
// �N���X
// -------- -------- -------- -------- -------- -------- -------- --------
class CSceneField;
class CCamera
{
public:
	struct ConstantBuffer
	{
		XMMATRIX mView;			// �r���[�ϊ��s��
		XMMATRIX mProjection;	// �����ˉe�ϊ��s��
	};

	struct CBufferModel
	{
		XMMATRIX mWorld;		// ���[���h�ϊ��s��
		XMFLOAT4 mLight;		// ���C�g
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

	// �J�����ɑ΂���ړ�����
	static float BEFORE(const float &y) { return y; }				// �O
	static float BACK(const float &y) { return y + PI; }			// ���
	static float RIGHT(const float &y) { return y - PI * 0.5f; }	// �E
	static float LEFT(const float &y) { return y + PI * 0.5f; }		// ��
	static float RBEFORE(const float &y) { return y - PI * 0.25f; }	// �E��
	static float RBACK(const float &y) { return y - PI * 0.75f; }	// �E��
	static float LBEFORE(const float &y) { return y + PI * 0.25f; }	// ����
	static float LBACK(const float &y) { return y + PI * 0.75f; }	// ����

	// �J������]
	void AddCamRoateY(const float &moveRot);
	void AddCamPosY(const float &move);
	void SetLockAt(XMFLOAT3 lookAt);

	// �ݒ�
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
	XMFLOAT3 m_pos;						// ���W
	XMFLOAT3 m_posAt;					// �����_
	XMFLOAT3 m_vecUp;					// �㕔
	XMFLOAT4X4 m_mtxWorld;				// ���[���h�s��
	XMFLOAT4X4 m_mtxView;				// �r���[�s��
	XMFLOAT4X4 m_mtxProj;				// �v���W�F�N�V�����s��
	XMFLOAT4 m_lightPos;

	// �R���X�^���g�o�b�t�@
	ID3D11Buffer *m_pConstantBuffer;	// �J����
	ID3D11Buffer *m_pConstantBufferModel;				// �I�u�W�F�N�g

	float m_len;
	AT_OBJTYPE m_atObjType;
	CSceneField* m_pField;

	bool m_mapCap;

	XMFLOAT3 m_rot;
	XMFLOAT3 m_moveRot;
	float m_moveRotFlame;
};

#endif
