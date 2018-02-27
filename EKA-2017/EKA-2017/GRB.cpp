#include "stdafx.h"
#include "GRB.h"
//синтакс
#define GRB_ERROR_SERIES 600

namespace GRB
{
	Greibach greibach(NS('S'), TS('$'), 6,
		Rule(NS('S'), GRB_ERROR_SERIES, 5,   // опис общую Структуру программы
			Rule::Chain(10, TS('q'), TS('f'), TS('i'), TS('('), NS('F'), TS(')'), TS('{'), NS('N'), TS('}'), NS('S')),
			Rule::Chain(10, TS('n'), TS('f'), TS('i'), TS('('), NS('F'), TS(')'), TS('{'), NS('N'), TS('}'), NS('S')),
			Rule::Chain(9, TS('q'), TS('f'), TS('i'), TS('('), TS(')'), TS('{'), NS('N'), TS('}'), NS('S')),
			Rule::Chain(9, TS('n'), TS('f'), TS('i'), TS('('), TS(')'), TS('{'), NS('N'), TS('}'), NS('S')),
			Rule::Chain(4, TS('s'), TS('{'), NS('N'), TS('}'))
		),
		Rule(NS('F'), GRB_ERROR_SERIES + 1, 4, //формальные параметры ф-ии
			Rule::Chain(4, TS('q'), TS('i'), TS(','), NS('F')),
			Rule::Chain(2, TS('q'), TS('i')),
			Rule::Chain(4, TS('n'), TS('i'), TS(','), NS('F')),
			Rule::Chain(2, TS('n'), TS('i'))
		),
		Rule(NS('N'), GRB_ERROR_SERIES + 2, 12,  // возможные конструкции в ф-иях
			Rule::Chain(5, TS('c'), TS('n'), TS('i'), TS(';'), NS('N')),
			Rule::Chain(5, TS('c'), TS('q'), TS('i'), TS(';'), NS('N')),
			Rule::Chain(5, TS('i'), TS('='), NS('E'), TS(';'), NS('N')),
			Rule::Chain(3, TS('z'), TS(';'), NS('N')),
			Rule::Chain(4, TS('o'), NS('E'), TS(';'), NS('N')),

			Rule::Chain(3, TS('r'), TS('i'), TS(';')),
			Rule::Chain(3, TS('r'), TS('l'), TS(';')),
			Rule::Chain(4, TS('c'), TS('n'), TS('i'), TS(';')),
			Rule::Chain(4, TS('c'), TS('q'), TS('i'), TS(';')),
			Rule::Chain(4, TS('i'), TS('='), NS('E'), TS(';')),
			Rule::Chain(2, TS('z'), TS(';')),

			Rule::Chain(3, TS('o'), NS('E'), TS(';'))
		),
		Rule(NS('E'), GRB_ERROR_SERIES + 3, 14, //описыв выражения
			Rule::Chain(1, TS('i')),
			Rule::Chain(2, TS('i'), NS('E')),
			Rule::Chain(1, TS('l')),
			Rule::Chain(2, TS('i'), NS('M')),
			Rule::Chain(2, TS('l'), NS('M')),

			Rule::Chain(3, TS('('), NS('W'), TS(')')),
			Rule::Chain(4, TS('('), NS('W'), TS(')'), NS('M')),
			Rule::Chain(4, TS('('), NS('E'), TS(')'), NS('M')),
			Rule::Chain(3, TS('i'), TS('('), TS(')')),
			Rule::Chain(4, TS('i'), TS('('), TS(')'), NS('M')),

			Rule::Chain(4, TS('k'), TS('('), TS('W'), TS(')')),
			Rule::Chain(5, TS('k'), TS('('), TS('W'), TS(')'), NS('M')),
			Rule::Chain(4, TS('b'), TS('('), NS('W'), TS(')')),
			Rule::Chain(5, TS('b'), TS('('), NS('W'), TS(')'), NS('M'))
		),
		Rule(NS('M'), GRB_ERROR_SERIES + 5, 4, // опис арифметич знаки
			Rule::Chain(2, TS('+'), NS('E')),
			Rule::Chain(2, TS('-'), NS('E')),
			Rule::Chain(2, TS('*'), NS('E')),
			Rule::Chain(2, TS('/'), NS('E'))
		),
		Rule(NS('W'), GRB_ERROR_SERIES + 4, 4, //фактически принимаемые параметры ф-ии
			Rule::Chain(1, TS('i')),
			Rule::Chain(1, TS('l')),
			Rule::Chain(3, TS('i'), TS(','), NS('W')),
			Rule::Chain(3, TS('l'), TS(','), NS('W'))
		)
	);

	Rule::Chain::Chain(short psize, GRBALPHABET s, ...)  //конструктор цепочки - правой части правила(кол-во символов в цепочке, терминал или нетерминал...)
	{
		nt = new GRBALPHABET[size = psize];    //цепочка терминалов
		int*p = (int*)&s;                      //присваиваем символ указателю п
		for (short i = 0; i < psize; ++i)
			nt[i] = (GRBALPHABET)p[i]; //заполняем цепочку терминалов
	};

	Rule::Rule(GRBALPHABET pnn, int piderror, short psize, Chain c, ...) //конструктор правила
	{															//(нетерминал, идентификатор диагностического сообщения, количество цепочек(правых частей правила), множество цепочек (правых частей правила)
		nn = pnn;    //нетерминал
		iderror = piderror; //идентификатор
		chains = new Chain[size = psize]; //место для цепочки
		Chain*p = &c;
		for (int i = 0; i < size; i++)
			chains[i] = p[i]; //заполняем множество цепочек
	};

	Greibach::Greibach(GRBALPHABET pstartN, GRBALPHABET pstbottom, short psize, Rule r, ...)//конструктор гграматики Грейбаха(стартовый символ, дно стека, количество правил, правила...)
	{
		startN = pstartN; //стартовый символ
		stbottomT = pstbottom;//дно стека
		rules = new Rule[size = psize];//выделяем память
		Rule*p = &r;
		for (int i = 0; i < size; i++) rules[i] = p[i];//заполняем правила
	};
	Greibach getGreibach() { return greibach; }; //получить грамматику

	short Greibach::getRule(GRBALPHABET pnn, Rule& prule) //получить правило (левый символ правила, возвращаемое правило грамматики)
	{
		short rc = -1;
		short k = 0;
		while (k < size&&rules[k].nn != pnn)
			k++;   //пока К меньше количества правил и пока левый символ правила не равен парметру ф-ции
		if (k < size)
			prule = rules[rc = k];    //возвращаемое правило граматики равно правилу с индексом К
		return rc; //возвращается номер правила или -1
	};

	Rule Greibach::getRule(short n) //получить правило по номеру
	{
		Rule rc;      //создаём правило рц
		if (n < size)rc = rules[n];  //присваиваем правилу рц правило н
		return rc; //возвращаем созданное правило
	};

	char*Rule::getCRule(char*b, short nchain) //получить правило в виде N->цепочка (буфер, номер цепочки(правой части) в правиле)
	{
		char bchain[200]; //строка
		b[0] = Chain::alphabet_to_char(nn); b[1] = '-'; b[2] = '>'; b[3] = 0x00; //терминал -> 
		chains[nchain].getCChain(bchain); //получает правую сторонц правила
		strcat_s(b, sizeof(bchain) + 5, bchain);//добавляем строку (куда, с какого элемента, строку)
		return b;
	};

	short Rule::getNextChain(GRBALPHABET t, Rule::Chain& pchain, short j) //получить следующую за j подходящую цепочку, вернуть её номер или -1 
	{                                                                //(первый символ цепочки, возвращаемая цепочка, номер цепочки)
		short rc = -1;
		while (j < size&&chains[j].nt[0] != t)++j;
		rc = (j < size ? j : -1);
		if (rc >= 0)pchain = chains[rc];
		return rc;
	};

	char*Rule::Chain::getCChain(char*b) //получить правую сторону правила
	{
		for (int i = 0; i < size; i++)b[i] = Chain::alphabet_to_char(nt[i]);
		b[size] = 0x00;
		return b;
	};
};