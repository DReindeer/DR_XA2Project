//================================================================================
//
//	title : �|���S���\���v���O����[ Scene2DCircleMap.h ]	Auter : KENSUKE WATANABE
//													Data  : 2016/11/30
//
//--------------------------------------------------------------------------------
#ifndef _SCENE2DCIRCLEMAP_H_
#define _SCENE2DCIRCLEMAP_H_

#include "main.h"
#include <string>
#include "scene2D.h"

//--------------------------------------------------------------------------------
// �N���X
//--------------------------------------------------------------------------------
class CScene2DCircleMap : public CScene
{
public:
	CScene2DCircleMap(int priority = 4) :CScene(priority) { m_classType = CLASSTYPE::SCENE_2D; }
	~CScene2DCircleMap() {}

	void Init();				// ������
	void Uninit();				// �I��
	void Update();				// �X�V
	void Draw();				// �`��
	static CScene2DCircleMap *Create(const D3DXVECTOR3 &pos, const D3DXVECTOR3 &rot, float len, int numOutVtx, D3DXVECTOR2 posUV, float lenUV, const std::string &strFilePass, int priority = 4);

	void SetStrFilePass(std::string strFilePass) { m_strFilePass = strFilePass; }
	const std::string &GetStrFilePass() { return m_strFilePass; }
	void SetTexturePos(D3DXVECTOR2 uv, D3DXVECTOR2 length);	
	void SetPosUV(const D3DXVECTOR2 posUV) { m_posUV = posUV; }
	void SetRotUV(const float rotUV) { m_rotUV = rotUV; }

protected:
	D3DXVECTOR2 m_posUV;		// �e�N�X�`�����W
	float m_lenUV;
	float m_rotUV;
	float m_widthUV;			// ��
	float m_heightUV;			// ����
	std::string m_strFilePass;	// �e�N�X�`��FilePass

	float m_len;
	int m_numOutVtx;

	LPDIRECT3DINDEXBUFFER9 m_pIdxBufferParAlpha[2];
private:

	void InitDeviceLock();
	bool m_bInitDeviceLock;
};

#endif
