
#ifndef _MAIN_CONTROLLER_H_
#define _MAIN_CONTROLLER_H_

#include <string>
#include "main.h"
#include "sceneModel.h"
#include "utility.h"

class CCar;
class CMainController
{
public:
	CMainController() { m_strName.clear(); }
	~CMainController() {}

	void Init();
	void Uninit();
	void Update();
	void Draw();

	D3DXVECTOR3 GetPosOld()const;
	D3DXVECTOR3 GetPos()const;
	D3DXVECTOR3 GetRot()const;
	D3DXVECTOR3 GetMove()const;
	float GetMaxSpeed()const;
	void SetCamPosAt();

	void SetName(std::string name) { m_strName = name; }
	static std::string GetName() { return m_strName; }

private:
	static std::string m_strName;
};

#endif
