//================================================================================
//
//	title : classMake[ camera.h ]				Auter : KENSUKE WATANABE
//												Data  : 2017/04/27
//
//--------------------------------------------------------------------------------
#ifndef _LIGHTCAMERA_H_
#define _LIGHTCAMERA_H_

class CSceneModel;
class CModelManager;

//--------------------------------------------------------------------------------
// �N���X
//--------------------------------------------------------------------------------
class CLightCamera
{
public:
	CLightCamera();
	~CLightCamera() {};

	void Init();
	void Uninit();
	void Update();
	void Draw() {}

	const D3DXVECTOR3 &GetPos()const { return m_pos; }
	const D3DXVECTOR3 &GetRot()const { return m_rot; }
	const D3DXVECTOR3 &GetVec() { D3DXVECTOR3 vec = m_posAt - m_pos; return *D3DXVec3Normalize(&vec, &vec); }
	const D3DXVECTOR3 &GetPosAt(void)const { return m_posAt; }
	const D3DXVECTOR3 &GetDir();

	const D3DXMATRIX &GetView()const { return m_mtxView; }
	const D3DXMATRIX &GetProj()const { return m_mtxProj; }

	const float &GetNear()const { return m_near; }
	const float &GetFar()const { return m_far; }

	void SetOrthoLH();
	D3DXMATRIX GetMtxView();
	D3DXMATRIX GetMtxProj();

private:
	D3DXVECTOR3 m_lightVecAt;
	D3DXVECTOR3 m_pos;			// ���W
	D3DXVECTOR3 m_posAt;		// �����_
	D3DXVECTOR3 m_rot;			// ���W
	D3DXVECTOR3 m_vecUp;		// �㕔
	int m_width, m_height;		// ���E�����߂镝, ����

	D3DXMATRIX m_mtxWorld;		// ���[���h���W
	D3DXMATRIX m_mtxView;		// �r���[�s��
	D3DXMATRIX m_mtxProj;		// �v���W�F�N�V�����s��

	float m_near, m_far;

	CModelManager *m_pModelManager;
	CSceneModel *m_p3DObj;
};

#endif