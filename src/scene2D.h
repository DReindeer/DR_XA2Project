//================================================================================
//
//	title : �|���S���\���v���O����[ CScene2D.h ]	Auter : KENSUKE WATANABE
//													Data  : 2016/05/30
//
//--------------------------------------------------------------------------------
#ifndef _SCENE2D_H_
#define _SCENE2D_H_

#include "main.h"
#include <string>
#include "scene.h"

//--------------------------------------------------------------------------------
// �O���錾
//--------------------------------------------------------------------------------
class CTexManager;

//--------------------------------------------------------------------------------
// �N���X
//--------------------------------------------------------------------------------
class CScene2D : public CScene
{
public:
	CScene2D(int priority = 4) :CScene(priority) { m_classType = CLASSTYPE::SCENE_2D; }
	~CScene2D();

	void Init();				// ������
	void Uninit();				// �I��
	void Update();				// �X�V
	void Draw();				// �`��
	static CScene2D *Create(const D3DXVECTOR3 &pos, const D3DXVECTOR3 &rot, const D3DXVECTOR3 &size, const std::string &strFilePass, int priority = 4, CTexManager *pUseTexManager = nullptr);

	void SetStrFilePass(std::string strFilePass) { m_strFilePass = strFilePass; }
	const std::string &GetStrFilePass() { return m_strFilePass; }
	void SetTexturePos(D3DXVECTOR2 uv, D3DXVECTOR2 length);	

	void ScreenResize();

protected:
	void SetUseTexManager(CTexManager *pUseTexManager) { m_pUseTexManager = pUseTexManager; }

	D3DXVECTOR2 m_posUV;		// �e�N�X�`�����W
	float m_widthUV;			// ��
	float m_heightUV;			// ����
	std::string m_strFilePass;	// �e�N�X�`��FilePass

private:
	CTexManager *m_pUseTexManager;	// �g�p����e�N�X�`���}�l�[�W��
};

#endif
