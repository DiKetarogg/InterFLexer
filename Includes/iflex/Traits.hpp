#pragma once
#ifndef IFLEX_TRAITS_HPP
#define IFLEX_TRAITS_HPP
#include <locale>
#include "String.hpp"
#include "Lexer.hpp"
#include "Parser.hpp"
#include "Token.hpp"
namespace iflex {

	template<class TokenEnum_, TokenEnum_ NOTAPPLICABLE,
		class Char_ = char, const char* const& Locale = GetEmptyString<char>::get>
	struct Traits {

		private:

		Traits() = delete;
		Traits(const Traits&) = delete;
		Traits(Traits&&) = delete;

		public:

		using TokenEnum		= TokenEnum_;
		using CharType		= Char_;

		using char_type 	= Char_;
		using enum_type		= TokenEnum_;
		typedef iflex::Lexer<Traits>		Lexer;
		typedef iflex::Parser<Traits>		Parser;
		typedef iflex::Tokenizer<Traits>	Tokenizer;
		typedef iflex::Tokenable<Traits>	Tokenable;
		typedef iflex::Token<Traits>		Token;

		typedef iflex::Lexer<Traits>		lexer_type;
		typedef iflex::Parser<Traits>		parser_type;
		typedef iflex::Tokenizer<Traits>	tokenizer_type;
		typedef iflex::Tokenable<Traits>	tokenable_type;
		typedef iflex::Token<Traits>		token_type;

		static constexpr TokenEnum NA = NOTAPPLICABLE;
		static constexpr const char* locale = Locale;
		//private:
		//static TokenableType* names = Names;
	};

}
#endif
