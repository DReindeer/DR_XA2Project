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
// �萔��`
//--------------------------------------------------------------------------------

// �O���錾
class CSceneModel;
class CSceneCircleShadow;
class XA2SourceVoice3D;

//--------------------------------------------------------------------------------
// �N���X
//--------------------------------------------------------------------------------
class CCar
{
public:
	CCar();
	~CCar();

	void Update();

	static void ReleaseAll();

	void Brake();			// �u���[�L
	void SoundEngin();		// �G���W�����ӂ���
	void MoveForward();		// �O�i
	void MoveBackward();	// ��i
	void MoveFrictional();	// ����
	void TurnRight();
	void TurnLeft();
	void MoveForward(float rT);		// �O�i
	void TurnRight(float lx);
	void TurnLeft(float lx);

	const float &GetFrictional()const { return m_frictional; }		// ���C
	const D3DXVECTOR3 &GetMove()const { return m_move; }			// �ړ���
	const D3DXVECTOR3 &GetCamPosAt()const { return m_camPosAt; }	// �J�����̒����_�ɂ��ė~�������W

	// ����
	static CCar* Create(D3DXVECTOR3 pos, D3DXVECTOR3 rot, std::string onlyCarName, int pry = 4);

	// �Ԃ��}�b�v����擾
	static CCar *GetCar(std::string name) {
		if (m_umapCarManager.count(name) != 0) { return m_umapCarManager[name]; }
		return nullptr;
	}

	// �擾
	const D3DXVECTOR3 &GetPos()const { return m_pos; }
	const D3DXVECTOR3 &GetPosOld()const { return m_posOld; }
	const D3DXVECTOR3 &GetRot()const { return m_rot; }
	const float &GetSpeed() { return m_speed; }
	const float &GetMaxSpeed() { return m_maxSpeed; }

	// �ݒ�
	void SetPos(const D3DXVECTOR3 &pos) { m_pos = pos; }
	void SetRot(const D3DXVECTOR3 &rot) { m_rot = rot; }

private:
	XA2SourceVoice3D *m_pSourceVoice3D = nullptr;
	IXAudio2SourceVoice *m_pEngine = nullptr;

	CSceneCircleShadow *m_pCircleShadow;

	CSceneModel *m_pBody;		// �{�f�B
	CSceneModel *m_pTire[4];	// �^�C�� - 0:���O, 1:�E�O, 2:����, 3:�E��

	D3DXVECTOR3 m_pos;			// �Ԃ̈ʒu
	D3DXVECTOR3 m_posOld;		// �Ԃ̑O��̈ʒu
	D3DXVECTOR3 m_move;			// �Ԃ̈ړ���
	D3DXVECTOR3 m_rot;			// �Ԃ̉�]

	float m_speed;
	float m_maxSpeed, m_moveSpeed, m_moveBackSpeed;
	static CSceneFieldX *m_pField;
	CSceneFieldX::FIELD_POLY m_fieldXPoly;	// ���݂���|���S���̒��_���

	float m_rotTire;
	float m_turn;				// �^�C������
	float m_moveTurn;			// �^�C�������
	float m_frictional;			// ���C
	float m_brake;
	D3DXVECTOR3 m_camPosAt;		// �J�����̒����_�ɂ��ė~�����ʒu

	int m_effCnt;
	int m_colSECnt;
	float m_direction;

	bool m_skid, m_idling;
	bool m_brakeSound;
	static std::unordered_map<std::string, CCar*> m_umapCarManager;
};

#endif