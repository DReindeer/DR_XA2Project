//--------------------------------------------------------------------------------
//
// �V�[��2D���� [number.h]
// Author : KENSUKE WATANABE
//
//--------------------------------------------------------------------------------
#ifndef _CNUMBER_H_
#define _CNUMBER_H_

#include "main.h"

//--------------------------------------------------------------------------------
// �N���X��`
//--------------------------------------------------------------------------------
class CNumber
{
public:
	CNumber();		// �R���X�g���N�^
	~CNumber();		// �f�X�g���N�^

	HRESULT Init(D3DXVECTOR3 pos, float rot, D3DXVECTOR3 size);	// ������
	void Uninit();	// �I��
	void Update();	// �X�V
	void Draw();	// �`��

	static CNumber *Create(D3DXVECTOR3 pos, float rot, D3DXVECTOR3 size);	// ����
	static CNumber *Create(D3DXVECTOR3 pos, D3DXVECTOR3 rot, D3DXVECTOR3 size, const int disit);	// ����
	
	void SetNumber(int nNumber);	// �`�悵�����ԍ��̐ݒ�(�e�N�X�`�����W�̐ݒ�)

	void SetCol(int r, int g, int b, int a) { m_r = r; m_g = g; m_b = b; m_a = a; }

private:
	LPDIRECT3DVERTEXBUFFER9 m_pVtxBuffer;		// ���_�o�b�t�@�ւ̃|�C���^
	
	D3DXVECTOR3 m_pos;		// �|���S���̈ʒu
	D3DXVECTOR3 m_size;		// �傫��
	float m_rot;			// �p�x
	int m_r, m_g, m_b, m_a;	// �F
};

#endif
