// ======== ======== ======== ======== ======== ======== ======== ========
//
//
//	title : 数学系関数群(+DirectXMathの補助関数)
//	Auter : KENSUKE WATANABE
//
//
// ======== ======== ======== ======== ======== ======== ======== ========
#ifndef _WMATH_H_
#define	_WMATH_H_

#include <DirectXMath.h>
using namespace DirectX;

#define RETURN_MIN(a, b) ((a) < (b) ? (a) : (b))	// 小さいほうを返す
#define RETURN_MAX(a, b) ((a) > (b) ? (a) : (b))	// 大きいほうを返す
#define RETURN_CLAMP(n, a, b) (min(RETURN_MAX((n),(a)),(b)))	// 値or最小値or最大値を返す

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
		// 足し算
		XMFLOAT2 Add(XMFLOAT2 v, XMFLOAT2 addv);
		// 引き算
		XMFLOAT2 Sub(XMFLOAT2 v, XMFLOAT2 subv);
		// 掛け算
		XMFLOAT2 Mul(XMFLOAT2 v, XMFLOAT2 mulv);
		// 割り算
		XMFLOAT2 Div(XMFLOAT2 v, XMFLOAT2 divv);
		// 長さ
		float Length(XMFLOAT2 v);
		// 距離
		float Distance(XMFLOAT2 start, XMFLOAT2 end);
		// 正規化
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
		// 足し算
		XMFLOAT3 Add(XMFLOAT3 v, XMFLOAT3 addv);
		// 引き算
		XMFLOAT3 Sub(XMFLOAT3 v, XMFLOAT3 subv);
		// 掛け算
		XMFLOAT3 Mul(XMFLOAT3 v, XMFLOAT3 mulv);
		// 割り算
		XMFLOAT3 Div(XMFLOAT3 v, XMFLOAT3 divv);
		// 長さ
		float Length(XMFLOAT3 v);
		// 距離
		float Distance(XMFLOAT3 start, XMFLOAT3 end);
		// 正規化
		XMFLOAT3 Normalize(XMFLOAT3 v);
		// 角度
		XMFLOAT3 LookAt(float rotY);
		// 角度
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
		// 足し算
		XMFLOAT4 Add(XMFLOAT4 v, XMFLOAT4 addv);
		// 引き算
		XMFLOAT4 Sub(XMFLOAT4 v, XMFLOAT4 subv);
		// 掛け算
		XMFLOAT4 Mul(XMFLOAT4 v, XMFLOAT4 mulv);
		// 割り算
		XMFLOAT4 Div(XMFLOAT4 v, XMFLOAT4 divv);
		// 長さ
		float Length(XMFLOAT4 v);
		// 距離
		float Distance(XMFLOAT4 start, XMFLOAT4 end);
		// 正規化
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
		// 掛け算
		XMFLOAT4X4 Mul(XMFLOAT4X4 m, XMFLOAT4X4 mulm);
		// 単位行列
		XMFLOAT4X4 Identity(XMFLOAT4X4 *m);
		// ワールドマトリクス
		XMFLOAT4X4 WorldMatrixMultiply(XMMATRIX *mtxScl, XMMATRIX *mtxRot, XMMATRIX *mtxPos);
	}
	// ======== ======== ======== ======== ======== ======== ======== ========
	// 
	// 
	//								マトリクス系
	// 
	// 
	// ======== ======== ======== ======== ======== ======== ======== ========
	// 行列から回転を取り出す
	void ComputeAnglesFromMatrix(XMMATRIX *R, float *angle_x, float *angle_y, float *angle_z);
	// キャラクタ姿勢行列算出関数
	XMMATRIX* CalcLookAtMatrix(XMMATRIX* pout, XMVECTOR* pPos, XMVECTOR* pLook, XMVECTOR* pUp);
	// キャラクタ束縛姿勢行列算出関数
	XMMATRIX* CalcLookAtMatrixAxisFix(XMMATRIX* pout, XMVECTOR* pPos, XMVECTOR* pLook, XMVECTOR* pUp);
};

#endif