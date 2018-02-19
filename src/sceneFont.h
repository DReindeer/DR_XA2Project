//--------------------------------------------------------------------------------
//
//	title : �|���S���\���v���O����[ CSceneFont.h ]	Auter : KENSUKE WATANABE
//													Data  : 2016/06/18
//
//--------------------------------------------------------------------------------
#ifndef _SCENEFONT_H_
#define _SCENEFONT_H_

#include "main.h"
#include <string>
#include "scene.h"
#include "scene2D.h"

//--------------------------------------------------------------------------------
// �N���X
//--------------------------------------------------------------------------------
class CSceneFont : public CScene
{
public:
	CSceneFont(int priority = 4) : CScene(priority){}
	~CSceneFont() {}

	void Init();				// ������
	void Uninit();				// �I��
	void Update();				// �X�V
	void Draw();				// �`��
	static CSceneFont *Create(const D3DXVECTOR3 &pos, const D3DXVECTOR3 &rot, const float &height, const int &weight, const wchar_t *wcFont);

private:
	std::string m_strFilePass;	// �e�N�X�`��FilePass
	GLYPHMETRICS m_gm;
	int m_fontWidth, m_fontHeight, m_fontRelativeVtxPos;
};

#endif
