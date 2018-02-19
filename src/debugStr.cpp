//================================================================================
//
//	Auter : KENSUKE WATANABE
//	Data  : 2017/04/27
//
//--------------------------------------------------------------------------------
#ifdef _DEBUG		// Debug���̂ݎ��s

//================================================================================
// �C���N���[�h
//================================================================================
#include "main.h"
#include <assert.h>
#include "debugStr.h"
#include "renderer.h"

//--------------------------------------------------------------------------------
// �ÓI�����o�ϐ�
//--------------------------------------------------------------------------------
std::unordered_map<std::string, CDebugStr*> CDebugStr::m_umapDebugStr;

//--------------------------------------------------------------------------------
// �R���X�g���N�^
//--------------------------------------------------------------------------------
CDebugStr::CDebugStr()
{

}

//--------------------------------------------------------------------------------
// �f�X�g���N�^
//--------------------------------------------------------------------------------
CDebugStr::~CDebugStr()
{

}

//--------------------------------------------------------------------------------
// ������
//--------------------------------------------------------------------------------
void CDebugStr::Init(int line, std::string name, std::vector<float> vecVal)
{
	assert(!vecVal.empty() && "�l����ł�");

	// �l�̑��
	m_nLine = line;
	m_nNumValue = vecVal.size();
	m_aName = name;
	for (int i = 0; i < m_nNumValue; i++)
	{
		m_vecValue.push_back(vecVal[i]);
	}

	// �������̐ݒ�
	LPDIRECT3DDEVICE9 pDevice = CRenderer::GetDevice();
	D3DXCreateFont(pDevice,		// �f�o�C�X
		18,						// �����̍���
		0,						// �����̕�(0�ɂ���ƍ����ɍ��킹���䗦�ɂȂ�)
		0,						// �t�H���g�̑���(0���ƃf�t�H���g)
		0,						// �~�b�v�}�b�v�i�d���Ȃ�)�@�f�o�b�O�Ȃ̂ŉ����Ă������[
		false,					// �C�^���b�N��
		SHIFTJIS_CHARSET,		// �t�H���g�̕����Z�b�g
		OUT_DEFAULT_PRECIS,		// �t�H���g�����v���Ȃ��������̎w��
		DEFAULT_QUALITY,		// �I�ԃt�H���g�̃N�I���e�B
		DEFAULT_PITCH,			// �s�b�`�ƃt�@�~���[
		"Terminal",
		&m_pDebugFont);
}

//--------------------------------------------------------------------------------
// �X�V
//--------------------------------------------------------------------------------
void CDebugStr::Update()
{
	std::string temp;
	for (int nCntLine = 0; nCntLine < DEBUG_DISPLAY_LINE; nCntLine++)
	{
		if (m_nLine == nCntLine)
		{
			// [�\���� + : ]
			temp += (m_aName + ": ");
			std::string strBuff;

			// [�l + , �l + , �l + ...]
			for(int i = 0; i < m_nNumValue; i++)
			{
				strBuff += std::to_string(m_vecValue.at(i));

				if (i != m_nNumValue - 1)strBuff += ", ";
			}
			temp += strBuff;
		}
		// ���s�����������ĉ��s
		if (nCntLine >= 1)temp += "\n";
	}
	m_strDebug = temp;
}

//--------------------------------------------------------------------------------
// �`��
//--------------------------------------------------------------------------------
void CDebugStr::Draw()
{
//	RECT rect = { 0, DEBUG_DISPLAY_POS_Y, SCREEN_WIDTH, SCREEN_HEIGHT };	// �f�o�b�O���\���̈�
//	m_pDebugFont->DrawText(
//		nullptr,							// �f�o�C�X�R���e�L�X�g nullptr�łł�
//		&m_strDebug[0],						// ������
//		-1,
//		&rect,								// �����͈�
//		DT_LEFT,							// ���`���@ DT_LEFT:���l
//		D3DCOLOR_RGBA(255, 255, 0, 255)		// �����F
//	);
}

//--------------------------------------------------------------------------------
// �S�X�V
//--------------------------------------------------------------------------------
void CDebugStr::UpdateAll()	
{
	std::for_each(m_umapDebugStr.begin(), m_umapDebugStr.end(), [](std::pair<const std::string, CDebugStr*> &umapDebugStr) {umapDebugStr.second->Update(); });
}

//--------------------------------------------------------------------------------
// �S�`��
//--------------------------------------------------------------------------------
void CDebugStr::DrawAll()
{
	std::for_each(m_umapDebugStr.begin(), m_umapDebugStr.end(), [](std::pair<const std::string, CDebugStr*> &umapDebugStr) {umapDebugStr.second->Draw(); });
}

//--------------------------------------------------------------------------------
// �S���
//--------------------------------------------------------------------------------
void CDebugStr::ReleaseAll()
{
	// �R���e�i�𖖔��܂Ń��[�v
	for (auto it = m_umapDebugStr.begin(); it != m_umapDebugStr.end();)
	{
		// �v�f(second)�̕\���p�̒l�̃N���A
		m_umapDebugStr[it->first]->m_vecValue.clear();

		// �v�f(second)�̃t�H���g�̉��
		if (m_umapDebugStr[it->first]->m_pDebugFont != nullptr)
		{
			m_umapDebugStr[it->first]->m_pDebugFont->Release();
			m_umapDebugStr[it->first]->m_pDebugFont = nullptr;
		}
		// �v�f(second)�̉��
		if (m_umapDebugStr[it->first] != nullptr)
		{
			delete m_umapDebugStr[it->first];
			m_umapDebugStr[it->first] = nullptr;
		}
		m_umapDebugStr.erase(it++);		// �t�@�C���p�X�폜
	}
	// �������g����ɂ���(�S�v�f�N���A)
	m_umapDebugStr.clear();
}

//--------------------------------------------------------------------------------
// �f�o�b�O�����ݒ�
//--------------------------------------------------------------------------------
void CDebugStr::Set(std::string name, std::vector<float> vecVal)
{
	// �v�f�̗L�����m���߂�
	if (m_umapDebugStr.count(name) == 1)
	{// ����܂���
		assert(!vecVal.empty() && "�l����ł�");

		for (int i = 0; i < m_umapDebugStr[name]->m_nNumValue; i++)
		{
			m_umapDebugStr[name]->m_vecValue[i] = vecVal[i];
		}
	}
}

//--------------------------------------------------------------------------------
// �f�o�b�O��������
//--------------------------------------------------------------------------------
void CDebugStr::Create(int line, std::string name, std::vector<float> vecVal)
{
	// �C���X�^���X�̐���
	CDebugStr *debugStr = new CDebugStr();

	// �v�f�̒ǉ�
	m_umapDebugStr[name] = debugStr;

	// ������
	debugStr->Init(line, name, vecVal);
}

#endif

