#include "stdafx.h"
#include "Error.h"

namespace Error
{
	ERROR errors[ERROR_MAX_ENTRY] =
	{
		ERROR_ENTRY(0, "Недопустимый код ошибки"),		//код ошибки вне диапазона 0 - ERROR_MAX_ENTRY
		ERROR_ENTRY(1, "Системный сбой"),
		ERROR_ENTRY_NODEF(2), ERROR_ENTRY_NODEF(3), ERROR_ENTRY_NODEF(4),
		ERROR_ENTRY_NODEF(5), ERROR_ENTRY_NODEF(6),
		ERROR_ENTRY_NODEF(7), ERROR_ENTRY_NODEF(8), ERROR_ENTRY_NODEF(9),
		ERROR_ENTRY_NODEF10(10), ERROR_ENTRY_NODEF10(20), ERROR_ENTRY_NODEF10(30), ERROR_ENTRY_NODEF10(40), ERROR_ENTRY_NODEF10(50),
		ERROR_ENTRY_NODEF10(60), ERROR_ENTRY_NODEF10(70), ERROR_ENTRY_NODEF10(80), ERROR_ENTRY_NODEF10(90),
		ERROR_ENTRY(100, "Параметр -in должен быть задан"),//ошибки входного кода
		ERROR_ENTRY_NODEF(101),
		ERROR_ENTRY_NODEF(102),
		ERROR_ENTRY(103, "Превышено максимальное количество параметров при вызове функции"),//lexanalize
		ERROR_ENTRY(104, "Превышена длина входного параметра"),//parm
		ERROR_ENTRY(105, "Превышен максимальный размер литерала"),//in
		ERROR_ENTRY(106, "Два сепаратора подряд"),//lexanalize
		ERROR_ENTRY(107, "Попытка переопределения"),//lezanalize
		ERROR_ENTRY(108, "Отсуствует предварительное определение"),//lexanalize
		ERROR_ENTRY(109, "Нет закрывающей кавычки"),//in
		ERROR_ENTRY(110, "Ошибка при открытии файла с исходным кодом(-in)"),//in
		ERROR_ENTRY(111, "Недопустимый символ в исходном файле (-in)"),//in
		ERROR_ENTRY(112, "Ошибка при создании файла протокола(-log)"),
		ERROR_ENTRY(113),
		ERROR_ENTRY(114, "Несоответствие типов"),//
		ERROR_ENTRY(115, "Запрещено присваивать значение функции"),//lexanalize
		ERROR_ENTRY(116),
		ERROR_ENTRY(117, "Превышен размер таблицы лексем"),//lt
		ERROR_ENTRY(118, "Отрицательное значение литерала"),//lexanalize
		ERROR_ENTRY(119, "Отсутствие сепаратора для разделения конструкций"),//lezanalize
		ERROR_ENTRY_NODEF10(120),ERROR_ENTRY_NODEF10(130), ERROR_ENTRY_NODEF10(140), ERROR_ENTRY_NODEF10(150),
		ERROR_ENTRY_NODEF10(160), ERROR_ENTRY_NODEF10(170), ERROR_ENTRY_NODEF10(180), ERROR_ENTRY_NODEF10(190),
		ERROR_ENTRY_NODEF100(200), ERROR_ENTRY_NODEF100(300),
		ERROR_ENTRY(400, "Точка входа main() не найдена"),//lexanalize на этапе семантич
		ERROR_ENTRY(401, "Попытка преобразования типов"),//lexanalize
		ERROR_ENTRY(402, "Использование стандартных функций без подключения стандартной библиотеки"),//lexanalize
		ERROR_ENTRY(403, "Подключено более одной стандартной библиотеки"),//lexanalize
		ERROR_ENTRY(404, "Попытка проведения операций со строковым типом данных"),//lexanalize
		ERROR_ENTRY(405, "Тип возвращаемого значения и тип функции не соответствуют"),//lexanalize
		ERROR_ENTRY(406, "Неправильное количество параметров при вызове функции"),//lexanalize
		ERROR_ENTRY(407, "Неправильный тип параметров при вызове функции"),//lexanalize
		ERROR_ENTRY(408, "Несоответствие типов"),//-
		ERROR_ENTRY(409, "Неправильный вызов функции"),//lexanalize
		ERROR_ENTRY_NODEF10(410),ERROR_ENTRY_NODEF10(420),ERROR_ENTRY_NODEF10(430), ERROR_ENTRY_NODEF10(440),
		ERROR_ENTRY_NODEF10(450),ERROR_ENTRY_NODEF10(460), ERROR_ENTRY_NODEF10(470), ERROR_ENTRY_NODEF10(480),
		ERROR_ENTRY_NODEF10(490),
		ERROR_ENTRY_NODEF100(500),
		ERROR_ENTRY(600, "Неверная структура программы"),//grb? на этапе синтаксич
		ERROR_ENTRY(601, "Ошибка в параметрах функции "),
		ERROR_ENTRY(602, "Ошибочный оператор"),
		ERROR_ENTRY(603, "Ошибка в выражении"),
		ERROR_ENTRY(604, "Ошибка в параметрах вызываемой функции"),
		ERROR_ENTRY(605, "Ошибка в построении выражения"),//lexanalize
		ERROR_ENTRY_NODEF100(700), ERROR_ENTRY_NODEF100(800)
	};


	ERROR GetError(int id)
	{
		if (id < 0 || id > ERROR_MAX_ENTRY)
			return errors[0];
		else
			return errors[id];
	}

	ERROR GetError(int id, int line, int cells)
	{
		ERROR dopError = GetError(id);
		dopError.position.cells = cells;
		dopError.position.line = line;
		return dopError;
	}
}