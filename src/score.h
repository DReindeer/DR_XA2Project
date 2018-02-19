//--------------------------------------------------------------------------------
//
// �V�[��2D���� [score.h]
// Author : KENSUKE WATANABE
//
//--------------------------------------------------------------------------------
#ifndef _SCORE_H_
#define _SCORE_H_

//--------------------------------------------------------------------------------
// �w�b�_�t�@�C��
//--------------------------------------------------------------------------------
#include "Scene.h"
#include "number.h"

//--------------------------------------------------------------------------------
// �萔��`
//--------------------------------------------------------------------------------
#define MAX_DIGIT (8)

//--------------------------------------------------------------------------------
// �N���X��`
//--------------------------------------------------------------------------------
class CScore : public CScene
{
public:
	CScore(int priority = 4) :CScene(priority), m_disit(MAX_DIGIT), m_score(0){};			// �R���X�g���N�^
	CScore(int digit, int priority = 4) :CScene(priority), m_disit(digit), m_score(0) {};	// �R���X�g���N�^
	~CScore() {};												// �f�X�g���N�^

	void Init();	// ������
	void Uninit();	// �I��
	void Update();	// �X�V
	void Draw();	// �`��

	static CScore *Create(D3DXVECTOR3 pos, float rot, D3DXVECTOR3 size, int digit, int priority = 4);	// ����
	
	void SetScore(int nValue) { m_score = nValue; }		// �X�R�A�̐ݒ�
	void AddScore(int nValue) { m_score += nValue; }	// �X�R�A�̉��Z
	int* GetScore() { return &m_score; }				// �X�R�A�̎擾

	void SetCol(int r, int g, int b, int a) { 
		for (int i = 0; i < m_disit; i++) { m_apNumber[i].SetCol(r, g, b, a); } 
	}

private:
	const int m_disit;
	CNumber* m_apNumber;	// �i���o�[
	int m_score;			// �X�R�A����
};

#endif
