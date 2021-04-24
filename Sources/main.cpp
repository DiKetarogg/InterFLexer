#include <iostream>
#include <iflex/Token.hpp>


namespace my {
	enum Tokens {NA, PROGRAM, NAME,};
	typedef iflex::Traits<Tokens, NA> Traits;
	class Tokenizer : public iflex::Tokenizer<Traits> {
		public:
		using Base = iflex::Tokenizer<Traits>;
		using Base::Tokenize;
		using CharType = typename Traits::CharType;
		private:
		Tokenizer(){
			Initialize(m_Names, sizeof(m_Names) / sizeof(Tokenable));
		}
		Tokenizer(const Tokenizer&) = delete;
		Tokenizer(Tokenizer&&) = delete;
		public:
		static Tokenizer& Get() {
			static Tokenizer t;
			return t;
		}
		private:
		Tokenable m_Names[3] {
			{"", NA},
			{"program", PROGRAM},
			{"[_A-Za-z]+[_A-Za-z0-9]*", NAME, true},	};};}
int main () {
	using namespace iflex;
	my::Tokenizer& tokenizer = my::Tokenizer::Get();
	iflex::Token<my::Traits> token(tokenizer.Find(my::PROGRAM), my::PROGRAM, 1, 1);
	Token<my::Traits> token2 = tokenizer.Tokenize("_aZ0", 2, 37);
	Token<my::Traits> token3 = tokenizer.Tokenize("_0", 2, 37);
	Token<my::Traits> token4 = tokenizer.Tokenize("aZ0", 2, 37);
	Token<my::Traits> token5 = tokenizer.Tokenize("0Z0", 2, 37);
	std::wcout << token << '\n' << token2 << '\n' 
	<< token3 << '\n' << token4 << '\n' << token5;
}
	/*
	class TokenList {
		public:
		using Tokenable = iflex::Tokenable<Tokens,NA>;
		private:
		Tokenable m_Tokenables[1] {
			{"", NA}
		};
		public:
		typedef iflex::Traits<Tokenable, sizeof(m_Tokenables) / sizeof(Tokenable)> Traits;
		using Tokenizer = Traits::Tokenizer;

		private:
		static TokenList& GetSelf() {
			static TokenList t;
			return t;
		}
		public:
		static Tokenable* Get() {
			return GetSelf().m_Tokenables;
		}

		private:
	};
	*/
//	typedef TokenList::Traits Traits;





