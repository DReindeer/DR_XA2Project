
#ifndef _MODE_H_
#define _MODE_H_

class CCamera;
class CLight;
class CMode
{
public:
	typedef enum
	{
		NONE = 0,
		TITLE,
		TUTORIAL,
		GAME,
		RESULT,
		LOADING,
		MAX
	}MODETYPE;

	virtual void Init() = 0;
	virtual void Uninit() = 0;
	virtual void Update() = 0;
	virtual void Draw() = 0;

	void SetModeType(MODETYPE modeType) { m_modeType = modeType; }
	MODETYPE GetModeType()const { return m_modeType; }

private:
	MODETYPE m_modeType;
};

#endif