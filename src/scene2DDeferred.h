//================================================================================
//
//	title : �|���S���\���v���O����[ CScene2D.h ]	Auter : KENSUKE WATANABE
//													Data  : 2016/05/30
//
//--------------------------------------------------------------------------------
#ifndef _SCENE2DDEFERRED_H_
#define _SCENE2DDEFERRED_H_

#include "scene2D.h"

//--------------------------------------------------------------------------------
// �O���錾
//--------------------------------------------------------------------------------
class CTexManager;

//--------------------------------------------------------------------------------
// �N���X
//--------------------------------------------------------------------------------
class CScene2DDeferred : public CScene2D
{
public:
	CScene2DDeferred(int priority = 4) :CScene2D(priority) { m_classType = CLASSTYPE::SCENE_2D; }
	~CScene2DDeferred();

	void Init();				// ������
	void Uninit();				// �I��
	void Update();				// �X�V
	void Draw();				// �`��
	static CScene2DDeferred *Create(const D3DXVECTOR3 &pos, const D3DXVECTOR3 &rot, const D3DXVECTOR3 &size, const std::string &strFilePass, int priority = 4, CTexManager *pUseTexManager = nullptr);
	
};

#endif