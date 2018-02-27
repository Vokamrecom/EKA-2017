#pragma once
#include "stdafx.h"
#define MAX_STRING 255
#define IN_MAX_LEN_TEXT 1024*1024
#define IN_MAX_STRING_CODE 1024
#define IN_CODE_ENDL '\n'
#define IN_CODE_SPACE ' '
#define IN_CODE_NULL '\0'

/*
SEP - ( ) * + - = , / { } ;
PR - пробел и TAB
NS - новая строка
F - запрещённый
T - разрешённый
I - игнорируемый
L - '
*/

#define IN_CODE_TABLE {\
	IN::F,  IN::F, IN::F, IN::F, IN::F, IN::F, IN::F, IN::F, IN::F,   IN::PR,  IN::NS,   IN::F,   IN::F,   IN::I,   IN::F, IN::F,\
	IN::F,  IN::F, IN::F, IN::F, IN::F, IN::F, IN::F, IN::F, IN::F,   IN::F,   IN::F,    IN::F,   IN::F,   IN::F,   IN::F, IN::F,\
	IN::PR, IN::F, IN::F, IN::F, IN::F, IN::F, IN::F, IN::L, IN::SEP, IN::SEP, IN::SEP,  IN::SEP, IN::SEP, IN::SEP, IN::T, IN::SEP,\
	IN::T,  IN::T, IN::T, IN::T, IN::T, IN::T, IN::T, IN::T, IN::T,   IN::T,   IN::F,    IN::SEP, IN::F,   IN::SEP, IN::F, IN::F,\
	IN::T,  IN::T, IN::T, IN::T, IN::T, IN::T, IN::T, IN::T, IN::T,   IN::T,   IN::T,    IN::T,   IN::T,   IN::T,   IN::T, IN::T,\
	IN::T,  IN::T, IN::T, IN::T, IN::T, IN::T, IN::T, IN::T, IN::T,   IN::T,   IN::T,    IN::F,   IN::F,   IN::F,   IN::F, IN::F,\
	IN::F,  IN::T, IN::T, IN::T, IN::T, IN::T, IN::T, IN::T, IN::T,   IN::T,   IN::T,    IN::T,   IN::T,   IN::T,   IN::T, IN::T,\
	IN::T,  IN::T, IN::T, IN::T, IN::T, IN::T, IN::T, IN::T, IN::T,   IN::T,   IN::T,    IN::SEP, IN::F,   IN::SEP, IN::F, IN::F,\
	\
	IN::F, IN::F, IN::F, IN::F, IN::F, IN::F, IN::F, IN::F, IN::F, IN::F, IN::F, IN::F, IN::F, IN::F, IN::F, IN::F,\
	IN::F, IN::F, IN::F, IN::F, IN::F, IN::F, IN::F, IN::F, IN::F, IN::F, IN::F, IN::F, IN::F, IN::F, IN::F, IN::F,\
	IN::F, IN::F, IN::F, IN::F, IN::F, IN::F, IN::F, IN::F, IN::F, IN::F, IN::F, IN::F, IN::F, IN::F, IN::F, IN::F,\
	IN::F, IN::F, IN::F, IN::F, IN::F, IN::F, IN::F, IN::F, IN::F, IN::F, IN::F, IN::F, IN::F, IN::F, IN::F, IN::F,\
	IN::F, IN::F, IN::F, IN::F, IN::F, IN::F, IN::F, IN::F, IN::F, IN::F, IN::F, IN::F, IN::F, IN::F, IN::F, IN::F,\
	IN::F, IN::F, IN::F, IN::F, IN::F, IN::F, IN::F, IN::F, IN::F, IN::F, IN::F, IN::F, IN::F, IN::F, IN::F, IN::F,\
	IN::F, IN::F, IN::F, IN::F, IN::F, IN::F, IN::F, IN::F, IN::F, IN::F, IN::F, IN::F, IN::F, IN::F, IN::F, IN::F,\
	IN::F, IN::F, IN::F, IN::F, IN::F, IN::F, IN::F, IN::F, IN::F, IN::F, IN::F, IN::F, IN::F, IN::F, IN::F, IN::F\
}  

namespace In
{
	struct StToken
	{
		static int tokensSize;  // Количество фраз
		int  line; // строка в которой находится фраза
		char token[1024];

	};

	struct IN
	{
		enum { T = 1, F, I, NS, SEP, L, PR };
		int code[256] = IN_CODE_TABLE;
		int size = NULL;
		int lines = NULL;
		int ignor = NULL;
		bool flag = 0;
		unsigned char *text;
		StToken *tokens;
	};

	IN getin(std::ofstream *stream, wchar_t Infile[]);
	StToken* parsOfToken(StToken *tokens, unsigned char *text, int *code);
	void addToken(StToken *tokens, char* &buffer, int &posbuf, int line);
}