#pragma once
#ifndef IFLEX_PARSER_HPP
#define IFLEX_PARSER_HPP

#include <dtg/Utilities.hpp>
#include <deque>
#include <set>

namespace iflex {

	template<class Traits>
	class Parser {
		public:
		using Token		= typename Traits::Token;
		using Enum		= typename Traits::TokenEnum;
		using CharType		= typename Traits::CharType;
		struct File {
			File(const char* name):filename(name) {}
			std::deque<Token> tokens;
			dtg::SimpleString filename;
		};
		private:	
		
		using const_iterator	= typename std::deque<File>::const_iterator;
		using iterator		= typename std::deque<File>::iterator;

		using const_token_iterator	= typename std::deque<Token>::const_iterator;
		using token_iterator		= typename std::deque<Token>::iterator;
		
		inline void CreateFileToken(const char*name) {
			m_Active = m_Rawtokens.emplace(m_Active, File(name));
		}
		public:

		Parser():m_Rawtokens(), m_Active(m_Rawtokens.end()){}

		void SetActiveFile(const char* filename) {
			for(auto it = m_Rawtokens.begin();
			it!=m_Rawtokens.end(); ++it) {
				if (it->filename == filename) {
					m_Active = it;
					return;
				}
			}
			CreateFileToken(filename);
		}

		void Push(Token token) {
			m_Active->tokens.push_back(token);
		}

		const CharType* PushName(const CharType* begin, const CharType* end) {
			return *(m_Names.emplace(begin, static_cast<size_t>(end - begin)).first);
		}

		const_iterator GetTokensFile(const char* filename) {
			for (const auto& e : m_Rawtokens) {
				//e.filename is not a const char*
				if (e.filename == filename)
					return e;
			}
			return m_Rawtokens.end();
		}
		
		const char* GetActiveFilename() {
			return m_Active->filename;
		}

		const auto GetNames() const {
			return m_Names;
		}

		const_iterator begin() const {
			return m_Rawtokens.begin();
		}

		const_iterator end() const {
			return m_Rawtokens.end();
		}

		const_iterator cbegin() const {
			return m_Rawtokens.cbegin();
		}

		const_iterator cend() const {
			return m_Rawtokens.cend();
		}

#if defined(_GLIBCXX_OSTREAM) or defined(_OSTREAM_)
		friend std::ostream& operator << (std::ostream& stream,
		const typename iflex::Parser<Traits>::File& tokens) {
			stream << tokens.filename << '\n';
			for (const auto& e : tokens.tokens)
				stream << e << '\n';
			return stream;
		}
#endif
		protected:
			bool ResetActiveToken() {
				m_Token = m_Active->tokens.begin();
				return m_Token != m_Active->tokens.begin();
			}
			Token& GetToken() {
				return *m_Token;
			}
			bool Advance() {
				if (m_Token + 1 == m_Active->tokens.end())
					return false;
				++m_Token;
				return true;
			}
			bool Back() {
				if (m_Token - 1 == m_Active->tokens.end())
					return false;
				--m_Token;
				return true;
			}
		private:
		std::set<dtg::BasicSimpleString<CharType>> m_Names;
		std::deque<File> m_Rawtokens;
		iterator m_Active;
		token_iterator m_Token;
	};
}
/*
   template<class Traits>
std::ostream& operator << (std::ostream& stream,
const typename iflex::Parser<Traits>::File& tokens) {
	stream << tokens.filename << '\n';
	for (const auto& e : tokens.tokens)
		stream << e;
	return stream;
}
*/


#endif
