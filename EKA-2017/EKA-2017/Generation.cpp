#include "stdafx.h"
#include "Generation.h"

ofstream fout("Gen.html");
void Head()
{
	fout << "<html>" << endl;
	fout << "<head>" << endl;
	fout << "<title>" << endl;
	fout << "EKA-2017" << endl;
	fout << "</title" << endl;
	fout << "<body>" << endl;
	fout << "<script>" << endl << endl;
}
void Down()
{
	fout << "</script>" << endl;
	fout << "</body>" << endl;
	fout << "</head>" << endl;
	fout << "</html>" << endl;
	fout.close();
}



void Generation(LT::LexTable &Lextable, In::StToken *tokens)
{
	Head();
	for (int i = 0; i < Lextable.size; i++)
	{
		switch (Lextable.table[i].lexema)
		{
		case LEX_PLUS:
		case LEX_STAR:
		case LEX_MINUS:
		case LEX_DIRSLASH:
		case  LEX_EQUAL:
		{
			fout << " " << Lextable.table[i].lexema << " ";
			break;
		}
		case LEX_COMMA:
		case SPACE:
		{
			fout << Lextable.table[i].lexema << " ";
			break;
		}

		case LEX_RIGHTTHESIS:
		case LEX_LEFTHESIS:
		{
			fout << Lextable.table[i].lexema;
			break;
		}
		case LEX_LEFTBRACE:
		{

			fout << endl << Lextable.table[i].lexema << endl;
			break;
		}
		case LEX_BRACELET:
		{

			fout << Lextable.table[i].lexema << endl << endl;
			break;
		}
		case LEX_SEPARATOR:
		{
			fout << Lextable.table[i].lexema << endl;
			break;
		}
		case  LEX_CREATE:
		{
			fout << "var ";
			break;
		}
		case LEX_LITERAL:
		case LEX_ID:
		{
			fout << tokens[i].token;
			break;
		}
		case LEX_OUT:
		{
			fout << "document.write" << endl;
			break;
		}
		case tochka:
		{
			fout << endl;
			break;
		}
		case LEX_FUNCTION:
		{
			fout << "function ";
			break;
		}
		case LEX_RETURN:
		{
			fout << "return ";
			break;
		}

		case LEX_KOREN:
		{
			fout << "Math.sqrt";
			break;
		}
		case LEX_STEPEN:
		{
			fout << "Math.pow (" << tokens[i + 2].token << ",2)";
			i = i + 3;
			break;
		}
		}
	}
	Down();

}
