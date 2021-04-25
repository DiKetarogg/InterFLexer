#pragma once
#ifndef IFLEX_TOKEN_TOKENTRAITS_HPP
#define IFLEX_TOKEN_TOKENTRAITS_HPP
namespace iflex {
	
	template<class Traits>
		class Tokenable;
	template<class Traits>
		class Tokenizer;
	template<class Traits>
		class Token;


	template<class TokenEnum_, TokenEnum_ NOTAPPLICABLE,
		class Char_ = char>
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
		typedef iflex::Tokenizer<Traits>	Tokenizer;
		typedef iflex::Tokenable<Traits>	Tokenable;
		typedef iflex::Token<Traits>		Token;

		typedef iflex::Tokenizer<Traits>	tokenizer_type;
		typedef iflex::Tokenable<Traits>	tokenable_type;
		typedef iflex::Token<Traits>		token_type;

		static constexpr TokenEnum NA = NOTAPPLICABLE;
		//private:
		//static TokenableType* names = Names;
	};

}
#endif
