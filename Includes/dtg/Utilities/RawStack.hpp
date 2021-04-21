#ifndef DTG_UTILITIES_RAWSTACK
#define DTG_UTILITIES_RAWSTACK
#include "dtg/Utilities/PunCast.hpp"

#include <stdexcept>
#define DTG_RAWSTACK_OVERFLOW throw std::runtime_error("dtg::RawStack overflow.")
#define DTG_RAWSTACK_LARGETYPE throw std::runtime_error("dtg::RawStack requested type is larger than the stack.")

namespace dtg {

template <size_t SIZE, class DefaultType>
class RawStack {
	public:
	template <class T = DefaultType>
	void Push(T val) {
		if (m_it + sizeof(T) < SIZE) {
			dtg::pun_cast<T>(m_container[m_it]) = val;
			m_it += sizeof(T);
		}
		else DTG_RAWSTACK_OVERFLOW;
	}

	template <class T = DefaultType>
	void Pop() {
		if (m_it > m_it - sizeof(T))
			m_it -= sizeof(T);
		else DTG_RAWSTACK_OVERFLOW;
	}

	template <class T = DefaultType>
	void Restore() {
		if (m_it + sizeof(T) < SIZE) {
			m_it += sizeof(T);
		}
		else DTG_RAWSTACK_OVERFLOW;
	}

	template <class T = DefaultType>
	T& Top() {
		if (m_it > m_it - sizeof(T))
			return dtg::pun_cast<T>(m_container[m_it - sizeof(T)]);
		DTG_RAWSTACK_LARGETYPE;
		return dtg::pun_cast<T>(m_container[0]);
	}

	void Clear() {
		m_it = 0;
	}

	void SetIterator (size_t it) {
		m_it = it;
	}

	size_t GetSize() {
		return SIZE;
	}

	size_t GetIterator() {
		return m_it;
	}

	private:
	size_t m_it;
	char m_container[SIZE];
};


}

#undef DTG_RAWSTACK_OVERFLOW
#undef DTG_RAWSTACK_LARGETYPE
#endif
