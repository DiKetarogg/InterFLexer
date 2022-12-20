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
#include <unordered_set>

//#define IFLEX_LEXER_MAKEFUNC_SKIPRAW(input, expr)

namespace iflex {

	template<class Traits>
	class Lexer {
		public:
		using CharType = typename Traits::CharType;
		using Tokenizer = typename Traits::Tokenizer;
		using Enum	= typename Traits::TokenEnum;
		using Token	= typename Traits::Token;
		using TokenList = typename Traits::TokenList;
		using TokenLists= typename Traits::TokenLists;

		using const_token_list_iterator	= typename TokenLists::const_iterator;
		using token_list_iterator	= typename TokenLists::iterator;

		using const_tokens_iterator	= typename TokenList::const_iterator;
		using tokens_iterator		= typename TokenList::iterator;

		using String = typename dtg::BasicSimpleString<CharType>;

		private:
		inline void Close() {
			if (m_Fd > 0)
				close(m_Fd);
			m_Fd = -1;
		}
/*//
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
			return (m_Fd = open(name, flags)) > 0;
		}

		protected:

		Lexer(size_t bufferSize, const Tokenizer& tokenizer)
		:m_Tokenizer(tokenizer)
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
			while (*m_End && TestValue<CharType, Expression...>(*m_End, locale)) {
				if(*m_End == GetLineFeed<CharType>::get) {
					m_CharEnd = 0;
					++m_LineEnd;
				}
				++m_End;
				++m_CharEnd;
			}
			if (!*m_End && m_Fd != -1) {
				if (m_Start != m_End)
					m_TempHolder.emplace_back(m_Start,
						reinterpret_cast<size_t>(m_End)
						- reinterpret_cast<size_t>(m_Start) + 1);
				ReadFile();
				continue;
			}
			break;
			} //for(;;)
		}

		template<class... Expression>
		void ReadOne() {
			for (;;) {
			if (*m_End && TestValue<CharType, Expression...>(*m_End, locale)) {
				if(*m_End == GetLineFeed<CharType>::get) {
					m_CharEnd = 0;
					++m_LineEnd;
				}
				++m_End;
				++m_CharEnd;
			}
			else break;
			if (!*m_End && m_Fd != -1) {
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
				while (*m_End && TestValue<CharType, Expression...>(*m_End)) {
					if(*m_End == GetLineFeed<CharType>::get) {
						m_CharEnd = 0;
						++m_LineEnd;
					}
					else
						++m_CharEnd;
					++m_End;
				}
				if (!*m_End && m_Fd != -1) {
					ReadFile();
					continue;
				}
				break;
			}
			m_CharStart = m_CharEnd;
			m_LineStart = m_LineEnd;
			m_Start = m_End;
		}

		template<class... Expression>
		void SkipOne() {
			/*
			if (m_Start != m_End)
				m_TempHolder.emplace_back(m_Start,
					reinterpret_cast<size_t>(m_End) -
					reinterpret_cast<size_t>(m_Start) + 1);
			*/
			for (;;) {
				if (*m_End && TestValue<CharType, Expression...>(*m_End)) {
					if(*m_End == GetLineFeed<CharType>::get) {
						m_CharEnd = 0;
						++m_LineEnd;
					}
					else
						++m_CharEnd;
					++m_End;
				}
				if (!*m_End && m_Fd != -1) {
					ReadFile();
					continue;
				}
				break;
			}
			m_CharStart = m_CharEnd;
			m_LineStart = m_LineEnd;
			m_Start = m_End;
		}

		protected:
		CharType* GetTokenStart() {
			return m_Start;
		}

		CharType* GetTokenEnd() {
			return m_End;
		}

		size_t GetLineStart() const {
			return m_LineStart;
		}

		size_t GetLineEnd() const {
			return m_LineEnd;
		}

		size_t GetCharStart() const {
			return m_CharStart;
		}

		size_t GetCharEnd() const {
			return m_CharEnd;
		}

		void ReadFile() {
			int r = read(m_Fd, m_Buffer, m_BufferBytes);
			*reinterpret_cast<CharType *>(
			&reinterpret_cast<char *>(m_Buffer)[r]) = 0;
			if (r < m_BufferBytes) {
				Close();
			}
		}
		//Read
		//Save Read token

		protected:
		bool Flush() {
			const CharType* token_str;
			{
				m_TempHolder.push_back(String::StealingConstructor(
					m_Start, dtg::length(m_Start, m_End)));
				token_str = PushName({m_TempHolder.begin(), m_TempHolder.end()});
				m_TempHolder.back().Lose();
				m_TempHolder.clear();
			}
			Token token = m_Tokenizer(token_str, m_LineStart + 1, m_CharStart + 1);
			PushToken(token);
			m_CharStart = m_CharEnd;
			m_LineStart = m_LineEnd;
			m_Start = m_End;
			return token.GetType() != Traits::NA;
		}

		protected:
		bool ReadEmpty() {
			return m_Start == m_End;
		}
		bool FileOpen(const CharType* name, size_t length) {
			if (m_Fd != -1)
				Close();
			Open(name);
			//stream.open(name);
			if (m_Fd == -1) {
				return false;
			}
			if (!m_HeapBuffer) {
				m_Buffer = new CharType[m_BufferSize];
				m_Buffer[m_BufferSize - 1] = 0;
				m_HeapBuffer = true;
			}
			m_Start	= m_Buffer;
			m_End	= m_Buffer;
			*m_End = 0;
			m_TokensLists.SetActive(name, length);
			return true;
		}

		protected:
		void StringOpen(const CharType* str, size_t size) {
			static uint64_t uid = 0;
			if (m_Fd != -1) {
				Close();
				//stream.clear(std::ios::failbit);
				if (m_HeapBuffer)
					delete[] m_Buffer;
				m_HeapBuffer = false;
			}
			m_Buffer = new CharType[size];
			m_HeapBuffer = true;
			memcpy(m_Buffer, str, size);
			m_Start	= m_Buffer;
			m_End	= m_Buffer;
			m_TokensLists.SetActive("%%_$string_file$_" + std::to_string(++uid));
		}

		bool IsFileOpen() {
			return m_Fd != -1;
		}

		protected:
		inline bool Lexing() const noexcept {
			return (m_Fd != -1 || *m_End);
		}

		inline void TransformToLower() {
			for (CharType* i = m_Start; i != m_End; ++i) {
				*i = std::tolower(*i);
			}
		}

		public:
		const TokenLists& GetTokens() const noexcept{
			return m_TokensLists;
		}

		TokenLists& GetTokens() noexcept {
			return m_TokensLists;
		}

		const std::unordered_set<dtg::BasicSimpleString<CharType>>& GetNames() const noexcept{
			return m_Names;
		}

		std::unordered_set<dtg::BasicSimpleString<CharType>>& GetNames() noexcept {
			return m_Names;
		}

		public:
		~Lexer() {
			if (m_HeapBuffer)
				delete[] m_Buffer;
		}

		private:
		const String& PushName(String&& string) {
			return *(m_Names.emplace(std::forward<String>(string)).first);
		}
		inline void PushToken(Token token) {
			m_TokensLists.Push(token);
		}
		private:
		//std::basic_ifstream<CharType> stream;
		//dtg::BasicSimpleString<CharType> m_Temp;
		std::unordered_set<dtg::BasicSimpleString<CharType>> m_Names;
		std::vector<dtg::BasicSimpleString<CharType>> m_TempHolder;
		TokenLists m_TokensLists;
		const Tokenizer& m_Tokenizer;
		CharType* m_Buffer;
		CharType* m_Start;
		CharType* m_End;
		size_t m_LineStart;
		size_t m_LineEnd;
		size_t m_CharStart;
		size_t m_CharEnd;
		int m_BufferSize;
		int m_BufferBytes;
		int m_Fd = -1;
		bool m_HeapBuffer = 0;
		bool m_ReadingToken = 0;
		static const std::locale locale;
	};//class Lexer
	template<class Traits>
	const std::locale Lexer<Traits>::locale = std::locale(Traits::locale);


}// namespace iflex


#endif
