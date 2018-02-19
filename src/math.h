
#ifndef _MATH_H_
#define _MATH_H_

#include "main.h"

namespace WMath
{
	// 関数 Segment_Segment_Analyse の結果を受け取る為の構造体
	typedef struct tagSEGMENT_SEGMENT_RESULT
	{
		float					SegA_SegB_MinDist_Square;	// 線分Ａと線分Ｂが最も接近する座標間の距離の二乗

		float					SegA_MinDist_Pos1_Pos2_t;	// 線分Ａと線分Ｂに最も接近する座標の線分Ａの t ( 0.0f ～ 1.0f 、最近点座標 = ( SegAPos2 - SegAPos1 ) * t + SegAPos1 )
		float					SegB_MinDist_Pos1_Pos2_t;	// 線分Ｂが線分Ａに最も接近する座標の線分Ｂの t ( 0.0f ～ 1.0f 、最近点座標 = ( SegBPos2 - SegBPos1 ) * t + SegBPos1 )

		D3DXVECTOR3				SegA_MinDist_Pos;			// 線分Ａが線分Ｂに最も接近する線分Ａ上の座標
		D3DXVECTOR3				SegB_MinDist_Pos;			// 線分Ｂが線分Ａに最も接近する線分Ｂ上の座標
	} SEGMENT_SEGMENT_RESULT;

	// 関数 Segment_Point_Analyse の結果を受け取る為の構造体
	typedef struct tagSEGMENT_POINT_RESULT
	{
		float					Seg_Point_MinDist_Square;	// 線分と点が最も接近する座標間の距離の二乗
		float					Seg_MinDist_Pos1_Pos2_t;	// 線分が点に最も接近する座標の線分の t ( 0.0f ～ 1.0f 、最近点座標 = ( SegPos2 - SegPos1 ) * t + SegPos1 )
		D3DXVECTOR3				Seg_MinDist_Pos;			// 線分が点に最も接近する線分上の座標
	} SEGMENT_POINT_RESULT;

	class Math
	{
	public:
		// 二つの線分の最接近点情報を解析する
		static int SegmentVSSegment(const D3DXVECTOR3 &SegmentAPos1, const D3DXVECTOR3 &SegmentAPos2,
			const D3DXVECTOR3 &SegmentBPos1, const D3DXVECTOR3 &SegmentBPos2, SEGMENT_SEGMENT_RESULT *Result);

		static int SegmentVSPoin(const D3DXVECTOR3 &SegmentPos1, const D3DXVECTOR3 &SegmentPos2, const D3DXVECTOR3 &PointPos, SEGMENT_POINT_RESULT *Result);

		static float VSquareSize(D3DXVECTOR3 in);

		static D3DXMATRIX* CalcLookAtMatrix(D3DXMATRIX* pout, D3DXVECTOR3* pPos, D3DXVECTOR3* pLook, D3DXVECTOR3* pUp);
	};
}

#endif
