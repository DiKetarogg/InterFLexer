#include <iostream>
#include <iflex/Token.hpp>

namespace my {
	enum Tokens {NA, PROGRAM, NAME,};
	typedef iflex::Traits<Tokens, NA> Traits;

	class Tokenizer : public iflex::Tokenizer<Traits> {
		public:
		using Base = iflex::Tokenizer<Traits>;
		using CharType = typename Traits::CharType;
		using Tokenable = typename Traits::Tokenable;
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
		Tokenable m_Names[2] {
			{"program", PROGRAM},
			{"[_A-Za-z]+[_A-Za-z0-9]*", NAME, true},
		};
	};
}
int main () {
	using namespace iflex;
	my::Tokenizer& tokenizer = my::Tokenizer::Get();
	Token<my::Traits> token1(tokenizer.Find(my::PROGRAM), my::PROGRAM, 1, 1);
	Token<my::Traits> token2 = tokenizer.Tokenize("_aZ0", 2, 37);
	Token<my::Traits> token3 = tokenizer.Tokenize("_0", 2, 37);
	Token<my::Traits> token4 = tokenizer.Tokenize("aZ0", 2, 37);
	Token<my::Traits> token5 = tokenizer.Tokenize("0Z0", 2, 37);
	std::cout << token1 << '\n' << token2 << '\n' 
	<< token3 << '\n' << token4 << '\n' << token5;
}
