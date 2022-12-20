#include <iostream>
#include <iflex/Traits.hpp>
#include <string>
#include <filesystem>
#include <dtg/Console.hpp>
extern "C" {
#include <fcntl.h>
}
//#include <Sort.hpp>
namespace my {
	enum Tokens {
		NA_,
		INT_,
		FLOAT_,
		EQUALS_,
		NAME_,
		INT_LITERAL_,
		FLOAT_LITERAL_,
		SEMI_,
		PRINT_,
	};
	enum Commands {
		FUNC,
	};
	enum Groups {
		NA,
		TYPE,
		EQUALS,
		NAME,
		LITERAL,
		SEMI,
		PRINT,
	};
	typedef iflex::Traits<Tokens, NA_, Groups, Commands> Traits;

	class Tokenizer : public iflex::Tokenizer<Traits> {
		public:
		using Base = iflex::Tokenizer<Traits>;
		using CharType = typename Traits::CharType;
		using Tokenable = typename Traits::Tokenable;
		using TokenableRegular = typename Traits::TokenableRegular;
		using TokenGroupMap = typename Traits::TokenGroupMap;
		//using typename Base::TokenablesArray;
		//using typename Base::RegularsArray;
		public:
		private:
		Tokenizer(std::initializer_list<Tokenable> toks
				, std::initializer_list<TokenableRegular> regs
				, TokenGroupMap&& map)
			: Base(toks
			, regs
			, std::forward<TokenGroupMap>(map)
		){}
		Tokenizer(const Tokenizer&) = delete;
		Tokenizer(Tokenizer&&) = delete;
		public:
		static const Tokenizer& Get() {
			static /*constexpr*/ Tokenizer t(
			//(TokenablesArray)
			{
				(Tokenable)
				{"int", INT_},
				{"float", FLOAT_},
				{"=", EQUALS_},
				{";", SEMI_},
				{"print", PRINT_}
			}// (TokenablesArray)
			//(RegularsArray)
			, {
				(TokenableRegular)
				{"[_A-Za-z][_A-Za-z0-9]*", NAME_},
				{"[+-]?[0-9][0-9]*", INT_LITERAL_},
				{"[+-]?[0-9][0-9]*\\.[0-9]*"
				"|" "[+-]?[0-9]*\\.[0-9][0-9]*", FLOAT_LITERAL_}
			}
			, (TokenGroupMap) {
				{Groups::NA, {NA_}},
				{TYPE, {INT_, FLOAT_}},
				{EQUALS, {EQUALS_}},
				{NAME, {NAME_}},
				{LITERAL, {INT_LITERAL_, FLOAT_LITERAL_}},
				{SEMI, {SEMI_}},
				{PRINT, {PRINT_}}
			}//(TokenGroupMap)
			);//static Tokenizer t
			return t;
		}
		private:
	};
	class Visitor {
		public:
		using Context = Traits::ParseableContext;
		using Token = Traits::Token;
		struct Variable {
			union Data {
				int i;
				float f;
				Data() = default;
				Data(int i):i(i){}
				Data(float f):f(f){}
			} data;
			enum {
				NA,
				INT,
				FLOAT,
			} type = NA;
			Variable() = default;
			Variable(int i):data(i), type(INT){}
			Variable(float f):data(f), type(FLOAT){}
			Variable& operator=(double f) {
				type = FLOAT;
				data.f = f;
				return *this;
			}
			Variable& operator=(float f) {
				type = FLOAT;
				data.f = f;
				return *this;
			}
			Variable& operator=(int i) {
				type = INT;
				data.i = i;
				return *this;
			}
			explicit operator int() const {
				switch (type) {
					case INT:
						return data.i;
					case FLOAT:
						return data.f;
					default:
						throw std::logic_error("invalid type");
				}
			}
			explicit operator float() const {
				switch (type) {
					case INT:
						return data.i;
					case FLOAT:
						return data.f;
					default:
						throw std::logic_error("invalid type");
				}
			}
			operator bool() const {
				return type;
			}
			friend std::ostream& operator <<(std::ostream& stream, Variable var) {
				switch (var.type) {
					case INT:
						stream << var.data.i;
					break; case FLOAT:
						stream << var.data.f;
					break; default:
						throw std::logic_error("invalid type");
				}
				return stream;
			}
		};
		public:
		//type name;
		void Declare(Context ctx) {
			DeclareVariable(*ctx.begin(), ctx[1]);
		}
		//type, name, equals, name;
		void DeclareAssignName(Context ctx) {
			Variable var = GetVariable(ctx[3]);
			DeclareVariable(*ctx.begin(), ctx[1], var);
		}
		//type, name, equals, literal;
		void DeclareAssignLiteral(Context ctx) {
			Variable var = VarFromLiteral(ctx[3]);
			DeclareVariable(*ctx.begin(), ctx[1], var);
		}
		//print, name;
		void PrintName(Context ctx) {
			Variable var = GetVariable(ctx[1]);
			if (var)
				std::cout << var << '\n';
		}
		//print, literal;
		void PrintLiteral(Context ctx) {
			Variable var = VarFromLiteral(ctx[1]);
			if (var)
				std::cout << var << '\n';
		}
		private:
		Variable VarFromLiteral(const Token& literal) {
			Variable var;
			switch (literal.GetType()) {
				case FLOAT_LITERAL_:
					var = std::atof(literal.GetText());
				break; case INT_LITERAL_:
					var = std::atoi(literal.GetText());
				break; default:;
			}
			return var;
		}
		void DeclareVariable(const Token& type, const Token& name
		, Variable var = 0) {
			if (FindVariable(name)) {
				Error(name, name.GetText(), ": Already exists");
				return;
			}
			if (!var)
				return;
			switch (type.GetType()) {
				case INT_:
					var = (int)var;
				break; case FLOAT_:
					var = (float)var;
				break; default:;
			}
			m_Variables.emplace(name.GetText(), var);
		}
		template<class... Args>
		static void Error(const Token& token, const Args&... args) {
			std::cerr << dtg::color<dtg::RED, dtg::BOLD>()
				<< "error"
				<< dtg::color<dtg::NORMAL>()
				<< "["
				<< dtg::color<dtg::CYAN>()
				<< token.GetLine()
				<< dtg::color<dtg::NORMAL>()
				<< ", "
				<< dtg::color<dtg::CYAN>()
				<< token.GetChar()
				<< dtg::color<dtg::NORMAL>()
				<< "]:\v";
			(std::cerr << ... << args) << '\n';
		}
		Variable GetVariable(const Token& name) {
			Variable var = FindVariable(name);
			if (!var)
				Error(name, name.GetText(), ": Implicit declaration");
			return var;
		}
		Variable FindVariable(const Token& name) {
			dtg::SimpleString temp
				= dtg::SimpleString
				::StealingConstructor(const_cast<char *>(name.GetText())
						, std::strlen(name.GetText()));
			auto it = m_Variables.find(temp);
			temp.Lose();
			Variable ret;
			if (it == m_Variables.end()) {
				ret.data.i = 0;
				ret.type = Variable::NA;
			}
			else {
				ret = it->second;
			}
			return ret;
		}
		std::unordered_map<dtg::SimpleString, Variable> m_Variables;
	};
	class Parser : public iflex::Parser<Traits, Visitor> {
		using Base = iflex::Parser<Traits, Visitor>;
		using TokenGroupMap = Traits::TokenGroupMap;
		using TokenGroup = Traits::TokenGroup;

		public:
		using typename Base::TokenLists;

		private:
		struct name_info {
			unsigned int stack_address;
			bool variable;
		};
		Parser(ParseableList&& parseables)
			:Base(m_Visitor, std::forward<ParseableList>(parseables)) {};
		Parser(const Parser&) = delete;
		Parser(Parser&&) = delete;
		public:
		static Parser& Get() {
			static Parser parser(
				ParseableList(
					Tokenizer::Get().GetTokenGroupMap(),
					{
						(Parseable)
						{{TYPE, NAME, SEMI}, &Visitor::Declare},
						{{TYPE, NAME, EQUALS, NAME, SEMI}
						, &Visitor::DeclareAssignName},
						{{TYPE, NAME, EQUALS, LITERAL, SEMI}
						, &Visitor::DeclareAssignLiteral},
						{{PRINT, NAME, SEMI}, &Visitor::PrintName},
						{{PRINT, LITERAL, SEMI}, &Visitor::PrintLiteral}
					}
				)
			);
			return parser;
		}
		private:
		Visitor m_Visitor;
		//TokenLists m_Tokens;
		//std::list<dtg::RawStack<ByteCode, 4096>> m_Info;
		//std::list<dtg::RawStack<ByteCode, 4096>>::iterator m_InfoIt;
		//std::list<dtg::RawStack<ByteCode, 4096>> m_Bytes;
		//std::list<dtg::RawStack<ByteCode, 4096>>::iterator m_BytesIt;
	};

	class Lexer : public iflex::Lexer<Traits> {
		using Base = iflex::Lexer<Traits>;
		Lexer():Base(8192, my::Tokenizer::Get()){}
		public:
		static Lexer& Get() {
			static Lexer t{};
			return t;
		}
		void Lex() {
			using namespace iflex::is;
			using namespace iflex::logic;
			Skip<Space>();
			while (Lexing()) {
				Read<Al, OR, Value<'_'>>();
				if (ReadEmpty()) {
					Read<Value<'+'>, OR, Value<'-'>>();
					Read<Digit>();
					ReadOne<Value<'.'>>();
					if (!ReadEmpty())
						Read<Digit>();
				}
				else
					Read<Alnum, OR, Value<'_'>>();
				if (ReadEmpty())
					ReadOne<Value<'='>, OR, Value<';'>>();
				if (ReadEmpty()) {
					std::cerr << dtg::color<dtg::RED, dtg::BOLD>()
						<< "error"
						<< dtg::color<dtg::NORMAL>()
						<< "["
						<< dtg::color<dtg::CYAN>()
						<< GetLineEnd() + 1
						<< dtg::color<dtg::NORMAL>()
						<< ", "
						<< dtg::color<dtg::CYAN>()
						<< GetCharEnd() + 1
						<< dtg::color<dtg::NORMAL>()
						<< "]:\v"
						<< "unsupported character found '"
						<< *GetTokenStart()
						<< '\'' << +*GetTokenStart() << '\n';
					SkipOne<Any>();
					continue;
				}
				if (!Flush())
					//not ((Space or AlphaNumeric) or value _)
					Skip<Space, OR, AlNu, NOR, Value<'_'>>();
					//not ((not (Space or AlphaNumeric)) or value _)
					//Skip<Space, OR, AlNu, NOR, Value<'_'>>();
				Skip<Space>();
			}
		}
		template<size_t size>
		void ReadString(const char (&str)[size]) {
			StringOpen(str, size);
		}
		void ReadFile(const char *str) {
			FileOpen(str, dtg::countElems(str));
		}
	};
}

int main (int argc, char *argv[]) {
	if (argc != 2) {
		std::cout << "Current compiler version only supports 1 argument.\n";
		return 0;
	}
	my::Lexer& lexer = my::Lexer::Get();
	my::Parser& parser = my::Parser::Get();
	lexer.ReadFile(argv[1]);
	lexer.Lex();
	std::cout << "Test\n";
	for (const auto& e : my::Lexer::Get().GetTokens()) {
		std::cout << e.first << '\n';
		for (const auto& l : e.second) {
			std::cout << l;
		}
	}
	std::cout << "\n\n";
	for (const auto& e : my::Lexer::Get().GetNames()) {
		std::cout << e << '\n';
	}
	parser.SetTokens(&my::Lexer::Get().GetTokens());
	my::Visitor visitor;
	std::cout << "\n///////////---Program running---///////////\n";
	parser.ParseNVisit(visitor);
	/*
	using namespace iflex;
	my::Lexer& lexer = my::Lexer::Get();
	lexer.ReadString("program _aZ0 _0 aZ0 0Z0 function");
	lexer.Lex();//    012345678901234567890123
	for (const auto& e : my::parser)
		std::cout << e;
	*/
	/*
	my::Tokenizer& tokenizer = my::Tokenizer::Get();
	Token<my::Traits> token1(tokenizer.Find(my::PROGRAM), my::PROGRAM, 1, 1);
	Token<my::Traits> token2 = tokenizer.Tokenize("_aZ0", 2, 37);
	Token<my::Traits> token3 = tokenizer.Tokenize("_0", 2, 37);
	Token<my::Traits> token4 = tokenizer.Tokenize("aZ0", 2, 37);
	Token<my::Traits> token5 = tokenizer.Tokenize("0Z0", 2, 37);
	std::cout << token1 << '\n' << token2 << '\n'
	<< token3 << '\n' << token4 << '\n' << token5;
	*/
}
