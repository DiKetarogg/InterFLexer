#pragma once
#ifndef IFLEX_LEXER_HPP
#define IFLEX_LEXER_HPP
#include <fstream>
#include <locale>
#include "String.hpp"
#define IFLEX_LEXER_MAKEFUNC_READRAW(input, expr)\
void Read(input/*, std::locale locale = Traits::locale*/) {\
	for (;;) {\
		while (expr) {\
			++m_End;\
			if(*m_End == GetLineFeed<CharType>::get) {\
				m_CharEnd = 0;\
				++m_LineEnd;\
			}\
			++m_CharEnd;\
		}\
		if (!*m_End && stream.is_open()) {\
			if (m_Start != m_End)\
			m_TempHolder.emplace_back(m_Start,\
			reinterpret_cast<size_t>(m_End) - reinterpret_cast<size_t>(m_Start) + 1);\
			ReadFile();\
			continue;\
		}\
		break;\
	}\
}
#define IFLEX_LEXER_MAKEFUNC_READNOTRAW(input, expr)\
void ReadNot(input/*, std::locale locale = Traits::locale*/) {\
	for (;;) {\
		while (expr) {\
			++m_End;\
			if(*m_End == GetLineFeed<CharType>::get) {\
				m_CharEnd = 0;\
				++m_LineEnd;\
			}\
			++m_CharEnd;\
		}\
		if (!*m_End && stream.is_open()) {\
			m_TempHolder.emplace_back(m_Start,\
			reinterpret_cast<size_t>(m_End) - reinterpret_cast<size_t>(m_Start) + 1);\
			ReadFile();\
			continue;\
		}\
		break;\
	}\
}
#define IFLEX_LEXER_MAKEFUNC_READ(enumname, func)\
IFLEX_LEXER_MAKEFUNC_READRAW(decltype(enumname), func(*m_End, locale))
#define IFLEX_LEXER_MAKEFUNC_READNOT(enumname, func)\
IFLEX_LEXER_MAKEFUNC_READNOTRAW(decltype(enumname), *m_End && !func(*m_End, locale))

#define IFLEX_LEXER_MAKEFUNC_READBOTH(enumname, func)\
IFLEX_LEXER_MAKEFUNC_READ(enumname, func)\
IFLEX_LEXER_MAKEFUNC_READNOT(enumname, func)



#define IFLEX_LEXER_MAKEFUNC_SKIPRAW(input, expr)\
void Skip(input/*, std::locale locale = Traits::locale*/) {\
	if (m_Start != m_End)\
		m_TempHolder.emplace_back(m_Start,\
		reinterpret_cast<size_t>(m_End) - reinterpret_cast<size_t>(m_Start) + 1);\
	for (;;) {\
		while (expr) {\
			++m_End;\
			if(*m_End == GetLineFeed<CharType>::get) {\
				++m_CharEnd = 0;\
				++m_LineEnd;\
			}\
			++m_CharEnd;\
		}\
		if (!*m_End && stream.is_open()) {\
			ReadFile();\
			continue;\
		}\
		break;\
	}\
	m_Start = m_End;\
}

#define IFLEX_LEXER_MAKEFUNC_SKIP(enumname, func)\
IFLEX_LEXER_MAKEFUNC_SKIPRAW(decltype(enumname), func(*m_End, locale))
#define IFLEX_LEXER_MAKEFUNC_SKIPNOT(enumname, func)\
IFLEX_LEXER_MAKEFUNC_SKIPRAW(decltype(enumname), *m_End && !func(*m_End, locale))

#define IFLEX_LEXER_MAKEFUNC_SKIPBOTH(enumname, func)\
IFLEX_LEXER_MAKEFUNC_SKIP(enumname, func)\
IFLEX_LEXER_MAKEFUNC_SKIPNOT(enumname, func)


namespace iflex {

	template<class Traits>
	class Lexer {
		public:
		using CharType = typename Traits::CharType;
		using Tokenizer = typename Traits::Tokenizer;
		using Enum	= typename Traits::TokenEnum;
		using Parser 	= typename Traits::Parser;
		protected:
		struct Is {
			Is() = delete;
			Is(const Is&) = delete;
			Is(Is&&) = delete;
			enum{S, SPACE, WHITESPACE, WHITE_SPACE};
			enum{P, PRINTABLE, PRINT, PRNT};
			enum{C, CONTROL, CTRL, CNTRL};
			enum{U, UPPER, UP, UPR, UPP, UPPR};
			enum{L, LOWER, LOW, LWR, LOWR};
			enum{A, ALPHA, ALPH, AL};
			enum{D, DIGIT, DGT};
			enum{PUNCTUATION, PUNCT, PNCT, PUNC};
			enum{H, HEX, HEXDIGIT, HEXDGT, HDIGIT, HDGT, HX,
				HXDIGIT, HXDGT, X, XDIGIT, XDGT, HEXD, HXD, XD};
			enum{ANUM, ALPHANUM, ALPHNUM, ALNUM, ANU, ALPHANU, ALPHNU, ALNU,
				AN, ALPHAN, ALPHN, ALN, ANUMERIC, ALPHANUMERIC, ALPHNUMERIC,
				ALNUMERIC, ANMRC, ALPHANMRC,ALPHNMRC, ALNMRC};
			enum{G, GRAPH, GRPAPHICAL, GRPH, GRPHCL, GRPHC};
			enum{B, BLANK, BLNK};
			enum{ALL, ANY};
		};
		private:
		inline auto Close() {
			return stream.close();
		}

		inline auto Open(const wchar_t* name, std::ios::openmode flags = std::ios::in) {
			return stream.open(name, flags);
		}

		inline auto Open(const char* name, std::ios::openmode flags = std::ios::in) {
			return stream.open(name, flags);
		}

		protected:
		Lexer(size_t bufferSize, Parser& parser, Tokenizer& tokenizer)
		:m_Parser(parser), m_Tokenizer(tokenizer),
		m_Buffer(0), m_BufferSize(bufferSize) {
			stream.imbue(std::locale(Traits::locale));
		}
		Lexer(const Lexer&) = delete;
		Lexer(Lexer&&) = delete;
		//READ

		IFLEX_LEXER_MAKEFUNC_READBOTH(Is::SPACE, std::isspace)//\t\ \n\v\r
		IFLEX_LEXER_MAKEFUNC_READBOTH(Is::PRINT, std::isprint)//not control
		IFLEX_LEXER_MAKEFUNC_READBOTH(Is::CTRL, std::iscntrl)//codes 0-31 + 0x7f
		IFLEX_LEXER_MAKEFUNC_READBOTH(Is::UPPER, std::isupper)//uppercase letters
		IFLEX_LEXER_MAKEFUNC_READBOTH(Is::LOWER, std::islower)//lowercase letters
		IFLEX_LEXER_MAKEFUNC_READBOTH(Is::ALPHA, std::isalpha)//letters
		IFLEX_LEXER_MAKEFUNC_READBOTH(Is::DIGIT, std::isdigit)//0-9
		IFLEX_LEXER_MAKEFUNC_READBOTH(Is::PUNCT, std::ispunct)//graph - alnum
		IFLEX_LEXER_MAKEFUNC_READBOTH(Is::XDIGIT, std::isxdigit)// 0x124
		IFLEX_LEXER_MAKEFUNC_READBOTH(Is::ALNUM, std::isalnum)// letters + 0-9
		IFLEX_LEXER_MAKEFUNC_READBOTH(Is::GRAPH, std::isgraph)// isprint - '\ '
		IFLEX_LEXER_MAKEFUNC_READBOTH(Is::BLANK, std::isblank)//\ and \t
		IFLEX_LEXER_MAKEFUNC_READRAW(decltype(Is::ALL), *m_End)//any character


		IFLEX_LEXER_MAKEFUNC_SKIPBOTH(Is::SPACE, std::isspace)//\t\ \n\v\r
		IFLEX_LEXER_MAKEFUNC_SKIPBOTH(Is::PRINT, std::isprint)//not control
		IFLEX_LEXER_MAKEFUNC_SKIPBOTH(Is::CTRL, std::iscntrl)//codes 0-31 + 0x7f
		IFLEX_LEXER_MAKEFUNC_SKIPBOTH(Is::UPPER, std::isupper)//uppercase letters
		IFLEX_LEXER_MAKEFUNC_SKIPBOTH(Is::LOWER, std::islower)//lowercase letters
		IFLEX_LEXER_MAKEFUNC_SKIPBOTH(Is::ALPHA, std::isalpha)//letters
		IFLEX_LEXER_MAKEFUNC_SKIPBOTH(Is::DIGIT, std::isdigit)//0-9
		IFLEX_LEXER_MAKEFUNC_SKIPBOTH(Is::PUNCT, std::ispunct)//graph - alnum
		IFLEX_LEXER_MAKEFUNC_SKIPBOTH(Is::XDIGIT, std::isxdigit)// 0x1FAC
		IFLEX_LEXER_MAKEFUNC_SKIPBOTH(Is::ALNUM, std::isalnum)// letters + 0-9
		IFLEX_LEXER_MAKEFUNC_SKIPBOTH(Is::GRAPH, std::isgraph)//isprint - '\ '
		IFLEX_LEXER_MAKEFUNC_SKIPBOTH(Is::BLANK, std::isblank)// \ + \t
		IFLEX_LEXER_MAKEFUNC_SKIPRAW(decltype(Is::ALL), *m_End)//any character


		void ReadFile() {
			stream.get(m_Buffer, m_BufferSize);
		}
		//Read
		//Save Read token
		bool Flush() {
			CharType temp = *m_End;
			*m_End = 0;
			m_Parser.push(m_Tokenizer(m_Start, m_LineStart, m_CharStart));
			*m_End = temp;
			return false;
		}
		bool FileOpen(const CharType* name) {
			if (stream.is_open())
				Close();
			stream.open(name);
			if (!stream.is_open()) {
				return false;
			}
			if (!HeapBuffer) {
				m_Buffer = new CharType[m_BufferSize];
				m_Buffer[m_BufferSize - 1] = 0;
				HeapBuffer = true;
			}
			m_Start	= m_Buffer;
			m_End	= m_Buffer;
			return true;
		}
		
		void StringOpen(const CharType* str) {
			if (stream.is_open()) {
				Close();
				stream.clear(std::ios::failbit);
				if (HeapBuffer)
					delete[] m_Buffer;
				HeapBuffer = false;
			}
			m_Buffer = str;
		}
		bool IsFileOpen() {
			return stream.is_open();
		}

		inline bool Parsing() const {
			return (stream.good() || *m_End);
		}
		public:
		~Lexer() {
			if (HeapBuffer)
				delete[] m_Buffer;
		}
		private:
		std::basic_ifstream<CharType> stream;
		//std::basic_string<CharType> m_Temp;
		std::vector<dtg::BasicSimpleString<CharType>> m_TempHolder;
		Parser& m_Parser;
		Tokenizer& m_Tokenizer;
		CharType* m_Buffer;
		CharType* m_Start;
		CharType* m_End;
		size_t m_BufferSize;
		size_t m_LineStart;
		size_t m_LineEnd;
		size_t m_CharStart;
		size_t m_CharEnd;
		bool HeapBuffer;
		static const std::locale locale; 
	};//class Lexer
	template<class Traits>
	const std::locale Lexer<Traits>::locale = std::locale(Traits::locale);


}// namespace iflex


#endif
