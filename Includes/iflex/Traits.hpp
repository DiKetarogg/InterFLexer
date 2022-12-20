#pragma once
#ifndef IFLEX_TRAITS_HPP
#define IFLEX_TRAITS_HPP
#include <locale>
#include "String.hpp"
#include "Lexer.hpp"
#include "Parser.hpp"
#include "Token.hpp"

namespace iflex {

	template<class TokenEnum_, TokenEnum_ NOTAPPLICABLE, class GroupEnum_, class CommandEnum_,
//		size_t TokenablesCount, size_t RegularsCount, size_t GroupsCount,
		class Char_ = char, const char* const& Locale = GetEmptyString<char>::get>
	struct Traits {
		private:

		Traits() = delete;
		Traits(const Traits&) = delete;
		Traits(Traits&&) = delete;

		public:

		using TokenEnum		= TokenEnum_;
		using CharType		= Char_;

		using CommandEnum	= CommandEnum_;
		using GroupEnum	= GroupEnum_;

		using char_type 	= Char_;
		using enum_type		= TokenEnum_;

		typedef iflex::Lexer<Traits>		Lexer;
		template<class Visitor>
		using Parser = iflex::Parser<Traits, Visitor>;
		typedef iflex::Tokenizer<Traits>	Tokenizer;
		typedef iflex::Tokenable<Traits>	Tokenable;
		typedef iflex::TokenableRegular<Traits> TokenableRegular;
//		typedef iflex::TokenablesList<Traits>	TokenablesList;
		template<class Visitor>
		using Parseable = iflex::Parseable<Traits, Visitor>;
		template<class Visitor>
		using ParseableList = iflex::ParseableList<Traits, Visitor>;
		typedef iflex::ParseableContext<Traits>	ParseableContext;
		typedef iflex::Token<Traits>		Token;
		typedef iflex::TokenGroup<Traits>	TokenGroup;
		typedef iflex::TokenGroupMap<Traits>	TokenGroupMap;
		typedef iflex::TokenLists<Traits>	TokenLists;
		typedef iflex::TokenList<Traits>	TokenList;

		typedef iflex::Lexer<Traits>		lexer_type;
		template<class Visitor>
		using parser_type = iflex::Parser<Traits, Visitor>;
		typedef iflex::Tokenizer<Traits>	tokenizer_type;
		typedef iflex::Tokenable<Traits>	tokenable_type;
//		typedef iflex::TokenablesList<Traits>	tokenables_list_type;
		typedef iflex::TokenableRegular<Traits> tokenable_regular_type;
		template<class Visitor>
		using parseable_type = iflex::Parseable<Traits, Visitor>;
		template<class Visitor>
		using parseable_list_type = iflex::ParseableList<Traits, Visitor>;
		typedef iflex::ParseableContext<Traits>	parseable_context_type;
		typedef iflex::Token<Traits>		token_type;
		typedef iflex::TokenGroup<Traits>	token_group_type;
		typedef iflex::TokenGroupMap<Traits>	token_group_map_type;
		typedef iflex::TokenList<Traits>	token_list_type;
		typedef iflex::TokenLists<Traits>	token_lists_type;

		static constexpr TokenEnum NA = NOTAPPLICABLE;
		static constexpr const char* locale = Locale;

		//static constexpr size_t tokenablesCount = TokenablesCount;
		//static constexpr size_t regularsCount = RegularsCount;
		//static constexpr size_t groupsCount = GroupsCount;
		//private:
		//static TokenableType* names = Names;
	};
}
#endif
