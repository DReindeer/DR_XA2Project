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

static const X3DAUDIO_DISTANCE_CURVE_POINT Emitter_LFE_CurvePoints[3] = { 0.0f, 1.0f, 0.25f, 0.0f, 1.0f, 0.0f };
static const X3DAUDIO_DISTANCE_CURVE       Emitter_LFE_Curve = { (X3DAUDIO_DISTANCE_CURVE_POINT*)&Emitter_LFE_CurvePoints[0], 3 };

static const X3DAUDIO_DISTANCE_CURVE_POINT Emitter_Reverb_CurvePoints[3] = { 0.0f, 0.5f, 0.75f, 1.0f, 1.0f, 0.0f };
static const X3DAUDIO_DISTANCE_CURVE       Emitter_Reverb_Curve = { (X3DAUDIO_DISTANCE_CURVE_POINT*)&Emitter_Reverb_CurvePoints[0], 3 };

//--------------------------------------------------------------------------------
// ��������
//--------------------------------------------------------------------------------
IXAudio2SourceVoice *XA2SourceVoice3D::Create(std::string strFilePath, int loopCount, XA2LoadWave *pLoadWave)
{
	// �~���[�e�b�N�X
#ifdef THREAD_ON
	std::unique_lock<std::recursive_mutex> locker = CManager::Locker();
#endif

	// �I�[�f�B�I�f�[�^�̐���
	XA2SourceVoiceData3D *pSourceVoiceData = new XA2SourceVoiceData3D;

	XA2Manager *pXA2Manager = CManager::GetXA2Manager();		// Manager
	XA2Core *pXA2Core = pXA2Manager->GetXA2Core();				// XAudio2�G���W��
	XA2SoundResourceManager *pXA2SoundResourceManager = pXA2Manager->GetSoundResourceManager();	// �T�E���h���\�[�X�}�l�[�W��

	// ���\�[�X�̎擾
	pLoadWave->Create(strFilePath, loopCount);	// wave�`���Ŏ擾
	XA2LoadWave *pSoundResource = pXA2SoundResourceManager->GetXA2SoundResource(strFilePath);

	// �\�[�X�{�C�X�̐���
	IXAudio2SourceVoice *pSourceVoice = pSourceVoiceData->GetSourceVoice();
	HRESULT hr = pXA2Core->GetXAudio2()->CreateSourceVoice(
		&pSourceVoice,							// �\�[�X�{�C�X
		&(pSoundResource->GetWfx()->Format),	// ���\�[�X�̃t�H�[�}�b�g
		0,										// �\�[�X�{�C�X�̓�����w�肷��t���O
		XAUDIO2_DEFAULT_FREQ_RATIO,				// �ő勖�e���g����
		nullptr,								// �R�[���o�b�N �C���^�[�t�F�C�X�ւ̃|�C���^
		nullptr,								// �\�[�X�{�C�X�ɑ΂����A�̃f�X�e�B�l�[�V�����{�C�X
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
	m_emitter.Position = D3DXVECTOR3(0.f, 0.f, 0.f);	// ���W
	m_emitter.OrientFront = D3DXVECTOR3(0.f, 0.f, 1.f);	// �O����
	m_emitter.OrientTop = D3DXVECTOR3(0.f, 1.f, 0.f);	// �����
	m_emitter.Velocity = D3DXVECTOR3(0.f, 0.f, 0.f);	// �ړ���
	// �R�[��
	m_emitter.pCone = new X3DAUDIO_CONE;				
	SetCone(D3DX_PI * 0.75f, 1.1f, 0.8f);				// �R�[���ݒ�
	// �����̎��̗͂̈�
	SetInnerRadius(3.f);								// �������a
	SetInnerRadiusAngle(X3DAUDIO_PI / 8.f);				// �����p�x
	// �}���`�`�����l��
	m_emitter.ChannelCount = 1;							// �`�����l����
	m_emitter.ChannelRadius = 0.0f;						// �}���`�`�����l�����̔��a
	m_emitter.pChannelAzimuths = new FLOAT32[m_emitter.ChannelCount];	// ���W�A���P�ʂ̕��ʊp�ŕ\�����`�����l���ʒu�̃e�[�u��
	*m_emitter.pChannelAzimuths = atan2(m_emitter.OrientFront.z, m_emitter.OrientFront.x) - D3DX_PI * 0.5f;

	m_emitter.pVolumeCurve = (X3DAUDIO_DISTANCE_CURVE*)&X3DAudioDefault_LinearCurve;	// �{�����[���J�[�u
	m_emitter.pLFECurve = (X3DAUDIO_DISTANCE_CURVE*)&Emitter_LFE_Curve;					// LFE(�ቹ����)�J�[�u
	m_emitter.pLPFDirectCurve = NULL;													// ���[�p�X�t�B���^�̃_�C���N�g�p�X�W�������J�[�u
	m_emitter.pLPFReverbCurve = NULL;													// ���[�p�X�t�B���^�̃��o�[�u�p�X�W�������J�[�u
	m_emitter.pReverbCurve = (X3DAUDIO_DISTANCE_CURVE*)&Emitter_Reverb_Curve;			// ���o�[�u�Z���h���x�������J�[�u
	m_emitter.CurveDistanceScaler = 10.f;												// ���[�U�[��`�̃��[���h�P�ʂɂ���X�P�[���[
	m_emitter.DopplerScaler = 3.f;														// �h�b�v���[�ΈڃX�P�[���[

	// ���X�g�ɓo�^
	AddSourceVoiceData(pSourceVoiceData);				// �o�^

	return pSourceVoice;
}

IXAudio2SourceVoice *XA2SourceVoice3D::CreatePlay(std::string strFilePath, int loopCount, XA2LoadWave *pLoadWave)
{
	// �~���[�e�b�N�X
#ifdef THREAD_ON
	std::unique_lock<std::recursive_mutex> locker = CManager::Locker();
#endif

	// �I�[�f�B�I�f�[�^�̐���
	XA2SourceVoiceData3D *pSourceVoiceData = new XA2SourceVoiceData3D;

	XA2Manager *pXA2Manager = CManager::GetXA2Manager();		// Manager
	XA2Core *pXA2Core = pXA2Manager->GetXA2Core();				// XAudio2�G���W��
	XA2SoundResourceManager *pXA2SoundResourceManager = pXA2Manager->GetSoundResourceManager();	// �T�E���h���\�[�X�}�l�[�W��

	// ���\�[�X�̎擾
	pLoadWave->Create(strFilePath, loopCount);	// wave�`���Ŏ擾
	XA2LoadWave *pSoundResource = pXA2SoundResourceManager->GetXA2SoundResource(strFilePath);

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

	// �G�~�b�^�[�̐ݒ�
	m_emitter.Position = D3DXVECTOR3(0.f, 0.f, 0.f);	// ���W
	m_emitter.OrientFront = D3DXVECTOR3(0.f, 0.f, 1.f);	// �O����
	m_emitter.OrientTop = D3DXVECTOR3(0.f, 1.f, 0.f);	// �����
	m_emitter.Velocity = D3DXVECTOR3(0.f, 0.f, 0.f);	// �ړ���
														// �R�[��
	m_emitter.pCone = new X3DAUDIO_CONE;
	SetCone(D3DX_PI * 0.75f, 1.1f, 0.8f);				// �R�[���ݒ�

	// �����̎��̗͂̈�
	SetInnerRadius(3.f);								// �������a
	SetInnerRadiusAngle(X3DAUDIO_PI / 8.f);				// �����p�x
	
	// �}���`�`�����l��
	m_emitter.ChannelCount = 1;							// �`�����l����
	m_emitter.ChannelRadius = 0.0f;						// �}���`�`�����l�����̔��a
	m_emitter.pChannelAzimuths = new FLOAT32[m_emitter.ChannelCount];	// ���W�A���P�ʂ̕��ʊp�ŕ\�����`�����l���ʒu�̃e�[�u��
	*m_emitter.pChannelAzimuths = atan2(m_emitter.OrientFront.z, m_emitter.OrientFront.x) - D3DX_PI * 0.5f;


	m_emitter.pVolumeCurve = (X3DAUDIO_DISTANCE_CURVE*)&X3DAudioDefault_LinearCurve;	// �{�����[���J�[�u
	m_emitter.pLFECurve = (X3DAUDIO_DISTANCE_CURVE*)&Emitter_LFE_Curve;					// LFE(�ቹ����)�J�[�u
	m_emitter.pLPFDirectCurve = NULL;													// ���[�p�X�t�B���^�̃_�C���N�g�p�X�W�������J�[�u
	m_emitter.pLPFReverbCurve = NULL;													// ���[�p�X�t�B���^�̃��o�[�u�p�X�W�������J�[�u
	m_emitter.pReverbCurve = (X3DAUDIO_DISTANCE_CURVE*)&Emitter_Reverb_Curve;			// ���o�[�u�Z���h���x�������J�[�u
	m_emitter.CurveDistanceScaler = 10.f;												// ���[�U�[��`�̃��[���h�P�ʂɂ���X�P�[���[
	m_emitter.DopplerScaler = 3.f;														// �h�b�v���[�ΈڃX�P�[���[

//	Update();

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
	else
	{
		//pLoadWaveStreaming = (XA2LoadWaveStreaming*)pSoundResource;
		//xa2buffer.AudioBytes = pLoadWaveStreaming->GetAudioSize();
		//xa2buffer.pAudioData = &pLoadWaveStreaming->GetAudioData()[0][0];
		//xa2buffer.Flags = XAUDIO2_END_OF_STREAM;
		//xa2buffer.LoopCount = pLoadWaveStreaming->GetLoopCount();
	}

	// ���X�g�ɓo�^
	pSourceVoiceData->SetFilePass(strFilePath);		// �t�@�C����
	AddSourceVoiceData(pSourceVoiceData);			// �o�^

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

	// �I������SE�̉��
	//--------------------------------------------------------------------------------
	XA2SourceVoiceInterface::ReleaseEndSE();

	static int i;
	if (++i % 2 != 0)return;

	// 3D���Z
	//--------------------------------------------------------------------------------
	X3DAUDIO_LISTENER *pListener = CManager::GetCamera()->GetListener()->GetListener();	// ���X�i�[

	// �v�Z���ʂ̊i�[�p
	X3DAUDIO_DSP_SETTINGS DSPSettings = { 0 };
	DSPSettings.SrcChannelCount = m_emitter.ChannelCount;
	DSPSettings.DstChannelCount = pXA2Core->GetChannels();
	FLOAT32 matrix[6];
	DSPSettings.pMatrixCoefficients = matrix;

	// �v�Z�����t���O
	DWORD dwCalcFlags = X3DAUDIO_CALCULATE_MATRIX | X3DAUDIO_CALCULATE_DOPPLER | X3DAUDIO_CALCULATE_LPF_DIRECT | X3DAUDIO_CALCULATE_REVERB;

	// �v�Z����
	X3DAudioCalculate(
		XA2Manager::GetX3DInstance(),	// �n���h��
		pListener,						// ���X�i�[
		&m_emitter,						// �G�~�b�^�[
		dwCalcFlags,					// �v�Z�������e
		&DSPSettings);					// �v�Z����

	// �}�X�^�[�{�C�X
	IXAudio2MasteringVoice *pMaster = XA2Manager::GetMasteringVoice()->GetMasteringVoice();

	// �f�o�C�X�̓��̓`�����l����
	UINT32 channels = pXA2Core->GetChannels();

	// �e�\�[�X�{�C�X�ɉ��Z���ʂ�K�p
	for (auto pData : m_sourceVoices)
	{
		// �\�[�X�{�C�X�̃{�����[������уs�b�`�̒l��K�p
		IXAudio2SourceVoice *pSourceVoice = pData->GetSourceVoice();
//		pSourceVoice->SetOutputMatrix(pMaster, 1, channels, DSPSettings.pMatrixCoefficients);
		pSourceVoice->SetFrequencyRatio(DSPSettings.DopplerFactor);

		// ���o�[�u���x�����T�u�~�b�N�X�{�C�X�ɓK�p
//		pSourceVoice->SetOutputMatrix(m_pSubmixVoice, 1, 1, &DSPSettings.ReverbLevel);

		// ���[�p�X�t�B���^�[�_�C���N�g�W�����\�[�X�{�C�X�ɓK�p
//		XAUDIO2_FILTER_PARAMETERS FilterParameters = { LowPassFilter, 2.0f * sinf(X3DAUDIO_PI / 6.0f * DSPSettings.LPFDirectCoefficient), 1.0f };
//		pSourceVoice->SetFilterParameters(&FilterParameters);
	}
}

//--------------------------------------------------------------------------------
// �|�[�����O���� - �ʃX���b�h����
//--------------------------------------------------------------------------------
void XA2SourceVoice3D::Polling()
{
	// �|�[�����O����
	XA2LoadWave *pWaveData = nullptr;
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
	m_emitter.pCone->InnerAngle = RETURN_MIN(innerAngle, X3DAUDIO_2PI);				// �R�[���̊p�x (���W�A���P��)
	m_emitter.pCone->OuterAngle = X3DAUDIO_2PI - m_emitter.pCone->InnerAngle;		// 
	m_emitter.pCone->InnerVolume = RETURN_MIN(innerVolume, 2.0f);					// �{�����[�� �X�P�[��
	m_emitter.pCone->OuterVolume = RETURN_MIN(outerVolume, 2.0f);					// 
	m_emitter.pCone->InnerLPF = lpf;												// �_�C���N�g �p�X�܂��̓��o�[�u �p�X�̌W���X�P�[��
	m_emitter.pCone->OuterLPF = lpf;												// 
	m_emitter.pCone->InnerReverb = reverb;											// ���o�[�u �Z���h ���x�� �X�P�[��
	m_emitter.pCone->OuterReverb = reverb;											// 
}

// �R�[���̊p�x (���W�A���P��)
//--------------------------------------------------------------------------------
void XA2SourceVoice3D::SetConeAngle(float innerAngle)
{
	m_emitter.pCone->InnerAngle = RETURN_MIN(innerAngle, X3DAUDIO_2PI);
	m_emitter.pCone->OuterAngle = X3DAUDIO_2PI - m_emitter.pCone->InnerAngle;
}

// �{�����[�� �X�P�[��
//--------------------------------------------------------------------------------
void XA2SourceVoice3D::SetConeVolume(float innerVolume, float outerVolume)
{
	m_emitter.pCone->InnerVolume = RETURN_MIN(innerVolume, 2.0f);
	m_emitter.pCone->OuterVolume = RETURN_MIN(outerVolume, 2.0f);
}

// �_�C���N�g �p�X�܂��̓��o�[�u �p�X�̌W���X�P�[��
//--------------------------------------------------------------------------------
void XA2SourceVoice3D::SetConeLPF(float innerLPF, float outerLPF)
{
	m_emitter.pCone->InnerLPF = RETURN_MIN(innerLPF, 1.0f);
	m_emitter.pCone->OuterLPF = RETURN_MIN(outerLPF, 1.0f);
}

// ���o�[�u �Z���h ���x�� �X�P�[��
//--------------------------------------------------------------------------------
void XA2SourceVoice3D::SetConeReverb(float innerReverb, float outerReverb)
{
	m_emitter.pCone->InnerReverb = RETURN_MIN(innerReverb, 2.0f);
	m_emitter.pCone->OuterReverb = RETURN_MIN(outerReverb, 2.0f);
}
