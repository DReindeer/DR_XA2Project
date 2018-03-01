// ======== ======== ======== ======== ======== ======== ======== ========
//
//
//	title : 便利関数群
//	Auter : KENSUKE WATANABE
//
//
// ======== ======== ======== ======== ======== ======== ======== ========
#include "utility.h"

// -------- -------- -------- -------- -------- -------- -------- --------
//
//
// 便利関数群
//
//
// -------- -------- -------- -------- -------- -------- -------- --------
namespace Utility
{
	//
	// char* - wchar_t* 変換
	// -------- -------- -------- -------- -------- -------- -------- --------
	wchar_t *Cast::CastWChae_t(const char *orig)
	{
		size_t newsize = strlen(orig) + 1;
		wchar_t * wcstring = new wchar_t[newsize];

		// 型変換
		size_t convertedChars = 0;
		mbstowcs_s(&convertedChars, wcstring, newsize, orig, _TRUNCATE);

		return wcstring;
	}

	//
	// 指定文字列まで読み飛ばす
	// -------- -------- -------- -------- -------- -------- -------- --------
	bool Character::CutStrToStr(std::string *strData, int *current, const std::string &strString)
	{
		while (1)
		{
			if (*current >= static_cast<int>(strData->size()))return false;

			// 指定文字列を含めて読み飛ばし
			// -------- -------- -------- -------- -------- -------- -------- --------
			int size = strString.size();
			if (strncmp(&strData[0][*current], &strString[0], size) == 0)
			{
				*current += strString.size();
				return true;
			}
			// 1文字読み飛ばし
			// -------- -------- -------- -------- -------- -------- -------- --------
			*current += 1;
		}
		return false;
	}

	//
	// 指定トークンまでの文字列の内、切り取りトークンを無視した文字列を取得する
	// -------- -------- -------- -------- -------- -------- -------- --------
	bool Character::GetStrToToken(std::string *strData, int *current,
		const std::string &strToken, const std::string &strCutToken, std::string *strBuf)
	{
		strBuf->clear();
		while(1)
		{
			if (*current >= static_cast<int>(strData->size()))return false;
			
			// 切り取りトークン
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
			// 読み取りトークン
			// -------- -------- -------- -------- -------- -------- -------- --------
			for (unsigned int i = 0; i < strToken.size(); i++)
			{
				if (strncmp(&strData[0][*current], &strToken[i], 1) == 0)
				{
					*current += 1;
					return true;
				}
			}
			// 1文字読み取り
			// -------- -------- -------- -------- -------- -------- -------- --------
			*strBuf += strData[0][*current];
			*current += 1;
		}
		return false;
	}

	//
	// 指定文字列までの文字列の内、切り取りトークンを無視した文字列を取得する
	// -------- -------- -------- -------- -------- -------- -------- --------
	bool Character::GetStrToStr(std::string *strData, int *current,
		const std::string &strString, const std::string &strCutToken, std::string *strBuf)
	{
		strBuf->clear();
		while (1)
		{
			if (*current >= static_cast<int>(strData->size()))return false;

			// 切り取りトークン
			// -------- -------- -------- -------- -------- -------- -------- --------
			for (unsigned int i = 0; i < strCutToken.size(); i++)
			{
				if (strncmp(&strData[0][*current], &strCutToken[i], 1) == 0)
				{
					*current += 1;
					i = 0;
				}
			}
			// 読み取りトークン
			// -------- -------- -------- -------- -------- -------- -------- --------
			if (strncmp(&strData[0][*current], &strString[0], strString.size()) == 0)
			{
				*current += strString.size();
				return true;
			}
			// 1文字読み取り
			// -------- -------- -------- -------- -------- -------- -------- --------
			*strBuf += strData[0][*current];
			*current += 1;
		}
		return false;
	}
}
