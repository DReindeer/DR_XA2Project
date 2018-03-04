// ======== ======== ======== ======== ======== ======== ======== ========
//
//
//	title : ���w�n�֐��Q(+DirectXMath�̕⏕�֐�)
//	Auter : KENSUKE WATANABE
//
//
// ======== ======== ======== ======== ======== ======== ======== ========
#ifndef _WMATH_H_
#define	_WMATH_H_

#include <DirectXMath.h>
using namespace DirectX;

#define RETURN_MIN(a, b) ((a) < (b) ? (a) : (b))	// �������ق���Ԃ�
#define RETURN_MAX(a, b) ((a) > (b) ? (a) : (b))	// �傫���ق���Ԃ�
#define RETURN_CLAMP(n, a, b) (min(RETURN_MAX((n),(a)),(b)))	// �lor�ŏ��lor�ő�l��Ԃ�

#define PI (3.141592654f) 
#define ToRadian( degree ) ((degree) * (PI / 180.0f))
#define ToDegree( radian ) ((radian) * (180.0f / PI))

namespace XM
{
	namespace F2
	{
		// ======== ======== ======== ======== ======== ======== ======== ========
		// 
		// 
		//								XMFLOAT2
		// 
		// 
		// ======== ======== ======== ======== ======== ======== ======== ========
		// �����Z
		XMFLOAT2 Add(XMFLOAT2 v, XMFLOAT2 addv);
		// �����Z
		XMFLOAT2 Sub(XMFLOAT2 v, XMFLOAT2 subv);
		// �|���Z
		XMFLOAT2 Mul(XMFLOAT2 v, XMFLOAT2 mulv);
		// ����Z
		XMFLOAT2 Div(XMFLOAT2 v, XMFLOAT2 divv);
		// ����
		float Length(XMFLOAT2 v);
		// ����
		float Distance(XMFLOAT2 start, XMFLOAT2 end);
		// ���K��
		XMFLOAT2 Normalize(XMFLOAT2 v);
	}
	namespace F3
	{
		// ======== ======== ======== ======== ======== ======== ======== ========
		// 
		// 
		//								XMFLOAT3
		// 
		// 
		// ======== ======== ======== ======== ======== ======== ======== ========
		// �����Z
		XMFLOAT3 Add(XMFLOAT3 v, XMFLOAT3 addv);
		// �����Z
		XMFLOAT3 Sub(XMFLOAT3 v, XMFLOAT3 subv);
		// �|���Z
		XMFLOAT3 Mul(XMFLOAT3 v, XMFLOAT3 mulv);
		// ����Z
		XMFLOAT3 Div(XMFLOAT3 v, XMFLOAT3 divv);
		// ����
		float Length(XMFLOAT3 v);
		// ����
		float Distance(XMFLOAT3 start, XMFLOAT3 end);
		// ���K��
		XMFLOAT3 Normalize(XMFLOAT3 v);
		// �p�x
		XMFLOAT3 LookAt(float rotY);
		// �p�x
		XMFLOAT3 RotY(XMFLOAT3 move);
	}
	namespace F4
	{
		// ======== ======== ======== ======== ======== ======== ======== ========
		// 
		// 
		//								XMFLOAT4
		// 
		// 
		// ======== ======== ======== ======== ======== ======== ======== ========
		// �����Z
		XMFLOAT4 Add(XMFLOAT4 v, XMFLOAT4 addv);
		// �����Z
		XMFLOAT4 Sub(XMFLOAT4 v, XMFLOAT4 subv);
		// �|���Z
		XMFLOAT4 Mul(XMFLOAT4 v, XMFLOAT4 mulv);
		// ����Z
		XMFLOAT4 Div(XMFLOAT4 v, XMFLOAT4 divv);
		// ����
		float Length(XMFLOAT4 v);
		// ����
		float Distance(XMFLOAT4 start, XMFLOAT4 end);
		// ���K��
		XMFLOAT4 Normalize(XMFLOAT4 v);
	}
	namespace F4X4
	{
		// ======== ======== ======== ======== ======== ======== ======== ========
		// 
		// 
		//								XMFLOAT4X4
		// 
		// 
		// ======== ======== ======== ======== ======== ======== ======== ========
		// �|���Z
		XMFLOAT4X4 Mul(XMFLOAT4X4 m, XMFLOAT4X4 mulm);
		// �P�ʍs��
		XMFLOAT4X4 Identity(XMFLOAT4X4 *m);
		// ���[���h�}�g���N�X
		XMFLOAT4X4 WorldMatrixMultiply(XMMATRIX *mtxScl, XMMATRIX *mtxRot, XMMATRIX *mtxPos);
	}
	// ======== ======== ======== ======== ======== ======== ======== ========
	// 
	// 
	//								�}�g���N�X�n
	// 
	// 
	// ======== ======== ======== ======== ======== ======== ======== ========
	// �s�񂩂��]�����o��
	void ComputeAnglesFromMatrix(XMMATRIX *R, float *angle_x, float *angle_y, float *angle_z);
	// �L�����N�^�p���s��Z�o�֐�
	XMMATRIX* CalcLookAtMatrix(XMMATRIX* pout, XMVECTOR* pPos, XMVECTOR* pLook, XMVECTOR* pUp);
	// �L�����N�^�����p���s��Z�o�֐�
	XMMATRIX* CalcLookAtMatrixAxisFix(XMMATRIX* pout, XMVECTOR* pPos, XMVECTOR* pLook, XMVECTOR* pUp);
};

#endif