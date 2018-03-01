
#ifndef _MAIN_CONTROLLER_H_
#define _MAIN_CONTROLLER_H_

#include <string>
#include "Wmath.h"
#include "utility.h"

static const float PLAYER_LIFE = 1000.f;
static const float PLAYER_JUMP_SPEED = 3.f;

class CSceneXModel;

class CMainController
{
public:
	CMainController() { m_strName.clear(); }
	~CMainController() {}

	void Init(void);
	void Uninit(void);
	void Update(void);
	void Draw(void);

	XMFLOAT3 GetPos(void);
	XMFLOAT3 GetRot(void);
	XMFLOAT3 GetMove(void);
	void SetCamPosAt(void);

	void SetName(std::string name) { m_strName = name; }
	static std::string GetName() { return m_strName; }

private:
	static std::string m_strName;
	CSceneXModel *m_pPlayer;
};

#endif