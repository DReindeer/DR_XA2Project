//================================================================================
//
//	title : �|���S���\���v���O����[ sceneCircleShadow.h ]	Auter : KENSUKE WATANABE
//													Data  : 2016/05/30
//
//--------------------------------------------------------------------------------
#ifndef _STENCILSHADOW_H_
#define _STENCILSHADOW_H_

#include "main.h"
#include <string>
#include "scene2D.h"

//--------------------------------------------------------------------------------
// �N���X
//--------------------------------------------------------------------------------
class CStencilShadow : public CScene2D
{
public:
	CStencilShadow(int priority = 7) :CScene2D(priority) { m_classType = CLASSTYPE::SCENE_2D; }
	~CStencilShadow() {}

	void Init();				// ������
	void Uninit();				// �I��
	void Update();				// �X�V
	void Draw();				// �`��
	static CStencilShadow *Create(const D3DXVECTOR3 &pos, const D3DXVECTOR3 &rot, const D3DXVECTOR3 &size, const std::string &strFilePass, int priority = 4);

protected:

private:
};

#endif
