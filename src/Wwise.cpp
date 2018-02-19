// ======== ======== ======== ======== ======== ======== ======== ========
//
//			[Wwise.cpp] 						Auter : KENSUKE WATANABE
//												Data  : 2017/09/06
//
// -------- -------- -------- -------- -------- -------- -------- --------
//��Update : 2017/09/06
//
// ======== ======== ======== ======== ======== ======== ======== ========
#include "main.h"
#include "Wwise.h"

// �������}�l�[�W���ɕK�v
#include <AK/SoundEngine/Common/AkMemoryMgr.h>				// �������}�l�[�W��
#include <AK/SoundEngine/Common/AkModule.h>					// �f�t�H���g������, �X�g���[���}�l�[�W��

// �X�g���[�~���O�Đ��ɕK�v
#include <AK/SoundEngine/Common/IAkStreamMgr.h>				// �X�g���[���}�l�[�W��
#include <AK/Tools/Common/AkPlatformFuncs.h>				// �X���b�h
#include "Wwise_Win32/AkFilePackageLowLevelIOBlocking.h"	// �჌�x��I/O

// �~���[�W�b�N�G���W���ɕK�v
#include <AK/MusicEngine/Common/AkMusicEngine.h>			// �~���[�W�b�N�G���W��

// #include <AK/Plugin/AkCompressorFXFactory.h>				// �v���O�C��
// #include <AK/Plugin/AllPluginsRegistrationHelpers.h>		// 

// Wwise�ƃQ�[���̊Ԃ̒ʐM�ɕK�v - �����[�X�o�[�W�����ł͕s�v
#ifndef AK_OPTIMIZED
#include <AK/Comm/AkCommunication.h>
#endif // AK_OPTIMIZED

#ifdef AK_MOTION
#include <AK/MotionEngine/Common/AkMotionEngine.h>	// ���[�V�������g�p����Ƃ��ɐ錾
#endif

#include <algorithm>

// ======== ======== ======== ======== ======== ======== ======== ========
//	�������t�b�N
// -------- -------- -------- -------- -------- -------- -------- --------
// �J�X�^��alloc/free�֐��B
// ������AkMemoryMgr.h�� "extern"�Ƃ��Đ錾����Ă���
// -------- -------- -------- -------- -------- -------- -------- --------
namespace AK
{
#ifdef WIN32
	void *AllocHook(size_t in_size) { return malloc(in_size); }

	void FreeHook(void *in_ptr) { free(in_ptr); }

	void *VirtualAllocHook(void * in_pMemAddress, size_t in_size, DWORD in_dwAllocationType, DWORD in_dwProtect)
	{
		return VirtualAlloc(in_pMemAddress, in_size, in_dwAllocationType, in_dwProtect);
	}

	void VirtualFreeHook(void *in_pMemAddress, size_t in_size, DWORD in_dwFreeType)
	{
		VirtualFree(in_pMemAddress, in_size, in_dwFreeType);
	}
#endif
}

// ======== ======== ======== ======== ======== ======== ======== ========
// ���O�ȗ�
using namespace std;

// ======== ======== ======== ======== ======== ======== ======== ========
// �}�N��
#define CHECK_SOUND_ENGINE if( !SoundEngine::IsInitialized() ) return;
#define CHECK_SOUND_ENGINE0 if( !SoundEngine::IsInitialized() ) return 0;
#define CHECK_NULLPTR(p) { if(p == nullptr) return; }

// ======== ======== ======== ======== ======== ======== ======== ========
// �ÓI�����o
CAkFilePackageLowLevelIOBlocking Wwise::m_lowLevelIO;
std::list<int> Wwise::dummies;
CAkLock Wwise::dummiesLock;
std::list<WWISE_ENDEVENT_GAMEOBJ *> Wwise::m_pEndEvents;

// ======== ======== ======== ======== ======== ======== ======== ========
// ������
void Wwise::Init(HWND hWnd)
{
	// -------- -------- -------- --------
	// �������}�l�[�W��
	//
	AkMemSettings memSettings;
	memSettings.uMaxNumPools = 20;

	if (MemoryMgr::Init(&memSettings) != AK_Success)
	{
		AKASSERT(!"Could not create the memory manager.");
		return;
	}

	// -------- -------- -------- --------
	// �X�g���[�~���O�}�l�[�W��
	//
	AkStreamMgrSettings stmSettings;
	AK::StreamMgr::GetDefaultSettings(stmSettings);

	if (!StreamMgr::Create(stmSettings))
	{
		AKASSERT(!"Could not create the Stream Manager");
		return;
	}

	// -------- -------- -------- --------
	// �X�g���[�~���O�f�o�C�X
	//
	AkDeviceSettings deviceSettings;
	AK::StreamMgr::GetDefaultDeviceSettings(deviceSettings);

	if (m_lowLevelIO.Init(deviceSettings) != AK_Success)
	{
		AKASSERT(!"Could not create the streaming device and Low-Level I/O system");
		return;
	}

	// -------- -------- -------- --------
	// �T�E���h�G���W��
	//
	AkInitSettings initSettings;
	AK::SoundEngine::GetDefaultInitSettings(initSettings);
	AkPlatformInitSettings platformInitSettings;
	AK::SoundEngine::GetDefaultPlatformInitSettings(platformInitSettings);
	platformInitSettings.hWnd = hWnd;

	// Setting pool sizes for this game. Here, allow for user content; every game should determine its own optimal values.
	initSettings.uDefaultPoolSize = 2 * 1024 * 1024;
	platformInitSettings.uLEngineDefaultPoolSize = 4 * 1024 * 1024;

	if (AK::SoundEngine::Init(&initSettings, &platformInitSettings) != AK_Success)
	{
		assert(!"Could not initialize the Sound Engine.");
		return;
	}

	// -------- -------- -------- --------
	// �~���[�W�b�N�G���W��
	//
	AkMusicSettings musicInit;
	AK::MusicEngine::GetDefaultInitSettings(musicInit);

	if (AK::MusicEngine::Init(&musicInit) != AK_Success)
	{
		assert(!"Could not initialize the Music Engine.");
		return;
	}

	/*
	// -------- -------- -------- --------
	// �v���O�C��
	//
	/// Note: This a convenience method for rapid prototyping. 
	/// To reduce executable code size register/link only the plug-ins required by your game 
	if (AK::SoundEngine::RegisterAllPlugins() != AK_Success)
	{
		AKASSERT(!"Error while registering plug-ins");
		return;
	}
	*/

	// -------- -------- -------- --------
	// �ʐM�̏�����
	//
#ifndef AK_OPTIMIZED
	AkCommSettings commSettings;
	AK::Comm::GetDefaultInitSettings(commSettings);
	if (AK::Comm::Init(commSettings) != AK_Success)
	{
		assert(!"Could not initialize communication.");
		return;
	}
#endif // AK_OPTIMIZED

	// -------- -------- -------- --------
	// �჌�x��I/O
	//
	// �o���N�X�N���v�g�̃p�X
	m_lowLevelIO.SetBasePath(AKTEXT("data/wwisePrj_InitialT/GeneratedSoundBanks/Windows/"));

	// �Q�[���̌��݂̌���̎w��
	AK::StreamMgr::SetCurrentLanguage(AKTEXT("English(US)"));

	// -------- -------- -------- --------
	// �o���N�̓ǂݍ���
	//
	AkBankID bankID;
	AKRESULT retValue;

	// �������o���N
	retValue = SoundEngine::LoadBank("Init.bnk", AK_DEFAULT_POOL_ID, bankID);
	assert(retValue == AK_Success);

	// ���C���o���N
	retValue = SoundEngine::LoadBank("main.bnk", AK_DEFAULT_POOL_ID, bankID);
	assert(retValue == AK_Success);

	/*
	// Wwise���[�V����
#ifdef AK_MOTION
	MotionEngine::AddPlayerMotionDevice(0, AKCOMPANYID_AUDIOKINETIC, AKMOTIONDEVICEID_RUMBLE, nullptr);
	MotionEngine::SetPlayerListener(0, 0);
	SoundEngine::SetListenerPipeline(0, true, true);
#endif
	*/

	// ���C�����X�i�[�̐ݒ�
	m_pMainListener = Wwise::CreateMainListenerGameObj(D3DXVECTOR3(0.f, 0.f, 0.f), D3DXVECTOR3(0.f, 0.f, 0.f));	// �Q�[���I�u�W�F�N�g�Ƃ��ēo�^
}

// ======== ======== ======== ======== ======== ======== ======== ========
// �I��
void Wwise::Uninit()
{
	// �Q�[���I�u�W�F�N�g�̏���
	for (auto it : m_umapGameObj)
	{
		if (it.first != nullptr)
		{
			Stop(it.first);
			unregistPtr(it.first);	// �Q�[���I�u�W�F�N�g�̓o�^������
			delete it.second;
			it.second = nullptr;
		}
	}
	m_umapGameObj.clear();

	// �ʐM
#ifndef AK_OPTIMIZED
	Comm::Term();
#endif // AK_OPTIMIZED

	// �~���[�W�b�N�G���W��
	MusicEngine::Term();

	// �T�E���h�G���W��
	SoundEngine::Term();

	// �჌�x�����o��
	m_lowLevelIO.Term();
	if (IAkStreamMgr::Get())
		IAkStreamMgr::Get()->Destroy();

	// �������}�l�[�W��
	MemoryMgr::Term();
}

// ======== ======== ======== ======== ======== ======== ======== ========
// �X�V
void Wwise::Update()
{
	CHECK_SOUND_ENGINE;

	AkSoundPosition MainListener;
	AkSoundPosition soundPos;
	float fRadPitch, fCosPitch, fSinPitch;
	float fRadYaw, fCosYaw, fSinYaw;
	D3DXVECTOR3 front, top;
	AkVector position;
	// ======== ======== ======== ========
	// �e�Q�[���I�u�W�F�N�g�̐ݒ�
	for (auto gameObject : m_umapGameObj)
	{
		if (gameObject.second == m_pMainListener)
		{
			// ======== ======== ======== ========
			// ���X�i�[�̐ݒ�
			fRadPitch = m_pMainListener->rot.z;
			fCosPitch = cos(fRadPitch);
			fSinPitch = sin(fRadPitch);

			fRadYaw = m_pMainListener->rot.y - D3DX_PI * 0.5f;
			fCosYaw = cos(fRadYaw);
			fSinYaw = sin(fRadYaw);

			// �O�����x�N�g��
			front.x = fSinYaw * fCosPitch;
			front.y = fSinPitch;
			front.z = fCosYaw * fCosPitch;
			D3DXVec3Normalize(&front, &front);

			// ������x�N�g��
			top.x = -fCosYaw * fSinPitch;
			top.y = fCosPitch;
			top.z = -fSinYaw * fSinPitch;
			D3DXVec3Normalize(&top, &top);

			// ���W�ݒ�
			VecToAkVector(m_pMainListener->pos, position);

			// ���X�i�[�̐ݒ�
			MainListener.Set(position, VecToAkVector(front), VecToAkVector(top));
			SoundEngine::SetPosition((AkGameObjectID)gameObject.first, MainListener);

			continue;
		}
		fRadPitch = gameObject.second->rot.z;
		fCosPitch = cos(fRadPitch);
		fRadYaw = gameObject.second->rot.y - D3DX_PI * 0.5f;

		// �O�����x�N�g��
		front.x = (float)(sin(fRadYaw) * fCosPitch);
		front.y = (float)(sin(fRadPitch));
		front.z = (float)(cos(fRadYaw) * fCosPitch);
		D3DXVec3Normalize(&front, &front);

		// ������x�N�g��
		top = { 0.f, 1.f, 0.f };

		// ���W�ݒ�
		VecToAkVector(gameObject.second->pos, position);

		// �Q�[���I�u�W�F�N�g�̐ݒ�
		soundPos.Set(position, VecToAkVector(front), VecToAkVector(top));
		SoundEngine::SetPosition((AkGameObjectID)gameObject.first, soundPos);
	}

	// �ݒ肳�ꂽ�C�x���g�̎��s
	SoundEngine::RenderAudio();
};

// ======== ======== ======== ======== ======== ======== ======== ========
// ���C�����X�i�[�̃Q�[���I�u�W�F�N�g��o�^
WWISE_GAMEOBJ *Wwise::CreateMainListenerGameObj(D3DXVECTOR3 pos, D3DXVECTOR3 rot, char *gameObjName)
{
	WWISE_GAMEOBJ *pGameObject = new WWISE_GAMEOBJ;
	pGameObject->pos = pos;							// ���W
	pGameObject->rot = rot;							// �p�x
	pGameObject->vel = D3DXVECTOR3(0.f, 0.f, 0.f);	// ���x(�h�b�v���[�p)

	// ���C�����X�i�[��ID�擾
	m_mainListenerID = (AkGameObjectID)pGameObject;
	
	// �G���W���֓o�^
	Wwise::registPtr(pGameObject, gameObjName);

	// ���X�g�֒ǉ�
	if (m_umapGameObj.count(pGameObject) >= 1)
	{
		Stop(pGameObject);
		Wwise::unregistPtr(pGameObject);
		delete m_umapGameObj[pGameObject];
		m_umapGameObj[pGameObject] = nullptr;
	}
	m_umapGameObj[pGameObject] = pGameObject;
	AK::SoundEngine::SetDefaultListeners(&m_mainListenerID, 1);

	return pGameObject;
}

// ======== ======== ======== ======== ======== ======== ======== ========
// ���X�i�[�����Q�[���I�u�W�F�N�g��o�^
WWISE_GAMEOBJ *Wwise::CreateNoListenerGameObj(D3DXVECTOR3 pos, D3DXVECTOR3 rot, void *thisPtr, char *gameObjName)
{
	WWISE_GAMEOBJ *pGameObject = new WWISE_GAMEOBJ;
	pGameObject->pos = pos;							// ���W
	pGameObject->rot = rot;							// �p�x
	pGameObject->vel = D3DXVECTOR3(0.f, 0.f, 0.f);	// ���x(�h�b�v���[�p)

	// �G���W���֓o�^
	Wwise::registPtr(thisPtr, gameObjName);

	// ���X�g�֒ǉ�
	if (m_umapGameObj.count(thisPtr) >= 1)
	{
		Stop(thisPtr);
		Wwise::unregistPtr(thisPtr);
		delete m_umapGameObj[thisPtr];
		m_umapGameObj[thisPtr] = nullptr;
	}
	m_umapGameObj[thisPtr] = pGameObject;

	return pGameObject;
}

// ======== ======== ======== ======== ======== ======== ======== ========
// �Q�[���I�u�W�F�N�g�̓o�^(�G���W���ɓo�^)
void Wwise::registPtr(void *thisPtr, char *pName)
{
	CHECK_SOUND_ENGINE;

	if (pName == nullptr)
	{
		static int cEntity = 0;
		char name[256];
		sprintf_s(name, "EntityNoListenerObj #%i", (int) ++cEntity);

		SoundEngine::RegisterGameObj((AkGameObjectID)thisPtr, name);
	}
	else
	{
		SoundEngine::RegisterGameObj((AkGameObjectID)thisPtr, pName);
	}
}

// ======== ======== ======== ======== ======== ======== ======== ========
// �Q�[���I�u�W�F�N�g�̓o�^(�G���W���ɓo�^)
void Wwise::registPtr(const AkGameObjectID &ID, char *pName)
{
	CHECK_SOUND_ENGINE;

	if (pName == nullptr)
	{
		static int cEntity = 0;
		char name[256];
		sprintf_s(name, "EntityObj #%i", (int) ++cEntity);

		SoundEngine::RegisterGameObj(ID, name);
	}
	else
	{
		SoundEngine::RegisterGameObj(ID, pName);
	}
}

// ======== ======== ======== ======== ======== ======== ======== ========
// �Q�[���I�u�W�F�N�g�̏���
void Wwise::DeleteGameObj(void *thisPtr)
{
	CHECK_SOUND_ENGINE;
	if (m_umapGameObj.count(thisPtr) == 0) { return; }

	// �w��Q�[���I�u�W�F�N�g�̏���
	Wwise::unregistPtr(thisPtr);
	delete m_umapGameObj[thisPtr];
	m_umapGameObj[thisPtr] = nullptr;
	m_umapGameObj.erase(thisPtr);
}

// ======== ======== ======== ======== ======== ======== ======== ========
// �Q�[���I�u�W�F�N�g�̏���(�G���W������폜)
void Wwise::unregistPtr(void *thisPtr)
{
	CHECK_SOUND_ENGINE;

	SoundEngine::UnregisterGameObj((AkGameObjectID)thisPtr);
}

// ======== ======== ======== ======== ======== ======== ======== ========
// 
// �C�x���g�̐ݒ�
// 
// -------- -------- -------- -------- -------- -------- -------- --------
// ���C�����X�i�[�ɃC�x���g�𑗐M
void Wwise::SetMainListenerEvent(int eventID)
{
	CHECK_SOUND_ENGINE;

	SoundEngine::PostEvent(
		eventID,
		(AkGameObjectID)m_pMainListener,
		AK_EndOfEvent | AK_MusicSyncBeat,	// �C�x���g���I�������Ƃ��ɃR�[���o�b�N�֐����Ăяo��
		EndEventCallback					// �R�[���o�b�N�֐�
	);
}


// ======== ======== ======== ======== ======== ======== ======== ========
// ID, �Q�[���I�u�W�F�N�g��
void Wwise::SetEvent(int eventID, void *thisPtr)
{
	CHECK_SOUND_ENGINE;
	if (m_umapGameObj.count(thisPtr) == 0) { return; }

	SoundEngine::PostEvent(
		eventID, 
		(AkGameObjectID)thisPtr,
		AK_EndOfEvent | AK_MusicSyncBeat,	// �C�x���g���I�������Ƃ��ɃR�[���o�b�N�֐����Ăяo��
		EndEventCallback					// �R�[���o�b�N�֐�
		);
}

// ======== ======== ======== ======== ======== ======== ======== ========
// �Q�[���I�u�W�F�N�g�̃C�x���g�̏I�����m�F
bool Wwise::CheckEndEvent(int eventID, void *thisPtr)
{
	// ���������ȗ�����Ă����ꍇ�A���C�����X�i�[�Ɍ����������Ƒ�����
	if (thisPtr == nullptr)
		thisPtr = m_pMainListener;

	bool eventEndFlag = false;

	// �I���C�x���g���X�g���m�F
	for (auto endEvent : m_pEndEvents)
	{
		if (endEvent->eventID == (AkUniqueID)eventID && endEvent->gameObjID == (AkGameObjectID)thisPtr)
		{
			// �C�x���g�I���t���O�𗧂Ă�
			eventEndFlag = true;
			break;
		}
	}
	
	// �C�x���g�I���t���O���������ꍇ�A�I���C�x���g���X�g���̊m�F�̊��������v�f�Ɠ����v�f��S����
	if (eventEndFlag)
	{
		m_pEndEvents.erase(remove_if(m_pEndEvents.begin(), m_pEndEvents.end(), [&, thisPtr](auto endEvent)
		{
			if (endEvent->eventID == (AkUniqueID)eventID && endEvent->gameObjID == (AkGameObjectID)thisPtr)
			{
				// �I���C�x���g�����X�g�������
				delete endEvent;
				endEvent = nullptr;
				return true;
			}
			else
			{
				return false;
			}
		}), m_pEndEvents.end());
	}

	return eventEndFlag;
}

// ======== ======== ======== ======== ======== ======== ======== ========
// �Đ����C�x���g�̒�~
void Wwise::Stop(void *thisPtr)
{
	CHECK_SOUND_ENGINE;
	if (m_umapGameObj.count(thisPtr) == 0) { assert(!"�����̖��O���s��"); }

	SoundEngine::StopAll((AkGameObjectID)thisPtr);
}

// ======== ======== ======== ======== ======== ======== ======== ========
// ���C�����X�i�[�̍Đ����C�x���g�̒�~
void Wwise::StopMainListener()
{
	CHECK_SOUND_ENGINE;
	if (m_pMainListener == nullptr) { return; }

	SoundEngine::StopAll((AkGameObjectID)m_pMainListener);
}

// ======== ======== ======== ======== ======== ======== ======== ========
// �X�e�[�g�̐ݒ�
void Wwise::SetState(int stateGroup, int stateID)
{
	CHECK_SOUND_ENGINE;

	SoundEngine::SetState(stateGroup, stateID);
}

// ======== ======== ======== ======== ======== ======== ======== ========
// RTPC�̐ݒ�
void Wwise::SetGameParam(int rtpcID, float value)
{
	CHECK_SOUND_ENGINE;

	SoundEngine::SetRTPCValue(rtpcID, value);
}

// ======== ======== ======== ======== ======== ======== ======== ========
// wwise�L���v�`���[���X�g�փG���[���b�Z�[�W�𑗂�
void Wwise::PostMessage(char *szMessage)
{
	Monitor::PostString(szMessage, Monitor::ErrorLevel_Message);
}

// ======== ======== ======== ======== ======== ======== ======== ========
// �_�~�[�R�[���o�b�N
void Wwise::dummyCallback(AkCallbackType type, AkCallbackInfo *pCallbackInfo)
{
	UNREFERENCED_PARAMETER(type);
	AkEventCallbackInfo *pInfo = (AkEventCallbackInfo *)pCallbackInfo;
	AkGameObjectID id = pInfo->gameObjID;

	dummiesLock.Lock();		// ���b�N

	// ID�̗L�������m���߂�
	auto it = std::find(dummies.begin(), dummies.end(), static_cast<int>(pInfo->playingID));
	if (it != dummies.end())
	{
		dummies.erase(it);
		AK::SoundEngine::UnregisterGameObj(id);
	}

	dummiesLock.Unlock();	// �A�����b�N
}

// ======== ======== ======== ======== ======== ======== ======== ========
// �C�x���g�I����m�点��R�[���o�b�N
void Wwise::EndEventCallback(AkCallbackType type, AkCallbackInfo* pCallbackInfo)
{
	UNREFERENCED_PARAMETER(type);

	// �R�[���o�b�N���Ă񂾑Ώۂ��擾
	WWISE_ENDEVENT_GAMEOBJ *setEvent = new WWISE_ENDEVENT_GAMEOBJ;
	AkEventCallbackInfo *pInfo = (AkEventCallbackInfo *)pCallbackInfo;
	setEvent->gameObjID = pInfo->gameObjID;
	setEvent->eventID = pInfo->eventID;

	// �o�^�O�Ɋ����̓���C�x���g���폜
	m_pEndEvents.erase(remove_if(m_pEndEvents.begin(), m_pEndEvents.end(), [&setEvent](auto endEvent)
	{
		if (endEvent->eventID == setEvent->eventID && endEvent->gameObjID == setEvent->gameObjID)
		{
			// �I���C�x���g�����X�g�������
			delete endEvent;
			endEvent = nullptr;
			return true;
		}
		else
			return false;
	}), m_pEndEvents.end());

	// �I���C�x���g���X�g�ɉ�����
	m_pEndEvents.push_back(setEvent);
}

// ======== ======== ======== ======== ======== ======== ======== ========
// ���O����}�e���A��ID�̎擾
int Wwise::GetMaterialId(char *pName)
{
	CHECK_SOUND_ENGINE0;

	return SoundEngine::GetIDFromString(pName);
}

/*
// ======== ======== ======== ======== ======== ======== ======== ========
// �Q�[���I�u�W�F�N�g�Ƀ}�e���A��ID��ݒ�
void Wwise::SetMaterial(void *gameObject, int materialid)
{
CHECK_SOUND_ENGINE;

SoundEngine::SetSwitch(SWITCHES::MATERIAL::GROUP, materialid, (AkGameObjectID)gameObject);
}
*/

// ======== ======== ======== ======== ======== ======== ======== ========
// DirectX��Vec3��Wwise��Vec3�ɕϊ�
void Wwise::VecToAkVector(const D3DXVECTOR3 &vec, AkVector &vector)
{
	vector.X = vec.x;
	vector.Y = vec.y;
	vector.Z = vec.z;
}

// ======== ======== ======== ======== ======== ======== ======== ========
// DirectX��Vec3��Wwise��Vec3�ɕϊ�
AkVector Wwise::VecToAkVector(const D3DXVECTOR3 &vec)
{
	AkVector vector;
	vector.X = vec.x;
	vector.Y = vec.y;
	vector.Z = vec.z;
	return vector;
}
