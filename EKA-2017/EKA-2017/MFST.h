#include <iostream>
#include <stack> 
#include "GRB.h"
#include "LexAnalize.h"
#include "Log.h"
//синт анализатор, представлена Спецификация структуры Mfst для моделирования магазинного конченого автомата 

#define MFST_DIAGN_MAXSIZE 2*ERROR_MAXSIZE_MESSAGE
#define MFST_DIAGN_NUMBER 3

typedef std::stack<short> MFSTSTSTACK;	//стек автомата

namespace MFST
{
//Структура: для сохранения состояния автомата; сохранять состояние
	//автомата необходимо для того, чтобы иметь возможность вернуться
	//	к этому состоянию и осуществить альтернативный  вариант 
	//	синтаксического разбора(в силу недетерминированности автомата).
	struct MfstState				//состояние автомата(для сохранения
	{
		short lenta_position;		//позиция на ленте
		short nrule;				//номер текущего правила
		short nrulechain;			//номер текущей цепочки
		MFSTSTSTACK st;				//стек автомата
		MfstState();
		MfstState(
			short pposition,		//позиция на ленте
			MFSTSTSTACK pst,		//стек автомата
			short pnrulechain		//номер текущей цепочки, текущего правила
		);
		MfstState(
			short pposition,		//позиция на ленте
			MFSTSTSTACK pst,		//стек автомата
			short pnrule,			//номер текущего правила
			short pnrulechain		//номер текущей цепочки, текущего правила
		);
	};
	//моделирование магазин конечного автомата
	struct Mfst					//магазинный автомат	
	{
		enum RC_STEPEN			//код возврата функции stepen
		{
			NS_OK,				//найдено правило и цепочка, цепочка записана в стек
			NS_NORULE,			//не найдено правило грамматики (ошибки в грамматике)
			NS_NORULECHAIN,		//не найдена подходящая цепочка правила (ошибка в исходном коде)
			NS_ERROR,			//неизвестный нетерминальный символ грамматики
			TS_OK,				//текущий символ ленты == вершине стека, продвинулась лента, рор стека
			TS_NOK,				//текущий символ ленты != вершине стека, восстановлено состояние
			LENTA_END,			//текущая позиция ленты >= lenta_size
			SURPRISE			//неожиданный код возврата ( ошибка в stepen)
		};
		struct MfstDiagnosis		//диагностика
		{
			short lenta_position;	//позиция на ленте
			RC_STEPEN rc_stepen;		//код завершения шага
			short nrule;			//номер правила
			short nrule_chain;		//номер цепочки правила
			MfstDiagnosis();
			MfstDiagnosis(			//диагностика
				short plenta_position,	//позиция на ленте
				RC_STEPEN prc_stepen,		//код завершения шага
				short pnrule,			//номер правила
				short pnrule_chain		//номер цепочки правила
			);
		}
		diagnosis[MFST_DIAGN_NUMBER];	//последние самые глубокие сообщения
		GRBALPHABET* lenta;				//перекодированная(TS/NS) лента (из LEX)
		short lenta_position;			//текущая позиция на ленте
		short nrule;					//номер текущего правила
		short nrulechain;				//номер текущей цепочки, текущего правила
		short lenta_size;				//размер ленты
		GRB::Greibach grebach;			//грамматика Грейбах
		Lexer::LEX lex;					//результат работы лексического анализатора
		MFSTSTSTACK st;					//стек автомата		
		std::stack<MfstState> storestate;	//стек для сохранения состояний
		Mfst();
		Mfst(
			Lexer::LEX plex,			//результат работы лексического анализатора
			GRB::Greibach pgrebach		//грамматика Грейбах
		);
		char* getCSt(char*buf);			//получить содержимое стека
		char* getCLenta(char* buf, short pos, short n = 25);//лента: n символов с pos
		char*getDiagnosis(short n, char*buf);			//получить n-ую строку диагностики или 0х00
		bool savestate(const Log::LOG &log);					//сохранить состояние автомата
		bool reststate(const Log::LOG &log);					//восстановить состояние автомата
		bool push_chain(					//поместить уепочку правила в стек
			GRB::Rule::Chain chain				//цепочка правила
		);
		RC_STEPEN stepen(const Log::LOG &log);			//выполнить шаг автомата
		bool start(const Log::LOG &log);			//запустить автомат
		bool savediagnois(
			RC_STEPEN pprc_stepen			//код завершения шага
		);
		void printrules(const Log::LOG &log);		//вывести последовательность правил
		struct Deducation		//вывод
		{
			short size;		//количество шагов в выводе
			short* nrules;	//номера правил нрамматики
			short* nrulechains;//номера цепочек правил грамматики (nrules)
			Deducation() { size = 0; nrules = 0; nrulechains = 0; };
		}deducation;
		bool savededucation();	//сохранить дерево вывода
	};
};
