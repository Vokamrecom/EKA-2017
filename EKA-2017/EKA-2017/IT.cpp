#include "stdafx.h"
#include "IT.h"

namespace IT
{
	IdTable Create(int size)
	{
		IdTable idtable;
		idtable.table = new Entry[size];
		idtable.size = NULL;
		return idtable;
	}

	void Add(IdTable &idtable, Entry entry)
	{
		strncpy_s(idtable.table[idtable.size].id, entry.id, ID_FOR_IT - 1); // 5 символов на идентификатор + 5 символов на принадлежность к ф-ии
		idtable.table[idtable.size].idxfirstLE = entry.idxfirstLE; // местонахождение в таблице лексем(первое упоминание)
		idtable.table[idtable.size].iddatatype = entry.iddatatype; // тип данных integer ...
		idtable.table[idtable.size].idtype = entry.idtype; // функции переменная ...
		idtable.table[idtable.size].value.vNUM = TI_INT_DEFAULT; // значение по ум-ию для int
		idtable.table[idtable.size].value.vSTR.len = NULL; // длина строки 0
		idtable.table[idtable.size].value.vSTR.str[0] = TI_STR_DEFAULT; // пустая строка
		idtable.size++;
	}

	int IsId(IdTable &idtable, char id[ID_MAXSIZE])
	{
		for (int i = 0; i < idtable.size; i++)
		{
			if (strcmp(idtable.table[i].id, id) == NULL)
				return i;
		}
		return -1;
	}

	void SetValue(IdTable &idtable, int indTI, char* token) // установка значения переменной
	{
		if (idtable.table[indTI].iddatatype == NUM)
		{
			int tempInt = atoi(token);
			if (tempInt > INT_MAXSIZE)
				tempInt = INT_MAXSIZE;
			else if (tempInt < INT_MINUS_MAXSIZE)
				tempInt = INT_MINUS_MAXSIZE;
			idtable.table[indTI].value.vNUM = tempInt;
		}
		else
		{
			idtable.table[indTI].value.vSTR.len = strlen(token);
			strcpy_s(idtable.table[indTI].value.vSTR.str, TI_STR_MAXSIZE, token);
		}
	}
}