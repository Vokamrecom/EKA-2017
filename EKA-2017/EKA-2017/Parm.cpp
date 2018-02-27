#include "stdafx.h" 
#include "Parm.h"
#include "Error.h"

namespace Parm
{
	PARM getparm(int argc, wchar_t* argv[])//argc – количество параметров (int), argv – массив указателей на строки (_TCHAR* -указатель строку wchar_t)
	{
		PARM param = { L"", L"", L"", L"", false, false, false };

		for (int i = 1; i < argc; i++)
		{
			if (wcslen(argv[i]) > PARM_MAX_SIZE)//проверка превышена ли длина входного параметра в 300
				throw ERROR_THROW(104)
			else
			{
				if (wcsncmp(argv[i], PARM_IN, wcslen(PARM_IN)) == 0) //сравнение строки с подстрокой
				{
					wcscpy_s(param.in, argv[i] + wcslen(PARM_IN)); //начнет копировать после двоеточия
				}
				else if (wcsncmp(argv[i], PARM_LOG, wcslen(PARM_LOG)) == 0)
				{
					wcscpy_s(param.log, argv[i] + wcslen(PARM_LOG));
				}
				else if (wcsncmp(argv[i], PARMLT, wcslen(PARMLT)) == 0)
				{
					param.LTtable = true;
				}
				else if (wcsncmp(argv[i], PARMIT, wcslen(PARMIT)) == 0)
				{
					param.ITtable = true;
				}
			}
		}

		if (param.in[0] == NULL)//проверка задан ли параметр -in файл
			throw ERROR_THROW(100);//генерирует ошибку  "Параметр -in должен быть задан"


		if (param.log[0] == NULL)
		{
			wcscpy_s(param.log, param.in);//копирует
			wcsncat_s(param.log, PARM_LOG_DEFAULT_EXT, sizeof(PARM_LOG_DEFAULT_EXT));//приписывает sizeof символов PARM_LOG_DEFAULT_EXT в parm.log
		};

		return param;
	};
}
