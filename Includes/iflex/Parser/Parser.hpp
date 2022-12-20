#pragma once
#ifndef IFLEX_PARSER_PARSER_HPP
#define IFLEX_PARSER_PARSER_HPP
#endif

#include <dtg/Utilities.hpp>
#include <deque>
#include <set>
#pragma message ("Temporary iostream should be removed later")
#include <iostream>

namespace iflex {

	template<class Traits, class Visitor>
	class Parser {
		public:
		using Token = typename Traits::Token;
		using TokenEnum = typename Traits::TokenEnum;
		using CharType = typename Traits::CharType;
		using Parseable		= typename Traits::template Parseable<Visitor>;
		using ParseableList	= typename Traits::template ParseableList<Visitor>;
		using ParseableContext = typename Traits::ParseableContext;
		using TokenList = typename Traits::TokenList;
		using TokenLists = typename Traits::TokenLists;
		using ContextIterator = typename ParseableContext::iterator;

		using const_token_list_iterator	= typename TokenLists::const_iterator;
		using token_list_iterator	= typename TokenLists::iterator;

		using const_tokens_iterator	= typename TokenList::const_iterator;
		using tokens_iterator		= typename TokenList::iterator;
		/*
		struct File {
			File(const char* name):filename(name) {}
			std::deque<Token> tokens;
			dtg::SimpleString filename;
		};
		*/
		private:
	/*
		inline void CreateFileToken(const char*name) {
			m_Active = m_Rawtokens.emplace(m_Active, File(name));
		}
	*/
		public:
//		Parser(Parseable* parseables)

		constexpr Parser(Visitor& visitor, ParseableList&& parseables)
			: m_Visitor(visitor)
			, m_TokenLists(0)
			, m_Parseables(std::forward<ParseableList>(parseables)){}

		auto SetActiveList(const std::string& name) {
			return m_TokenLists->SetActive(name);
		}

		void SetTokens(TokenLists* tokens) {
			m_TokenLists = tokens;
		}

		void ParseNVisit(Visitor& visitor) {
			for (ContextIterator it = m_TokenLists->GetActive()->second.begin()
				, end = m_TokenLists->GetActive()->second.end()
				; it != end
				; ) {
				std::set<const Parseable*> set = m_Parseables.FindParseables(it->GetType());
				if (set.empty()) {
					std::cerr << "No recipe found for " << *it << '\n';
					++it;
					continue;
				}
				auto et = set.begin();
				ContextIterator last = it;
				for (auto end_s = set.end(); et != end_s; ++et) {
					last = (*et)->Match(it, end);
					if (last != it)
						break;
				}
				if (last != it) {
					(*et)->Execute(visitor, it, last);
					it = last;
				}
				else {
					std::cerr << "No recipe found for " << *it << '\n';
					++it;
				}
			}
		}

		public:
		protected:
		/*
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
		*/
		private:
		Visitor& m_Visitor;
		TokenLists* m_TokenLists;
		ParseableList m_Parseables;
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
/*
#if defined(_GLIBCXX_OSTREAM) or defined(_OSTREAM_)
		friend std::ostream& operator << (std::ostream& stream,
		const typename iflex::Parser<Traits>::File& tokens) {
			stream << tokens.filename << '\n';
			for (const auto& e : tokens.tokens)
				stream << e << '\n';
			return stream;
		}
#endif
*/
