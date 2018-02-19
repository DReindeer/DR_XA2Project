
#ifndef _BGCYLINDER_H_
#define _BGCYLINDER_H_

#include "sceneCylinder.h"

#define MAX_CREATEBOXFLAME (5)

class CBgCylinder : public CSceneCylinder
{
public:
	CBgCylinder(int priority = 4) : CSceneCylinder(priority) {}
	~CBgCylinder() {}

	void Init();
	void Uninit();
	void Update();
	void Draw();
	HRESULT SetVtx(int stacks, int slices);
	static CBgCylinder *Create(const D3DXVECTOR3 &pos, const D3DXVECTOR3 &rot, const D3DXVECTOR3 &size, const std::string &strFilePass);

private:
	D3DXVECTOR2 m_moveUV;
};

#endif