//================================================================================
//
//	Auter : KENSUKE WATANABE
//	Data  : 2017/12/17
//
//--------------------------------------------------------------------------------
#ifndef _CHECKPOINT_H_
#define _CHECKPOINT_H_

#include "main.h"
#include <string>
#include "sceneModel.h"

//--------------------------------------------------------------------------------
// ƒNƒ‰ƒX
//--------------------------------------------------------------------------------
class SceneCheckPoint : public CSceneModel
{
public:
	SceneCheckPoint(int priority = 4) :CSceneModel(priority), m_startRot(0.f){ m_classType = CLASSTYPE::SCENE_MODEL; }
	~SceneCheckPoint() {}

	void Init();
	void Uninit();
	void Update();
	void Draw();

	// Žæ“¾
	D3DXVECTOR3 GetPoint(const int &idx) { 
		if (idx >= 2 || idx < 0)return D3DXVECTOR3(0.f, 0.f, 0.f);
		return m_point[idx]; 
	}

	static SceneCheckPoint* Create(const D3DXVECTOR3 &pos, const D3DXVECTOR3 &rot, const D3DXVECTOR3 &scl, const std::string &strFilePass, int pry = 4);

	// ƒ‚ƒfƒ‹¶¬
	void CreateModel(std::string strFilePass);

private:
	D3DXVECTOR3 m_point[2];
	float m_startRot;
};

#endif
