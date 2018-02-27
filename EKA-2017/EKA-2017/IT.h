#pragma once

#include "stdafx.h"
//спецификация таблицы идентификаторов  
#define ID_MAXSIZE	15				//макс число символов идентификатора
#define ID_FOR_IT   30              //макс число символов идентификатор + принадлежность ф-ии
#define TI_MAXSIZE	4096			//макс число количество строк в таблице идентификаторов
#define TI_INT_DEFAULT	0x00000000	//значение по умолчанию для int
#define TI_STR_DEFAULT	0x00		//значение по умолчанию для sting
#define TI_NULLIDX		0xffffffff	//нет элемента таблицы идентификаторов
#define TI_STR_MAXSIZE	200
#define INT_MAXSIZE     2147483647
#define INT_MAXSIZE_PARM   10
#define INT_MINUS_MAXSIZE   0

namespace IT
{
	enum IDDATATYPE { NUM = 1, STR = 2 };			//типы данных идентификаторов
	enum IDTYPE { V = 1, F = 2, P = 3, L = 4, S = 5 };	//типы идентификаторов: переменная, функция, параметр, литерал, стандартная функция
	struct Entry                            //Структура содержимого таблицы идентификаторов
	{
		int	idxfirstLE;						//индекс в таблице лексем		
		char		id[ID_FOR_IT];					//идентификатор, автоматом усекается до ID_MAXSIZE
		IDDATATYPE	iddatatype;						//тип данных
		IDTYPE		idtype;							//тип идентификатора
		int count_parm;
		char parms[INT_MAXSIZE_PARM];

		union
		{
			int	vNUM;            	//значение integer
			struct
			{
				int len;							//количество символов в string
				char str[TI_STR_MAXSIZE - 1];		//символы string
			}vSTR;									//значение string
		}value;	//значение идентификатора
		Entry()
		{
			this->value.vNUM = TI_INT_DEFAULT;
			this->value.vSTR.len = NULL;
		};
		Entry(char* id, int idxLT, IDDATATYPE datatype, IDTYPE idtype)
		{
			strncpy_s(this->id, id, ' ');
			this->idxfirstLE = idxLT;
			this->iddatatype = datatype;
			this->idtype = idtype;
		};
	};
	struct IdTable		//экземпляр таблицы идентификаторов
	{
		int maxsize;	//ёмкость таблицы идентификаторов < TI_MAXSIZE
		int size;		//текущий размер таблицы идентификаторов < maxsize
		Entry* table;	//массив строк таблицы идентификаторов
	};

	IdTable Create(int size = NULL);	//ёмкость таблицы идентификаторов < TI_MAXSIZE

	void Add(					//добавить строку в таблицу идентификаторов
		IdTable &idtable,		//экземпляр таблицы идентификаторов
		Entry entry);			//строка таблицы идентификаторов 

	int IsId(					//возврат: номер строки(если есть), TI_NULLIDX(если есть)
		IdTable &idtable,		//экземпляр таблицы идентификаторов
		char id[ID_MAXSIZE]);	//идентификатор

	void SetValue(IdTable &idtable, int, char*);//задать значение идентификатора
}