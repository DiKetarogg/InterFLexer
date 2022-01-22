#pragma once
#ifndef IFLEX_LEXER_HPP
#define IFLEX_LEXER_HPP
extern "C" {
#include <fcntl.h>
#include <unistd.h>
}
#include <locale>
#include <vector>
#include "String.hpp"
#include "CrossPlatform.hpp"
#include "TestValue.hpp"
#include <dtg/Pack.hpp>
#include <dtg/Utilities.hpp>


//#define IFLEX_LEXER_MAKEFUNC_SKIPRAW(input, expr)


namespace iflex {

	template<class Traits>
	class Lexer {
		public:
		using CharType = typename Traits::CharType;
		using Tokenizer = typename Traits::Tokenizer;
		using Enum	= typename Traits::TokenEnum;
		using Parser 	= typename Traits::Parser;
		using Token	= typename Traits::Token;
		protected:

		private:
		inline auto Close() {
			if (fd > 0)
				close(fd);
			fd = -1;
		}
/*
		inline auto Open(const wchar_t* name, std::ios::openmode flags = std::ios::in) {
			return stream.open(name, flags);
		}

		inline auto Open(const char* name, std::ios::openmode flags = std::ios::in) {
			return stream.open(name, flags);
		}
*/
/*
		inline auto Open(const wchar_t* name, int flags = O_RDONLY) {
			return (fd = open(name, flags)) > 0;
		}
*/
		inline auto Open(const char* name, int flags = O_RDONLY) {
			return (fd = open(name, flags)) > 0;
		}

		protected:



		Lexer(size_t bufferSize, Parser& parser, Tokenizer& tokenizer)
		:m_Parser(parser), m_Tokenizer(tokenizer)
		,m_Buffer(0)
		,m_Start(0)
		,m_End(0)
		,m_LineStart(0)
		,m_LineEnd(0)
		,m_CharStart(0)
		,m_CharEnd(0)
		,m_BufferSize(bufferSize)
		,m_BufferBytes((m_BufferSize - 1) * sizeof(CharType)) {
			//stream.imbue(std::locale(Traits::locale));
		}
		Lexer(const Lexer&) = delete;
		Lexer(Lexer&&) = delete;


		//READ
		template<class... Expression>
		void Read() {
			for (;;) {
			while (TestValue<CharType, Expression...>(*m_End, locale)) {
				if(*m_End == GetLineFeed<CharType>::get) {
					m_CharEnd = 0;
					++m_LineEnd;
				}
				++m_End;
				++m_CharEnd;
			}
			if (!*m_End && fd != -1) {
				if (m_Start != m_End)
					m_TempHolder.emplace_back(m_Start,
						reinterpret_cast<size_t>(m_End) -
						reinterpret_cast<size_t>(m_Start) + 1);
				ReadFile();
				continue;
			}
			break;
			} //for(;;)
		}

		template<class... Expression>
		void ReadOne() {
			for (;;) {
			if (TestValue<CharType, Expression...>(*m_End, locale)) {
				if(*m_End == GetLineFeed<CharType>::get) {
					m_CharEnd = 0;
					++m_LineEnd;
				}
				++m_End;
				++m_CharEnd;
			}
			else break;
			if (!*m_End && fd != -1) {
				if (m_Start != m_End)
					m_TempHolder.emplace_back(m_Start,
							reinterpret_cast<size_t>(m_End) -
							reinterpret_cast<size_t>(m_Start) + 1);
				ReadFile();
				continue;
			}
			break;
			}
		}

		template<class... Expression>
		void Skip() {
			/*
			if (m_Start != m_End)
				m_TempHolder.emplace_back(m_Start,
					reinterpret_cast<size_t>(m_End) -
					reinterpret_cast<size_t>(m_Start) + 1);
			*/
			for (;;) {
				while (TestValue<CharType, Expression...>(*m_End)) {
					if(*m_End == GetLineFeed<CharType>::get) {
						m_CharEnd = 0;
						++m_LineEnd;
					}
					else
						++m_CharEnd;
					++m_End;
				}
				if (!*m_End && fd != -1) {
					ReadFile();
					continue;
				}
				break;
			}
			m_CharStart = m_CharEnd;
			m_LineStart = m_LineEnd;
			m_Start = m_End;
		}
		void ReadFile() {
			int r = read(fd, m_Buffer, m_BufferBytes);
			*reinterpret_cast<CharType *>(
			&reinterpret_cast<char *>(m_Buffer)[r]) = 0;
			if (r < m_BufferBytes) {
				Close();
			}
		}
		//Read
		//Save Read token
		bool Flush() {
			auto token_str = m_Parser.PushName(m_Start, m_End);
			Token token = m_Tokenizer(token_str, m_LineStart + 1, m_CharStart + 1);
			m_Parser.Push(token);
			m_CharStart = m_CharEnd;
			m_LineStart = m_LineEnd;
			m_Start = m_End;
			return token.GetType() != Traits::NA;
		}
		bool TokenEmpty() {
			return m_Start == m_End;
		}
		bool FileOpen(const CharType* name) {
			if (fd != -1)
				Close();
			Open(name);
			//stream.open(name);
			if (fd == -1) {
				return false;
			}
			if (!m_HeadBuffer) {
				m_Buffer = new CharType[m_BufferSize];
				m_Buffer[m_BufferSize - 1] = 0;
				m_HeadBuffer = true;
			}
			m_Start	= m_Buffer;
			m_End	= m_Buffer;
			*m_End = 0;
			m_Parser.SetActiveFile(name);
			return true;
		}

		void StringOpen(const CharType* str, size_t size) {
			static uint64_t uid = 0;
			if (fd != -1) {
				Close();
				//stream.clear(std::ios::failbit);
				if (m_HeadBuffer)
					delete[] m_Buffer;
				m_HeadBuffer = false;
			}
			m_Buffer = new CharType[size];
			m_HeadBuffer = true;
			memcpy(m_Buffer, str, size);
			m_Start	= m_Buffer;
			m_End	= m_Buffer;
			m_Parser.SetActiveFile(("%%_$string_file$_" + std::string(++uid)).c_str());
		}
		bool IsFileOpen() {
			return fd != -1;
		}

		inline bool Lexing() const {
			return (fd != -1 || *m_End);
		}
		public:
		~Lexer() {
			if (m_HeadBuffer)
				delete[] m_Buffer;
		}
		private:
		//std::basic_ifstream<CharType> stream;
		//std::basic_string<CharType> m_Temp;
		std::vector<dtg::BasicSimpleString<CharType>> m_TempHolder;
		Parser& m_Parser;
		Tokenizer& m_Tokenizer;
		CharType* m_Buffer;
		CharType* m_Start;
		CharType* m_End;
		size_t m_LineStart;
		size_t m_LineEnd;
		size_t m_CharStart;
		size_t m_CharEnd;
		int m_BufferSize;
		int m_BufferBytes;
		int fd = -1;
		bool m_HeadBuffer = 0;
		bool m_ReadingToken = 0;
		static const std::locale locale;
	};//class Lexer
	template<class Traits>
	const std::locale Lexer<Traits>::locale = std::locale(Traits::locale);


}// namespace iflex


#endif
