#pragma once
#include "stdafx.h"
#include "Parm.h"
#include "Error.h"
#include "In.h"
#include "LT.h"
#include "IT.h"

namespace Log
{
	struct LOG
	{
		wchar_t logfile[PARM_MAX_SIZE] = L" ";
		ofstream *stream = NULL;
	};
	bool static flaw = false;
	bool static Error = true;
	LOG getstream(wchar_t logfile[]);//на открытие -in файла
	void writeLog(LOG &log);//протокол
	void writeParm(LOG &log, Parm::PARM &parm);//параметры
	void writeIn(LOG &log, In::IN &in);//исходные данные
	void writeError(ofstream *stream, Error::ERROR &error);//ошибки
	void writeLexTable(LOG &log, LT::LexTable &lextable);//таблица лексем
	void writeIDtable(LOG &log, IT::IdTable &IDtable);//таблица идентификаторов
	void writeLexTableConsole(LOG &log, LT::LexTable &Lextable);//таблица лексем
	void writeIDTableConsole(LOG &log, IT::IdTable &IDtable);////таблица идентификаторов
	void writeIntermediateCode(LOG &log, LT::LexTable &lextable);//польская нотация
	void Close(LOG &log);
}