//================================================================================
//
//	Auter : KENSUKE WATANABE
//	Data  : 2018/02/10
//
//--------------------------------------------------------------------------------
#include "math.h"

using namespace WMath;

// 二つの線分の最接近点情報を解析する
int Math::SegmentVSSegment(const D3DXVECTOR3 &SegmentAPos1, const D3DXVECTOR3 &SegmentAPos2,
	const D3DXVECTOR3 &SegmentBPos1, const D3DXVECTOR3 &SegmentBPos2, SEGMENT_SEGMENT_RESULT *Result)
{
	D3DXVECTOR3 tp;
	float a, b, c, d, e, f, w, s, t = 0.0f;
	float tmpA, tmpB;
	SEGMENT_POINT_RESULT spres;

	D3DXVECTOR3 segA1_2 = SegmentAPos2 - SegmentAPos1;
	D3DXVECTOR3 segB1_2 = SegmentBPos2 - SegmentBPos1;
	D3DXVECTOR3 segB1_A1 = SegmentAPos1 - SegmentBPos1;
	a = D3DXVec3Dot(&segA1_2, &segA1_2);
	b = -D3DXVec3Dot(&segA1_2, &segB1_2);
	c = -b;
	d = -D3DXVec3Dot(&segB1_2, &segB1_2);
	e = -D3DXVec3Dot(&segA1_2, &segB1_A1);
	f = -D3DXVec3Dot(&segB1_2, &segB1_A1);

	// SegmentAPos1 - SegmentAPos2 または SegmentBPos1 - SegmentBPos2 の距離が限りなくゼロに近いかどうかのチェック
	tmpA = a < 0.0f ? -a : a;
	tmpB = d < 0.0f ? -d : d;
	if (tmpA < 0.00000001f)
	{
		Result->SegA_MinDist_Pos1_Pos2_t = 0.0f;
		Result->SegA_MinDist_Pos = SegmentAPos1;
		if (tmpB < 0.00000001f)
		{
			Result->SegB_MinDist_Pos1_Pos2_t = 0.0f;
			Result->SegB_MinDist_Pos = SegmentBPos1;
			Result->SegA_SegB_MinDist_Square = VSquareSize(segB1_A1);
		}
		else
		{
			SegmentVSPoin(SegmentBPos1, SegmentBPos2, SegmentAPos1, &spres);

			Result->SegB_MinDist_Pos1_Pos2_t = spres.Seg_MinDist_Pos1_Pos2_t;
			Result->SegB_MinDist_Pos = spres.Seg_MinDist_Pos;
			Result->SegA_SegB_MinDist_Square = spres.Seg_Point_MinDist_Square;
		}
		return 0;
	}
	else
		if (tmpB < 0.00000001f)
		{
			Result->SegB_MinDist_Pos1_Pos2_t = 0.0f;
			Result->SegB_MinDist_Pos = SegmentBPos1;

			SegmentVSPoin(SegmentAPos1, SegmentAPos2, SegmentBPos1, &spres);

			Result->SegA_MinDist_Pos1_Pos2_t = spres.Seg_MinDist_Pos1_Pos2_t;
			Result->SegA_MinDist_Pos = spres.Seg_MinDist_Pos;
			Result->SegA_SegB_MinDist_Square = spres.Seg_Point_MinDist_Square;

			return 0;
		}

	s = -0.1f;
	w = a * d - b * c;
	if (w <= -0.00000001f)
	{
		// クラーメルの公式
		s = (e * d - b * f) / w;
		t = (a * f - e * c) / w;
	}

	// 二つの線分が縮退していたら点と見なして点同士の距離を返す
	if (a <= 0.0f && -d <= 0.0f)
	{
		Result->SegA_MinDist_Pos1_Pos2_t = 0.0f;
		Result->SegB_MinDist_Pos1_Pos2_t = 0.0f;
		Result->SegA_MinDist_Pos = SegmentAPos1;
		Result->SegB_MinDist_Pos = SegmentBPos1;
		Result->SegA_SegB_MinDist_Square = VSquareSize(segB1_A1);

		return 0;
	}

	if (a <= 0.0f)
	{
		s = 0.0f;
		if (t > 1.0f || t < 0.0f)
			t = t > 1.0f ? 1.0f : 0.0f;
	}
	else if (-d <= 0.0f)
	{
		t = 0.0f;
		if (s > 1.0f || s < 0.0f)
			s = s > 1.0f ? 1.0f : 0.0f;
	}
	else
	{
		if (s > 1.0f || s < 0.0f)
		{
			s = s > 1.0f ? 1.0f : 0.0f;

			t = (-b * s + -f) / -d;		// ガウス消去法
			if (t > 1.0f || t < 0.0f)
			{
				t = t > 1.0f ? 1.0f : 0.0f;
				s = (-b * t - -e) / a;	// ガウス消去法
				if (s > 1.0f) s = 1.0f;
				else if (s < 0.0f) s = 0.0f;
			}
		}
		else
			if (t > 1.0f || t < 0.0f)
			{
				t = t > 1.0f ? 1.0f : 0.0f;
				s = (-b * t - -e) / a;		// ガウス消去法

				if (s > 1.0f || s < 0.0f)
				{
					s = s > 1.0f ? 1.0f : 0.0f;
					t = (-b * s + -f) / -d;	// ガウス消去法
					if (t > 1.0f) t = 1.0f;
					else if (t < 0.0f) t = 0.0f;
				}
			}
	}
	Result->SegA_MinDist_Pos1_Pos2_t = s;
	Result->SegB_MinDist_Pos1_Pos2_t = t;

	Result->SegA_MinDist_Pos = segA1_2 * s;
	Result->SegA_MinDist_Pos = Result->SegA_MinDist_Pos + SegmentAPos1;

	Result->SegB_MinDist_Pos = segB1_2 * t;
	Result->SegB_MinDist_Pos = Result->SegB_MinDist_Pos + SegmentBPos1;

	tp = Result->SegA_MinDist_Pos, Result->SegB_MinDist_Pos;
	Result->SegA_SegB_MinDist_Square = VSquareSize(tp);

	return 0;
}

// 線分と点の最接近点情報を解析する
int Math::SegmentVSPoin(const D3DXVECTOR3 &SegmentPos1, const D3DXVECTOR3 &SegmentPos2, const D3DXVECTOR3 &PointPos, SEGMENT_POINT_RESULT *Result)
{
	float SizeSquSeg1_2;
	float DotSeg1_2_Pnt;
	D3DXVECTOR3 Seg1_2, Seg1_Pnt;

	Seg1_2 = SegmentPos2 - SegmentPos1;
	Seg1_Pnt = PointPos - SegmentPos1;
	DotSeg1_2_Pnt = D3DXVec3Dot(&Seg1_2, &Seg1_Pnt);
	if (DotSeg1_2_Pnt <= 0.0f)
	{
		Result->Seg_MinDist_Pos = SegmentPos1;
		Result->Seg_MinDist_Pos1_Pos2_t = 0.0f;
		Result->Seg_Point_MinDist_Square = VSquareSize(PointPos - Result->Seg_MinDist_Pos);

		return 0;
	}

	SizeSquSeg1_2 = VSquareSize(Seg1_2);
	if (SizeSquSeg1_2 == 0.0f)
	{
		Result->Seg_MinDist_Pos = SegmentPos1;
		Result->Seg_MinDist_Pos1_Pos2_t = 0.0f;
		Result->Seg_Point_MinDist_Square = VSquareSize(PointPos - Result->Seg_MinDist_Pos);

		return 0;
	}

	if (SizeSquSeg1_2 < DotSeg1_2_Pnt)
	{
		Result->Seg_MinDist_Pos = SegmentPos2;
		Result->Seg_MinDist_Pos1_Pos2_t = 1.0f;
		Result->Seg_Point_MinDist_Square = VSquareSize(PointPos - Result->Seg_MinDist_Pos);

		return 0;
	}

	Result->Seg_MinDist_Pos1_Pos2_t = DotSeg1_2_Pnt / SizeSquSeg1_2;
	Result->Seg_MinDist_Pos = (Seg1_2 * Result->Seg_MinDist_Pos1_Pos2_t) + SegmentPos1;
	Result->Seg_Point_MinDist_Square = VSquareSize(PointPos - Result->Seg_MinDist_Pos);

	return 0;
}

// ベクトルのサイズの２乗
float Math::VSquareSize(D3DXVECTOR3 in)
{
	return in.x * in.x + in.y * in.y + in.z * in.z;
}

// キャラクタ姿勢行列算出関数
D3DXMATRIX* Math::CalcLookAtMatrix(
	D3DXMATRIX* pout,
	D3DXVECTOR3* pPos,
	D3DXVECTOR3* pLook,
	D3DXVECTOR3* pUp)
{
	D3DXVECTOR3 X, Y, Z;
	Z = *pLook - *pPos;
	D3DXVec3Normalize(&Z, &Z);
	D3DXVec3Cross(&X, D3DXVec3Normalize(&Y, pUp), &Z);
	D3DXVec3Normalize(&X, &X);
	D3DXVec3Normalize(&Y, D3DXVec3Cross(&Y, &Z, &X));

	pout->_11 = X.x; pout->_12 = X.y; pout->_13 = X.z; pout->_14 = 0;
	pout->_21 = Y.x; pout->_22 = Y.y; pout->_23 = Y.z; pout->_24 = 0;
	pout->_31 = Z.x; pout->_32 = Z.y; pout->_33 = Z.z; pout->_34 = 0;
	pout->_41 = 0.f; pout->_42 = 0.f; pout->_43 = 0.f; pout->_44 = 1.f;

	return pout;
}