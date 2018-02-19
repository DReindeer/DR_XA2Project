//================================================================================
//
// �X�s�[�h���[�^�[���� [speedMeter.h]
// Author : KENSUKE WATANABE
//
//================================================================================
#ifndef _SPEEDMATER_H_
#define _SPEEDMATER_H_

#include "main.h"

//================================================================================
// �N���X��`
//--------------------------------------------------------------------------------
// �O���錾
class CScene2D;
class CScore;
class CSpeedMeter
{
public:
	CSpeedMeter() : m_needleRot(0.f), m_pSpeedMeter(nullptr), m_pSpeedMeterNeedle(nullptr){}
	~CSpeedMeter() {}

	void Init();	// ������
	void Uninit();	// �I��
	void Update();	// �X�V
	void Draw();	// �`��
	static CSpeedMeter *Create(const D3DXVECTOR2 &pos2D, const float &len);	// ����

//	static const CScore &GetScore(){ return *m_pScore; }		// �X�R�A�̎擾

private:
	float m_needleRot;

	CScene2D *m_pSpeedMeter;
	CScene2D *m_pSpeedMeterNeedle;
	CScore *m_pSpeed;				// �X�s�[�h�\��
};

#endif
