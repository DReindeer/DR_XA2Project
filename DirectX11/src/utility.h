// ======== ======== ======== ======== ======== ======== ======== ========
//
//
//	title : �֗��֐��Q
//	Auter : KENSUKE WATANABE
//
//
// ======== ======== ======== ======== ======== ======== ======== ========
#ifndef _UTILITY_H_
#define _UTILITY_H_

#include "main.h"
#include <string>

#define SAFE_RELEASE(p) { if(p) { (p)->Release(); (p)=nullptr; } }
// 
namespace Utility
{
	class Cast
	{
	public:
		static wchar_t *CastWChae_t(const char *orig);
	};

	class Character
	{
	public:
		// �w�蕶����܂œǂݔ�΂�
		// -------- -------- -------- -------- -------- -------- -------- --------
		static bool CutStrToStr(std::string *strData, int *current, const std::string &strString);

		// �w��g�[�N���܂ł̕�����̓��A�؂���g�[�N���𖳎�������������擾����
		// -------- -------- -------- -------- -------- -------- -------- --------
		static bool GetStrToToken(std::string *strData, int *current,
			const std::string &strToken, const std::string &strCutToken, std::string *strBuf);

		// �w�蕶����܂ł̕�����̓��A�؂���g�[�N���𖳎�������������擾����
		// -------- -------- -------- -------- -------- -------- -------- --------
		static bool GetStrToStr(std::string *strData, int *current,
			const std::string &strString, const std::string &strCutToken, std::string *strBuf);
	};
}

#endif