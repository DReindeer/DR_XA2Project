
#ifndef _BAR_H_
#define _BAR_H_

#include <unordered_map>
#include <assert.h>
#include "scene2D.h"
#include "scene3D.h"

class CBar2D
{
public:
	CBar2D(): m_basePos(D3DXVECTOR3(0.f, 0.f, 0.f)), m_maxVal(0) { for (int i = 0; i < 3; i++) { m_pBar[i] = nullptr; } }
	~CBar2D() {}

	void Init();
	void Uninit();
	void Update();
	void Draw();

	void SetCol(int r, int g, int b, int a, int n) {
		assert(n < 3 && n >= 0 && "ˆø”n‚Ì”’l‚ª•s³‚Å‚·");
		m_pBar[n]->SetCol(r, g, b, a);
	}

	static CBar2D* Create(D3DXVECTOR2 pos, D3DXVECTOR2 size, int r, int g, int b, int a, float maxVal, std::string strName, int priority = 5);
	static void UpdateAll();
	static void DrawAll();
	static void ReleaseAll();
	static void AddVal(float addVal, std::string strName);

private:
	void followingBar();

	std::string m_strName;
	CScene2D *m_pBar[3];
	float m_maxVal;
	D3DXVECTOR3 m_basePos;

	static std::unordered_map<std::string, CBar2D*> umapBar2D;
};

class CBar3D : public CScene3D
{
public:
	CBar3D(int priority = 4) :CScene3D(priority) {}
	~CBar3D() {}

	void Init() {}
	void Uninit() {}
	void Update() {}
	void Draw() {}



private:

};

#endif