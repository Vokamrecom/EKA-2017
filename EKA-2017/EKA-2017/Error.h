#pragma once
#define ERROR_MAXSIZE_MESSAGE 200	
#define ERROR_MAX_ENTRY 1000		
#define ERROR_THROW(id)\
	Error::GetError(id);				
#define ERROR_THROW_IN(id, line, cells)\
	Error::GetError(id, line, cells);	
#define ERROR_ENTRY(id, message) {id, message}	//элемент таблицы ошибок
#define ERROR_ENTRY_NODEF(id)		ERROR_ENTRY(-id, "Неопределенная ошибка")

#define ERROR_ENTRY_NODEF10(id)		ERROR_ENTRY_NODEF(id+0),ERROR_ENTRY_NODEF(id+1),ERROR_ENTRY_NODEF(id+2),ERROR_ENTRY_NODEF(id+3), \
									ERROR_ENTRY_NODEF(id + 4), ERROR_ENTRY_NODEF(id + 5), ERROR_ENTRY_NODEF(id + 6), ERROR_ENTRY_NODEF(id + 7), \
									ERROR_ENTRY_NODEF(id + 8), ERROR_ENTRY_NODEF(id + 9)

#define ERROR_ENTRY_NODEF100(id)	ERROR_ENTRY_NODEF10(id+0), ERROR_ENTRY_NODEF10(id+10),ERROR_ENTRY_NODEF10(id+20), ERROR_ENTRY_NODEF10(id+30), \
									ERROR_ENTRY_NODEF10(id + 40), ERROR_ENTRY_NODEF10(id + 50), ERROR_ENTRY_NODEF10(id + 60), ERROR_ENTRY_NODEF10(id + 70), \
									ERROR_ENTRY_NODEF10(id + 80), ERROR_ENTRY_NODEF10(id + 90)

namespace Error
{
	struct ERROR								//тип исключения для throw ERROR_THROW | ERROR_THROW_IN в catch(ERROR)
	{
		int id;
		char message[ERROR_MAXSIZE_MESSAGE];
		struct
		{
			short line = -1;							//номер строки (0, 1, 2, ...)
			short cells = -1;							//номер позиции в строке(0, 1, 2, ...)
		} position;
	};

	ERROR GetError(int id);							//сформировать ERROR для ERROR_THROW
	ERROR GetError(int id, int line, int cells);	//сформировать ERROR для ERROR_THROW_IN
}