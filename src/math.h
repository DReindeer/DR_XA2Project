
#ifndef _MATH_H_
#define _MATH_H_

#include "main.h"

namespace WMath
{
	// �֐� Segment_Segment_Analyse �̌��ʂ��󂯎��ׂ̍\����
	typedef struct tagSEGMENT_SEGMENT_RESULT
	{
		float					SegA_SegB_MinDist_Square;	// �����`�Ɛ����a���ł��ڋ߂�����W�Ԃ̋����̓��

		float					SegA_MinDist_Pos1_Pos2_t;	// �����`�Ɛ����a�ɍł��ڋ߂�����W�̐����`�� t ( 0.0f �` 1.0f �A�ŋߓ_���W = ( SegAPos2 - SegAPos1 ) * t + SegAPos1 )
		float					SegB_MinDist_Pos1_Pos2_t;	// �����a�������`�ɍł��ڋ߂�����W�̐����a�� t ( 0.0f �` 1.0f �A�ŋߓ_���W = ( SegBPos2 - SegBPos1 ) * t + SegBPos1 )

		D3DXVECTOR3				SegA_MinDist_Pos;			// �����`�������a�ɍł��ڋ߂�������`��̍��W
		D3DXVECTOR3				SegB_MinDist_Pos;			// �����a�������`�ɍł��ڋ߂�������a��̍��W
	} SEGMENT_SEGMENT_RESULT;

	// �֐� Segment_Point_Analyse �̌��ʂ��󂯎��ׂ̍\����
	typedef struct tagSEGMENT_POINT_RESULT
	{
		float					Seg_Point_MinDist_Square;	// �����Ɠ_���ł��ڋ߂�����W�Ԃ̋����̓��
		float					Seg_MinDist_Pos1_Pos2_t;	// �������_�ɍł��ڋ߂�����W�̐����� t ( 0.0f �` 1.0f �A�ŋߓ_���W = ( SegPos2 - SegPos1 ) * t + SegPos1 )
		D3DXVECTOR3				Seg_MinDist_Pos;			// �������_�ɍł��ڋ߂��������̍��W
	} SEGMENT_POINT_RESULT;

	class Math
	{
	public:
		// ��̐����̍Őڋߓ_������͂���
		static int SegmentVSSegment(const D3DXVECTOR3 &SegmentAPos1, const D3DXVECTOR3 &SegmentAPos2,
			const D3DXVECTOR3 &SegmentBPos1, const D3DXVECTOR3 &SegmentBPos2, SEGMENT_SEGMENT_RESULT *Result);

		static int SegmentVSPoin(const D3DXVECTOR3 &SegmentPos1, const D3DXVECTOR3 &SegmentPos2, const D3DXVECTOR3 &PointPos, SEGMENT_POINT_RESULT *Result);

		static float VSquareSize(D3DXVECTOR3 in);

		static D3DXMATRIX* CalcLookAtMatrix(D3DXMATRIX* pout, D3DXVECTOR3* pPos, D3DXVECTOR3* pLook, D3DXVECTOR3* pUp);
	};
}

#endif
