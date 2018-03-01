// ======== ======== ======== ======== ======== ======== ======== ========
//
//
//	title : �֗��֐��Q
//	Auter : KENSUKE WATANABE
//
//
// ======== ======== ======== ======== ======== ======== ======== ========
#include "utility.h"

// -------- -------- -------- -------- -------- -------- -------- --------
//
//
// �֗��֐��Q
//
//
// -------- -------- -------- -------- -------- -------- -------- --------
namespace Utility
{
	//
	// char* - wchar_t* �ϊ�
	// -------- -------- -------- -------- -------- -------- -------- --------
	wchar_t *Cast::CastWChae_t(const char *orig)
	{
		size_t newsize = strlen(orig) + 1;
		wchar_t * wcstring = new wchar_t[newsize];

		// �^�ϊ�
		size_t convertedChars = 0;
		mbstowcs_s(&convertedChars, wcstring, newsize, orig, _TRUNCATE);

		return wcstring;
	}

	//
	// �w�蕶����܂œǂݔ�΂�
	// -------- -------- -------- -------- -------- -------- -------- --------
	bool Character::CutStrToStr(std::string *strData, int *current, const std::string &strString)
	{
		while (1)
		{
			if (*current >= static_cast<int>(strData->size()))return false;

			// �w�蕶������܂߂ēǂݔ�΂�
			// -------- -------- -------- -------- -------- -------- -------- --------
			int size = strString.size();
			if (strncmp(&strData[0][*current], &strString[0], size) == 0)
			{
				*current += strString.size();
				return true;
			}
			// 1�����ǂݔ�΂�
			// -------- -------- -------- -------- -------- -------- -------- --------
			*current += 1;
		}
		return false;
	}

	//
	// �w��g�[�N���܂ł̕�����̓��A�؂���g�[�N���𖳎�������������擾����
	// -------- -------- -------- -------- -------- -------- -------- --------
	bool Character::GetStrToToken(std::string *strData, int *current,
		const std::string &strToken, const std::string &strCutToken, std::string *strBuf)
	{
		strBuf->clear();
		while(1)
		{
			if (*current >= static_cast<int>(strData->size()))return false;
			
			// �؂���g�[�N��
			// -------- -------- -------- -------- -------- -------- -------- --------
			int size = strCutToken.size();
			for (int i = 0; i < size; i++)
			{
				if (strncmp(&strData[0][*current], &strCutToken[i], 1) == 0)
				{
					*current += 1;
					i = 0;
				}
			}
			// �ǂݎ��g�[�N��
			// -------- -------- -------- -------- -------- -------- -------- --------
			for (unsigned int i = 0; i < strToken.size(); i++)
			{
				if (strncmp(&strData[0][*current], &strToken[i], 1) == 0)
				{
					*current += 1;
					return true;
				}
			}
			// 1�����ǂݎ��
			// -------- -------- -------- -------- -------- -------- -------- --------
			*strBuf += strData[0][*current];
			*current += 1;
		}
		return false;
	}

	//
	// �w�蕶����܂ł̕�����̓��A�؂���g�[�N���𖳎�������������擾����
	// -------- -------- -------- -------- -------- -------- -------- --------
	bool Character::GetStrToStr(std::string *strData, int *current,
		const std::string &strString, const std::string &strCutToken, std::string *strBuf)
	{
		strBuf->clear();
		while (1)
		{
			if (*current >= static_cast<int>(strData->size()))return false;

			// �؂���g�[�N��
			// -------- -------- -------- -------- -------- -------- -------- --------
			for (unsigned int i = 0; i < strCutToken.size(); i++)
			{
				if (strncmp(&strData[0][*current], &strCutToken[i], 1) == 0)
				{
					*current += 1;
					i = 0;
				}
			}
			// �ǂݎ��g�[�N��
			// -------- -------- -------- -------- -------- -------- -------- --------
			if (strncmp(&strData[0][*current], &strString[0], strString.size()) == 0)
			{
				*current += strString.size();
				return true;
			}
			// 1�����ǂݎ��
			// -------- -------- -------- -------- -------- -------- -------- --------
			*strBuf += strData[0][*current];
			*current += 1;
		}
		return false;
	}
}
