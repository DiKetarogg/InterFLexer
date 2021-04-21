#ifndef DTG_SIMPLESTRING_HPP
#define DTG_SIMPLESTRING_HPP

#include <stdexcept>
#include "ArrayCopy.hpp"
#include "ArrayCompare.hpp"
#include "CountElems.hpp"
#include "Spaceship.hpp"


#ifndef DTG_NO_EXCEPT
#define DTG_STRING_OUT_OF_MEMORY throw std::runtime_error("dtg::SimpleString out of memory")
#else
#define DTG_STRING_OUT_OF_MEMORY
#endif


namespace dtg {

	template <class CharType>
	class BasicSimpleString {
		public:
			using value_type	= CharType;
			using pointer		= value_type*;
			using const_pointer	= const value_type*;
		public:
			BasicSimpleString():m_String(0){}

			BasicSimpleString(const BasicSimpleString& other):
				m_String(new value_type[countElems(other.m_String) + 1]) {
				if (m_String)
					dtg::TerminatedArrayCopy(m_String, other.m_String);
				else
					DTG_STRING_OUT_OF_MEMORY;

			}

			BasicSimpleString(BasicSimpleString&& other) noexcept
				:m_String(other.m_String) {
				other.m_String = 0;
			}

			BasicSimpleString(const_pointer string)
				:m_String(new value_type[countElems(string) + 1]) {
				if (m_String)
					dtg::TerminatedArrayCopy(m_String, string);
				else
					DTG_STRING_OUT_OF_MEMORY;
			}

			BasicSimpleString(const_pointer string, size_t length)
				:m_String(new value_type[length]) {
				if (m_String)
					dtg::ArrayCopy(m_String, string, length);
				else
					DTG_STRING_OUT_OF_MEMORY;
			}


			BasicSimpleString& operator = (const BasicSimpleString& other) {
				m_String = new value_type[countElems(other.m_String) + 1];
				if (m_String)
					dtg::TerminatedArrayCopy(m_String, other.m_String);
				else
					DTG_STRING_OUT_OF_MEMORY;
				return *this;
			}

			BasicSimpleString& operator = (BasicSimpleString&& other) noexcept {
				if (this != &other) {
					delete[] m_String;

					m_String = other.m_String;
					other.m_String = 0;
				}
				return *this;
			}

			BasicSimpleString& operator = (const_pointer string) {
				delete[] m_String;
				m_String = new value_type[countElems(string) + 1];
				if (m_String)
					dtg::TerminatedArrayCopy(m_String, string);
				else
					DTG_STRING_OUT_OF_MEMORY;

				return *this;
			}
			~BasicSimpleString() {
				delete m_String;
			}

			void Set(const_pointer string, size_t length) {
				delete m_String;

				m_String = new value_type[length];
				if (m_String)
					dtg::ArrayCopy(m_String, string, length);
				else
					DTG_STRING_OUT_OF_MEMORY;
				m_String[length - 1] = 0;
			}

			void Set(const_pointer str) {
				return operator=(str);
			}

			DTG_SPACESHIP_OPERATOR(ArrayCompare(m_String, second),
					value_type(0), pointer second)

			DTG_SPACESHIP_OPERATOR(ArrayCompare(m_String, other.m_String),
					value_type(0), const BasicSimpleString& other)

			operator const_pointer() const noexcept {
				return m_String;
			}

			operator pointer() noexcept {
				return m_String;
			}

		private:

		pointer m_String;
	};
	typedef BasicSimpleString<char>		SimpleString;
	typedef BasicSimpleString<wchar_t>	WSimpleString;
}

#undef DTG_DTG_STRING_OUT_OF_MEMORY

#endif
