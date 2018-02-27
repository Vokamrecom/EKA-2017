#include "stdafx.h"
#include "PN.h"

int PN::prior(char znak)
{
	switch (znak)
	{
	case '+':
	case '-':
		return 2;
	case '*':
	case '/':
		return 3;
	default:
		return 0;
	}
}

int PN::search(int index, Lexer::LEX *tables) //поиск выражения
{
	for (int i = index; i < tables->Lextable.size; i++) //перебор лексем
	{
		if (tables->Lextable.table[i].lexema == LEX_PLUS || tables->Lextable.table[i].lexema == LEX_MINUS
			|| tables->Lextable.table[i].lexema == LEX_STAR || tables->Lextable.table[i].lexema == LEX_DIRSLASH) //находим операцию
		{
			for (; ; i--) //перебираем лексемы обратно
			{
				if (tables->Lextable.table[i].lexema == LEX_EQUAL || tables->Lextable.table[i].lexema == LEX_RIGHTTHESIS)
					return i + 1;
			}
		}
	}
	return -1; //если не нашли возвращаем минус 1
}

bool PN::CreatePolishNotation(Lexer::LEX *tables)
{
	std::stack <LT::Entry> stack_operators;// хранятся операторы
	bool build = false;

	int i = 0;
	int j = 0;
	int k = 0;

	while (search(i, tables) != -1)
	{
		j = i = search(i, tables);//индекс первого символа после равно
		for (; tables->Lextable.table[i].lexema != LEX_SEPARATOR; i++, k++) //читаем до символа конца строки, увеличивая счётчик К
		{
			if (tables->Lextable.table[i].lexema == LEX_LEFTHESIS || tables->Lextable.table[i].lexema == LEX_RIGHTTHESIS) //если нашли ( или )
			{
				switch (tables->Lextable.table[i].lexema) //смотрим какая лексема
				{
				case LEX_LEFTHESIS:
				{
					if ((tables->Lextable.table[i - 1].idxTI != -1 && tables->IDtable.table[tables->Lextable.table[i - 1].idxTI].idtype == IT::F)
						|| tables->Lextable.table[i - 1].lexema == 'k' || tables->Lextable.table[i - 1].lexema == 'b')  // Вернёт -1 если это не будет являтся переменной
					{
						while (tables->Lextable.table[i - 1].lexema != LEX_RIGHTTHESIS)
						{
							i++;
						}
						break;
					}
					stack_operators.push(tables->Lextable.table[i]);// ( добавляем в стек Операторов элемент таблицы лексем
					break;
				}
				case LEX_RIGHTTHESIS:
					while (stack_operators.top().lexema != LEX_LEFTHESIS) // ) посмотреть вехний элемент из стека
					{
						result[size++] = stack_operators.top();
						stack_operators.pop(); //извлекает вершину стека операторов
					}
					stack_operators.pop(); //извлекает вершину стека операторов
					break;
				}
			}
			else
			{
				switch (tables->Lextable.table[i].lexema) //если это не скобки()
				{
				case LEX_STAR:
				case LEX_DIRSLASH:
				{
					stack_operators.push(tables->Lextable.table[i]);//добавляем в стек Операторов элемент таблицы лексем
					break;
				}
				case LEX_PLUS:
				case LEX_MINUS:
					if (stack_operators.empty() || prior(tables->Lextable.table[i].lexema) == prior(stack_operators.top().lexema))//пуст
						stack_operators.push(tables->Lextable.table[i]);//добавляем в стек Операторов элемент таблицы лексем 
					else //стек Операторов не пустой
					{
						while (!stack_operators.empty() && stack_operators.top().lexema != LEX_LEFTHESIS &&
							prior(tables->Lextable.table[i].lexema) < prior(stack_operators.top().lexema)) //если лексема в стеке не (, и приоретет операции меньше либо равен приоритету лежащему в голове стека, то..
						{
							result[size++] = stack_operators.top(); //заносим верхушку стека в результат
							stack_operators.pop();//извлекает верхушку стека
						}
						if (!stack_operators.empty() && stack_operators.top().lexema == LEX_LEFTHESIS)// проверяем верхушку стека
						{
							stack_operators.push(tables->Lextable.table[i]);//добавить в стек операторов элемент таблицы лексем
						}
						if (stack_operators.empty())// проверяем верхушку стека
						{
							stack_operators.push(tables->Lextable.table[i]);//добавить в стек операторов элемент таблицы лексем
						}
					}
					break;
				default:
					result[size++] = tables->Lextable.table[i];//заносим таблицу лексем в результат
					break;
				}
			}
		}
		for (; !stack_operators.empty(); i++)	//выбить все операторы в Выходной стек
		{
			result[size++] = stack_operators.top(); //заносим все оперыторы в результат
			stack_operators.pop();// вынимаем верхушку стека
		}
		for (int i = 0; i < size; j++, i++, k--) //заполняяем таблицу лексем, для данного выражения
		{
			tables->Lextable.table[j].lexema = result[i].lexema;
			tables->Lextable.table[j].idxTI = result[i].idxTI;
			tables->Lextable.table[j].sn = result[i].sn; //нормер стр в инсх тект
			tables->IDtable.table[result[i].idxTI].idxfirstLE;
		}
		for (; k > 0; k--, j++)	//заместить все элементы, "удалённые в процессе создания польской"
		{
			tables->Lextable.table[j].lexema = NULL;
			tables->Lextable.table[j].idxTI = NULL;
		}
		size = 0;
		build = true;
	}
	return build;
}