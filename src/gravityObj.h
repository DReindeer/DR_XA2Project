
#ifndef _GRAVITYOBJ_H_
#define _GRAVITYOBJ_H_

#include "main.h"
#include <string>
#include "sceneModel.h"

class CGravityObj : public CSceneModel
{
public:
	CGravityObj() : m_move(D3DXVECTOR3(0.f, 0.f, 0.f)), m_frictional(0.8f) { SetObjType(OBJTYPE::GRAVITY_OBJ); }
	~CGravityObj() {}

	void Init();
	void Uninit();
	void Update();
	void Draw();
	CGravityObj *Create(const D3DXVECTOR3 &pos, const D3DXVECTOR3 &rot, const D3DXVECTOR3 &scl, const std::string &strFilePass);

	const float &GetFrictional()const { return m_frictional; }
	const D3DXVECTOR3 &GetMove()const { return m_move; }
private:
	D3DXVECTOR3 m_move;
	float m_frictional;			// –€ŽC
	float m_posOldY;
	bool m_jump;
};

#endif