//================================================================================
//
// Auter : KENSUKE WATANABE
// Data  : 2018/02/01
//
//================================================================================
#include <algorithm>
#include "manager.h"
#include "camera.h"
#include "xa2Listener.h"
#include "xa2Manager.h"
#include "xa2Core.h"
#include "xa2MasteringVoice.h"
#include "xa2SourceVoiceManager.h"
#include "xa2SourceVoice3D.h"
#include "xa2SoundResourceManager.h"

#include "scene.h"
#include "sceneXModelListener.h"

static const X3DAUDIO_DISTANCE_CURVE_POINT Emitter_LFE_CurvePoints[3] = { 0.0f, 1.0f, 0.25f, 0.0f, 1.0f, 0.0f };
static const X3DAUDIO_DISTANCE_CURVE       Emitter_LFE_Curve = { (X3DAUDIO_DISTANCE_CURVE_POINT*)&Emitter_LFE_CurvePoints[0], 3 };

static const X3DAUDIO_DISTANCE_CURVE_POINT Emitter_Reverb_CurvePoints[3] = { 0.0f, 0.5f, 0.75f, 1.0f, 1.0f, 0.0f };
static const X3DAUDIO_DISTANCE_CURVE       Emitter_Reverb_Curve = { (X3DAUDIO_DISTANCE_CURVE_POINT*)&Emitter_Reverb_CurvePoints[0], 3 };

//--------------------------------------------------------------------------------
// ��������
//--------------------------------------------------------------------------------
IXAudio2SourceVoice *XA2SourceVoice3D::Create(std::string strFilePath, int loopCount, XA2LoadAudio *pLoadWave)
{
	// �~���[�e�b�N�X
#ifdef THREAD_ON
	std::unique_lock<std::recursive_mutex> locker = CManager::Locker();
#endif

	// �I�[�f�B�I�f�[�^�̐���
	XA2SourceVoiceData3D *pSourceVoiceData = new XA2SourceVoiceData3D;

	XA2Core *pXA2Core = XA2Manager::GetXA2Core();				// XAudio2�G���W��
	XA2SoundResourceManager *pXA2SoundResourceManager = XA2Manager::GetSoundResourceManager();	// �T�E���h���\�[�X�}�l�[�W��

	// ���\�[�X�̎擾
	pLoadWave->Create(strFilePath, loopCount);	// wave�`���Ŏ擾
	XA2LoadAudio *pSoundResource = pXA2SoundResourceManager->GetXA2SoundResource(strFilePath);

	// �}�X�^�[�{�C�X
	IXAudio2MasteringVoice *pMaster = XA2Manager::GetMasteringVoice()->GetMasteringVoice();

	// �f�X�e�l�[�V�����{�C�X�̐ݒ�
	XAUDIO2_SEND_DESCRIPTOR sendDescriptors[2];
	sendDescriptors[0].Flags = XAUDIO2_SEND_USEFILTER; // LPF direct-path
	sendDescriptors[0].pOutputVoice = m_pSubmixVoice;
	sendDescriptors[1].Flags = XAUDIO2_SEND_USEFILTER; // LPF direct-path
	sendDescriptors[1].pOutputVoice = pMaster;
	const XAUDIO2_VOICE_SENDS sendList = { 2, sendDescriptors };

	// �\�[�X�{�C�X�̐���
	IXAudio2SourceVoice *pSourceVoice = pSourceVoiceData->GetSourceVoice();
	HRESULT hr = pXA2Core->GetXAudio2()->CreateSourceVoice(
		&pSourceVoice,							// �\�[�X�{�C�X
		&(pSoundResource->GetWfx()->Format),	// ���\�[�X�̃t�H�[�}�b�g
		0,										// �\�[�X�{�C�X�̓�����w�肷��t���O
		XAUDIO2_DEFAULT_FREQ_RATIO,				// �ő勖�e���g����
		nullptr,								// �R�[���o�b�N �C���^�[�t�F�C�X�ւ̃|�C���^
		&sendList,								// �\�[�X�{�C�X�ɑ΂����A�̃f�X�e�B�l�[�V�����{�C�X
		nullptr);								// �\�[�X�{�C�X�Ŏg�p����G�t�F�N�g�`�F�[��
	if (FAILED(hr))
	{
		MessageBox(nullptr, "�\�[�X�{�C�X�̐����Ɏ��s�I", "�x��", MB_ICONWARNING);
		return nullptr;
	}

	// �\�[�X�{�C�X�̐ݒ�
	pSourceVoiceData->SetSourceVoice(pSourceVoice);

	// �I�[�f�B�I�f�[�^�̃|�C���^�̐ݒ�
	pSourceVoiceData->SetWaveData(pSoundResource);

	// �t�@�C����
	pSourceVoiceData->SetFilePass(strFilePath);			

	// �G�~�b�^�[�̐ݒ�
	m_emitter.Position = XMFLOAT3(0.f, 0.f, 0.f);	// ���W
	m_emitter.OrientFront = XMFLOAT3(0.f, 0.f, 1.f);	// �O����
	m_emitter.OrientTop = XMFLOAT3(0.f, 1.f, 0.f);	// �����
	m_emitter.Velocity = XMFLOAT3(0.f, 0.f, 0.f);	// �ړ���

	// �R�[��
	m_emitter.pCone = new X3DAUDIO_CONE;				
	SetCone(PI * 0.75f, 1.1f, 0.8f);				// �R�[���ݒ�

	// �����̎��̗͂̈�
	SetInnerRadius(10.f);								// �������a
	SetInnerRadiusAngle(X3DAUDIO_PI / 4.f);				// �����p�x

	// �}���`�`�����l��
	m_emitter.ChannelCount = INPUTCHANNELS;				// �`�����l����
	m_emitter.ChannelRadius = 1.0f;						// �}���`�`�����l�����̔��a
	m_emitter.pChannelAzimuths = m_azimuths;			// ���W�A���P�ʂ̕��ʊp�ŕ\�����`�����l���ʒu�̃e�[�u��
//	*m_emitter.pChannelAzimuths = atan2(m_emitter.OrientFront.z, m_emitter.OrientFront.x) - PI * 0.5f;

	m_emitter.pVolumeCurve = (X3DAUDIO_DISTANCE_CURVE*)&X3DAudioDefault_LinearCurve;	// �{�����[���J�[�u
	m_emitter.pLFECurve = (X3DAUDIO_DISTANCE_CURVE*)&Emitter_LFE_Curve;					// LFE(�ቹ����)�J�[�u
	m_emitter.pLPFDirectCurve = NULL;													// ���[�p�X�t�B���^�̃_�C���N�g�p�X�W�������J�[�u
	m_emitter.pLPFReverbCurve = NULL;													// ���[�p�X�t�B���^�̃��o�[�u�p�X�W�������J�[�u
	m_emitter.pReverbCurve = (X3DAUDIO_DISTANCE_CURVE*)&Emitter_Reverb_Curve;			// ���o�[�u�Z���h���x�������J�[�u
	m_emitter.CurveDistanceScaler = 30.f;												// ���[�U�[��`�̃��[���h�P�ʂɂ���X�P�[���[
	m_emitter.DopplerScaler = 1.f;														// �h�b�v���[�ΈڃX�P�[���[

	// �v�Z���ʂ̊i�[�p
	m_dspSettings.SrcChannelCount = m_emitter.ChannelCount;
	m_dspSettings.DstChannelCount = pXA2Core->GetDetailsChannels();
	m_dspSettings.pMatrixCoefficients = m_matrixCoefficients;

	// ���X�g�ɓo�^
	AddSourceVoiceData(pSourceVoiceData);				// �o�^

	return pSourceVoice;
}

//--------------------------------------------------------------------------------
// �������čĐ�
//--------------------------------------------------------------------------------
IXAudio2SourceVoice *XA2SourceVoice3D::CreatePlay(std::string strFilePath, int loopCount, XA2LoadAudio *pLoadWave)
{
	// ����
	IXAudio2SourceVoice *pSourceVoice = Create(strFilePath, loopCount, pLoadWave);

	// ���\�[�X�̎擾
	XA2SoundResourceManager *pXA2SoundResourceManager = XA2Manager::GetSoundResourceManager();		// �T�E���h���\�[�X�}�l�[�W��
	XA2LoadAudio *pSoundResource = pXA2SoundResourceManager->GetXA2SoundResource(strFilePath);
	
	// �o�b�t�@�̒l�ݒ�
	XAUDIO2_BUFFER xa2buffer;
	memset(&xa2buffer, 0, sizeof(XAUDIO2_BUFFER));
	XA2LoadWaveOnAll *pLoadWaveOnAll = nullptr;
	if (pSoundResource->GetStreamingFlag() == false)
	{
		pLoadWaveOnAll = (XA2LoadWaveOnAll*)pSoundResource;
		xa2buffer.AudioBytes = pLoadWaveOnAll->GetAudioSize();
		xa2buffer.pAudioData = &pLoadWaveOnAll->GetAudioData()[0][0];
		xa2buffer.Flags = XAUDIO2_END_OF_STREAM;
		xa2buffer.LoopCount = pLoadWaveOnAll->GetLoopCount();

		// �I�[�f�B�I�o�b�t�@�̓o�^
		pSourceVoice->SubmitSourceBuffer(&xa2buffer);

		// �Đ�
		pSourceVoice->Start(0);
	}

	return pSourceVoice;
}

//--------------------------------------------------------------------------------
// �I��
//--------------------------------------------------------------------------------
void XA2SourceVoice3D::Uninit()
{
	// �G�~�b�^�[�̉��
	//--------------------------------------------------------------------------------
	if (m_emitter.pChannelAzimuths)
	{
		delete m_emitter.pChannelAzimuths;
		m_emitter.pChannelAzimuths = nullptr;
	}
	if (m_emitter.pCone)
	{
		delete m_emitter.pCone;
		m_emitter.pCone = nullptr;
	}
	if (m_emitter.pLFECurve)
	{
		delete m_emitter.pLFECurve;
		m_emitter.pLFECurve = nullptr;
	}
	if (m_emitter.pLPFDirectCurve)
	{
		delete m_emitter.pLPFDirectCurve;
		m_emitter.pLPFDirectCurve = nullptr;
	}
	if (m_emitter.pLPFReverbCurve)
	{
		delete m_emitter.pLPFReverbCurve;
		m_emitter.pLPFReverbCurve = nullptr;
	}
	if (m_emitter.pReverbCurve)
	{
		delete m_emitter.pReverbCurve;
		m_emitter.pReverbCurve = nullptr;
	}
	if (m_emitter.pVolumeCurve)
	{
		delete m_emitter.pVolumeCurve;
		m_emitter.pVolumeCurve = nullptr;
	}
}

//--------------------------------------------------------------------------------
// �X�V
//--------------------------------------------------------------------------------
void XA2SourceVoice3D::Update()
{
	XA2Core *pXA2Core = XA2Manager::GetXA2Core();	// XAudio2�G���W��

	// �I������SE�̉��(�K�x�[�W�R���N�V����)
	//--------------------------------------------------------------------------------
	XA2SourceVoiceInterface::ReleaseEndSE();

	// 3D���Z
	//--------------------------------------------------------------------------------
	X3DAUDIO_LISTENER *pListener = nullptr;
	for (int n = 0; n < MAX_SCENE_PRIORITY; n++)
	{
		std::list<CScene*> *lisScene = CScene::GetScene(n);
		for (CScene* pScene : *lisScene)
		{
			if (pScene == nullptr)continue;
			// �^�C�v�̎擾
			CScene::OBJTYPE objType = pScene->GetObjType();

			if (objType == CScene::OBJTYPE::LISTENER)
			{
				// �_�E���L���X�g
				CSceneXModelListener *pSceneListener = (CSceneXModelListener*)pScene;
				pListener = pSceneListener->GetListener()->GetListener();	// ���X�i�[
				break;
			}
		}
	}

	if (pListener == nullptr)return;

	// �v�Z�����t���O
	DWORD dwCalcFlags = X3DAUDIO_CALCULATE_MATRIX | X3DAUDIO_CALCULATE_DOPPLER
		| X3DAUDIO_CALCULATE_LPF_DIRECT | X3DAUDIO_CALCULATE_LPF_REVERB
		| X3DAUDIO_CALCULATE_REVERB;

//	pListener->Position = XMFLOAT3(0.f, 0.f, 0.f);
//	m_emitter.Position = XMFLOAT3(-20.f, 0.f, 0.f);

	// �v�Z����
	X3DAudioCalculate(
		XA2Manager::GetX3DInstance(),	// �n���h��
		pListener,						// ���X�i�[
		&m_emitter,						// �G�~�b�^�[
		dwCalcFlags,					// �v�Z�������e
		&m_dspSettings);				// �v�Z����

	// �}�X�^�[�{�C�X
	IXAudio2MasteringVoice *pMasteringVoice = XA2Manager::GetMasteringVoice()->GetMasteringVoice();

	// �f�o�C�X�̓��̓`�����l����
	UINT32 channels = pXA2Core->GetDetailsChannels();

	// �e�\�[�X�{�C�X�ɉ��Z���ʂ�K�p
	IXAudio2SourceVoice *pSourceVoice = nullptr;
	for (auto pData : m_sourceVoices)
	{
		// �\�[�X�{�C�X�̃{�����[������уs�b�`�̒l��K�p
		pSourceVoice = pData->GetSourceVoice();
		pSourceVoice->SetOutputMatrix(pMasteringVoice, INPUTCHANNELS, channels, m_matrixCoefficients);
		pSourceVoice->SetFrequencyRatio(m_dspSettings.DopplerFactor);

		// ���o�[�u���x�����T�u�~�b�N�X�{�C�X�ɓK�p
		pSourceVoice->SetOutputMatrix(m_pSubmixVoice, 1, 1, &m_dspSettings.ReverbLevel);

		// ���[�p�X�t�B���^�[�_�C���N�g�W�����\�[�X�{�C�X�ɓK�p
		float mulValue = 6.0f * m_dspSettings.LPFDirectCoefficient;
		if (mulValue != 0.f)
		{
			XAUDIO2_FILTER_PARAMETERS FilterParameters = { LowPassFilter, 2.0f * sinf(X3DAUDIO_PI / mulValue), 1.0f };
			pSourceVoice->SetFilterParameters(&FilterParameters);
		}
	}
}

//--------------------------------------------------------------------------------
// �|�[�����O���� - �ʃX���b�h����
//--------------------------------------------------------------------------------
void XA2SourceVoice3D::Polling()
{
	if (m_sourceVoices.size() == 0 || m_sourceVoices.size() > MAX_SAUCEVOICE)return;

	// �R���e�i���󂩔���
	if (m_sourceVoices.empty())
	{
		return;
	}

	// ���b�N
	std::unique_lock<std::recursive_mutex> locker = XA2Manager::Locker();

	// �|�[�����O����
	XA2LoadAudio *pWaveData = nullptr;
	for (auto it : m_sourceVoices)
	{
		if (it == nullptr)continue;
		// �X�g���[�~���O�Đ��̃\�[�X�{�C�X�̂ݎ��s
		pWaveData = it->GetWaveData();
		if (pWaveData->GetStreamingFlag())
		{
			XA2LoadWaveStreaming* pLoadWaveStreaming = (XA2LoadWaveStreaming*)pWaveData;
			pLoadWaveStreaming->Polling(it->GetSourceVoice());
		}
	}
}

// �R�[��
//--------------------------------------------------------------------------------
void XA2SourceVoice3D::SetCone(float innerAngle, float innerVolume, float outerVolume, float lpf, float reverb)
{
	m_emitter.pCone->InnerAngle = min(innerAngle, X3DAUDIO_2PI);				// �R�[���̊p�x (���W�A���P��)
	m_emitter.pCone->OuterAngle = X3DAUDIO_2PI - m_emitter.pCone->InnerAngle;		// 
	m_emitter.pCone->InnerVolume = min(innerVolume, 2.0f);					// �{�����[�� �X�P�[��
	m_emitter.pCone->OuterVolume = min(outerVolume, 2.0f);					// 
	m_emitter.pCone->InnerLPF = lpf;												// �_�C���N�g �p�X�܂��̓��o�[�u �p�X�̌W���X�P�[��
	m_emitter.pCone->OuterLPF = lpf;												// 
	m_emitter.pCone->InnerReverb = reverb;											// ���o�[�u �Z���h ���x�� �X�P�[��
	m_emitter.pCone->OuterReverb = reverb;											// 
}

// �R�[���̊p�x (���W�A���P��)
//--------------------------------------------------------------------------------
void XA2SourceVoice3D::SetConeAngle(float innerAngle)
{
	m_emitter.pCone->InnerAngle = min(innerAngle, X3DAUDIO_2PI);
	m_emitter.pCone->OuterAngle = X3DAUDIO_2PI - m_emitter.pCone->InnerAngle;
}

// �{�����[�� �X�P�[��
//--------------------------------------------------------------------------------
void XA2SourceVoice3D::SetConeVolume(float innerVolume, float outerVolume)
{
	m_emitter.pCone->InnerVolume = min(innerVolume, 2.0f);
	m_emitter.pCone->OuterVolume = min(outerVolume, 2.0f);
}

// �_�C���N�g �p�X�܂��̓��o�[�u �p�X�̌W���X�P�[��
//--------------------------------------------------------------------------------
void XA2SourceVoice3D::SetConeLPF(float innerLPF, float outerLPF)
{
	m_emitter.pCone->InnerLPF = min(innerLPF, 1.0f);
	m_emitter.pCone->OuterLPF = min(outerLPF, 1.0f);
}

// ���o�[�u �Z���h ���x�� �X�P�[��
//--------------------------------------------------------------------------------
void XA2SourceVoice3D::SetConeReverb(float innerReverb, float outerReverb)
{
	m_emitter.pCone->InnerReverb = min(innerReverb, 2.0f);
	m_emitter.pCone->OuterReverb = min(outerReverb, 2.0f);
}
