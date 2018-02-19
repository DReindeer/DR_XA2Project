//================================================================================
//
//	Auter : KENSUKE WATANABE
//	Data  : 2018/02/10
//
//--------------------------------------------------------------------------------
#include "main.h"
#include "manager.h"
#include "scene.h"
#include "score.h"
#include "renderer.h"
#include "number.h"

//--------------------------------------------------------------------------------
// CScore::����������
//--------------------------------------------------------------------------------
void CScore::Init()
{
	m_score = 0;
}

//--------------------------------------------------------------------------------
// CScore::�I������
//--------------------------------------------------------------------------------
void CScore::Uninit()
{
	for(int nCnt = 0; nCnt < m_disit; nCnt++)
	{
		m_apNumber[nCnt].Uninit();
	}
}

//--------------------------------------------------------------------------------
// CScore::�X�V����
//--------------------------------------------------------------------------------
void CScore::Update()
{
	int num, value = m_score;
	for(int nCnt = m_disit - 1; nCnt >= 0; nCnt--)
	{
		num = value % 10;
		m_apNumber[nCnt].SetNumber(num);
		if(value != 0)value /= 10;
	}
	
	for(int nCnt = 0; nCnt < m_disit; nCnt++)
	{
		m_apNumber[nCnt].Update();
	}
}

//--------------------------------------------------------------------------------
// CScore::�`�揈��
//--------------------------------------------------------------------------------
void CScore::Draw()
{
	for(int nCnt = 0; nCnt < m_disit; nCnt++)
	{
		m_apNumber[nCnt].Draw();
	}
}

//--------------------------------------------------------------------------------
// CScore::�쐬����
//--------------------------------------------------------------------------------
CScore *CScore::Create(D3DXVECTOR3 pos, float rot, D3DXVECTOR3 size, int digit, int priority)
{
	// CScore�̐���
	CScore *pScore;
	pScore = new CScore(digit, priority);
	pScore->Init();

	pScore->m_pos = pos;		// ���������W����
	pScore->m_size = size;		// �傫��
	pScore->m_rot = D3DXVECTOR3(0.f, 0.f, rot);

	// �i���o�[�̍쐬
	pScore->m_apNumber = CNumber::Create(pScore->m_pos, pScore->m_rot, pScore->m_size, pScore->m_disit);

	return pScore;
}
