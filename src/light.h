//================================================================================
//
//	title : classMake[ light.h ]			Auter : KENSUKE WATANABE
//													Data  : 2017/04/27
//
//--------------------------------------------------------------------------------
#ifndef _LIGHT_H_
#define _LIGHT_H_

static const unsigned int MAX_LIGHT = 1;

class CLight
{
public:
	CLight();
	~CLight() {}

	void Init();

	D3DXVECTOR3 GetPos(int n);
	D3DXVECTOR3 GetVecAtZERO(int n);
	D3DXVECTOR3 GetDir(int n); 
	D3DXVECTOR4 GetAmbient(int n);
private:

	D3DLIGHT9 m_light[MAX_LIGHT];
};

#endif