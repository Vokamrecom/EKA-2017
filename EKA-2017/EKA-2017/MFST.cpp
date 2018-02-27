#include "stdafx.h"
#include "GRB.h"
#include "MFST.h"
#include "LexAnalize.h"
#include "Log.h"
//синтакс
//работа конечного автомата
int FST_TRACE_n = -1;

char rbuf[205], sbuf[205], lbuf[1024];

namespace MFST
{
	MFST::MfstState::MfstState() // конструктор по умолчанию
	{
		lenta_position = 0; //позиция на ленте
		nrule = -1; //номер текущего правила
		nrulechain = -1; //номер текущей цепочки
	};
	MfstState::MfstState(short pposition, MFSTSTSTACK pst, short pnrulechain) //конструктор (позиция на ленте, стек автомата, номер текущей цепочки и текущего правила)
	{
		lenta_position = pposition; //позиция на ленте
		st = pst; //стек автомата
		nrulechain = pnrulechain;//номер текущего правила и цепочки
	};
	MfstState::MfstState(short pposition, MFSTSTSTACK pst, short pnrule, short pnrulechain) //конструктор (позиция на ленте, стек автомата,номер текущего правила, номер текущей цепочки)
	{
		lenta_position = pposition;//позиция в ленте
		st = pst;//стек автомата
		nrule = pnrule;//номер текущего правила
		nrulechain = pnrulechain;//номер текущей цепочки
	};
	Mfst::MfstDiagnosis::MfstDiagnosis()//  для  строк диагностики
	{
		lenta_position = -1;
		rc_stepen = SURPRISE;
		nrule = -1;
		nrule_chain = -1;
	};
	Mfst::MfstDiagnosis::MfstDiagnosis(short plenta_position, RC_STEPEN prc_stepen, short pnrule, short pnrule_chain)//диагностика
	{                                                     //(позиция на ленте, код завершения шага, номер правила, номер цепочки правила)
		lenta_position = plenta_position; //позиция на ленте
		rc_stepen = prc_stepen;//код завершения шага
		nrule = pnrule;//номер правила
		nrule_chain = pnrule_chain;//номер цепочки правила
	};
	Mfst::Mfst() { lenta = 0; lenta_size = lenta_position = 0; }; //конструктор по умолчанию 
	Mfst::Mfst(Lexer::LEX plex, GRB::Greibach pgrebach)//(результат работы лексического анализатора, грамматика Грейбах)
	{
		grebach = pgrebach; //Грейбах
		lex = plex; //рез работы лекс анализатора
		lenta = new short[lenta_size = lex.Lextable.size];//размер ленты = текущий размер таблицы лексем
		for (int k = 0; k < lenta_size; k++)lenta[k] = TS(lex.Lextable.table[k].lexema);//заносит в ленту терминалы
		lenta_position = 0;
		st.push(grebach.stbottomT);//добавляет дно стека
		st.push(grebach.startN);//добавляет стартовый символ
		nrulechain = -1;//изначально правило равно -1
	};//пошли методы и функции
	Mfst::RC_STEPEN Mfst::stepen(const Log::LOG &log) //выполнить шаг автомата
	{
		RC_STEPEN rc = SURPRISE; //код возврата = ошибка возврата
		if (lenta_position < lenta_size)
		{
			if (ISNS(st.top())) //извлекаем последний элемент стека и проверяем нетерминал ли он
			{
				GRB::Rule rule;
				if ((nrule = grebach.getRule(st.top(), rule)) >= 0)//смотрим, если такое правило есть идём дальше, если нет то елсе
				{
					GRB::Rule::Chain chain;
					if ((nrulechain = rule.getNextChain(lenta[lenta_position], chain, nrulechain + 1)) >= 0)//получаем следующую цепочку и выводим её номер, илбо возвращаем -1
					{
						MFST_TRACE1(log)//вывод
							savestate(log);//сохранить состояние автомата
						st.pop();//извлекаем верхушку стека.
						push_chain(chain); //поместить цепочку правила в стек
						rc = NS_OK; //найдено правило и цепочка... цепочка записана в стек
						MFST_TRACE2(log)//вывод
					}
					else
					{
						MFST_TRACE4(log, "TNS_NORULECHAIN/NS_NORULE")//вывод
							savediagnois(NS_NORULECHAIN); //код завершения
						rc = reststate(log) ? NS_NORULECHAIN : NS_NORULE; //восстановить состояние автомата
					};
				}
				else rc = NS_ERROR;//неизвестный нетерминальный символ грамматики
			}
			else if ((st.top() == lenta[lenta_position]))//если текущий символ ленты равен вершине стека
			{
				lenta_position++; //продвигаем ленту
				st.pop();//вершина стека
				nrulechain = -1; //номер текущего правила равен -1
				rc = TS_OK;
				MFST_TRACE3(log)
			}
			else { MFST_TRACE4(log, "TS_NOK/NS_NORULECHIN") rc = reststate(log) ? TS_NOK : NS_NORULECHAIN; };
		}
		else { rc = LENTA_END; MFST_TRACE4(log, "LENTA_END") };
		return rc;
	};

	bool Mfst::push_chain(GRB::Rule::Chain chain) //поместить цепочку правила в стек (цепочка правила)
	{
		for (int k = chain.size - 1; k >= 0; k--)st.push(chain.nt[k]); //к = длинне цепочке-1. заносим цепочку в стек
		return true;
	};
	bool Mfst::savestate(const Log::LOG &log) //сохранить состояние автомата
	{
		storestate.push(MfstState(lenta_position, st, nrule, nrulechain)); //стек для сохранения состояния. заносим сохраняемое состояние
		MFST_TRACE6(log, "SAVESTATE:", storestate.size());
		return true;
	};
	bool Mfst::reststate(const Log::LOG &log) //восстановить состояние автомата
	{
		bool rc = false;
		MfstState state;
		if (rc = (storestate.size() > 0))
		{
			state = storestate.top();
			lenta_position = state.lenta_position;
			st = state.st;
			nrule = state.nrule;
			nrulechain = state.nrulechain;
			storestate.pop();
			MFST_TRACE5(log, "RESSTATE")
				MFST_TRACE2(log)
		};
		return rc;
	};
	bool Mfst::savediagnois(RC_STEPEN prc_stepen)
	{
		bool rc = false;
		short k = 0;
		while (k < MFST_DIAGN_NUMBER&&lenta_position <= diagnosis[k].lenta_position)k++;
		if (rc = (k < MFST_DIAGN_NUMBER))
		{
			diagnosis[k] = MfstDiagnosis(lenta_position, prc_stepen, nrule, nrulechain);
			for (short j = k + 1; j < MFST_DIAGN_NUMBER; j++)diagnosis[j].lenta_position = -1;
		};
		return rc;
	};
	bool Mfst::start(const Log::LOG &log)
	{
		bool rc = false;
		RC_STEPEN rc_stepen = SURPRISE;
		char buf[MFST_DIAGN_MAXSIZE];
		rc_stepen = stepen(log);
		while (rc_stepen == NS_OK || rc_stepen == NS_NORULECHAIN || rc_stepen == TS_OK || rc_stepen == TS_NOK)rc_stepen = stepen(log);
		switch (rc_stepen)
		{
		case LENTA_END:         MFST_TRACE4(log, "------>LENTA_END")
			*log.stream << "-------------------------------------------------------------------------------------" << std::endl;
			sprintf_s(buf, MFST_DIAGN_MAXSIZE, "%d:всего строк %d, синтаксический анализ выполнен без ошибок ", 0, lenta_size);
			*log.stream << std::setw(4) << std::left << 0 << ":всего строк " << lenta_size << ", синтаксический анализ выполнен без ошибок " << std::endl;
			rc = true;
			break;
		case NS_NORULE:         MFST_TRACE4(log, "------>NS_NORULE")
			*log.stream << "-------------------------------------------------------------------------------------" << std::endl;
			*log.stream << getDiagnosis(0, buf) << std::endl;
			*log.stream << getDiagnosis(1, buf) << std::endl;
			*log.stream << getDiagnosis(2, buf) << std::endl;
			break;
		case NS_NORULECHAIN:       MFST_TRACE4(log, "------>NS_NORULECHAIN") break;
		case NS_ERROR:             MFST_TRACE4(log, "------>NS_ERROR") break;
		case SURPRISE:             MFST_TRACE4(log, "------>SURPRISE") break;
		};
		return rc;

	};

	char* Mfst::getCSt(char*buf)
	{
		for (int k = (signed)st.size() - 1; k >= 0; --k)
		{
			short p = st._Get_container()[k];
			buf[st.size() - 1 - k] = GRB::Rule::Chain::alphabet_to_char(p);
		};
		buf[st.size()] = 0x00;
		return buf;
	};
	char *Mfst::getCLenta(char*buf, short pos, short n)
	{
		short i, k = (pos + n < lenta_size) ? pos + n : lenta_size;
		for (i = pos; i < k; i++)buf[i - pos] = GRB::Rule::Chain::alphabet_to_char(lenta[i]);
		buf[i - pos] = 0x00;
		return buf;
	};
	char* Mfst::getDiagnosis(short n, char*buf)//формир ошибки синтаксич
	{
		char *rc = "";
		int errid = 0;
		int lpos = -1;
		if (n < MFST_DIAGN_NUMBER && (lpos = diagnosis[n].lenta_position) >= 0)
		{
			errid = grebach.getRule(diagnosis[n].nrule).iderror;
			Error::ERROR err = Error::GetError(errid);
			sprintf_s(buf, MFST_DIAGN_MAXSIZE, "%d: строка %d,%s", err.id, lex.Lextable.table[lpos].sn, err.message);
			rc = buf;
		};
		return rc;
	};
	void Mfst::printrules(const Log::LOG &log)//отладка: вывести правила вывода
	{
		*log.stream << endl;
		*log.stream << endl;
		*log.stream << "-------------------Дерево разбора-------------------------- " << endl;
		MfstState state;
		GRB::Rule rule;
		for (unsigned short k = 0; k < storestate.size(); k++)
		{
			state = storestate._Get_container()[k];
			rule = grebach.getRule(state.nrule);
			MFST_TRACE7(log)
		};
	};
	bool Mfst::savededucation()//сохранить вывести правила вывода
	{
		MfstState state;
		GRB::Rule rule;
		deducation.nrules = new short[deducation.size = storestate.size()];
		deducation.nrulechains = new short[deducation.size];
		for (unsigned short k = 0; k < storestate.size(); k++)
		{
			state = storestate._Get_container()[k];
			deducation.nrules[k] = state.nrule;
			deducation.nrulechains[k] = state.nrulechain;
		};
		return true;
	};
};