#ifndef DTG_UTILITIES_RAWARRAY
#define DTG_UTILITIES_RAWARRAY
#include "dtg/Utilities/PunCast.hpp"

namespace dtg {

template <size_t SIZE, class DefaultType>
class RawArray {
	public:

		template <class T = DefaultType>
		T& operator[](size_t i) {
			return pun_cast<T>(m_container[i]);
		}

		size_t GetSize() {
			return SIZE;
		}

	private:
		char m_container[SIZE];
};


}

#endif
