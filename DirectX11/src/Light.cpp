// ======== ======== ======== ======== ======== ======== ======== ========
//
//
//	title : ƒ‰ƒCƒg
//	Auter : KENSUKE WATANABE
//
//
// ======== ======== ======== ======== ======== ======== ======== ========
#include "Light.h"
#include "manager.h"
#include "camera.h"

void CLight::Init(void)
{

}

void CLight::Update(void)
{
	m_pos = XMFLOAT4(0.0f, 40.0f, -70.0f, 1.0f);
	//CManager::GetCamera()->GetConstantBuffer()->
}
