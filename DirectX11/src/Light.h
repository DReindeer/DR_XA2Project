// ======== ======== ======== ======== ======== ======== ======== ========
//
//
//	title : ƒ‰ƒCƒg
//	Auter : KENSUKE WATANABE
//
//
// ======== ======== ======== ======== ======== ======== ======== ========
#ifndef _LIGHT_H_
#define _LIGHT_H_

#include "Wmath.h"

static const unsigned int MAX_LIGHT = 2;

class CLight
{
public:
	CLight() {}
	~CLight() {}

	void Init(void);
	void Update(void);

	XMFLOAT4 GetPos() { return m_pos; }

private:

	XMFLOAT4 m_pos;
};

#endif