#include "stdafx.h"
#include "In.h"
#include "Error.h"
#include "Log.h"

int In::StToken::tokensSize = NULL;

namespace In
{
	IN getin(std::ofstream *stream, wchar_t Infile[])
	{
		std::ifstream txtfile(Infile);

		if (!txtfile.is_open())
			throw ERROR_THROW(110);

		IN StrIn;
		StrIn.text = new unsigned char[IN_MAX_LEN_TEXT];
		bool literalFlag = false;
		int pos = 0;
		unsigned char readSymbol = IN_CODE_NULL;

		while ((readSymbol = txtfile.get()) && (!txtfile.eof()))
		{
			if (StrIn.code[readSymbol] == IN::L)
			{
				StrIn.text[StrIn.size++] = readSymbol;
				readSymbol = txtfile.get();
				int count = 0;
				while (StrIn.code[readSymbol] != IN::L)
				{
					count++; pos++;
					if (StrIn.code[readSymbol] == IN::NS)
					{
						throw ERROR_THROW_IN(109, StrIn.lines, pos);
					}
					if (count == MAX_STRING - 2)
					{
						Log::writeError(stream, Error::GetError(105, StrIn.lines, pos));
						while (StrIn.code[readSymbol] != IN::SEP && StrIn.code[readSymbol] != IN::L)
						{
							readSymbol = txtfile.get();
						}
						StrIn.text[StrIn.size] = readSymbol;
						break;
					}
					StrIn.text[StrIn.size++] = readSymbol;
					readSymbol = txtfile.get();
				}
				StrIn.text[StrIn.size++] = readSymbol;
				continue;
			}

			switch (StrIn.code[readSymbol])
			{
			case IN::NS:
			{
				StrIn.text[StrIn.size++] = readSymbol;
				StrIn.lines++;
				pos = 0;
				break;
			}
			case IN::PR:
			{
				pos++;
				if (StrIn.text[StrIn.size - 1] == IN::PR)
					StrIn.ignor++;
				else
					StrIn.text[StrIn.size++] = readSymbol;
				break;
			}
			case IN::SEP:
			{
				pos++;
				if (StrIn.text[StrIn.size - 1] == IN_CODE_SPACE)
					StrIn.text[StrIn.size - 1] = readSymbol;
				else
					StrIn.text[StrIn.size++] = readSymbol;
				break;
			}
			case IN::F:
			{
				Log::writeError(stream, Error::GetError(111, StrIn.lines, StrIn.size));
				break;
			}
			case IN::I:
			{
				pos++;
				StrIn.ignor++;
				break;
			}
			case IN::T:
			{
				pos++;
				if (StrIn.code[StrIn.text[StrIn.size - 2]] == IN::SEP && StrIn.code[StrIn.text[StrIn.size - 1]] == IN::PR)
					StrIn.text[StrIn.size - 1] = readSymbol;
				else
					StrIn.text[StrIn.size++] = readSymbol;
				break;
			}
			case IN::L:
			{
				pos++;
				StrIn.text[StrIn.size++] = readSymbol;
				break;
			}
			}
		}


		StrIn.text[StrIn.size] = IN_CODE_NULL;

		return StrIn;
	}



	void addToken(StToken *tokens, char* &buf, int &posbuf, int line)
	{
		buf[posbuf++] = IN_CODE_NULL;
		strcpy_s((tokens[StToken::tokensSize].token), posbuf, buf);
		tokens[StToken::tokensSize].line = line;
		StToken::tokensSize++;
		buf[0] = IN_CODE_NULL;
		posbuf = NULL;
	}

	StToken* parsOfToken(StToken *tokens, unsigned char *text, int *code)
	{
		tokens = new StToken[IN_MAX_LEN_TEXT];
		char *buf = new char[IN_MAX_STRING_CODE];
		int posBuf = NULL;
		bool literalFlag = false;
		int line = NULL;

		for (int i = NULL; text[i] != IN_CODE_NULL; i++)
		{
			if (code[text[i]] == IN::L)
			{
				if (buf[NULL] != IN_CODE_NULL)
					addToken(tokens, buf, posBuf, line);

				buf[posBuf++] = text[i++];
				int count = 0;
				while (code[text[i]] != IN::L)
				{
					count++;
					if (count < 253)
						buf[posBuf++] = text[i++];
					else
						i++;
				}

				buf[posBuf++] = text[i];
				addToken(tokens, buf, posBuf, line);
				continue;
			}

			switch (code[text[i]])
			{
			case IN::NS:
				line++;
			case IN::PR:
			{
				if (buf[NULL] != IN_CODE_NULL)
					addToken(tokens, buf, posBuf, line);
				break;
			}
			case IN::SEP:
			{
				if (buf[NULL] != IN_CODE_NULL)
					addToken(tokens, buf, posBuf, line);

				buf[posBuf++] = text[i];
				addToken(tokens, buf, posBuf, line);
				break;
			}
			default:
			{
				buf[posBuf++] = text[i];
				break;
			}
			}
		}
		return tokens;
	}
}
